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


//
//void Actors::SimpleDealer::RefreshStack() {
//    m_stack->GenNewStacks();
//}
//
//void Actors::SimpleDealer::Hit(const GameCard::Cards & card) {
//    handler->Hit(std::shared_ptr<Actors::IActor>(this), card);
//}
//
//void Actors::SimpleDealer::BlackJackCheck() {
//    handler->BlackJackCheck(std::shared_ptr<IActor>(this));
//}
//
//void Actors::SimpleDealer::GiveCard() {
//    handler->GiveCard(std::shared_ptr<IDealer>(this));
//}
//
//void Actors::SimpleDealer::SwapPlayer() {
//    handler->SwapPlayer(std::shared_ptr<IDealer>(this));
//}
//
//void Actors::SimpleDealer::PlayOut() {
//    handler->PlayOut(std::shared_ptr<IDealer>(this), std::shared_ptr<IActor>(this));
//}
//
//void Actors::SimpleDealer::NewRound() {
//    handler->NewRound(std::shared_ptr<IDealer>(this));
//}
//
//void Actors::SimpleDealer::TakeBet(double bet) {
//    handler->TakeBet(std::shared_ptr<IDealer>(this), bet);
//}
//
//void Actors::SimpleDealer::GiveDoubleDown() {
//    handler->GiveDoubleDown(std::shared_ptr<IDealer>(this));
//}
//
//const GameCard::Hand &Actors::SimpleDealer::ShowHand() const {
//    return this->m_hand;
//}
//
//double Actors::SimpleDealer::GetPlayerCost() const {
//    return this->bank;
//}
//
//const GameCard::Hand &Actors::SimpleDealer::GetPlayerHand() const {
//    return this->current_player_hand;
//}
//
//void Actors::SimpleDealer::ConfigPlayerHand(GameCard::Cards &card) {
//    this->current_player_hand.SetNewCard(card);
//}
//
//GameCard::Cards Actors::SimpleDealer::GetCard() {
//    return this->m_stack->GetCard();
//}
//
//void Actors::SimpleDealer::ClearCurPlayerHand() {
//    this->current_player_hand.Clear();
//}
//
//void Actors::SimpleDealer::SetBet(double d) {
//    this->current_bet = d;
//}
//
//double Actors::SimpleDealer::GetBet() const{
//    return this->current_bet;
//}
//
//double &Actors::SimpleDealer::GetCasinoWin() {
//    return this->casino_win;
//}
//
//void Actors::SimpleDealer::SetPlayerHand(GameCard::Hand & hand) {
//    this->m_hand = hand;
//}
//
//
//Actors::SimpleDealer::SimpleDealer(std::shared_ptr<IController> cntr) : IDealer(cntr), IActor(){}
//
//Actors::IDealer::IDealer(std::shared_ptr<IController> cntr) {
//    m_stack = std::make_shared<GameCard::CardStack>();
//    controller = std::move(cntr);
//}
//
//std::shared_ptr<IController> Actors::IDealer::GetController() {
//    return controller;
//}
//
//void Actors::IDealer::set_current(std::shared_ptr<DealerHandler::IHandler> h)  {
//    handler = h;
//    handler->SetController(controller);
//}
//
//Actors::IDealer::IDealer(std::shared_ptr<IController> cntr) : controller (cntr) {}
//
//void Actors::SimpleDealer::Hit(const GameCard::Cards & card) {
//    m_hand.SetNewCard(card);
//}
//
//const GameCard::Hand &Actors::SimpleDealer::ShowHand() const {
//    return m_hand;
//}
//
//double Actors::SimpleDealer::GetPlayerCost() const {
//    return bank;
//}
//
//void Actors::SimpleDealer::GiveCard() {
//    auto card = this->GetCard();
//    Event sender (Event::Type::GIVECARD, card);
//    controller->HandleEvent(sender);
//    if (this->GetPlayerHand() > BLACKJACK) {
//        Event result(Event::Type::LOSE, this->GetBet());
//        this->GetCasinoWin() += this->GetBet();
//        controller->HandleEvent(result);
//        SwapPlayer();
//    } else if (this->GetPlayerHand() == BLACKJACK){
//        Event result(Event::Type::WIN, this->GetBet());
//        this->GetCasinoWin() -= this->GetBet();
//        controller->HandleEvent(result);
//        SwapPlayer();
//    }
//}
//
//// запрашивает своп плеера на следующего (с этим запросы закончены)
//void Actors::SimpleDealer::SwapPlayer() {
//    this->ClearCurPlayerHand();
//    this->SetBet(0);
//    Event next(Event::Type::SWAPPLAYER, std::string("Change player"));
//    controller->HandleEvent(next);
//    NewRound();
//}
//
//// разыгрывается в конце раунда
//void Actors::SimpleDealer::PlayOut() {
//    if (this->ShowHand() == BLACKJACK){
//        Event lose(Event::Type::LOSE, this->GetBet());
//        this->GetCasinoWin() += this->GetBet();
//        controller->HandleEvent(lose);
//    }else if (this->GetPlayerHand() > this->ShowHand()){
//        Event won(Event::Type::WIN, this->GetBet());
//        this->GetCasinoWin() -= this->GetBet();
//        controller->HandleEvent(won);
//    } else if (this->GetPlayerHand() < this->ShowHand()){
//        Event lose(Event::Type::LOSE, this->GetBet());
//        this->GetCasinoWin() += this->GetBet();
//        controller->HandleEvent(lose);
//    } else if (this->GetPlayerHand() == this->ShowHand()) {
//        Event draw(Event::Type::DRAW, this->GetBet());
//        controller->HandleEvent(draw);
//    }
//}
//
//void Actors::SimpleDealer::NewRound() {
//    assert(this->GetPlayerHand().LookAtCards().empty());
//    Event Start (Event::Type::NEWROUND, std::string("Round Started for next player"));
//    controller->HandleEvent(Start);
//}
//
//void Actors::SimpleDealer::TakeBet(double bet) {
//    if (bet > static_cast<int>(this->max)){
//        Event event(Event::Type::WARN, std::string("Too high bet, maximum is " + std::to_string(static_cast<int>(this->max))));
//        controller->HandleEvent(event);
//    }
//    Event to_bet(Event::Type::MAKEBET, bet);
//    controller->HandleEvent(to_bet);
//}
