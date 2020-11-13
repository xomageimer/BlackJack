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
    cur_handler->TakeBet(this, current_player, bet);
}

void Controller::SimpleDealer::GiveCard() {
    cur_handler->GiveCard(this, current_player);
}

void Controller::SimpleDealer::SwapPlayer() {
    cur_handler->SwapPlayer(this, current_player);
}

void Controller::SimpleDealer::PlayOut() {
    cur_handler->PlayOut(this, current_player);
}

void Controller::SimpleDealer::NewRound() {
    cur_handler->NewRound(this, current_player);
}

void Controller::SimpleDealer::GiveDoubleDown() {
    cur_handler->GiveDoubleDown(this, current_player);
}
