#ifndef BLACKJACK_DEALER_H
#define BLACKJACK_DEALER_H

#include "Actors/IPlayer.h"

struct GameGround;

namespace DealerHandlers {
    struct IDealerHandler;
}

namespace Controller {
    struct IDealer {
    public:
        enum class states : int {
            DEALERABLE,
            DISTRIBUTION,
            PLAYABLE,
            BETABLE
        };
    protected:
        std::map<states, std::shared_ptr<DealerHandlers::IDealerHandler>> cmd_handles;
        std::shared_ptr<DealerHandlers::IDealerHandler> cur_handler;

    public:
        explicit IDealer();
        virtual ~IDealer() = default;

        virtual void set_current(states state) {
            cur_handler = cmd_handles[state];
        }
        [[nodiscard]] bool IsPlayerDealer() const;

        static inline const int max = 600;
        static inline const int min = 10;

        void SetBase(GameGround *);
        virtual void HandleEvent(const Event &);
        void Reset();
        void MakeBet(int bet);

        virtual void Next();
        virtual void SetPlayer(Actors::IPlayer *, int) = 0;
        virtual void TimeToShuffle() = 0;
        virtual void TakeBet(int bet) = 0;
        virtual void GiveCard() = 0;
        virtual void SwapPlayer() = 0;
        virtual void PlayOut() = 0;
        virtual void NewRound() = 0;
        virtual void GiveDoubleDown() = 0;

    protected:
        std::shared_ptr<GameCard::CardStack> m_stack;

        Actors::IPlayer * current_player = nullptr;
        GameGround * ground = nullptr;
    };

    struct SimpleDealer : public Actors::IPlayer, public IDealer {
    protected:
        int player_bet = 0;
        int m_bank = 1'000'000;
        GameCard::Hand m_hand;

    public:
        explicit SimpleDealer(int bank) : m_bank(bank) {};

        void SetCard(const GameCard::Cards &) override;
        [[nodiscard]] bool BlackJackCheck() const override;
        void GetRoundResult(int) override;

        [[nodiscard]] const GameCard::Hand &ShowHand() const override;
        [[nodiscard]] int GetPlayerCost() const override;

        void SetPlayer(Actors::IPlayer *, int) override;
        void TimeToShuffle() override;
        void TakeBet(int bet) override;
        void GiveCard() override;
        void SwapPlayer() override;
        void PlayOut() override;
        void NewRound() override;
        void GiveDoubleDown() override;
    };

}

#endif //BLACKJACK_DEALER_H
