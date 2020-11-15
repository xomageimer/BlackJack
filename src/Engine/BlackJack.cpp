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
