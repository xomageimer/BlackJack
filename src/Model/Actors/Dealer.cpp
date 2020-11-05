#include "Dealer.h"

#include <utility>

void Actors::SimpleDealer::RefreshStack() {
    m_stack->GenNewStacks();
}

void Actors::SimpleDealer::Hit(const GameCard::Cards & card) {
    handler->Hit(std::shared_ptr<IActor>(this), card);
}

void Actors::SimpleDealer::DoubleDown(const GameCard::Cards &, const GameCard::Cards &) {
    throw std::bad_exception();
}

void Actors::SimpleDealer::GetResult(double points) {
    handler->GetResult(std::shared_ptr<IActor>(this, points));
}

void Actors::SimpleDealer::BlackJackCheck() {
    handler->BlackJackCheck(std::shared_ptr<IActor>(this));
}

void Actors::SimpleDealer::GiveCard() {
    handler->GiveCard(std::shared_ptr<IDealer>(this));
}

void Actors::SimpleDealer::SwapPlayer() {
    handler->SwapPlayer(std::shared_ptr<IDealer>(this));
}

void Actors::SimpleDealer::PlayOut() {
    handler->PlayOut(std::shared_ptr<IDealer>(this));
}

void Actors::SimpleDealer::NewRound() {
    handler->NewRound(std::shared_ptr<IDealer>(this));
}

void Actors::SimpleDealer::TakeBet(double bet) {
    handler->TakeBet(std::shared_ptr<IDealer>(this, bet));
}

void Actors::SimpleDealer::GiveDoubleDown() {
    handler->GiveDoubleDown(std::shared_ptr<IDealer>(this));
}


void Actors::SimpleDealer::SetCard(const GameCard::Cards & card) {
    this->m_hand.SetNewCard(card);
}

void Actors::SimpleDealer::SetPoints(double fract) {
    this->bank += fract;
}

const GameCard::Hand &Actors::SimpleDealer::ShowHand() const {
    return this->m_hand;
}

double Actors::SimpleDealer::GetPlayerCost() const {
    return this->bank;
}

const GameCard::Hand &Actors::SimpleDealer::GetPlayerHand() const {
    return this->current_player_hand;
}

void Actors::SimpleDealer::ConfigPlayerHand(GameCard::Cards &card) {
    this->current_player_hand.SetNewCard(card);
}

GameCard::Cards Actors::SimpleDealer::GetCard() {
    return this->m_stack->GetCard();
}

void Actors::SimpleDealer::ClearCurPlayerHand() {
    this->current_player_hand.Clear();
}

void Actors::SimpleDealer::SetBet(double d) {
    this->current_bet = d;
}

double Actors::SimpleDealer::GetBet() const{
    return this->current_bet;
}

double &Actors::SimpleDealer::GetCasinoWin() {
    return this->casino_win;
}

void Actors::SimpleDealer::SetPlayerHand(GameCard::Hand & hand) {
    this->m_hand = hand;
}

Actors::IDealer::IDealer(std::shared_ptr<IController> cntr) {
    m_stack = std::make_shared<GameCard::CardStack>();
    controller = std::move(cntr);
}

std::shared_ptr<IController> Actors::IDealer::GetController() {
    return controller;
}
