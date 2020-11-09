#ifndef BLACKJACK_IACTOR_H
#define BLACKJACK_IACTOR_H

#include <memory>

#include "Events.h"
#include "Cards/CardStack.h"

namespace Actors {
    struct IActor {
    public:
        virtual ~IActor() = default;
        IActor() = default;
        // это сеттеры
        virtual void Hit(const GameCard::Cards &) = 0;
        virtual void SetCard(const GameCard::Cards &) = 0;
        virtual void DoubleDown(const GameCard::Cards &, const GameCard::Cards &) = 0;
        virtual void GetResult(double) = 0;
        [[nodiscard]] virtual bool BlackJackCheck() const = 0;

        [[nodiscard]] virtual const GameCard::Hand & ShowHand() const = 0;
        [[nodiscard]] virtual double GetPlayerCost() const = 0;
    };
}

namespace Actors {
    struct OfflinePlayer : public IActor{
    protected:
        GameCard::Hand m_hand;
        double m_bank;
    public:
        OfflinePlayer(double bank);

        void Hit(const GameCard::Cards &) override;
        void DoubleDown(const GameCard::Cards &, const GameCard::Cards &) override;
        void SetCard(const GameCard::Cards &) override;
        void GetResult(double) override;
        [[nodiscard]] bool BlackJackCheck() const override;

        [[nodiscard]] const GameCard::Hand & ShowHand() const override;
        [[nodiscard]] double GetPlayerCost() const override; // можно создать бд отдельно от player и хранить деньги там, player просто будет ключем, не управляя от класса сколько у него денег и тп. Все операции над деньгами там
    };

}

#endif //BLACKJACK_IACTOR_H