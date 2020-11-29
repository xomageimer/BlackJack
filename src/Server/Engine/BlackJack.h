#ifndef BLACKJACK_BLACKJACK_H
#define BLACKJACK_BLACKJACK_H

#undef BOOST_ASIO_ENABLE_HANDLER_TRACKING

#include "Server/Controller/Game_Server.h"

struct BlackJack {
public:
    static BlackJack & Instance(){
        static BlackJack jack_black{};
        return jack_black;
    }

    template <typename T>
    void SetDealer(std::shared_ptr<T> concrete_dealer){
       SubscribeDealer(concrete_dealer);
    }

    template <typename T>
    inline constexpr std::enable_if_t<Controller::Is_Inherited_v<T, Controller::IDealer> && Controller::Is_Inherited_v<T, Actors::IPlayer>, void> SubscribeDealer(std::shared_ptr<T> new_dealer){
        dealer = new_dealer;
        dealer->SetPlayerDealer(new_dealer);
    }
    template <typename T>
    inline constexpr std::enable_if_t<!Controller::Is_Inherited_v<T, Controller::IDealer> && !Controller::Is_Inherited_v<T, Actors::IPlayer>, void> SubscribeDealer([[maybe_unused]] std::shared_ptr<T> new_dealer){
        throw std::logic_error("\nERROR; FROM SUBSCRIBE DEALER METHOD; FILE: " + std::string(__FILE__) + "; ON LINE: " + std::to_string(__LINE__ - 1) + ";\nARGUMENT IS NOT INHERITED FROM IDealer AND/OR IActor;");
    }

    void Run(int argc, char* argv[]);

private:
    BlackJack() = default;
    std::shared_ptr<Controller::IDealer> dealer;
};


#endif //BLACKJACK_BLACKJACK_H
