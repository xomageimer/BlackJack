#include "GameGround.h"

#include <algorithm>

GameGround::GameGround(std::shared_ptr<OutputManager> output) : om(std::move(output)) {
    controllerrs.emplace(std::piecewise_construct, std::forward_as_tuple(IController::DealerLogic::BETABLE),
                         std::forward_as_tuple(std::make_shared<BetableController>(this)));
    controllerrs.emplace(std::piecewise_construct, std::forward_as_tuple(IController::DealerLogic::DEALERABLE),
                         std::forward_as_tuple(std::make_shared<DealerableController>(this)));
    controllerrs.emplace(std::piecewise_construct, std::forward_as_tuple(IController::DealerLogic::DISTRIB),
                         std::forward_as_tuple(std::make_shared<DistributionController>(this)));
    controllerrs.emplace(std::piecewise_construct, std::forward_as_tuple(IController::DealerLogic::PLAYABLE),
                         std::forward_as_tuple(std::make_shared<PlayeableController>(this)));

    cur_controller = controllerrs.at(IController::DealerLogic::BETABLE);
}

bool GameGround::SubscribePlayer(std::string player_nickname, std::shared_ptr<Actors::IActor> new_player) {
    // TODO можно проверять хватает ли у плеера фишек
    if (queue.size() < MAX_PLAYER_COUNT) {
        queue.emplace_back(player_nickname);
        bets.emplace_back(0.f);
        players.emplace(std::piecewise_construct, std::forward_as_tuple(player_nickname),
                        std::forward_as_tuple(new_player));
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
    om->subscribe("Console_Logger", std::move(man));
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

void GameGround::IssuingCards() {
    dealer->GiveCard();
}

void GameGround::IssuingStop() {
    dealer->SwapPlayer();
}

void GameGround::IssuingBet(const Event & event) {
    dealer->TakeBet(event.GetData<int>());
}

void GameGround::MakeBet(const Event & event) {
    bets[current_number] = event.GetData<int>();
    om->notify("Player " + std::to_string(current_number) + " made a bet: " + std::to_string(event.GetData<int>()));
}

void GameGround::OutWarn(const Event & event) {
    om->notify(event.GetData<std::string>());
}

void GameGround::Output() {
    om->drop();
}

void GameGround::ChangePlayer(const Event & event) {

}

void GameGround::SetState(const IController::DealerLogic & logic) {
    cur_controller = controllerrs.at(logic);
    dealer->SetController(controllerrs.at(logic));
}

std::shared_ptr<IController> GameGround::GetState() {
    return cur_controller;
}
