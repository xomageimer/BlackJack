#ifndef BLACKJACK_DEALER_H
#define BLACKJACK_DEALER_H

#include <memory>
#include <stdexcept>

#include "IActor.h"
#include "Events.h"
#include "Cards/CardStack.h"

namespace DealerHandler{
    struct IHandler;
}

struct IController;

// pattern template method
namespace Actors {
    struct IDealer {
    public:
        enum class MaxBet { VALUE = 100 } max;

        explicit IDealer(std::shared_ptr<IController> cntr);
        virtual ~IDealer() = default;

        virtual void TakeBet(double bet) = 0;
        virtual void GiveCard() = 0;
        virtual void SwapPlayer() = 0;
        virtual void PlayOut() = 0;
        virtual void NewRound() = 0;
        virtual void GiveDoubleDown() = 0;

        virtual void SetBet(double) = 0;
        [[nodiscard]] virtual double GetBet() const = 0;

        virtual double & GetCasinoWin() = 0;

        virtual void SetPlayerHand(GameCard::Hand &) = 0;
        [[nodiscard]] virtual const GameCard::Hand & GetPlayerHand() const = 0;
        virtual void ClearCurPlayerHand() = 0;
        virtual void ConfigPlayerHand(GameCard::Cards& card) = 0;
        virtual GameCard::Cards GetCard() = 0;

        virtual void RefreshStack() = 0;

    protected:
        std::shared_ptr<IController> controller;
        std::shared_ptr<DealerHandler::IHandler> handler;
    };

    struct SimpleDealer : public IActor, public IDealer {
    protected:
        std::shared_ptr<GameCard::CardStack> m_stack;
        GameCard::Hand current_player_hand;
        double current_bet;
        double casino_win = 0;
        GameCard::Hand m_hand;
        double bank;
    public:
        explicit SimpleDealer(std::shared_ptr<IController>);

        void Hit(const GameCard::Cards &) override;

        [[nodiscard]] const GameCard::Hand & ShowHand() const override;
        [[nodiscard]] double GetPlayerCost() const override;

        void GiveCard() override;
        void SwapPlayer() override;
        void PlayOut() override;
        void NewRound() override;
        void TakeBet(double) override;
        void GiveDoubleDown() override;
        [[nodiscard]] bool BlackJackCheck() const override;

        void RefreshStack() override;

        void SetBet(double) override;
        [[nodiscard]] double GetBet() const override;

        void SetPlayerHand(GameCard::Hand &) override;
        [[nodiscard]] const GameCard::Hand & GetPlayerHand() const override;
        void ClearCurPlayerHand() override;
        void ConfigPlayerHand(GameCard::Cards& card) override;

        GameCard::Cards GetCard() override;

    protected:
        double & GetCasinoWin() override;
        void DoubleDown(const GameCard::Cards &, const GameCard::Cards &) override{
            throw std::logic_error("");
        }
        void GetResult(double) override{
            throw std::logic_error("");
        }
    };
}
#endif //BLACKJACK_DEALER_H
