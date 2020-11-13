#include "GameGround.h"

#include <algorithm>

bool GameGround::SubscribePlayer(std::string player_nickname, std::shared_ptr<Actors::IActor> new_player) {
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

//void GameGround::MakeBet(const Event & event) {
//    bets[current_number] = event.GetData<int>();
//    om->notify("Player " + std::to_string(current_number) + " made a bet: " + std::to_string(event.GetData<int>()));
//}
//
//void GameGround::OutWarn([[maybe_unused]]const Event & event) {
//    om->notify("You cant do it!");
//}

void GameGround::Output() {
    om->drop();
}

void GameGround::ChangePlayer(const Event & event) {
    current_number = (current_number == queue.size()) ? 0 : current_number + 1;
    current_player = players.at(queue.at(current_number));
    om->notify(event.GetData<std::string>() + " on " + std::to_string(current_number));
}

void GameGround::Destroy() {
    om->destroy();
}

//void GameGround::SetResult(const Event & event) {
//    if (event.type == Event::Type::WIN) {
//        current_player->GetResult(event.GetData<int>());
//        om->notify("Player won: " + std::to_string(event.GetData<int>()));
//    } else if (event.type == Event::Type::LOSE){
//        current_player->GetResult((-1) * event.GetData<int>());
//        om->notify("Player lost: " + std::to_string(event.GetData<int>()));
//
//        // TODO вырезать лузера из списка текущих игроков
//      //  queue.erase(current_number);
//     //   auto tmp = players.at(queue.at(current_number));
//        //AFK_players.emplace(
//    } else if (event.type == Event::Type::DRAW){
//        current_player->GetResult(event.GetData<int>());
//        om->notify("Draw, Player get back: " + std::to_string(event.GetData<int>()));
//    }
//}

//void GameGround::GiveCards(const Event & event) {
//    current_player->Hit(event.GetData<GameCard::Cards>());
//    om->notify("Player took a card: ");
//    om->notify(event.GetData<GameCard::Cards>());
//}

void GameGround::Result() {
    dealer->PlayOut();
}

GameGround::GameGround(std::shared_ptr<OutputManager> output) : om(output) {}