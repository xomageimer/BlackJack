#include "Dealer.h"

#include <iostream>
#include <cassert>
#include <utility>

#include "IHandler.h"
#include "Controller.h"


const int BLACKJACK = 21;
const double WinFactor = 1.5f;
const int DEALERBORDER = 17;

void Actors::SimpleDealer::RefreshStack() {
    m_stack->GenNewStacks();
}

void Actors::SimpleDealer::Hit(const GameCard::Cards & card) {
    handler->Hit(std::shared_ptr<Actors::IActor>(this), card);
}

bool Actors::SimpleDealer::BlackJackCheck() const {
    return m_hand == BLACKJACK;
}

void Actors::SimpleDealer::GiveCard() {
    handler->GiveCard(std::shared_ptr<IDealer>(this));
}

void Actors::SimpleDealer::SwapPlayer() {
    handler->SwapPlayer(std::shared_ptr<IDealer>(this));
}

void Actors::SimpleDealer::PlayOut() {
    handler->PlayOut(std::shared_ptr<IDealer>(this), std::shared_ptr<IActor>(this));
}

void Actors::SimpleDealer::NewRound() {
    handler->NewRound(std::shared_ptr<IDealer>(this));
}

void Actors::SimpleDealer::TakeBet(double bet) {
    handler->TakeBet(std::shared_ptr<IDealer>(this), bet);
}

void Actors::SimpleDealer::GiveDoubleDown() {
    handler->GiveDoubleDown(std::shared_ptr<IDealer>(this));
}

Actors::IDealer::IDealer(std::shared_ptr<IController> cntr) {
    controller = std::move(cntr);
}

void Actors::IDealer::SetHandler(std::shared_ptr<DealerHandler::IHandler> new_handler) {
    handler = std::move(new_handler);
    handler->SetController(controller);
}

Actors::SimpleDealer::SimpleDealer(std::shared_ptr<IController> cntr, double bank) : IDealer(cntr), m_bank(bank){
    m_stack = std::make_shared<GameCard::CardStack>(std::make_shared<GameCard::Mersenne_Generator>());
    m_stack->GenNewStacks();
    std::cerr << "Ready \n";
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

void Actors::SimpleDealer::TimeToShuffle() {
    if ((2 * (m_stack->CardShoeSize() + m_stack->GoneCardsSize()) / 3) <= m_stack->GoneCardsSize())
        m_stack->GenNewStacks();
}

void Actors::SimpleDealer::SetCard(const GameCard::Cards & card) {
    m_hand.SetNewCard(card);
}

const GameCard::Hand &Actors::SimpleDealer::ShowHand() const {
    return m_hand;
}

double Actors::SimpleDealer::GetPlayerCost() const {
    return casino_win;
}
