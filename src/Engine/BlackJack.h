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

    void Run(std::ostream& output);

private:
    BlackJack();
    std::shared_ptr<GameGround> _grounds;
};

struct TestBlackJack {
public:
    TestBlackJack();

    template <typename T>
    void SetDealer(std::shared_ptr<T> concrete_dealer){
        _grounds->SubscribeDealer(concrete_dealer);
    }

    bool PlayerConnect(const std::string & nick_name, std::shared_ptr<Actors::IPlayer> player);
    bool PlayerDisconnect(const std::string & nick_name);

    void Run(std::ostream& output);

    void Run(std::ostream& output, size_t command_count); // For tests

private:
    std::shared_ptr<GameGround> _grounds;
};


#endif //BLACKJACK_BLACKJACK_H
