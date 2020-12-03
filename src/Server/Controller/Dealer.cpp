#include "Dealer.h"
#include "IHandler.h"
#include "Game_Server.h"

#define MIN 10
#define MAX 100

Controller::IDealer::IDealer() {
     cmd_handles.emplace(std::piecewise_construct, std::forward_as_tuple(states::BET_SERVANT),
                        std::forward_as_tuple(std::make_shared<DealerHandlers::BetHandler>()));
    cmd_handles.emplace(std::piecewise_construct, std::forward_as_tuple(states::MOVE_SERVANT),
                        std::forward_as_tuple(std::make_shared<DealerHandlers::MoveHandler>()));
    cmd_handles.emplace(std::piecewise_construct, std::forward_as_tuple(states::PLAYOUT_SERVANT),
                        std::forward_as_tuple(std::make_shared<DealerHandlers::PlayoutHandler>()));
    cmd_handles.emplace(std::piecewise_construct, std::forward_as_tuple(states::ROUND_SERVANT),
                        std::forward_as_tuple(std::make_shared<DealerHandlers::RoundHandler>()));
    cmd_handles.emplace(std::piecewise_construct, std::forward_as_tuple(states::YOURSELF_SERVANT),
                        std::forward_as_tuple(std::make_shared<DealerHandlers::PlayingHandler>()));
    cmd_handles.emplace(std::piecewise_construct, std::forward_as_tuple(states::DEAL_SERVANT),
                        std::forward_as_tuple(std::make_shared<DealerHandlers::DealHandler>()));


    cur_state = states::BET_SERVANT;
    cur_handler = cmd_handles.at(cur_state);
}

void Controller::IDealer::SetPlayer(std::shared_ptr<Actors::IPlayer> new_player) {
    m_players.emplace_back(std::move(new_player), 0);
}

std::pair<std::shared_ptr<Actors::IPlayer>, int> & Controller::IDealer::getPlayer() {
    return m_players.at(cursor);
}

std::shared_ptr<Actors::IPlayer> Controller::IDealer::getDealerPlayer() {
    return player_dealer;
}

void Controller::IDealer::SetPlayerDealer(std::shared_ptr<Actors::IPlayer> del) {
    player_dealer = del;
}

void Controller::IDealer::SetRoom(Game_Room * room) {
    my_room = room;
}

void Controller::IDealer::RefreshPlayer(std::shared_ptr<Actors::IPlayer> pl) {
    auto it = std::find_if(m_players.begin(), m_players.end(), [&pl](auto player) {
        return player.first == pl;
    });

    if (it != m_players.end()){
        m_players.erase(it);
        if (cursor >= m_players.size()){
            if (cur_state == states::MOVE_SERVANT)
                set_current(states::ROUND_SERVANT);
            else if (cur_state == states::BET_SERVANT)
                set_current(states::MOVE_SERVANT);
            else if (cur_state == states::DEAL_SERVANT){
                CheckBJ();
            }
        } else
            Process();
    }
}

void Controller::IDealer::RestartDealer() {
    m_players.clear();
    my_room->NewRound();
    if (m_players.size() > 0) {
        set_current(states::BET_SERVANT);
    }
}

void Controller::IDealer::Notify_about_player(int num) {
    json event;
    event["command"] = "PlayerChanged";

    event["data"]["id"] = num;
    event["data"]["isDealer"] = false;
    for (size_t i = 0; i < m_players.at(num).first->ShowHand().GetSize(); i++) {
        std::string cardN = "card" + std::to_string(i);
        auto cur_card = m_players.at(num).first->ShowHand().LookAtCards()[i];
        json card_val;
        card_val["isOpen"] = !m_players.at(num).first->ShowHand().LookAtCards()[i].is_secret;
        if (GameCard::Cards::m_value.find(cur_card.price) != GameCard::Cards::m_value.end()) {
            card_val["rank"] = GameCard::Cards::m_value.at(cur_card.price);
        } else {
            card_val["rank"] = std::to_string(static_cast<int>(cur_card.price));
        }
        card_val["suit"] = GameCard::Cards::m_suit.at(cur_card.suit);
        event["data"]["hand"].push_back(card_val);
    }
    event["data"]["name"] = m_players.at(num).first->GetName();
    event["data"]["rate"] = m_players.at(num).second;
    event["data"]["bank"] = m_players.at(num).first->GetPlayerCost();
    auto ins = insurances.find(m_players.at(num).first);
    event["data"]["insurance"] = (ins != insurances.end()) && ins->second;

    my_room->deliver(event.dump());
}

void Controller::IDealer::Notify_about_dealer() {
    json event;
    event["command"] = "PlayerChanged";

    event["data"]["isDealer"] = true;
    for (size_t i = 0; i < getDealerPlayer()->ShowHand().GetSize(); i++) {
        std::string cardN = "card" + std::to_string(i);
        auto cur_card = getDealerPlayer()->ShowHand().LookAtCards()[i];
        json card_val;
        card_val["isOpen"] = !getDealerPlayer()->ShowHand().LookAtCards()[i].is_secret;
        if (card_val["isOpen"]) {
            if (GameCard::Cards::m_value.find(cur_card.price) != GameCard::Cards::m_value.end()) {
                card_val["rank"] = GameCard::Cards::m_value.at(cur_card.price);
            } else {
                card_val["rank"] = std::to_string(static_cast<int>(cur_card.price));
            }
            card_val["suit"] = GameCard::Cards::m_suit.at(cur_card.suit);
        }
        event["data"]["hand"].push_back(card_val);
    }

    my_room->deliver(event.dump());
}

void Controller::IDealer::CheckBJ() {
    getDealerPlayer()->ShowHand().UnSecret(1);
    auto is_blackJack = getDealerPlayer()->ShowHand().total();
    getDealerPlayer()->ShowHand().MakeSecret(1);
    if (is_blackJack == BLACKJACK) {
        set_current(states::YOURSELF_SERVANT);
    } else {
        size_t i = 0;
        for (auto &[player, bet] : m_players) {
            auto it = insurances.find(player);
            if (it != insurances.end() && it->second) {
                player->GetRoundResult((-1) * bet / 2);
                getDealerPlayer()->GetRoundResult(bet / 2);
            }
            i++;
        }
        insurances.clear();
        cursor = 0;
        set_current(states::MOVE_SERVANT);
    }
}

void Controller::SimpleDealer::ServeBet() {
    cur_handler->serveBet(this);
}

void Controller::SimpleDealer::ServeMove() {
    cur_handler->serveMove(this);
}

void Controller::SimpleDealer::ServeRound() {
    cur_handler->serveRound(this);
}

void Controller::SimpleDealer::ServePlayout() {
    cur_handler->servePlayout(this);
}

void Controller::SimpleDealer::ServeYourself() {
    cur_handler->serveYourself(this);
}

void Controller::SimpleDealer::Process() {
    switch (cur_state) {
        case IDealer::states::BET_SERVANT:
            ServeBet();
            break;
        case IDealer::states::DEAL_SERVANT:
        case IDealer::states::MOVE_SERVANT:
            ServeMove();
            break;
        case IDealer::states::ROUND_SERVANT:
            ServeRound();
            break;
        case IDealer::states::PLAYOUT_SERVANT:
            ServePlayout();
            break;
        case IDealer::states::YOURSELF_SERVANT:
            ServeYourself();
            break;
        default:
            ServeBet();
            break;
    }
}

void Controller::SimpleDealer::Move() {
    if (ShowHand().total() < DEALERBORDER) {
        Event take_card(Event::DealerResponse::GIVECARD, std::string("Dealer take a card: "));
    } else {
        Event stand(Event::DealerResponse::SWAPPLAYER, std::string("Enough"));
    }
}

void Controller::SimpleDealer::Bet() {
    Event bet(Event::DealerResponse::MAKEBET, 10);
}

void Controller::SimpleDealer::Answer() {
    Event answ(Event::DealerResponse::YES, 0);
}

void Controller::SimpleDealer::SetCard(const GameCard::Cards & card) {
    m_hand.SetNewCard(card);
}

bool Controller::SimpleDealer::BlackJackCheck() const {
    return m_hand == BLACKJACK;
}

void Controller::SimpleDealer::GetRoundResult(int bet) {
    m_bank += bet;
}

void Controller::SimpleDealer::ClearHand() {
    m_hand.Clear();
}

const GameCard::Hand &Controller::SimpleDealer::ShowHand() const {
    return m_hand;
}

int Controller::SimpleDealer::GetPlayerCost() const {
    return m_bank;
}

void Controller::SimpleDealer::Maker(std::string json) {
    switch (cur_state) {
        case IDealer::states::BET_SERVANT:
            MakeBet(json);
            break;
        case IDealer::states::MOVE_SERVANT:
            MakeMove(json);
            break;
        case IDealer::states::DEAL_SERVANT:
            MakeDeal(json);
            break;
        default:
            break;
    }
}

void Controller::SimpleDealer::MakeBet(std::string json_str) {
    json response = json::parse(json_str);

    if (response["command"] == "OK" && response.size() > 1) {
        if ((response["data"]["bet"] >= MIN) && (response["data"]["bet"] <= MAX) &&
            (getPlayer().first->GetPlayerCost() >= response["data"]["bet"])) {
            getPlayer().second = response["data"]["bet"];

            if (++cursor == m_players.size())
                set_current(states::ROUND_SERVANT);
            else
                Process();

        }
    } else if (response["command"] != "OK" && cur_state == states::BET_SERVANT) {
        Process();
    }
}

void Controller::SimpleDealer::MakeMove(std::string json_str) {
    json answer;
    json response = json::parse(json_str);

    if (response["command"] == "OK" && response.size() > 1) {
        if (response["data"]["action"] == "Hit") {
            auto card = m_stack->GetCard();
            getPlayer().first->SetCard(card);

            Notify_about_player(cursor);

            if (getPlayer().first->ShowHand().total() > BLACKJACK) {
                getPlayer().first->GetRoundResult((-1) * getPlayer().second);
                getDealerPlayer()->GetRoundResult(getPlayer().second);

                if (++cursor == m_players.size())
                    set_current(states::YOURSELF_SERVANT);
            }
        } else if (response["data"]["action"] == "Stand") {
            if (++cursor == m_players.size())
                set_current(states::YOURSELF_SERVANT);
        } else if (response["data"]["action"] == "Double") {
            if (getPlayer().first->ShowHand().GetSize() == 2 &&
                getPlayer().first->GetPlayerCost() > getPlayer().second) {

                auto card = m_stack->GetCard();
                getPlayer().first->SetCard(card);

                getPlayer().second *= 2;

                Notify_about_player(cursor);

                if (++cursor == m_players.size())
                    set_current(states::YOURSELF_SERVANT);
            }
        }
        if (cur_state == states::MOVE_SERVANT)
            Process();
    }
}


void Controller::SimpleDealer::MakeDeal(std::string json_str) {
    json answer;
    json response = json::parse(json_str);


    if (response["command"] == "OK" && response.size() > 1) {
        if (getPlayer().first->GetPlayerCost() >= getPlayer().second + getPlayer().second / 2)
            insurances[getPlayer().first] = response["data"]["insurance"];

        if (++cursor == m_players.size()) {
            CheckBJ();
        } else
            Process();
    }
}
