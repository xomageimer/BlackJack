#include "IHandler.h"

#include <utility>

const int BLACKJACK = 21;

const int DEALERBORDER = 17;

void DealerHandler::DealerableHandler::GiveCard(std::shared_ptr<Actors::IDealer> dealer) {
    auto card = dealer->GetCard();
    Event sender (Event::Type::GIVECARD, card);
    controller->HandleEvent(sender);
    if (dealer->GetPlayerHand() > BLACKJACK) {
        Event result(Event::Type::LOSE, dealer->GetBet());
        dealer->GetCasinoWin() += dealer->GetBet();
        controller->HandleEvent(result);
        SwapPlayer(dealer);
    } else if (dealer->GetPlayerHand() == BLACKJACK){
        Event result(Event::Type::WIN, dealer->GetBet());
        dealer->GetCasinoWin() -= dealer->GetBet();
        controller->HandleEvent(result);
        SwapPlayer(dealer);
    }
}

// TODO вызов контроллера от туда.

void DealerHandler::DealerableHandler::SwapPlayer(std::shared_ptr<Actors::IDealer> dealer) {
    dealer->ClearCurPlayerHand();
    dealer->SetBet(0);
    Event next(Event::Type::SWAPPLAYER, std::string("Change player"));
    controller->HandleEvent(next);
    NewRound(dealer);
}

// TODO playout вызывать из вне как event (внутри controller'a)
//  Event EndRound (Event::Type::PLAYOUT, "Round Ended");
//  controller->HandleEvent(EndRound);

void DealerHandler::DealerableHandler::PlayOut(std::shared_ptr<Actors::IDealer> dealer, std::shared_ptr<Actors::IActor> player_dealer) {
    if (player_dealer->ShowHand() == BLACKJACK){
        Event lose(Event::Type::LOSE, dealer->GetBet());
        dealer->GetCasinoWin() += dealer->GetBet();
        controller->HandleEvent(lose);
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
}

void DealerHandler::DealerableHandler::NewRound(std::shared_ptr<Actors::IDealer> dealer) {
    assert(dealer->GetPlayerHand().LookAtCards().empty());
    Event Start (Event::Type::NEWROUND, std::string("Round Started for next player"));
    controller->HandleEvent(Start);
}

void DealerHandler::DealerableHandler::TakeBet(std::shared_ptr<Actors::IDealer> dealer, double bet) {
    Event event(Event::Type::WARN, std::string("You already have placed a bet!"));
    controller->HandleEvent(event);
}

void DealerHandler::DealerableHandler::GiveDoubleDown(std::shared_ptr<Actors::IDealer> dealer) {
    dealer->SetBet(dealer->GetBet() * 2);
    GiveCard(dealer);
}

void DealerHandler::BetableHandler::TakeBet(std::shared_ptr<Actors::IDealer> dealer, double bet) {
    if (bet > static_cast<int>(dealer->max)){
        Event event(Event::Type::WARN, std::string("Too high bet, maximum is " + std::to_string(static_cast<int>(dealer->max))));
        controller->HandleEvent(event);
    }
    Event to_bet(Event::Type::MAKEBET, bet);
    controller->HandleEvent(to_bet);
}

void DealerHandler::BetableHandler::SwapPlayer(std::shared_ptr<Actors::IDealer> dealer) {
    Event next(Event::Type::SWAPPLAYER, std::string("Change player"));
    controller->HandleEvent(next);
}

void DealerHandler::PlayableHandler::Hit(std::shared_ptr<Actors::IActor> dealer, const GameCard::Cards &card) {
    if (dealer->ShowHand().total() < DEALERBORDER){
        dealer->SetCard(card);
        Event hit(Event::Type::HIT, std::string("Dealer take new card!"));
        controller->HandleEvent(hit);
    } else {
        Event stand(Event::Type::STAND, std::string("Dealer pack ready!"));
        controller->HandleEvent(stand);
    }
}

void DealerHandler::PlayableHandler::BlackJackCheck(std::shared_ptr<Actors::IActor> dealer) {
    if (dealer->ShowHand().total() == BLACKJACK)
    {
        Event end(Event::Type::PLAYOUT, std::string("BlackJack for dealer!"));
        controller->HandleEvent(end);
    }
}


void DealerHandler::IHandler::SetController(std::shared_ptr<IController> cntr) {
    controller = std::move(cntr);
}
