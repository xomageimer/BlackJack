#ifndef BLACKJACK_IPLAYER_H
#define BLACKJACK_IPLAYER_H

namespace Actors {
    struct IPlayer {
    public:
        enum class PlayerCommands : int {
            BET,
            HIT,
            STAND,
            DOUBLEDOWN
        };

        virtual ~IPlayer() = default;
        IPlayer() = default;

        virtual void SetCard(const GameCard::Cards &) = 0;
        [[nodiscard]] virtual bool BlackJackCheck() const = 0;
        virtual void GetRoundResult(int) = 0;

        [[nodiscard]] virtual const GameCard::Hand &ShowHand() const = 0;
        [[nodiscard]] virtual int GetPlayerCost() const = 0;
    };

    struct OfflinePlayer : public IPlyaer{
    protected:
        GameCard::Hand m_hand;
        int m_bank;
    public:
        explicit OfflinePlayer(int bank) : m_bank(bank) {}

        void SetCard(const GameCard::Cards &) override;
        [[nodiscard]] virtual bool BlackJackCheck() const override;
        void GetRoundResult(int) = 0;

        [[nodiscard]] const GameCard::Hand &ShowHand() const override;
        [[nodiscard]] int GetPlayerCost() const override;

    };


}


#endif //BLACKJACK_IPLAYER_H
