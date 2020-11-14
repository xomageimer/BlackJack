#include "Dealer.h"
#include "IHandler.h"
#include "Actors/GameGround.h"

// TODO мб стоит засунуть ground внутрь диллера и передавать shared_ptr на player

Controller::IDealer::IDealer() {
     cmd_handles.emplace(std::piecewise_construct, std::forward_as_tuple(states::DEALERABLE),
                        std::forward_as_tuple(std::make_shared<DealerHandlers::DealerableHandler>()));
    cmd_handles.emplace(std::piecewise_construct, std::forward_as_tuple(states::BETABLE),
                        std::forward_as_tuple(std::make_shared<DealerHandlers::BetableHandler>()));
    cmd_handles.emplace(std::piecewise_construct, std::forward_as_tuple(states::DISTRIBUTION),
                        std::forward_as_tuple(std::make_shared<DealerHandlers::DistributionHandler>()));
    cmd_handles.emplace(std::piecewise_construct, std::forward_as_tuple(states::PLAYABLE),
                        std::forward_as_tuple(std::make_shared<DealerHandlers::PlayableHandler>()));

    cur_handler = cmd_handles.at(states::BETABLE);
}

void Controller::IDealer::SetBase(GameGround * new_ground) {
    ground = new_ground;
}

void Controller::IDealer::HandleEvent(const Event & event) {
    ground->Display(event);
}

void Controller::IDealer::Next() {
    ground->ChangePlayer();
}

bool Controller::IDealer::IsPlayerDealer() const {
    return ground->CheckPlayerEQDealer();
}

void Controller::IDealer::Reset() {
    ground->Reset();
}

void Controller::IDealer::MakeBet(int bet) {
    ground->TakeBet(bet);
}

GameCard::Cards Controller::IDealer::GetCard() {
    return m_stack->GetCard();
}

Actors::IPlayer *Controller::IDealer::GetPlayer() {
    return current_player;
}

bool Controller::IDealer::HasSomePlayer() const {
    return ground->ActivePlayers();
}

void Controller::SimpleDealer::SetCard(const GameCard::Cards & card) {
    m_hand.SetNewCard(card);
}

bool Controller::SimpleDealer::BlackJackCheck() const {
    return m_bank == Actors::BLACKJACK;
}

void Controller::SimpleDealer::GetRoundResult(int b) {
    m_bank += b;
}

const GameCard::Hand &Controller::SimpleDealer::ShowHand() const {
    return m_hand;
}

int Controller::SimpleDealer::GetPlayerCost() const {
    return m_bank;
}

void Controller::SimpleDealer::SetPlayer(Actors::IPlayer * next_player, int next_bet) {
    current_player = next_player;
    player_bet = next_bet;
}

void Controller::SimpleDealer::TimeToShuffle() {
    m_stack->TimeToShuffle();
}

void Controller::SimpleDealer::TakeBet(int bet) {
    cur_handler->TakeBet(this, bet);
}

void Controller::SimpleDealer::GiveCard() {
    cur_handler->GiveCard(this);
}

void Controller::SimpleDealer::SwapPlayer() {
    cur_handler->SwapPlayer(this);
}

void Controller::SimpleDealer::PlayOut() {
    cur_handler->PlayOut(this);
}

void Controller::SimpleDealer::NewRound() {
    cur_handler->NewRound(this);
}

void Controller::SimpleDealer::GiveDoubleDown() {
    cur_handler->GiveDoubleDown(this);
}

int Controller::SimpleDealer::GetBet() const {
    return player_bet;
}

void Controller::SimpleDealer::ExtraEnd() {
    set_current(Controller::IDealer::states::PLAYABLE);
    TimeToShuffle();
}

void Controller::SimpleDealer::ClearHand() {
    m_hand.Clear();
}

GameCard::Hand Controller::SimpleDealer::GetDealerHand() const {
    return m_hand;
}
