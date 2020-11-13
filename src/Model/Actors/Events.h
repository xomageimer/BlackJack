#ifndef BLACKJACK_VENTS_H
#define BLACKJACK_VENTS_H

#include <variant>
#include <utility>

#include "Cards/CardStack.h"

using DataType = std::variant<int, GameCard::Cards, GameCard::Hand, std::string>;
// , std::pair<GameCard::Cards, GameCard::Cards>

struct Event {
public:
    enum class Type {
        WARN,
        DOUBLEDOWN,
        WIN,
        LOSE,
        STAND,
        DRAW,
        MAKEBET,
        GIVECARD,
        SWAPPLAYER,
        STATE
    } type;

    template <typename T>
    Event(const Type & new_type, const T & val){
        type = new_type;
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
