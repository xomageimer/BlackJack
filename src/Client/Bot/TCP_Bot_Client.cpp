#include "TCP_Bot_Client.h"

#define MAX 100
#define MIN 10

using json = nlohmann::json;

using p_EVENT = Event::PlayerRequests;

void TCP_Bot_Client::Request(std::string str) {
    if(!str.empty() && !nickname.empty()) {
        json request = json::parse(str);

        if (request["command"] == "PlayerList") {
            UpdateLeftCards();
            om->notify_PlayerList(request);
            for (auto & player : request["data"]["Players"]){
                if (std::stoi(player["id"].get<std::string>()) == my_id){
                    m_bank = player["bank"];
                }
            }
            json j;
            j["command"] = "OK";
            write(j.dump());
        } else if (request["command"] == "Bet") {
            om->notify_Bet(request);
            cur_state = states::BET;
        } else if (request["command"] == "Insurance") {
            om->notify_Insurance(request);
            cur_state = states::DEAL;
        } else if (request["command"] == "PlayerChanged") {
            Collect(request["data"]);
            om->notify_PlayerChanged(request);
            json j;
            j["command"] = "OK";
            write(j.dump());
        } else if (request["command"] == "RequestAction") {
            om->notify_RequestAction(request);
            cur_state = states::MOVE;
        } else if (request["command"] == "OK" && request.size() > 1){
            my_id = std::stoi(request["data"]["id"].get<std::string>());
            m_bank = request["data"]["Bank"];
        } else if (request["command"] == "Shuffle"){
            lefts_cards.clear();
            std::cerr << "Shuffle" << std::endl;
            json j;
            j["command"] = "OK";
            write(j.dump());
        }
    }
}

void TCP_Bot_Client::SetName() {
    json j;
    j["command"] = "Authorize";
    j["data"]["name"] = nickname;
    write(j.dump());
}

void TCP_Bot_Client::SetManager(std::shared_ptr<OutputManager> manager) {
    om = manager;
}

void TCP_Bot_Client::Process() {
    switch (cur_state) {
        case states::BET :
            Bet();
            break;
        case states::MOVE :
            Move();
            break;
        case states::DEAL :
            Answer();
            break;
        case states::NOTHING :
        default :
            break;
    }
}

void TCP_Bot_Client::Move() {
    auto branch = (is_ace) ? tactic_with_ace.find(dealer_hand.total()) : tactic.find(dealer_hand.total());
    auto comm = branch->second.find(m_hand.total());
    std::string command = "STAND";
    if (comm != branch->second.end())
        command = comm->second;
    if (command == "DOUBLEDOWN" && (m_hand.GetSize() != 2 || m_bank <= MAX)) {
        command = "HIT";
    }

    auto request = commands.find(command);
    if (request != commands.end()) {
        json j;
        j["command"] = "OK";
        switch (request->second) {
            case p_EVENT::HIT :
                j["data"]["action"] = "Hit";
                cur_state = states::NOTHING;
                break;
            case p_EVENT::STAND :
                j["data"]["action"] = "Stand";
                cur_state = states::NOTHING;
                break;
            case p_EVENT::DOUBLEDOWN :
                j["data"]["action"] = "Double";
                cur_state = states::NOTHING;
                break;
            case p_EVENT::BANK :
                std::cout << "Your bank : " << m_bank << std::endl;
                break;
            default:
                j["data"]["action"] = "Stand";
        }
        write(j.dump());
    }
}

void TCP_Bot_Client::Bet() {
    int bet = MIN;
    if (lefts_cards.size() > 10) {
        auto koef = Mathematical_Expectation();
            if (koef <= 0) {
                bet = MIN;
            } else if (koef >= 3) {
                bet = 2 * MIN;
            } else if (koef >= 7) {
                bet = MAX / 2;
            } else if (koef >= 10) {
                bet = MAX;
            }
    }
    if (m_bank < bet) {
        bet = MIN;
    }
    json j;
    j["command"] = "OK";
    j["data"]["bet"] = bet;
    cur_state = states::NOTHING;
    write(j.dump());
}

void TCP_Bot_Client::Answer() {
    json j;
    j["command"] = "OK";
    if (BlackJackCheck()) {
        j["data"]["insurance"] = true;
    } else {
        j["data"]["insurance"] = false;
    }
    cur_state = states::NOTHING;
    write(j.dump());
}

void TCP_Bot_Client::SetCard(const GameCard::Cards & card) {
    m_hand.SetNewCard(card);
}

void TCP_Bot_Client::SetDealerCard(const GameCard::Cards & card) {
    dealer_hand.SetNewCard(card);
}

bool TCP_Bot_Client::BlackJackCheck() const {
    return m_hand == BLACKJACK;
}

void TCP_Bot_Client::GetRoundResult(int bet) {
    m_bank += bet;
}

void TCP_Bot_Client::ClearHand() {
    m_hand.Clear();
}

const GameCard::Hand &TCP_Bot_Client::ShowHand() const {
    return m_hand;
}

int TCP_Bot_Client::GetPlayerCost() const {
    return m_bank;
}

void TCP_Bot_Client::Collect(const json & request) {
    if (!request["isDealer"]) {
        auto str = request["id"].get<std::string>();
        auto id = std::stoi(str);
        if (id == my_id) {
            m_hand.Clear();
            for (auto &hand : request["hand"]) {
                auto card = GameCard::FromStr(hand["value"].get<std::string>(), hand["suit"].get<std::string>(),
                                              !hand["isOpen"].get<bool>());
                SetCard(card);
                if (card.price == GameCard::Cards::CardPrice::ACE && m_hand.GetSize() == 2){
                    is_ace = true;
                } else {
                    is_ace = false;
                }
            }
        } else {
            round_cards.clear();
            for (auto &hand : request["hand"]) {
                auto card = GameCard::FromStr(hand["value"].get<std::string>(), hand["suit"].get<std::string>(),
                                              !hand["isOpen"].get<bool>());

                round_cards[card]++;
            }
        }
    } else {
        dealer_hand.Clear();
        for (auto &hand : request["hand"]) {
            if (hand["isOpen"].get<bool>()) {
                auto card = GameCard::FromStr(hand["value"].get<std::string>(),
                                              hand["suit"].get<std::string>(),
                                              !hand["isOpen"].get<bool>());
                SetDealerCard(card);
            }
        }
    }
}

void TCP_Bot_Client::UpdateLeftCards() {
    for (auto & card : m_hand.LookAtCards()){
        round_cards[card]++;
    }
    for (auto & card : dealer_hand.LookAtCards()){
        round_cards[card]++;
    }
    auto tmpe = this->round_cards;
    for (auto & card : round_cards){
        lefts_cards[card.first.price] += card.second;
    }
    round_cards.clear();
}

int TCP_Bot_Client::Mathematical_Expectation() const {
    int hi_lo = 0;
    auto tmp = lefts_cards;
    for (auto & card : lefts_cards) {
        hi_lo += TorpSys.at(card.first);
    }
    return hi_lo;
}
