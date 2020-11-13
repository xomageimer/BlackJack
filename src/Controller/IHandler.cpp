#include "IHandler.h"
#include "Dealer.h"
#include "Actors/GameGround.h"

#include <utility>

const int BLACKJACK = 21;
const double WinFactor = 1.5f;
const int DEALERBORDER = 17;

void DealerHandler::DealerableHandler::GiveCard(Actors::IDealer * dealer) {
    auto card = dealer->GetCard();
    Event sender (Event::Type::GIVECARD, card);
    dealer->HandleEvent(sender);
    if (dealer->GetPlayerHand() > BLACKJACK) {
        Event result(Event::Type::LOSE, dealer->GetBet());
        dealer->GetCasinoWin() += dealer->GetBet();
        dealer->HandleEvent(result);
        SwapPlayer(dealer);
    } else if (dealer->GetPlayerHand() == BLACKJACK){
        SwapPlayer(dealer);
    }
}

void DealerHandler::DealerableHandler::SwapPlayer(Actors::IDealer * dealer) {
    if (!dealer->NextPlayer()) {
        Event next(Event::Type::STATE, std::string("Change state"));
        dealer->HandleEvent(next);
        dealer->set_current(Actors::IDealer::states::PLAYABLE);
    }
}

void DealerHandler::DealerableHandler::PlayOut(Actors::IDealer * dealer, Actors::IActor * player_dealer) {
    while (dealer->NextPlayer()) {
        if (player_dealer->ShowHand() == BLACKJACK && dealer->GetPlayerHand() != BLACKJACK) {
            Event lose(Event::Type::LOSE, dealer->GetBet());
            dealer->GetCasinoWin() += dealer->GetBet();
            dealer->HandleEvent(lose);
        } else if (dealer->GetPlayerHand() == BLACKJACK) {
            auto bet = WinFactor * dealer->GetBet();
            Event won(Event::Type::WIN, bet);
            dealer->GetCasinoWin() -= bet;
            dealer->HandleEvent(won);
        } else if (dealer->GetPlayerHand() > player_dealer->ShowHand()) {
            Event won(Event::Type::WIN, dealer->GetBet());
            dealer->GetCasinoWin() -= dealer->GetBet();
            dealer->HandleEvent(won);
        } else if (dealer->GetPlayerHand() < player_dealer->ShowHand()) {
            Event lose(Event::Type::LOSE, dealer->GetBet());
            dealer->GetCasinoWin() += dealer->GetBet();
            dealer->HandleEvent(lose);
        } else if (dealer->GetPlayerHand() == player_dealer->ShowHand()) {
            Event draw(Event::Type::DRAW, dealer->GetBet());
            dealer->HandleEvent(draw);
        }
    }
}

void DealerHandler::DealerableHandler::TakeBet([[maybe_unused]] Actors::IDealer * dealer, [[maybe_unused]] double bet) {
    Event event(Event::Type::WARN, std::string("You already have placed a bet!"));
    dealer->HandleEvent(event);
}

void DealerHandler::DealerableHandler::GiveDoubleDown(Actors::IDealer * dealer) {
    if (dealer->GetPlayerHand().GetSize() > 2)
    {
        Event denied(Event::Type::WARN, std::string("You cant make an doubledown!"));
        dealer->HandleEvent(denied);
    }
    else
    {
        dealer->SetBet(dealer->GetBet() * 2);
        Event to_bet(Event::Type::MAKEBET, dealer->GetBet());
        dealer->HandleEvent(to_bet);
        GiveCard(dealer);
        SwapPlayer(dealer);
    }
}

void DealerHandler::BetableHandler::TakeBet(Actors::IDealer * dealer, double bet) {
    if (bet > static_cast<int>(dealer->max)) {
        Event event(Event::Type::WARN,
                    std::string("Too high bet, maximum is " + std::to_string(static_cast<int>(dealer->max))));
        dealer->HandleEvent(event);
    } else if (bet < static_cast<int>(dealer->min)) {
        Event event(Event::Type::WARN,
                    std::string("Too low bet, minimum is " + std::to_string(static_cast<int>(dealer->min))));
        dealer->HandleEvent(event);
    } else {
        Event to_bet(Event::Type::MAKEBET, bet);
        dealer->HandleEvent(to_bet);
        SwapPlayer(dealer);
    }
}

void DealerHandler::BetableHandler::SwapPlayer([[maybe_unused]] Actors::IDealer * dealer) {
    if (dealer->NextPlayer()) {
        Event next(Event::Type::SWAPPLAYER, std::string("Change player"));
        dealer->HandleEvent(next);
    } else {
        Event next(Event::Type::STATE, std::string("Change state"));
        dealer->HandleEvent(next);
        dealer->set_current(Actors::IDealer::states::DISTRIBUTION);
        dealer->NewRound();
    }
}

void DealerHandler::BetableHandler::GiveCard(Actors::IDealer * dealer) {
    Event event(Event::Type::WARN, std::string("You have not placed a bet!"));
    dealer->HandleEvent(event);
}

void DealerHandler::BetableHandler::GiveDoubleDown(Actors::IDealer * dealer) {
    Event event(Event::Type::WARN, std::string("You have not placed a bet!"));
    dealer->HandleEvent(event);
}

void DealerHandler::PlayableHandler::SwapPlayer(Actors::IDealer * dealer) {
    Event next(Event::Type::STATE, std::string("Change state"));
    dealer->HandleEvent(next);
    dealer->set_current(Actors::IDealer::states::DEALERABLE);
    dealer->PlayOut();
}

void DealerHandler::PlayableHandler::GiveCard(Actors::IDealer * dealer) {
    auto card = dealer->GetCard();
    Event sender (Event::Type::GIVECARD, card);
    dealer->HandleEvent(sender);
}

void DealerHandler::PlayableHandler::Hit(Actors::IDealer * dealer, Actors::IActor *player_dealer, const GameCard::Cards &card) {
    if (player_dealer->ShowHand() < DEALERBORDER){
        player_dealer->SetCard(card);
        Event hit(Event::Type::GIVECARD, std::string("Dealer take new card!"));
        dealer->HandleEvent(hit);
    } else {
        Event stand(Event::Type::STAND, std::string("Dealer pack ready!"));
        dealer->HandleEvent(stand);
    }
}

void DealerHandler::DistributionHandler::NewRound(Actors::IDealer * dealer, Actors::IActor * player_dealer) {
    player_dealer->Hit(dealer->GetCard());
    player_dealer->Hit(dealer->GetCard());
    while (dealer->NextPlayer()) {
        dealer->GiveCard();
        dealer->GiveCard();
        SwapPlayer(dealer);
    }
}

void DealerHandler::DistributionHandler::GiveCard(Actors::IDealer * dealer) {
    auto card = dealer->GetCard();
    Event sender (Event::Type::GIVECARD, card);
    dealer->HandleEvent(sender);
}

void DealerHandler::DistributionHandler::Hit(Actors::IDealer  * dealer, Actors::IActor *player_dealer, const GameCard::Cards &card) {
    player_dealer->SetCard(card);
}
