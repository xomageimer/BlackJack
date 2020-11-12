#ifndef BLACKJACK_OUTPUTMANAGER_H
#define BLACKJACK_OUTPUTMANAGER_H

#include <map>
#include <vector>
#include <string>
#include <memory>

#include "Cards/CardStack.h"

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
            {GameCard::Cards::CardSuit::HEARTS, "♥"},
            {GameCard::Cards::CardSuit::SPADES, "♠"},
            {GameCard::Cards::CardSuit::DIAMONDS, "♦"},
            {GameCard::Cards::CardSuit::CLUBS, "♣"}
    };

public:
    OutputManager() = default;

    void subscribe(const std::string & name_listener, std::shared_ptr<struct ILogger> new_listener);
    void notify(const std::string & new_info);
    void notify(const GameCard::Cards & new_info);
    void unsubscribe(const std::string & listener);
    void drop();
    void destroy();
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
