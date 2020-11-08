#include "Controller.h"

#include <utility>

// TODO тут не должно быть рекурсии, он должен вызывать определенные сеттеры у плеера и диллера

// TODO из актора мы задаем хендлер, кт должен задавать event

// TODO controller -> dealer -> handler
//  return value from dealer_handler -> controller -> player -> handler (конец итерации цикла)
//  -> controller

// TODO HIT у плеера - сетает ему карту из Event
//  а GiveCards у диллера возвращает карту в виде Event

// реквесты - запросы, остальное выполнение ответки от диллера

void RelationshipController::HandleEvent(const Event &event) {
   switch (event.type){
       case Event::Type::BET:
           RequestBet(event);
           break;
       case Event::Type::MAKEBET:
           MakeBet(event);
           break;
       case Event::Type::WARN :
           OutWarn(event);
           break;
       case Event::Type::HIT :
           RequestCard();
           break;
       case Event::Type::STAND :
           RequestStop();
           break;
       case Event::Type::DOUBLEDOWN :
           RequestDoubleCard();
           break;
       case Event::Type::LOSE :
       case Event::Type::WIN :
           SetResult(event);
           Output();
           break;
       case Event::Type::GIVECARD :
           GiveCards(event);
           Output();
           break;
       case Event::Type::SWAPPLAYER :
           ChangePlayer(event);
           Output();
           break;
       case Event::Type::PLAYOUT :
           Result(); // дальше играет диллер, он должен стать плеером, выполнить свой скрипт, затем стать снова диллером и подвести итоги
           Output();
           break;
       case Event::Type::NEWROUND :
           GiveCards(event);
           GiveCards(event);
           Output();
           break;
       default:
           Output();
   }
}

void RelationshipController::SubscribePlayer(const std::string &player_nickname, std::shared_ptr<Actors::IActor> new_player) {
//    players.emplace(std::piecewise_construct_t(), std::forward_as_tuple(player_nickname), std::forward_as_tuple(new_player));
}

void RelationshipController::SubscribeDealer(std::shared_ptr<Actors::IDealer> new_dealer) {
    dealer = std::move(new_dealer);
}

void RelationshipController::UnSubscribePlayer(const std::string &player_nickname) {
    players.erase(player_nickname);
}

void RelationshipController::RequestCard() {

}

void RelationshipController::RestartGame(const Event &) {
    dealer->set_current(std::make_shared<DealerHandler::BetableHandler>());

    current_player = player_dealer;

    current_player = players.begin()->second.first;
    current_bet = players.begin()->second.second;
}


