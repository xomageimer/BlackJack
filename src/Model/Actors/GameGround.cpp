#include "GameGround.h"

#include <algorithm>
#include <utility>

bool GameGround::SubscribePlayer(std::string player_nickname, std::shared_ptr<Actors::IPlayer> new_player) {
    // TODO можно проверять хватает ли у плеера фишек
    if (queue.size() < MAX_PLAYER_COUNT) {
        queue.emplace_back(player_nickname);
        bets.emplace_back(0);
        players.emplace(std::piecewise_construct, std::forward_as_tuple(player_nickname),
                        std::forward_as_tuple(new_player));
        if (queue.size() == 1){
            current_player = players.at(queue.at(0));
        }
        return true;
    } else if (!vacancy.empty()) {
        queue[vacancy.back()] = player_nickname;
        bets[vacancy.back()] = 0.f;
        vacancy.pop_back();
        players.emplace(std::piecewise_construct, std::forward_as_tuple(player_nickname),
                        std::forward_as_tuple(new_player));
        return true;
    }
    return false;
}

// TODO переделать для всех логгеров
void GameGround::SetViewManager(std::shared_ptr<ILogger> man) {
    om->subscribe("View", std::move(man));
}

bool GameGround::UnSubscribePlayer(const std::string &player_nickname) {
    if (AFK_players.find(player_nickname) == AFK_players.end())
        return false;
    size_t i = -1;
    auto it = std::find_if(queue.begin(), queue.end(),[&](auto & word) {
        i++;
        return word == player_nickname;
    });
    it->clear();
    vacancy.push_back(i);
    players.erase(player_nickname);
    return true;
}

void GameGround::Output() {
    om->drop();
}

void GameGround::Destroy() {
    om->destroy();
}

GameGround::GameGround(std::shared_ptr<OutputManager> output) : om(std::move(output)) {}

// сначала change потом действие
void GameGround::ChangePlayer() {
    int current_bet;
    current_player = (current_number == queue.size()) ? (current_bet = 0, player_dealer)
            : (current_bet = bets[current_number], players.at(queue.at(current_number++)));
    dealer->SetPlayer(current_player.get(), current_bet);
}

void GameGround::Reset() {
    current_number = 0;
}

void GameGround::Display(const Event & event) {
    // свитчом бегать по евентам и выплевывать в om вывод
    // Если это специфичный евент можно вывести сразу, а можно накопить его и вывести потом
}

bool GameGround::CheckPlayerEQDealer() const {
    return current_player == player_dealer;
}

void GameGround::TakeBet(int bet) {
    bets[current_number - 1] = bet;
}

void GameGround::Listen(const Event & event) {
    switch (event.Request) {
        case Event::PlayerRequests::HIT :
            dealer->GiveCard();
            break;
        case Event::PlayerRequests::BET :
            dealer->TakeBet(event.GetData<int>());
            break;
        case Event::PlayerRequests::STAND :
            dealer->SwapPlayer();
            break;
        case Event::PlayerRequests::DOUBLEDOWN :
            dealer->GiveDoubleDown();
            break;
        default :
            std::cout << "YOU CANT DO IT" << std::endl;
    }
}

