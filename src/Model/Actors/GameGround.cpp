#include "GameGround.h"

#include <algorithm>
#include <utility>

bool GameGround::SubscribePlayer(std::string player_nickname, std::shared_ptr<Actors::IPlayer> new_player) {
    // TODO можно проверять хватает ли у плеера фишек
    if (queue.size() < MAX_PLAYER_COUNT) {
        queue.emplace_back(player_nickname);
        bets.emplace_back(0);
        players.emplace(std::piecewise_construct, std::forward_as_tuple(player_nickname),
                        std::forward_as_tuple(new_player));
        if (queue.size() == 1){
            current_player = players.at(queue.at(0));
        }
        return true;
    } else if (!vacancy.empty()) {
        queue[vacancy.back()] = player_nickname;
        bets[vacancy.back()] = 0.f;
        vacancy.pop_back();
        players.emplace(std::piecewise_construct, std::forward_as_tuple(player_nickname),
                        std::forward_as_tuple(new_player));
        return true;
    }
    return false;
}

// TODO переделать для всех логгеров
void GameGround::SetViewManager(std::shared_ptr<ILogger> man) {
    om->subscribe("View", std::move(man));
}

bool GameGround::UnSubscribePlayer(const std::string &player_nickname) {
    if (AFK_players.find(player_nickname) == AFK_players.end())
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

void GameGround::Output() {
    om->drop();
}

void GameGround::Destroy() {
    om->destroy();
}

GameGround::GameGround(std::shared_ptr<OutputManager> output) : om(std::move(output)) {}

// сначала change потом действие
void GameGround::ChangePlayer() {
    if (!players.empty()) {
        if (current_number != queue.size()) {
            while (AFK_players.count(queue.at(current_number))) {
                ++current_number;
            }
        }
        int current_bet;
        current_player = (current_number == queue.size()) ? (current_bet = 0, player_dealer)
                                                          : (current_bet = bets[current_number], players.at(
                        queue.at(current_number++)));
        dealer->SetPlayer(current_player.get(), current_bet);
    } else {
        current_player = player_dealer;
        dealer->SetPlayer(current_player.get(), 0);
        dealer->ExtraEnd();
    }
}

void GameGround::Reset() {
    current_number = 0;
}

void GameGround::Display(const Event & event) {
    switch (event.Response) {
        case Event::DealerResponse::WARN :
            om->notify(event.GetData<std::string>());
            Output();
            break;
        case Event::DealerResponse::MAKEBET :
            om->notify(std::string("Player " + std::to_string(current_number) + " Made a Bet: " +
                                   std::to_string(event.GetData<int>())));
            Output();
            break;
        case Event::DealerResponse::GIVECARD :
            if (CheckPlayerEQDealer()) {
                om->notify(std::string("Dealer Take a Card: "));
            } else {
                om->notify(std::string("Player " + std::to_string(current_number) + " Take a Card: "));
            }
            om->notify(event.GetData<GameCard::Cards>());
            Output();
            break;
        case Event::DealerResponse::DOUBLEDOWN :
            om->notify(std::string("Player " + std::to_string(current_number) + " Doubled the Bet  : " +
                                   std::to_string(bets[current_number - 1]) + " and Take Card: "));
            om->notify(event.GetData<GameCard::Cards>());
            Output();
            break;
        case Event::DealerResponse::SWAPPLAYER :
            om->notify(std::string("Change Player"));
            Output();
            break;
        case Event::DealerResponse::STATE :
            om->notify(std::string("Change State"));
            Output();
            break;
        case Event::DealerResponse::LOSE : {
            // вот так вот
            auto tmp = players.extract(queue.at(current_number - 1));
            AFK_players.insert(std::move(tmp));
        }
        case Event::DealerResponse::WIN :
        case Event::DealerResponse::DRAW :
            om->notify(event.GetData<std::string>());
            player_dealer->GetRoundResult((-1) * bets[current_number - 1]);
            current_player->GetRoundResult(bets[current_number - 1]);
            current_player->ClearHand();
            Output();
            break;
        case Event::DealerResponse::RESTART : {
            player_dealer->ClearHand();
            current_number = 0;
            while (!AFK_players.empty()) {
                auto tmp = AFK_players.extract(queue.at(current_number++));
                players.insert(std::move(tmp));
            }
            om->notify(std::string("New Round: "));
            Output();
            break;
        }
        default:
            Destroy();
            break;
    }
    // свитчом бегать по евентам и выплевывать в om вывод
    // Если это специфичный евент можно вывести сразу, а можно накопить его и вывести потом
}

bool GameGround::CheckPlayerEQDealer() const {
    return current_player == player_dealer;
}

void GameGround::TakeBet(int bet) {
    bets[current_number - 1] = bet;
}

void GameGround::Listen(const Event & event) {
    switch (event.Request) {
        case Event::PlayerRequests::HIT :
            dealer->GiveCard();
            break;
        case Event::PlayerRequests::BET :
            dealer->TakeBet(event.GetData<int>());
            break;
        case Event::PlayerRequests::STAND :
            dealer->SwapPlayer();
            break;
        case Event::PlayerRequests::DOUBLEDOWN :
            dealer->GiveDoubleDown();
            break;
        default :
            std::cout << "YOU CANT DO IT" << std::endl;
    }
}

