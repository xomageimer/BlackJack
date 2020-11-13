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
    cur_handler->Hit(this, this, card);
}

bool Actors::SimpleDealer::BlackJackCheck() const {
    return m_hand == BLACKJACK;
}

void Actors::SimpleDealer::GiveCard() {
    cur_handler->GiveCard(this);
}

void Actors::SimpleDealer::SwapPlayer() {
    cur_handler->SwapPlayer(this);
}

void Actors::SimpleDealer::PlayOut() {
    cur_handler->PlayOut(this, this);
}

void Actors::SimpleDealer::NewRound() {
    cur_handler->NewRound(this, this);
}

void Actors::SimpleDealer::TakeBet(int bet) {
    cur_handler->TakeBet(this, bet);
}

void Actors::SimpleDealer::GiveDoubleDown() {
    cur_handler->GiveDoubleDown(this);
}

Actors::SimpleDealer::SimpleDealer(int bank) : m_bank(bank){
    m_stack = std::make_shared<GameCard::CardStack>(std::make_shared<GameCard::Mersenne_Generator>());
    m_stack->GenNewStacks();
    std::cout << "Ready \n";
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

int Actors::SimpleDealer::GetPlayerCost() const {
    return m_bank;
}

void Actors::SimpleDealer::SetBet(int bet) {
    player_bet = bet;
}

int Actors::SimpleDealer::GetBet() const {
    return player_bet;
}

void Actors::SimpleDealer::SetPlayer(std::shared_ptr<IActor> next) {
    current_player = next;
}

GameCard::Cards Actors::SimpleDealer::GetCard() {
    return this->m_stack->GetCard();
}

const GameCard::Hand &Actors::SimpleDealer::GetPlayerHand() const {
    return current_player->ShowHand();
}

Actors::IDealer::IDealer() {
    cmd_handles.emplace(std::piecewise_construct, std::forward_as_tuple(states::DEALERABLE),
            std::forward_as_tuple(std::make_shared<DealerHandler::DealerableHandler>()));
    cmd_handles.emplace(std::piecewise_construct, std::forward_as_tuple(states::BETABLE),
            std::forward_as_tuple(std::make_shared<DealerHandler::BetableHandler>()));
    cmd_handles.emplace(std::piecewise_construct, std::forward_as_tuple(states::DISTRIBUTION),
                        std::forward_as_tuple(std::make_shared<DealerHandler::DistributionHandler>()));
    cmd_handles.emplace(std::piecewise_construct, std::forward_as_tuple(states::PLAYABLE),
                        std::forward_as_tuple(std::make_shared<DealerHandler::PlayableHandler>()));

    cur_handler = cmd_handles.at(states::BETABLE);
}

bool Actors::IDealer::NextPlayer() {
  //  ground->ChangePlayer(Event{});
}

void Actors::IDealer::HandleEvent(const Event & event) {
    // отправить в ground, а отткуда в оутпут соостветсвующий event
}

void Actors::IDealer::SetFacade(GameGround * new_ground) {
    ground = new_ground;
}
