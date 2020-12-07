
#include <Client/Player/GraphicPlayer/TCP_Player_Client.h>

#include "TCP_Player_Client.h"


using json = nlohmann::json;

using p_EVENT = Event::PlayerRequests;

void TCP_Player_Client::Request(std::string str) {
    if(!str.empty() && !nickname.empty()) {
        try {
            json request = json::parse(str);

            if (request["command"] == "PlayerList") {
                om->notify_PlayerList(request);
                for (auto & player : request["data"]["Players"]){
                    if (stoi(player["id"].get<std::string>()) == my_id){
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
            } else if (request["command"] == "OK"){
                my_id = std::stoi(request["data"]["id"].get<std::string>());
                m_bank = request["data"]["Bank"];
            } else if (request["command"] == "Shuffle") {
                std::cerr << "Shuffle" << std::endl;
                json j;
                j["command"] = "OK";
                write(j.dump());
            }
        } catch (const std::exception & e) {
            std::cerr << "some error input: " << e.what() << ", continue!" << std::endl;
            std::cerr << "Msg has: " << str << std::endl;
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
        j["data"]["insurance"] = true;
    } else {
        j["data"]["insurance"] = false;
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

void TCP_Player_Client::SetName() {
    std::string command;
    json j;
    j["command"] = "Authorize";
    std::cin >> command;
    j["data"]["name"] = command;
    nickname = command;
    authorize = true;
    write(j.dump());
}

void TCP_Player_Client::Collect(const json &request) {
    m_hand.Clear();
    if (!request["isDealer"]) {
        if (stoi(request["id"].get<std::string>()) == my_id) {
            for (auto &hand : request["hand"]) {
                SetCard(GameCard::FromStr(hand["value"].get<std::string>(), hand["suit"].get<std::string>(),
                                          !hand["isOpen"].get<bool>()));
            }
        }
    }
}