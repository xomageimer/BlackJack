#ifndef BLACKJACK_CARDSTACK_H
#define BLACKJACK_CARDSTACK_H

#include <list>
#include <vector>
#include <random>
#include <memory>

namespace GameCard {
    struct Generator {
    public:
        virtual size_t seed(size_t seed_) = 0;
    };

    struct Mersenne_Generator : Generator {
    private:
        std::mt19937 mersenne;
    public:
        Mersenne_Generator();
        size_t seed(size_t seed_) override;
    };

    struct CardStack {
    private:
        // TODO использовать свой аллокатор для LIST
        std::list<struct Cards> m_CardShoe;
        std::list<struct Cards> m_goneCards;

        size_t card_stack_count;
        std::shared_ptr<Generator> _gen;
    public:
        explicit CardStack(std::shared_ptr<Generator> gen, size_t count_of_card_stacks = 4);

        friend void GenerateCardPack(CardStack &);

        [[nodiscard]] size_t GoneCardsSize() const;
        [[nodiscard]] size_t CardShoeSize() const;

        void TimeToShuffle();

        void GenNewStacks();

        struct Cards GetCard();
    };

    struct Hand {
    private:
        std::vector<struct Cards> m_Cards;
        std::vector<bool> is_open;
    public:
        explicit Hand(size_t max_card_per_hand = 11); // reserve 10

        [[nodiscard]] const std::vector<struct Cards> &LookAtCards() const;

        void MakeSecret(size_t i);
        void UnSecret(size_t i);

        void SetNewCard(struct Cards);

        [[nodiscard]] size_t GetSize() const;

        friend bool operator>(const Hand&, const Hand &);
        friend bool operator<(const Hand&, const Hand &);

        friend bool operator>(const Hand&, int);
        friend bool operator<(const Hand&, int);

        friend bool operator==(const Hand&, const Hand &);
        friend bool operator==(const Hand&, int);

        friend bool operator!=(const Hand&, const Hand &);
        friend bool operator!=(const Hand&, int);

        void Clear();

        [[nodiscard]] int total() const;
    };

    struct Cards {
        enum class CardPrice : int {
            ACE = 1,
            TWO,
            THREE,
            FOUR,
            FIVE,
            SIX,
            SEVEN,
            EIGHT,
            NINE,
            TEN,
            JACK,
            QUEEN,
            KING,


            STOPPER
        };

        enum class CardSuit : int {
            SPADES = 0,
            CLUBS,
            DIAMONDS,
            HEARTS,

            STOPPER
        };

        CardPrice price;
        CardSuit suit;
    };


}

#endif //BLACKJACK_CARDSTACK_H
