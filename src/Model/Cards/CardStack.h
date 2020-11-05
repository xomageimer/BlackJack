#ifndef BLACKJACK_CARDSTACK_H
#define BLACKJACK_CARDSTACK_H

#include <list>
#include <vector>
#include <random>

namespace GameCard {
    struct CardStack {
    private:
        // TODO использовать свой аллокатор для LIST
        std::list<struct Cards> m_CardShoe;
        std::list<struct Cards> m_goneCards;
    public:
        explicit CardStack(size_t count_of_card_stacks = 4);

        friend void GenerateCardPack(CardStack &);

        void GenNewStacks();

        struct Cards GetCard();
    };

    struct Hand {
    private:
        std::vector<struct Cards> m_Cards;
    public:
        explicit Hand(size_t max_card_per_hand = 11); // reserve 10

        [[nodiscard]] const std::vector<struct Cards> &LookAtCards() const;

        void SetNewCard(struct Cards);

        friend bool operator>(const Hand&, const Hand &);
        friend bool operator<(const Hand&, const Hand &);

        friend bool operator>(const Hand&, int);
        friend bool operator<(const Hand&, int);

        friend bool operator==(const Hand&, const Hand &);
        friend bool operator==(const Hand&, int);

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
            KING
        };

        enum class CardSuit : int {
            SPADES,
            CLUBS,
            DIAMONDS,
            HEARTS
        };

        CardPrice price;
        CardSuit suit;
    };

    Cards::CardPrice& operator++(Cards::CardPrice & cp);
    Cards::CardSuit& operator++(Cards::CardSuit & cp);

}

#endif //BLACKJACK_CARDSTACK_H
