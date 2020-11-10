#ifndef BLACKJACK_BLACKJACK_H
#define BLACKJACK_BLACKJACK_H

// TODO функция которая будет вечно ожидать ввод для обработки в контроллер

#include <Actors/IHandler.h>
#include "Actors/Dealer.h"
#include "Actors/IActor.h"
#include "Controller.h"
#include "Cards/CardStack.h"
#include "Actors/Events.h"
#include "OutputManager.h"

struct BlackJack {
public:
    static BlackJack & Instance(){
        static BlackJack jack_black{};
        return jack_black;
    }

    void SetController(std::shared_ptr<RelationshipController> contr);

    template <typename T>
    void SetDealer(std::shared_ptr<T> concrete_dealer){
        game_controller->SubscribeDealer(concrete_dealer);
        game_controller->SubscribeHandlers(DealerHandler::DealerLogic::PLAYABLE, std::make_shared<DealerHandler::PlayableHandler>());
        game_controller->SubscribeHandlers(DealerHandler::DealerLogic::BETABLE, std::make_shared<DealerHandler::BetableHandler>());
        game_controller->SubscribeHandlers(DealerHandler::DealerLogic::DEALERABLE, std::make_shared<DealerHandler::DealerableHandler>());
        game_controller->SubscribeHandlers(DealerHandler::DealerLogic::DISTRIB, std::make_shared<DealerHandler::DistributionHandler>());
    }

    bool PlayerConnect(const std::string & nick_name, std::shared_ptr<Actors::IActor> player);
    bool PlayerDisconnect(const std::string & nick_name);

    void Run(std::istream& input, std::ostream& output);

private:
    BlackJack() = default;
    std::shared_ptr<RelationshipController> game_controller;

    std::map<std::string, Event::Type> commands{
            {"BET", Event::Type::BET},
            {"HIT", Event::Type::HIT},
            {"STAND", Event::Type::STAND},
            {"DOUBLEDOWN", Event::Type::DOUBLEDOWN}
    };
};


#endif //BLACKJACK_BLACKJACK_H
