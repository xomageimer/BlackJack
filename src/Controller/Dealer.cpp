#include "Dealer.h"
#include "IHandler.h"
#include "Actors/GameGround.h"

// TODO мб стоит засунуть ground внутрь диллера и передавать shared_ptr на player

Controller::IDealer::IDealer() {
     cmd_handles.emplace(std::piecewise_construct, std::forward_as_tuple(states::BET_SERVANT),
                        std::forward_as_tuple(std::make_shared<DealerHandlers::BetHandler>()));
    cmd_handles.emplace(std::piecewise_construct, std::forward_as_tuple(states::MOVE_SERVANT),
                        std::forward_as_tuple(std::make_shared<DealerHandlers::MoveHandler>()));
    cmd_handles.emplace(std::piecewise_construct, std::forward_as_tuple(states::PLAYOUT_SERVANT),
                        std::forward_as_tuple(std::make_shared<DealerHandlers::PlayoutHandler>()));
    cmd_handles.emplace(std::piecewise_construct, std::forward_as_tuple(states::ROUND_SERVANT),
                        std::forward_as_tuple(std::make_shared<DealerHandlers::RoundHandler>()));
    cmd_handles.emplace(std::piecewise_construct, std::forward_as_tuple(states::YOURSELF_SERVANT),
                        std::forward_as_tuple(std::make_shared<DealerHandlers::PlayingHandler>()));
    cmd_handles.emplace(std::piecewise_construct, std::forward_as_tuple(states::DEAL_SERVANT),
                        std::forward_as_tuple(std::make_shared<DealerHandlers::DealHandler>()));


    cur_state = states::BET_SERVANT;
    cur_handler = cmd_handles.at(cur_state);
}

void Controller::IDealer::SetPlayer(std::shared_ptr<Actors::IPlayer> new_player) {
    m_players.emplace_back(std::move(new_player), 0);
}

std::pair<std::shared_ptr<Actors::IPlayer>, int> & Controller::IDealer::getPlayer() {
    return m_players.at(cursor);
}

const std::vector<std::shared_ptr<Actors::IPlayer>> & Controller::IDealer::kickAFK() {
    return AFK_players;
}

std::shared_ptr<Actors::IPlayer> Controller::IDealer::getDealerPlayer() {
    return player_dealer;
}

void Controller::IDealer::AFKCurrentPlayer() {
    AFK_players.push_back(m_players.at(cursor).first);
    m_players.erase(m_players.begin() + cursor);
}

void Controller::SimpleDealer::ServeBet() {
    cur_handler->serveBet(this);
}

void Controller::SimpleDealer::ServeMove() {
    cur_handler->serveMove(this);
}

void Controller::SimpleDealer::ServeRound() {
    cur_handler->serveRound(this);
}

void Controller::SimpleDealer::ServePlayout() {
    cur_handler->servePlayout(this);
}

void Controller::SimpleDealer::ServeYourself() {
    cur_handler->serveYourself(this);
}

void Controller::SimpleDealer::Process() {
    switch (cur_state) {
        case IDealer::states::BET_SERVANT:
            ServeBet();
            break;
        case IDealer::states::DEAL_SERVANT:
        case IDealer::states::MOVE_SERVANT:
            ServeMove();
            break;
        case IDealer::states::ROUND_SERVANT:
            ServeRound();
            break;
        case IDealer::states::PLAYOUT_SERVANT:
            ServePlayout();
            break;
        case IDealer::states::YOURSELF_SERVANT:
            ServeYourself();
            break;
        default:
            ServeBet();
            break;
    }
}
