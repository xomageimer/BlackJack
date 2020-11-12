#include "OutputManager.h"

void OutputManager::subscribe(const std::string &name_listener, std::shared_ptr<struct ILogger> new_listener) {
    subscribers.emplace(std::piecewise_construct, std::forward_as_tuple(name_listener), std::forward_as_tuple(new_listener));
}

void OutputManager::notify(const std::string &new_info) {
    for (auto & i : subscribers){
        i.second->update(new_info);
    }
}

void OutputManager::notify(const GameCard::Cards &new_info) {
    auto value = m_value.find(new_info.price);
    auto suit = m_suit.find(new_info.suit);
    for (auto & i : subscribers){
        i.second->update(((value == m_value.end())
                   ? std::to_string(static_cast<int>(new_info.price))
                   : value->second) + suit->second);
    }
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
