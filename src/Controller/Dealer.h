#ifndef BLACKJACK_DEALER_H
#define BLACKJACK_DEALER_H

#include "Actors/IPlayer.h"

const int BLACKJACK = 21;
const double WinFactor = 1.5f;
const int DEALERBORDER = 17;

struct GameGround;

namespace DealerHandlers {
    struct IDealerHandler;
    struct BetHandler;
    struct MoveHandler;
    struct PlayoutHandler;
    struct PlayingHandler;
    struct RoundHandler;
    struct DealHandler;
}

namespace Controller {
    struct IDealer {
    public:
        enum class states : int {
            BET_SERVANT,
            MOVE_SERVANT,
            ROUND_SERVANT,
            PLAYOUT_SERVANT,
            YOURSELF_SERVANT,
            DEAL_SERVANT
        };
    protected:
        std::map<states, std::shared_ptr<DealerHandlers::IDealerHandler>> cmd_handles;
        std::shared_ptr<DealerHandlers::IDealerHandler> cur_handler;
        states cur_state;

        friend DealerHandlers::IDealerHandler;
        friend DealerHandlers::BetHandler;
        friend DealerHandlers::MoveHandler;
        friend DealerHandlers::PlayoutHandler;
        friend DealerHandlers::PlayingHandler;
        friend DealerHandlers::RoundHandler;
        friend DealerHandlers::DealHandler;

        virtual void set_current(states state) {
            cur_state = state;
            cur_handler = cmd_handles[cur_state];
        }

    public:
        explicit IDealer();
        virtual ~IDealer() = default;

        virtual void SetView(std::shared_ptr<OutputManager>);
        virtual void SetPlayer(std::shared_ptr<Actors::IPlayer>);

        virtual void ServeBet() = 0;
        virtual void ServeMove() = 0;
        virtual void ServeRound() = 0;
        virtual void ServePlayout() = 0;
        virtual void ServeYourself() = 0;

        virtual void Process() = 0;

        virtual const std::vector<std::shared_ptr<Actors::IPlayer>> & kickAFK();

        virtual void SetPlayerDealer(std::shared_ptr<Actors::IPlayer>);

        static inline const int max = 600;
        static inline const int min = 10;

    protected:
        std::shared_ptr<OutputManager> general_view_manager;

        std::shared_ptr<Actors::IPlayer> player_dealer;

        std::shared_ptr<GameCard::CardStack> m_stack;
        std::vector<std::pair<std::shared_ptr<Actors::IPlayer>, int>> m_players;
        std::map<std::shared_ptr<Actors::IPlayer>, bool> insurances;

        std::vector<std::shared_ptr<Actors::IPlayer>> AFK_players;

        size_t cursor = 0;
        // может быть бд, где ID плеера будет ключем для того, чтобы получить доступ
        // к его ставке и фишкам
        virtual void AFKCurrentPlayer();

        virtual std::pair<std::shared_ptr<Actors::IPlayer>, int> & getPlayer();
        virtual std::shared_ptr<Actors::IPlayer> getDealerPlayer();
    };

    struct SimpleDealer : public Actors::IPlayer, public IDealer {
    protected:
        int player_bet = 0;
        int m_bank = 1'000'000;
        GameCard::Hand m_hand;

    public:
        explicit SimpleDealer(int bank) : m_bank(bank) {
            m_stack = std::make_shared<GameCard::CardStack>(std::make_shared<GameCard::Mersenne_Generator>());
            m_stack->GenNewStacks();
        };

        void ServeBet() override;
        void ServeMove() override;
        void ServeRound() override;
        void ServePlayout() override;
        void ServeYourself() override;

        void Process() override;

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

    struct TRUE {bool m;};
    struct FALSE {bool m[2]; };

    template <typename C, typename P>
    struct Is_Inherited{
        static TRUE check(C *) { return TRUE {}; };
        static FALSE check(...) { return FALSE{}; };

        static bool const value = sizeof(check((P *)(nullptr)))
                == sizeof(TRUE);
    };

    template <typename T>
    struct Is_Inherited<T, T> {
        static bool const value = false;
    };

    template <typename Child, typename Parent>
    auto Is_Inherited_v = Is_Inherited<Child, Parent>::value;

}

#endif //BLACKJACK_DEALER_H
