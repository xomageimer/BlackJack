#include "Controller.h"

#include <utility>
#include <algorithm>

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

void RelationshipController::RestartGame() {
    current_number = 0;
    current_player = player_dealer;
    dealer->NewRound();
    for (auto &current : players) {
        current_player = current.second.first;
        current_bet = &current.second.second;
        dealer->NewRound();
    }
}

void RelationshipController::RequestStop() {
    dealer->SwapPlayer();
}

void RelationshipController::GiveCards(const Event & event) {
    current_player->Hit(event.GetData<GameCard::Cards>());
    om->notify("Player took a card: ");
    om->notify(event.GetData<GameCard::Cards>());
}

void RelationshipController::RequestBet(const Event & event) {
    dealer->TakeBet(event.GetData<double>());
}

void RelationshipController::RequestDoubleCard() {
    dealer->GiveDoubleDown();
}

void RelationshipController::OutWarn(const Event & event) {
    // для логов
    om->notify(event.GetData<std::string>());
}

void RelationshipController::MakeBet(const Event & event) {
    // тут можно интегрировать бд
    players.at(queue[current_number]).second = event.GetData<double>();
    om->notify("Player made a bet: " + std::to_string(event.GetData<double>()));
}

void RelationshipController::ChangePlayer(const Event & event) {
    auto cur = (current_number == queue.size()) ? players.at(queue[0]) : players.at(queue[current_number++]);
    current_player = cur.first;
    current_bet = &cur.second;
    om->notify(event.GetData<std::string>());
    om->notify("Player number " +  std::to_string(current_number - 1) + "'s turn");
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
    om->drop();
}

void RelationshipController::SetViewManager(std::shared_ptr<ILogger> man) {
    om->subscribe("Console_Logger", man);
}

RelationshipController::RelationshipController() : om(std::make_shared<OutputManager>()) {}



