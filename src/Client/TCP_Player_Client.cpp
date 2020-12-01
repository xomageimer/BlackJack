#include "TCP_Player_Client.h"


using json = nlohmann::json;

using p_EVENT = Event::PlayerRequests;

void TCP_Player_Client::Request(std::string str) {
    if(!str.empty()) {
        try {
            json request = json::parse(str);
            str.clear();

            if (request["command"] == "PlayerList") {
                om->notify_PlayerList(request);
            } else if (request["command"] == "Bet") {
                om->notify_Bet(request);
                cur_state = states::BET;
            } else if (request["command"] == "Insurance") {
                om->notify_Insurance(request);
                cur_state = states::DEAL;
            } else if (request["command"] == "PlayerChanged") {
                om->notify_PlayerChanged(request);
//        SetCard(request["data"]["hand"].back().dump());
            } else if (request["command"] == "RequestAction") {
                om->notify_RequestAction(request);
                cur_state = states::MOVE;
            }
        } catch (...) {
            std::cerr << "some error input, continue!" << std::endl;
        }
    }
}

void TCP_Player_Client::SetManager(std::shared_ptr<OutputManager> manager) {
    om = manager;
}

void TCP_Player_Client::Move() {
    std::cout << "Your input:\n";
    std::string command;
    while (std::cin >> command){
        auto request = commands.find(command);
        if (request != commands.end()) {
            json j;
            j["command"] = "OK";
            switch (request->second) {
                case p_EVENT::HIT :
                   j["action"] = "Hit";
                   break;
                case p_EVENT::STAND :
                   j["action"] = "Stand";
                   break;
                case p_EVENT::DOUBLEDOWN :
                   j["action"] = "Double";
                   break;
                case p_EVENT::BANK :
                    std::cout << "Your bank : " << m_bank << std::endl;
                    break;
                default:
                    j["action"] = "Stand";
            }
            cur_state = states::NOTHING;
            write(j.dump());
            return;
        } else {
            std::cout << "WRONG INPUT" << std::endl; // в графике не может быть вронг инпута
            std::cin.clear();
            std::cin.ignore(256, '\n');
        }
    }
}

void TCP_Player_Client::Bet() {
    std::cout << "Your input:\n";
    std::string command;
    while (std::cin >> command){
        if (p_EVENT::BANK == commands.find(command)->second) {
            std::cout << "Your bank : " << m_bank << std::endl;
        } else if (p_EVENT::BET == commands.find(command)->second) {
            int bet;
            std::cin >> bet;
            json j;
            j["command"] = "OK";
            j["data"]["bet"] = bet;
            cur_state = states::NOTHING;
            write(j.dump());
            return;
        } else {
            std::cout << "WRONG INPUT" << std::endl; // в графике не может быть вронг инпута
            std::cin.clear();
            std::cin.ignore(256, '\n');
        }
    }
}

void TCP_Player_Client::Answer() {
    std::cout << "Your input:\n";
    std::string command;
    json j;
    j["command"] = "OK";
    std::cin >> command;
    if (p_EVENT::YES == commands.find(command)->second) {
        j["insurance"] = true;
    } else {
        j["insurance"] = false;
    }
    cur_state = states::NOTHING;
    write(j.dump());
}

void TCP_Player_Client::SetCard(const GameCard::Cards & card) {
    m_hand.SetNewCard(card);
}

bool TCP_Player_Client::BlackJackCheck() const {
    return m_hand == BLACKJACK;
}

void TCP_Player_Client::GetRoundResult(int bet) {
    m_bank += bet;
}

const GameCard::Hand &TCP_Player_Client::ShowHand() const {
    return m_hand;
}

int TCP_Player_Client::GetPlayerCost() const {
    return m_bank;
}

void TCP_Player_Client::ClearHand() {
    m_hand.Clear();
}

void TCP_Player_Client::Process() {
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
