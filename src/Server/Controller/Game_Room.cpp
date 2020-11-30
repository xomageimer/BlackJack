#include "Game_Room.h"

#include "Server/Controller/Dealer.h"

bool Game_Room::SubscribePlayer(std::string player_nickname, std::shared_ptr<Actors::IPlayer> new_player) {
    if (queue.size() < MAX_PLAYER_COUNT) {
        new_player->SetName(player_nickname);
        queue.emplace_back(player_nickname);
        players.emplace(std::piecewise_construct, std::forward_as_tuple(player_nickname),
                        std::forward_as_tuple(new_player));
        if (queue.size() == 1) {
            dealer->RestartDealer();
        }
     // сказать всем что плеер подключился
        return true;
    }
    return false;
}

bool Game_Room::UnSubscribePlayer(const std::string &player_nickname) {
    dealer->RefreshPlayer(players[player_nickname]);
    // сказать всем что плеер ливнул
    players.erase(player_nickname);
    return true;
}

void Game_Room::SetDealer(std::shared_ptr<Controller::IDealer> n_dealer) {
    dealer = std::move(n_dealer);
}

void Game_Room::NewRound() {
    recent_msgs_.clear();
    for (auto & player : players){
        dealer->SetPlayer(player.second);
    }
}
