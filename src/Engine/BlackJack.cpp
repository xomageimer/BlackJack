#include <iostream>
#include <utility>

#include "BlackJack.h"

bool BlackJack::PlayerConnect(const std::string &nick_name, std::shared_ptr<Actors::IPlayer> player) {
    return _grounds->SubscribePlayer(nick_name, std::move(player));
}

bool BlackJack::PlayerDisconnect(const std::string &nick_name) {
    return _grounds->UnSubscribePlayer(nick_name);
}

void BlackJack::Run(std::ostream &output) {
    _grounds->SetViewManager(std::make_shared<ConsoleLogger>(output));
    _grounds->Execute();
}

BlackJack::BlackJack() {
    _grounds = std::make_shared<GameGround>(std::make_shared<OutputManager>());
}

void BlackJack::Run(std::ostream &output, size_t command_count) {
    _grounds->SetViewManager(std::make_shared<ConsoleLogger>(output));
    _grounds->Execute(command_count);
    Reset();
}

void BlackJack::Reset() {
    std::shared_ptr<GameGround> new_ground (new GameGround(std::make_shared<OutputManager>()));
    _grounds.swap(new_ground);
}
