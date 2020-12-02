#include "OutputManager.h"


void OutputManager::subscribe(const std::string &name_listener, std::shared_ptr<struct ILogger> new_listener) {
    subscribers.emplace(std::piecewise_construct, std::forward_as_tuple(name_listener), std::forward_as_tuple(new_listener));
}

void OutputManager::notify(const std::string &new_info) {
    for (auto & i : subscribers){
        i.second->update(new_info);
    }
    drop(); // без буферизирования
}

void OutputManager::notify(const GameCard::Cards &new_info) {
    auto value = m_value.find(new_info.price);
    auto suit = m_suit.find(new_info.suit);
    for (auto & i : subscribers){
        i.second->update(((value == m_value.end())
                   ? std::to_string(static_cast<int>(new_info.price))
                   : value->second) + suit->second);
    }
    drop();
}

void OutputManager::unsubscribe(const std::string &listener) {
    subscribers.erase(listener);
}

void OutputManager::drop() {
    for(auto & i : subscribers){
        i.second->output();
    }
}

void OutputManager::destroy() {
    for(auto & i : subscribers){
        i.second->clear();
    }
}

void OutputManager::notify_PlayerList(json j) {
    std::string text = j["command"];
    text += ":\n";
    for (auto & player_stats : j["data"]["Players"]){
        text += "Name: " + std::string(player_stats["name"]);
        text += ", Id: " + player_stats["id"].dump() + '\n';
        text += "Bank: " + player_stats["bank"].dump() + '\n';
    }
    for (auto & i : subscribers){
        i.second->update(text);
    }
    drop();
}

void OutputManager::notify_Bet(json j) {
    std::string text = j["command"];
    text += ":\n";
    text += "min: " + j["data"]["min"].dump() + '\n';
    text += "max: " + j["data"]["max"].dump() + '\n';
    for (auto & i : subscribers){
        i.second->update(text);
    }
    drop();
}

void OutputManager::notify_Insurance(json j) {
    std::string text = j["command"];
    text += ":";
    for (auto & i : subscribers){
        i.second->update(text);
    }
    drop();
}

void OutputManager::notify_PlayerChanged(json j) {
    std::string text = (j["data"]["isDealer"]) ? "DealerChanged" : j["command"];
    if (!j["data"]["isDealer"]) text += " " + std::to_string(j["data"]["number"].get<int>());
    text += ":\nCards: ";
    for (auto & hand_stats : j["data"]["hand"]){
        if (hand_stats["isOpen"]) {
            text += hand_stats["rank"];
            text += hand_stats["suit"];
            text += " ";
        } else {
            text += std::string("SECRET") + " ";
        }
    }
    for (auto & i : subscribers){
        i.second->update(text);
    }
    drop();
}

void OutputManager::notify_RequestAction(json j) {
    std::string text = j["command"].dump();
    for (auto & i : subscribers){
        i.second->update(text);
    }
    drop();
}


void ConsoleLogger::output() {
    for (auto & i : buffer){
        out << i + '\n';
    }
    out << std::flush;
    buffer.clear();
}

void ILogger::update(std::string str) {
    buffer.push_back(str);
}

void ILogger::clear() {
    buffer.clear();
}
