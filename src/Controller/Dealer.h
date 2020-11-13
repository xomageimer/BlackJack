#ifndef BLACKJACK_DEALER_H
#define BLACKJACK_DEALER_H

#include <memory>
#include <stdexcept>

#include "Actors/IActor.h"
#include "Actors/Events.h"
#include "Cards/CardStack.h"

struct GameGround;

namespace DealerHandler {
    struct IHandler;
}

// pattern template method
namespace Actors {
    struct IDealer {
    public:
        enum class states : int {
            DEALERABLE,
            DISTRIBUTION,
            PLAYABLE,
            BETABLE
        };

        std::map<states, std::shared_ptr<DealerHandler::IHandler>> cmd_handles;

        void SetFacade(GameGround * );

        static inline const int max = 600;
        static inline const int min = 10;

        explicit IDealer();
        virtual ~IDealer() = default;

        virtual void TimeToShuffle() = 0;

        virtual void set_current(states state) {
            cur_handler = cmd_handles[state];
        }

        virtual void HandleEvent(const Event &);

        virtual void TakeBet(int bet) = 0;
        virtual void GiveCard() = 0;
        virtual void SwapPlayer() = 0;
        virtual void PlayOut() = 0;
        virtual void NewRound() = 0;
        virtual void GiveDoubleDown() = 0;

        virtual void SetPlayer(std::shared_ptr<IActor>) = 0;
        [[nodiscard]] virtual const GameCard::Hand & GetPlayerHand() const = 0;
        virtual void ClearCurPlayerHand() = 0;
        virtual GameCard::Cards GetCard() = 0;

        virtual void SetBet(int) = 0;
        [[nodiscard]] virtual int GetBet() const = 0;

        virtual int & GetCasinoWin() = 0;

        bool NextPlayer();

        virtual void RefreshStack() = 0;

    protected:
        std::shared_ptr<DealerHandler::IHandler> cur_handler;
        GameGround * ground{};
    };

    struct SimpleDealer : public IActor, public IDealer {
    protected:
        // TODO хранить руку (мб указатель на нее) и ставку в паре
        std::shared_ptr<GameCard::CardStack> m_stack;

        std::shared_ptr<IActor> current_player;

        int player_bet = 0;

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

        void SetPlayer(std::shared_ptr<IActor>) override;
        [[nodiscard]] const GameCard::Hand & GetPlayerHand() const override;
        GameCard::Cards GetCard() override;

        void RefreshStack() override;

        void SetBet(int) override;
        [[nodiscard]] int GetBet() const override;

        void SetCard(const GameCard::Cards &) override;

    protected:
        void GetResult(int) override{
            throw std::logic_error("");
        }
    };
}
#endif //BLACKJACK_DEALER_H
