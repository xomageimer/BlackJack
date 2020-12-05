#include "Game_Room.h"

#include <chrono>
#include <thread>

#include "Server/Controller/Dealer.h"

#define MIN 10

bool Game_Room::SubscribePlayer(std::string player_nickname, std::shared_ptr<Actors::IPlayer> new_player, int id) {
    new_player->SetName(player_nickname);
    new_player->setId(id);
    queue.emplace_back(player_nickname);
    players.emplace(std::piecewise_construct, std::forward_as_tuple(player_nickname),
                    std::forward_as_tuple(new_player));

    json answ;
    answ["command"] = "OK";
    answ["data"] = {{"Bank", 1000}, {"id", std::to_string(id)}, {"name", player_nickname}};
    deliver(answ.dump(), id);

    if (queue.size() >= 1) {
        dealer->RestartDealer();
    }

    return true;
}

bool Game_Room::UnSubscribePlayer(const std::string &player_nickname) {
    //dealer->RefreshPlayer(players[player_nickname]);
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
    using namespace std::chrono_literals;
    Notify_result();
    std::cerr << rounds << std::endl;
    if (rounds++ >= max_rounds){
        std::cerr << "\nend of rounds\n";
        exit(0);
    }
    recent_msgs_.clear();
    for (auto & player : players){
        if (player.second->GetPlayerCost() >= MIN){
            dealer->SetPlayer(player.second);
        }
    }
    for (auto it = players.begin(); it != players.end();){
        if (it->second->GetPlayerCost() >= 1'000'000){
            exit(0);
        }
        if (it->second->GetPlayerCost() < MIN) {
            Notify_result();
            std::this_thread::sleep_for(100ms);
            exit(0);
            //            auto tmp_it = ++it;
//            leave(participants_.find((--it)->second->GetId())->second);
//            it = tmp_it;
        } else {
            it++;
        }
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
        player["id"] = std::to_string(i);
        player["bank"] = players[queue[i]]->GetPlayerCost();
        j["data"]["Players"].push_back(player);
    }
    deliver(j.dump());
}
