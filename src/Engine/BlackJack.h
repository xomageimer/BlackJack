#ifndef BLACKJACK_BLACKJACK_H
#define BLACKJACK_BLACKJACK_H

// TODO функция которая будет вечно ожидать ввод для обработки в контроллер
#include "Actors/GameGround.h"

struct BlackJack {
public:
    static BlackJack & Instance(){
        static BlackJack jack_black{};
        return jack_black;
    }

    template <typename T>
    void SetDealer(std::shared_ptr<T> concrete_dealer){
        _grounds->SubscribeDealer(concrete_dealer);
    }

    bool PlayerConnect(const std::string & nick_name, std::shared_ptr<Actors::IPlayer> player);
    bool PlayerDisconnect(const std::string & nick_name);

    void Run(std::istream& input, std::ostream& output);

private:
    BlackJack();
    std::shared_ptr<GameGround> _grounds;

    std::map<std::string, Event::PlayerRequests> commands{
            {"BET", Event::PlayerRequests::BET},
            {"HIT", Event::PlayerRequests::HIT},
            {"STAND", Event::PlayerRequests::STAND},
            {"DOUBLEDOWN", Event::PlayerRequests::DOUBLEDOWN}
    };
};


#endif //BLACKJACK_BLACKJACK_H
