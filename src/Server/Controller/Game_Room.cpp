#include "Game_Room.h"

bool Game_Room::SubscribePlayer(std::string player_nickname, std::shared_ptr<Actors::IPlayer> new_player) {
    if (queue.size() < MAX_PLAYER_COUNT) {
        queue.emplace_back(player_nickname);
        players.emplace(std::piecewise_construct, std::forward_as_tuple(player_nickname),
                        std::forward_as_tuple(new_player));
        return true;
    }
    return false;
}

bool Game_Room::UnSubscribePlayer(const std::string &player_nickname) {
    players.erase(player_nickname);
    return true;
}
