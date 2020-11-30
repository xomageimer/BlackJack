#ifndef BLACKJACK_IPLAYER_H
#define BLACKJACK_IPLAYER_H

#include "Events.h"
#include "Cards/CardStack.h"
#include <iostream>

namespace Actors {
    const int BLACKJACK = 21;
    struct IPlayer {
    public:
        std::map<std::string, Event::PlayerRequests> commands{
                {"BET", Event::PlayerRequests::BET},
                {"HIT", Event::PlayerRequests::HIT},
                {"STAND", Event::PlayerRequests::STAND},
                {"DOUBLEDOWN", Event::PlayerRequests::DOUBLEDOWN},
                {"BANK", Event::PlayerRequests::BANK},
                {"YES", Event::PlayerRequests::YES}
        };
    protected:
        std::string nickname;
        int my_id = 0;
    public:
        virtual ~IPlayer() = default;
        IPlayer() = default;

        virtual void SetName(const std::string &  name);
        virtual const std::string & GetName() const;

        virtual Event Move() = 0;
        virtual Event Bet() = 0;
        virtual Event Answer() = 0;

        virtual void SetCard(const GameCard::Cards &) = 0;
        [[nodiscard]] virtual bool BlackJackCheck() const = 0;
        virtual void GetRoundResult(int) = 0;

        virtual void ClearHand() = 0;

        [[nodiscard]] virtual const GameCard::Hand &ShowHand() const = 0;
        [[nodiscard]] virtual int GetPlayerCost() const = 0;
    };

    struct Player : public IPlayer{
    protected:
        GameCard::Hand m_hand;
        int m_bank;

    public:
        explicit Player(int bank) : m_bank(bank) {}

        Event Move() override;
        Event Bet() override;
        Event Answer() override;

        void SetCard(const GameCard::Cards &) override;
        [[nodiscard]] bool BlackJackCheck() const override;
        void GetRoundResult(int) override;

        void ClearHand() override;

        [[nodiscard]] const GameCard::Hand &ShowHand() const override;
        [[nodiscard]] int GetPlayerCost() const override;
    };


}


#endif //BLACKJACK_IPLAYER_H
