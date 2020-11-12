#include <iostream>
#include <utility>

#include "BlackJack.h"


bool BlackJack::PlayerConnect(const std::string &nick_name, std::shared_ptr<Actors::IActor> player) {
    return _grounds->SubscribePlayer(nick_name, std::move(player));
}

bool BlackJack::PlayerDisconnect(const std::string &nick_name) {
    return _grounds->UnSubscribePlayer(nick_name);
}

void BlackJack::Run(std::istream &input, std::ostream &output) {
    _grounds->SetViewManager(std::make_shared<ConsoleLogger>(output));

    std::string word;
    double bet = 0;
    while (true){
        input >> word;
        if (commands.count(word)){
            if (commands[word] == Event::Type::BET){
                input >> bet;
            }
            Event request(commands[word], bet);
            _grounds->GetState()->HandleEvent(request);
        }else if (word == "EXIT")
            break;
        else{
            output << "WRONG INPUT\n";
            input.clear();
            input.ignore(256, '\n');
        }
    }
}
