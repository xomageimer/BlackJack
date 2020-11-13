#ifndef BLACKJACK_IPLAYER_H
#define BLACKJACK_IPLAYER_H

#include "Events.h"
#include "Cards/CardStack.h"

namespace Actors {
    const int BLACKJACK = 21;
    struct IPlayer {
    public:
        virtual ~IPlayer() = default;
        IPlayer() = default;

        virtual void SetCard(const GameCard::Cards &) = 0;
        [[nodiscard]] virtual bool BlackJackCheck() const = 0;
        virtual void GetRoundResult(int) = 0;

        [[nodiscard]] virtual const GameCard::Hand &ShowHand() const = 0;
        [[nodiscard]] virtual int GetPlayerCost() const = 0;
    };

    struct OfflinePlayer : public IPlayer{
    protected:
        GameCard::Hand m_hand;
        int m_bank;
    public:
        explicit OfflinePlayer(int bank) : m_bank(bank) {}

        void SetCard(const GameCard::Cards &) override;
        [[nodiscard]] bool BlackJackCheck() const override;
        void GetRoundResult(int) override;

        [[nodiscard]] const GameCard::Hand &ShowHand() const override;
        [[nodiscard]] int GetPlayerCost() const override;
    };


}


#endif //BLACKJACK_IPLAYER_H
