#ifndef BLACKJACK_VENTS_H
#define BLACKJACK_VENTS_H

#include <variant>
#include <utility>

#include "Cards/CardStack.h"

using DataType = std::variant<int, GameCard::Cards, GameCard::Hand, std::string>;
// , std::pair<GameCard::Cards, GameCard::Cards>

struct Event {
public:
    enum class PlayerRequests : int {
        BET,
        HIT,
        STAND,
        DOUBLEDOWN,
        YES,
        NO,
        BANK
    } Request;
    enum class DealerResponse : int {
        MAKEBET,
        GIVECARD,
        SWAPPLAYER,
        YES
    } Response;

    template <typename T>
    Event(const DealerResponse & new_type, const T & val){
        Response = new_type;
        data.emplace<T>(val);
    }

    template <typename T>
    Event(const PlayerRequests & new_type, const T & val){
        Request = new_type;
        data.emplace<T>(val);
    }

    template <typename T>
    auto GetData() const {
        return std::get<T>(data);
    }

private:
    DataType data;
};


#endif //BLACKJACK_VENTS_H
