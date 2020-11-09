#include "IHandler.h"
#include "Dealer.h"
#include "Controller.h"

#include <utility>

const int BLACKJACK = 21;
const double WinFactor = 1.5f;
const int DEALERBORDER = 17;

void DealerHandler::DealerableHandler::GiveCard(Actors::IDealer * dealer) {
    auto card = dealer->GetCard();
    Event sender (Event::Type::GIVECARD, card);
    controller->HandleEvent(sender);
    if (dealer->GetPlayerHand() > BLACKJACK) {
        Event result(Event::Type::LOSE, dealer->GetBet());
        dealer->GetCasinoWin() += dealer->GetBet();
        controller->HandleEvent(result);
        SwapPlayer(dealer);
    } else if (dealer->GetPlayerHand() == BLACKJACK){
        SwapPlayer(dealer);
    }
}

void DealerHandler::DealerableHandler::SwapPlayer(Actors::IDealer * dealer) {
    dealer->ClearCurPlayerHand();
    dealer->SetBet(0);
    Event next(Event::Type::SWAPPLAYER, std::string("Change player"));
    controller->HandleEvent(next);
}

void DealerHandler::DealerableHandler::PlayOut(Actors::IDealer * dealer, Actors::IActor * player_dealer) {
    if (player_dealer->ShowHand() == BLACKJACK && dealer->GetPlayerHand() != BLACKJACK){
        Event lose(Event::Type::LOSE, dealer->GetBet());
        dealer->GetCasinoWin() += dealer->GetBet();
        controller->HandleEvent(lose);
    }else if (dealer->GetPlayerHand() == BLACKJACK){
        auto bet = dealer->GetBet() + WinFactor * dealer->GetBet();
        Event won(Event::Type::WIN, bet);
        dealer->GetCasinoWin() -= bet;
        controller->HandleEvent(won);
    }else if (dealer->GetPlayerHand() > player_dealer->ShowHand()){
        Event won(Event::Type::WIN, dealer->GetBet());
        dealer->GetCasinoWin() -= dealer->GetBet();
        controller->HandleEvent(won);
    } else if (dealer->GetPlayerHand() < player_dealer->ShowHand()){
        Event lose(Event::Type::LOSE, dealer->GetBet());
        dealer->GetCasinoWin() += dealer->GetBet();
        controller->HandleEvent(lose);
    } else if (dealer->GetPlayerHand() == player_dealer->ShowHand()) {
        Event draw(Event::Type::DRAW, dealer->GetBet());
        controller->HandleEvent(draw);
    }
    Event next(Event::Type::SWAPPLAYER, std::string("Change player"));
    controller->HandleEvent(next);
}

void DealerHandler::DealerableHandler::NewRound(Actors::IDealer * dealer) {
    assert(dealer->GetPlayerHand().LookAtCards().empty());
    Event Start (Event::Type::NEWROUND, std::string("Round Started for next player"));
    controller->HandleEvent(Start);
    SwapPlayer(dealer);
}

void DealerHandler::DealerableHandler::TakeBet([[maybe_unused]] Actors::IDealer * dealer, [[maybe_unused]] double bet) {
    Event event(Event::Type::WARN, std::string("You already have placed a bet!"));
    controller->HandleEvent(event);
}

void DealerHandler::DealerableHandler::GiveDoubleDown(Actors::IDealer * dealer) {
    if (dealer->GetPlayerHand().GetSize() > 2)
    {
        Event denied(Event::Type::WARN, std::string("You cant make an doubledown!"));
        controller->HandleEvent(denied);
    }
    else
    {
        dealer->SetBet(dealer->GetBet() * 2);
        GiveCard(dealer);
    }
}

void DealerHandler::BetableHandler::TakeBet(Actors::IDealer * dealer, double bet) {
    if (bet > static_cast<int>(dealer->max)) {
        Event event(Event::Type::WARN,
                    std::string("Too high bet, maximum is " + std::to_string(static_cast<int>(dealer->max))));
        controller->HandleEvent(event);
    } else if (bet < static_cast<int>(dealer->min)) {
        Event event(Event::Type::WARN,
                    std::string("Too low bet, minimum is " + std::to_string(static_cast<int>(dealer->min))));
        controller->HandleEvent(event);
    } else {
        Event to_bet(Event::Type::MAKEBET, bet);
        controller->HandleEvent(to_bet);
        SwapPlayer(dealer);
    }
}

// TODO а нужно ли это делать так?
void DealerHandler::BetableHandler::SwapPlayer([[maybe_unused]] Actors::IDealer * dealer) {
    Event next(Event::Type::SWAPPLAYER, std::string("Change player"));
    controller->HandleEvent(next);
}

void DealerHandler::BetableHandler::GiveCard(Actors::IDealer *) {
    Event event(Event::Type::WARN, std::string("You have not placed a bet!"));
    controller->HandleEvent(event);
}

void DealerHandler::BetableHandler::GiveDoubleDown(Actors::IDealer *) {
    Event event(Event::Type::WARN, std::string("You have not placed a bet!"));
    controller->HandleEvent(event);
}

void DealerHandler::PlayableHandler::Hit(Actors::IActor * dealer, const GameCard::Cards &card) {
    if (dealer->ShowHand() < DEALERBORDER){
        dealer->SetCard(card);
        Event hit(Event::Type::HIT, std::string("Dealer take new card!"));
        controller->HandleEvent(hit);
    } else {
        Event stand(Event::Type::STAND, std::string("Dealer pack ready!"));
        controller->HandleEvent(stand);
    }
}

void DealerHandler::PlayableHandler::SwapPlayer(Actors::IDealer *) {
    Event next(Event::Type::SWAPPLAYER, std::string("Change player"));
    controller->HandleEvent(next);
}


void DealerHandler::IHandler::SetController(std::shared_ptr<IController> cntr) {
    controller = std::move(cntr);
}