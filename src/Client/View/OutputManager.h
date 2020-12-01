#ifndef BLACKJACK_OUTPUTMANAGER_H
#define BLACKJACK_OUTPUTMANAGER_H

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <memory>

#include "Cards/CardStack.h"
#include "Actors/Events.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct OutputManager {
private:
    std::map<std::string, std::shared_ptr<struct ILogger>> subscribers;

    std::map<GameCard::Cards::CardPrice, std::string> m_value{
            {GameCard::Cards::CardPrice::ACE, "A"},
            {GameCard::Cards::CardPrice::KING, "K"},
            {GameCard::Cards::CardPrice::QUEEN, "Q"},
            {GameCard::Cards::CardPrice::JACK, "J"}

    };
    std::map<GameCard::Cards::CardSuit, std::string> m_suit{
            {GameCard::Cards::CardSuit::HEARTS, "Hearts"},
            {GameCard::Cards::CardSuit::SPADES, "Spades"},
            {GameCard::Cards::CardSuit::DIAMONDS, "Diamonds"},
            {GameCard::Cards::CardSuit::CLUBS, "Clubs"}
    };

public:
    OutputManager() = default;
    ~OutputManager() = default;

    void subscribe(const std::string & name_listener, std::shared_ptr<struct ILogger> new_listener);
    void notify(const std::string & new_info);
    void notify_PlayerList(json j);
    void notify_Bet(json j);
    void notify_Insurance(json j);
    void notify_PlayerChanged(json j);
    void notify_RequestAction(json j);
    void notify(const GameCard::Cards & new_info);
    void unsubscribe(const std::string & listener);
    void drop();
    void destroy();
};

// TODO продумать как переделать ивенты, чтобы не нужно было парсить строки и можно было получать информацию о картах
struct AI_view{
private:
    std::vector<GameCard::Hand *> all_hands;
    const GameCard::Hand * dealer_hand;
public:
    AI_view() = default;
    void add_dealer_hand(const GameCard::Hand * hnd){
        dealer_hand = hnd;
    }
};

struct ILogger {
protected:
    std::vector<std::string> buffer;
public:
    virtual void update(std::string);
    virtual void output() = 0;
    virtual void clear();
};

struct ConsoleLogger : public ILogger {
private:
    std::ostream& out;
public:
    explicit ConsoleLogger(std::ostream& os) : out(os){};
    void output() override;
};

#endif //BLACKJACK_OUTPUTMANAGER_H
