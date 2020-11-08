#include "Dealer.h"

#include <cassert>
#include <utility>

#include "Controller.h"

const int BLACKJACK = 21;
const int WinFactor = 1.5;
const int DEALERBORDER = 17;

Actors::IDealer::IDealer(std::shared_ptr<IController> cntr) {
    controller = std::move(cntr);
}

Actors::SimpleDealer::SimpleDealer(std::shared_ptr<IController> cntr) : IDealer(cntr){
    m_stack = std::make_shared<GameCard::CardStack>();
}

void Actors::SimpleDealer::Hit(const GameCard::Cards & card) {
    if (this->ShowHand() < DEALERBORDER){
        this->SetCard(card);
        Event hit(Event::Type::HIT, std::string("Dealer take new card!"));
        controller->HandleEvent(hit);
    } else {
        Event stand(Event::Type::STAND, std::string("Dealer pack ready!"));
        controller->HandleEvent(stand);
    }
}

const GameCard::Hand &Actors::SimpleDealer::ShowHand() const {
    return m_hand;
}

double Actors::SimpleDealer::GetPlayerCost() const {
    return bank;
}

void Actors::SimpleDealer::GiveCard() {
    auto card = this->GetCard();
    Event sender (Event::Type::GIVECARD, card);
    controller->HandleEvent(sender);
    if (this->GetPlayerHand() > BLACKJACK) {
        Event result(Event::Type::LOSE, this->GetBet());
        this->GetCasinoWin() += this->GetBet();
        controller->HandleEvent(result);
        SwapPlayer();
    } else if (this->GetPlayerHand() == BLACKJACK){
        SwapPlayer();
    }
}

void Actors::SimpleDealer::SwapPlayer() {
    this->ClearCurPlayerHand();
    this->SetBet(0);
    Event next(Event::Type::SWAPPLAYER, std::string("Change player"));
    controller->HandleEvent(next);
    NewRound();
}

void Actors::SimpleDealer::PlayOut() {
    if (this->ShowHand() == BLACKJACK && this->GetPlayerHand() != BLACKJACK){
        Event lose(Event::Type::LOSE, this->GetBet());
        this->GetCasinoWin() += this->GetBet();
        controller->HandleEvent(lose);
    }else if (this->GetPlayerHand() == BLACKJACK){
        auto bet = this->GetBet() + WinFactor * this->GetBet();
        Event won(Event::Type::WIN, bet);
        this->GetCasinoWin() -= bet;
        controller->HandleEvent(won);
    }else if (this->GetPlayerHand() > this->ShowHand()){
        Event won(Event::Type::WIN, this->GetBet());
        this->GetCasinoWin() -= this->GetBet();
        controller->HandleEvent(won);
    } else if (this->GetPlayerHand() < this->ShowHand()){
        Event lose(Event::Type::LOSE, this->GetBet());
        this->GetCasinoWin() += this->GetBet();
        controller->HandleEvent(lose);
    } else if (this->GetPlayerHand() == this->ShowHand()) {
        Event draw(Event::Type::DRAW, this->GetBet());
        controller->HandleEvent(draw);
    }
    Event next(Event::Type::SWAPPLAYER, std::string("Change player"));
    controller->HandleEvent(next);
}

void Actors::SimpleDealer::NewRound() {
    assert(this->GetPlayerHand().LookAtCards().empty());
    Event Start (Event::Type::NEWROUND, std::string("Round Started for next player"));
    controller->HandleEvent(Start);
}

void Actors::SimpleDealer::TakeBet(double bet) {
    if (bet > static_cast<int>(this->max)){
        Event event(Event::Type::WARN, std::string("Too high bet, maximum is " + std::to_string(static_cast<int>(this->max))));
        controller->HandleEvent(event);
    }
    Event to_bet(Event::Type::MAKEBET, bet);
    controller->HandleEvent(to_bet);
}

void Actors::SimpleDealer::GiveDoubleDown() {
    if (this->GetPlayerHand().GetSize() > 2)
    {
        Event denied(Event::Type::WARN, std::string("You cant make an doubledown!"));
        controller->HandleEvent(denied);
    }
    else{
        this->SetBet(this->GetBet() * 2);
        GiveCard();
    }
}

bool Actors::SimpleDealer::BlackJackCheck() const {
    if (this->ShowHand() == BLACKJACK)
    {
        Event end(Event::Type::PLAYOUT, std::string("BlackJack for this!"));
        controller->HandleEvent(end);
    }
}

void Actors::SimpleDealer::RefreshStack() {
    m_stack->GenNewStacks();
}

void Actors::SimpleDealer::SetBet(double d) {
    this->current_bet = d;
}

double Actors::SimpleDealer::GetBet() const {
    return this->current_bet;
}

double &Actors::SimpleDealer::GetCasinoWin() {
    return this->casino_win;
}

void Actors::SimpleDealer::SetPlayerHand(GameCard::Hand & hand) {
    this->m_hand = hand;
}

const GameCard::Hand &Actors::SimpleDealer::GetPlayerHand() const {
    return this->current_player_hand;;
}

void Actors::SimpleDealer::ClearCurPlayerHand() {
    this->current_player_hand.Clear();
}

void Actors::SimpleDealer::ConfigPlayerHand(GameCard::Cards &card) {
    this->current_player_hand.SetNewCard(card);
}

GameCard::Cards Actors::SimpleDealer::GetCard() {
    return this->m_stack->GetCard();
}
