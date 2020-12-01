#include "IPlayer.h"

using p_EVENT = Event::PlayerRequests;
using d_EVENT = Event::DealerResponse;

void Actors::Player::SetCard(const GameCard::Cards & card) {
    m_hand.SetNewCard(card);
}

bool Actors::Player::BlackJackCheck() const {
    return m_hand == BLACKJACK;
}

void Actors::Player::GetRoundResult(int bet) {
    m_bank += bet;
}

const GameCard::Hand &Actors::Player::ShowHand() const {
    return m_hand;
}

int Actors::Player::GetPlayerCost() const {
    return m_bank;
}

void Actors::Player::ClearHand() {
    m_hand.Clear();
}

void Actors::Player::Move() {
//    std::string command;
//    while (is >> command){
//        auto request = commands.find(command);
//        if (request != commands.end()) {
//            switch (request->second) {
//                case p_EVENT::HIT :
//                    return Event(d_EVENT::GIVECARD, std::string());
//                case p_EVENT::STAND :
//                    return Event(d_EVENT::SWAPPLAYER, std::string());
//                case p_EVENT::DOUBLEDOWN :
//                    return Event(d_EVENT::DOUBLEDOWN, std::string());
//                case p_EVENT::BANK :
//                    std::cout << "Your bank : " << m_bank << std::endl;
//                    break;
//                default:
//                    return Event(d_EVENT::SWAPPLAYER, std::string());
//            }
//        } else {
//            std::cout << "WRONG INPUT" << std::endl; // в графике не может быть вронг инпута
//            is.clear();
//            is.ignore(256, '\n');
//        }
//    }
}

void Actors::Player::Bet() {
//    std::string command;
//    while (is >> command){
//        if (p_EVENT::BANK == commands.find(command)->second) {
//            std::cout << "Your bank : " << m_bank << std::endl;
//        } else if (p_EVENT::BET == commands.find(command)->second) {
//            int bet;
//            is >> bet;
//            return Event(d_EVENT::MAKEBET, bet);
//        } else {
//            std::cout << "WRONG INPUT" << std::endl; // в графике не может быть вронг инпута
//            is.clear();
//            is.ignore(256, '\n');
//        }
//    }
}

void Actors::Player::Answer() {
//    std::string command;
//    while (is >> command){
//        if (p_EVENT::YES == commands.find(command)->second) {
//            return Event(d_EVENT::YES, std::string());
//        } else {
//            return Event(d_EVENT::SWAPPLAYER, std::string());
//        }
//    }
}

void Actors::IPlayer::SetName(const std::string & name) {
    nickname = name;
}

const std::string &Actors::IPlayer::GetName() const {
    return nickname;
}
