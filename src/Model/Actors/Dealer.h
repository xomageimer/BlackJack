#ifndef BLACKJACK_DEALER_H
#define BLACKJACK_DEALER_H

#include <memory>

#include "Events.h"
#include "Cards/CardStack.h"
#include "IActor.h"
#include "IHandler.h"

#include "Controller.h"

// pattern template method
namespace Actors {
    struct IDealer {
    protected:
        std::shared_ptr<GameCard::CardStack> m_stack;
        std::shared_ptr<DealerHandler::IHandler> handler;
        GameCard::Hand current_player_hand;
        double current_bet;
        double casino_win = 0;
    public:
        std::shared_ptr<IController> controller;

        enum class MaxBet { VALUE = 100 } max;

        IDealer(std::shared_ptr<IController> cntr);
        virtual ~IDealer() = default;

        virtual void TakeBet(double bet) = 0;
        virtual void GiveCard() = 0;
        virtual void SwapPlayer() = 0;
        virtual void PlayOut() = 0;
        virtual void NewRound() = 0;
        virtual void GiveDoubleDown() = 0;
        virtual void BlackJackCheck() = 0;

        inline void set_current(std::shared_ptr<IHandler> h) {
            handler = h;
            handler->SetController(controller);
        }

        virtual std::shared_ptr<IController> GetController();

        virtual void SetBet(double) = 0;
        virtual double GetBet() const = 0;

        virtual double & GetCasinoWin() = 0;

        virtual void SetPlayerHand(GameCard::Hand &) = 0;
        virtual const GameCard::Hand & GetPlayerHand() const = 0;
        virtual void ClearCurPlayerHand() = 0;
        virtual void ConfigPlayerHand(GameCard::Cards& card) = 0;
        virtual GameCard::Cards GetCard() = 0;

        virtual void RefreshStack() = 0;
    };

    struct SimpleDealer : public IDealer, public IActor {
    public:
        SimpleDealer() = default;

        void Hit(const GameCard::Cards &) override;
        void DoubleDown(const GameCard::Cards &, const GameCard::Cards &) override;
        void GetResult(double) override;

        void SetCard(const GameCard::Cards &);
        void SetPoints(double);

        const GameCard::Hand & ShowHand() const override;
        double GetPlayerCost() const override;

        void GiveCard() override;
        void SwapPlayer() override;
        void PlayOut() override;
        void NewRound() override;
        void TakeBet(double) override;
        void GiveDoubleDown() override;
        void BlackJackCheck() override;

        void RefreshStack() override;

        void SetBet(double) override;
        double GetBet() const override;

        double & GetCasinoWin() override;

        void SetPlayerHand(GameCard::Hand &) override;
        const GameCard::Hand & GetPlayerHand() const override;
        void ClearCurPlayerHand() override;
        virtual void ConfigPlayerHand(GameCard::Cards& card) override;

        GameCard::Cards GetCard() override;
    };
}
#endif //BLACKJACK_DEALER_H
