#include "Controller.h"

#include <utility>
#include <algorithm>
#include <Actors/IHandler.h>

// TODO тут не должно быть рекурсии, он должен вызывать определенные сеттеры у плеера и диллера

// TODO из актора мы задаем хендлер, кт должен задавать event

// TODO controller -> dealer -> handler
//  return value from dealer_handler -> controller -> player -> handler (конец итерации цикла)
//  -> controller

// TODO HIT у плеера - сетает ему карту из Event
//  а GiveCards у диллера возвращает карту в виде Event

// реквесты - запросы, остальное выполнение ответки от диллера

// TODO ввод должен давать плееру время на решение иначе делать пропуск за него (если игрок пропускает больше 2 раундов подряд, его кикает)

const size_t MAX_PLAYER_COUNT = 7;


void RelationshipController::HandleEvent(const Event &event) {
   switch (event.type){
       case Event::Type::BET:
           RequestBet(event);
           break;
       case Event::Type::MAKEBET:
           MakeBet(event);
           Output();
           break;
       case Event::Type::WARN :
           OutWarn(event);
           Output();
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

bool RelationshipController::SubscribePlayer(std::string player_nickname, std::shared_ptr<Actors::IActor> new_player) {
    // TODO можно проверять хватает ли у плеера фишек
    if (queue.size() < MAX_PLAYER_COUNT) {
        queue.emplace_back(player_nickname);
        players.emplace(std::piecewise_construct, std::forward_as_tuple(player_nickname),
                        std::forward_as_tuple(new_player, new_player->GetPlayerCost()));
        return true;
    } else if (!vacancy.empty()) {
        queue[vacancy.back()] = player_nickname;
        vacancy.pop_back();
        players.emplace(std::piecewise_construct, std::forward_as_tuple(player_nickname),
                        std::forward_as_tuple(new_player, new_player->GetPlayerCost()));
        return true;
    }
    return false;
}

bool RelationshipController::UnSubscribePlayer(const std::string &player_nickname) {
    if (players.find(player_nickname) == players.end())
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

void RelationshipController::RequestCard() {
    dealer->GiveCard();
}

void RelationshipController::RequestStop() {
    dealer->SwapPlayer();
}

void RelationshipController::RequestBet(const Event & event) {
    dealer->TakeBet(event.GetData<double>());
}

void RelationshipController::RequestDoubleCard() {
    dealer->GiveDoubleDown();
}

// TODO научить диллера менять хендлер
// TODO можно добавить состояние New_State кт будет давать следующее состояние
// TODO ПОЧЕМУ-ТО НЕТ В ДИЛЕРАБЛ И ПЛЕЕБЛ КОНТРОЛЛЕРА

void RelationshipController::RestartGame() {
    current_logic = DealerHandler::DealerLogic::BETABLE;
    dealer->SetHandler(Control_Logic.at(current_logic));
    current_number = 0;
    dealer->SwapPlayer();
}

void RelationshipController::GiveCards(const Event & event) {
    current_player->Hit(event.GetData<GameCard::Cards>());
    om->notify("Player took a card: ");
    om->notify(event.GetData<GameCard::Cards>());
}

void RelationshipController::OutWarn(const Event & event) {
    // для логов
    om->notify(event.GetData<std::string>());
}

void RelationshipController::MakeBet(const Event & event) {
    // тут можно интегрировать бд
    *current_bet = event.GetData<double>();
    om->notify("Player made a bet: " + std::to_string(event.GetData<double>()));
}

void RelationshipController::ChangePlayer(const Event & event) {
    auto cur = (current_number == queue.size()) ? std::pair(player_dealer, double(0.f)) : players.at(queue[current_number++]);
    current_player = cur.first;
    current_bet = &cur.second;
    if (current_player == player_dealer){
        current_number = 0;
        NextHandler();
    } else {
        om->notify(event.GetData<std::string>());
        om->notify("Player number " + std::to_string(current_number) + "'s turn");
    }
}

void RelationshipController::Result() {
    current_player = player_dealer;
    RequestCard();
    if (current_number != queue.size()) {
        dealer->PlayOut();
    } else {
        dealer->TimeToShuffle();
        RestartGame();
    }
}

void RelationshipController::SetResult(const Event & event) {
    if (event.type == Event::Type::WIN) {
        current_player->GetResult(event.GetData<double>());
        om->notify("Player won: " + std::to_string(event.GetData<double>()));
    } else if (event.type == Event::Type::LOSE){
        current_player->GetResult((-1) * event.GetData<double>());
        om->notify("Player lost: " + std::to_string(event.GetData<double>()));
    } else if (event.type == Event::Type::DRAW){
        current_player->GetResult(event.GetData<double>());
        om->notify("Draw, Player get back: " + std::to_string(event.GetData<double>()));
    }
}

void RelationshipController::Output() {
    // показывать все карты
    if (current_logic != DealerHandler::DealerLogic::BETABLE) {
        size_t number = 1;
        std::string message = "";
        for (auto &i : players) {
            message += std::to_string(number++) + "'st Player hand: ";
            for (GameCard::Cards j : i.second.first->ShowHand().LookAtCards()) {
                message += j.operator std::string() + ' ';
            }
            om->notify(message);
            message.clear();
        }
    }
    om->drop();
}

void RelationshipController::SetViewManager(std::shared_ptr<ILogger> man) {
    om->subscribe("Console_Logger", man);
}

RelationshipController::RelationshipController() : om(std::make_shared<OutputManager>()) {}

void RelationshipController::SubscribeHandlers(const DealerHandler::DealerLogic &logic,
                                               std::shared_ptr<DealerHandler::IHandler> handler) {
    auto it = Control_Logic.emplace(std::piecewise_construct, std::forward_as_tuple(logic), std::forward_as_tuple(handler));
    it.first->second->SetController(std::shared_ptr<RelationshipController>(this));
}

void RelationshipController::NextHandler() {
    switch (current_logic) {
        case DealerHandler::DealerLogic::BETABLE :
            current_logic = DealerHandler::DealerLogic::DEALERABLE;
      //      dealer->SwapPlayer();
       //     dealer->NewRound();
            break;
        case DealerHandler::DealerLogic::DEALERABLE :
            current_logic = DealerHandler::DealerLogic::PLAYABLE;
            break;
        case DealerHandler::DealerLogic::PLAYABLE :
            current_logic = DealerHandler::DealerLogic::BETABLE;
            break;
        default:
            current_logic = DealerHandler::DealerLogic::BETABLE;
            break;
    }
    dealer->SetHandler(Control_Logic.at(current_logic));
}



