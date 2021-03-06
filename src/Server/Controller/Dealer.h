#ifndef BLACKJACK_DEALER_H
#define BLACKJACK_DEALER_H

#include <utility>

#include "Game_Room.h"
#include "Actors/IPlayer.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

const int BLACKJACK = 21;
const double WinFactor = 1.5f;
const int DEALERBORDER = 17;

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
            ROUND_SERVANT,
            MOVE_SERVANT,
            YOURSELF_SERVANT,
            PLAYOUT_SERVANT,
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
            std::cerr << "\n" << "change context" << '\n' << std::endl;
            cursor = 0;
            cur_state = state;
            cur_handler = cmd_handles[cur_state];
            this->Process();
        }

        virtual void CheckBJ();

    public:
        explicit IDealer();
        virtual ~IDealer() = default;

        virtual void Notify_about_dealer();
        virtual void Notify_about_player(int num);

        virtual void SetPlayer(std::shared_ptr<Actors::IPlayer>);

        virtual void RestartDealer();

        virtual void SetRoom(Game_Room * room);

        virtual void ServeBet() = 0;
        virtual void ServeMove() = 0;
        virtual void ServeRound() = 0;
        virtual void ServePlayout() = 0;
        virtual void ServeYourself() = 0;

        virtual void MakeBet(std::string json) = 0;
        virtual void MakeMove(std::string json) = 0;
        virtual void MakeDeal(std::string json) = 0;

        bool FindPlayer(std::shared_ptr<Actors::IPlayer>);

        virtual void Process() = 0;
        virtual void Maker(std::string json) = 0;

        virtual void SetPlayerDealer(std::shared_ptr<Actors::IPlayer>);

        static inline const int max = 600;
        static inline const int min = 10;

    protected:
        // TODO замена на своего Сендера, через game_ground подключение к диллеру новых плееров или
        //  их отключение. В диллере только игровая логика сервера, в game_ground'е - обслуживание комнаты

        Game_Room * my_room;

        std::shared_ptr<Actors::IPlayer> player_dealer;

        std::shared_ptr<GameCard::ICardStack> m_stack;
        std::vector<std::pair<std::shared_ptr<Actors::IPlayer>, int>> m_players;
        std::map<std::shared_ptr<Actors::IPlayer>, bool> insurances;

        size_t cursor = 0;

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

        explicit SimpleDealer(int bank, std::shared_ptr<GameCard::ICardStack> stack) : m_bank(bank) {
            m_stack = std::move(stack);
        };

        void ServeBet() override;
        void ServeMove() override;
        void ServeRound() override;
        void ServePlayout() override;
        void ServeYourself() override;

        void MakeBet(std::string json) override;
        void MakeMove(std::string json) override;
        void MakeDeal(std::string json) override;

        void Process() override;
        void Maker(std::string json) override;

        void Move() override;
        void Bet() override;
        void Answer() override;

        void SetCard(const GameCard::Cards &) override;
        [[nodiscard]] bool BlackJackCheck() const override;
        void GetRoundResult(int) override;

        void ClearHand() override;

        [[nodiscard]] const GameCard::Hand &ShowHand() const override;
        [[nodiscard]] int GetPlayerCost() const override;
    };

    struct TRUE_ {bool m;};
    struct FALSE_ {bool m[2]; };

    template <typename C, typename P>
    struct Is_Inherited{
        static TRUE_ check(P *) { return TRUE_ {}; };
        static FALSE_ check(...) { return FALSE_{}; };

        static bool const value = sizeof(check((C *)(nullptr)))
                == sizeof(TRUE_);
    };

    template <typename T>
    struct Is_Inherited<T, T> {
        static bool const value = false;
    };

    template <typename Child, typename Parent>
    constexpr auto Is_Inherited_v = Is_Inherited<Child, Parent>::value;

}

#endif //BLACKJACK_DEALER_H
