#include "GameGround.h"

#include <algorithm>
#include <utility>

bool GameGround::SubscribePlayer(std::string player_nickname, std::shared_ptr<Actors::IPlayer> new_player) {
    // TODO можно проверять хватает ли у плеера фишек
    if (queue.size() < MAX_PLAYER_COUNT) {
        queue.emplace_back(player_nickname);
        players.emplace(std::piecewise_construct, std::forward_as_tuple(player_nickname),
                        std::forward_as_tuple(new_player));
        if (queue.size() == 1){
            current_player = players.at(queue.at(0));
        }
        return true;
    } else if (!vacancy.empty()) {
        queue[vacancy.back()] = player_nickname;
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

void GameGround::Execute() {
    dealer->SetView(om);
    for (auto & i : players){
        dealer->SetPlayer(i.second);
    }

    while (true){
        dealer->Process();
    }
}
