#include "Game_Room.h"

#include <chrono>
#include <thread>

#include "Server/Controller/Dealer.h"

#define MIN 10

bool Game_Room::SubscribePlayer(std::string player_nickname, std::shared_ptr<Actors::IPlayer> new_player, int id) {
    new_player->SetName(player_nickname);
    new_player->setId(id);
    queue.emplace_back(player_nickname);
    players.emplace(std::piecewise_construct, std::forward_as_tuple(id),
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

bool Game_Room::UnSubscribePlayer(int player_id) {
    if (players.find(player_id) != players.end()) {
        std::cerr << "\nPlayer " << player_id << " left the game" << std::endl;
        queue.erase(std::remove(queue.begin(), queue.end(), players.at(player_id)->GetName()), queue.end());
        bool restart = dealer->FindPlayer(players.at(player_id));
        players.erase(player_id);
        vacancy.push_back(player_id);
        if (restart) dealer->RestartDealer();
        return true;
    }
    return false;
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
            leave(participants_.find((--it)->second->GetId())->second);
        } else {
            it++;
        }
    }
    std::cerr << players.size() << '\n';
}

void Game_Room::Notify_result() {
    json j;
    j["command"] = "PlayerList";
    for (auto & player : players) {
        json pl;
        pl["name"] = player.second->GetName();
        pl["id"] = std::to_string(player.second->GetId());
        pl["bank"] = player.second->GetPlayerCost();
        j["data"]["Players"].push_back(pl);
    }
    deliver(j.dump());
}
