#ifndef BLACKJACK_DEALER_H
#define BLACKJACK_DEALER_H

#include <memory>
#include <stdexcept>

#include "IActor.h"
#include "Events.h"
#include "Cards/CardStack.h"

struct IController;

// pattern template method
namespace Actors {
    struct IDealer {
    public:
        static inline const int max = 600;
        static inline const int min = 10;

        explicit IDealer() = default;
        virtual ~IDealer() = default;

        virtual void TimeToShuffle() = 0;

        virtual void SetController(std::shared_ptr<IController> cntr);

        virtual void TakeBet(int bet) = 0;
        virtual void GiveCard() = 0;
        virtual void SwapPlayer() = 0;
        virtual void PlayOut() = 0;
        virtual void NewRound() = 0;
        virtual void GiveDoubleDown() = 0;

        virtual void SetBet(int) = 0;
        [[nodiscard]] virtual int GetBet() const = 0;

        virtual int & GetCasinoWin() = 0;

        virtual void SetPlayerHand(GameCard::Hand &) = 0;
        virtual const GameCard::Hand & GetPlayerHand() const = 0;
        virtual GameCard::Cards GetCard() = 0;

        virtual void RefreshStack() = 0;

    protected:
        std::shared_ptr<IController> controller = nullptr;
    };

    struct SimpleDealer : public IActor, public IDealer {
    protected:
        // TODO хранить руку (мб указатель на нее) и ставку в паре
        std::shared_ptr<GameCard::CardStack> m_stack;

        GameCard::Hand * current_player_hand = nullptr;
        int current_bet = 0;

        int casino_win = 0;
        int m_bank = 1'000'000;
        GameCard::Hand m_hand;
    public:
        explicit SimpleDealer(int bank);

        void Hit(const GameCard::Cards &) override;

        [[nodiscard]] const GameCard::Hand & ShowHand() const override;
        [[nodiscard]] int GetPlayerCost() const override;

        void TimeToShuffle() override;

        void GiveCard() override;
        void SwapPlayer() override;
        void PlayOut() override;
        void NewRound() override;
        void TakeBet(int) override;
        void GiveDoubleDown() override;
        [[nodiscard]] bool BlackJackCheck() const override;

        void RefreshStack() override;

        void SetBet(int) override;
        [[nodiscard]] int GetBet() const override;

        void SetPlayerHand(GameCard::Hand &) override;
        [[nodiscard]] const GameCard::Hand & GetPlayerHand() const override;
        GameCard::Cards GetCard() override;

        void SetCard(const GameCard::Cards &) override;

    protected:
        int & GetCasinoWin() override;
        void DoubleDown(const GameCard::Cards &, const GameCard::Cards &) override{
            throw std::logic_error("");
        }
        void GetResult(int) override{
            throw std::logic_error("");
        }
    };
}
#endif //BLACKJACK_DEALER_H
