#include "Game_Room.h"

#include "Server/Controller/Dealer.h"

bool Game_Room::SubscribePlayer(std::string player_nickname, std::shared_ptr<Actors::IPlayer> new_player, int id) {
    new_player->SetName(player_nickname);
    queue.emplace_back(player_nickname);
    players.emplace(std::piecewise_construct, std::forward_as_tuple(player_nickname),
                    std::forward_as_tuple(new_player));

    json answ;
    answ["command"] = "OK";
    answ["data"] = {{"Bank", 1000}, {"id", std::to_string(id)}, {"name", player_nickname}};
    deliver(answ.dump(), id);

    if (queue.size() == 1) {
        dealer->RestartDealer();
    }

    return true;
}

bool Game_Room::UnSubscribePlayer(const std::string &player_nickname) {
    dealer->RefreshPlayer(players[player_nickname]);
    // сказать всем что плеер ливнул
    players.erase(player_nickname);
    int i = -1;
    queue.erase(std::find_if(queue.begin(), queue.end(), [&i, &player_nickname](auto player_name) {
        i++;
        return player_name == player_nickname;
    }));
    vacancy.push_back(i);
    return true;
}

void Game_Room::SetDealer(std::shared_ptr<Controller::IDealer> n_dealer) {
    dealer = std::move(n_dealer);
}

void Game_Room::NewRound() {
    Notify_result();
    recent_msgs_.clear();
    for (auto & player : players){
        dealer->SetPlayer(player.second);
    }
    std::cerr << players.size() << '\n';
    //dealer->Process();
}

void Game_Room::Notify_result() {
    json j;
    j["command"] = "PlayerList";
    for (size_t i = 0; i < players.size(); i++) {
        json player;
        player["name"] = queue[i];
        player["id"] = i;
        player["bank"] = players[queue[i]]->GetPlayerCost();
        j["data"]["Players"].push_back(player);
    }
    deliver(j.dump());
}
