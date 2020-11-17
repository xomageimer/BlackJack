#ifndef BLACKJACK_CARDSTACK_H
#define BLACKJACK_CARDSTACK_H

#include <map>
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

    struct ICardStack {
        virtual ~ICardStack() = default;

        [[nodiscard]] virtual size_t GoneCardsSize() const = 0;
        [[nodiscard]] virtual size_t CardShoeSize() const = 0;

        virtual void TimeToShuffle() = 0;

        virtual void GenNewStacks() = 0;

        virtual struct Cards GetCard() = 0;
    };

    struct CardStack : public ICardStack {
    private:
        // TODO использовать свой аллокатор для LIST
        std::list<struct Cards> m_CardShoe;
        std::list<struct Cards> m_goneCards;

        size_t card_stack_count;
        std::shared_ptr<Generator> _gen;
    public:
        explicit CardStack(std::shared_ptr<Generator> gen, size_t count_of_card_stacks = 4);

        friend void GenerateCardPack(CardStack &);

        [[nodiscard]] size_t GoneCardsSize() const override;
        [[nodiscard]] size_t CardShoeSize() const override;

        void TimeToShuffle() override;

        void GenNewStacks() override;

        struct Cards GetCard() override;
    };

    void GenerateCardPack(CardStack &);

    // stack for test
    struct TestCardStack : public ICardStack {
    private:
        // TODO использовать свой аллокатор для LIST
        std::list<struct Cards> m_CardShoe;
        std::list<struct Cards> m_goneCards;

        size_t card_stack_count;
        std::shared_ptr<Generator> _gen;
    public:
        explicit TestCardStack(std::shared_ptr<Generator> gen, size_t count_of_card_stacks = 4): card_stack_count(count_of_card_stacks), _gen(std::move(gen)){};
        [[nodiscard]] size_t GoneCardsSize() const override;
        [[nodiscard]] size_t CardShoeSize() const override;

        template <typename ... Args>
        friend void GenerateCardPack(TestCardStack &, Args... args);

        [[deprecated]] void TimeToShuffle() override;

        [[deprecated]] void GenNewStacks() override;

        struct Cards GetCard() override;
    };

    template <typename ... Args>
    void GenerateCardPack(TestCardStack & cs, Args... args){
        cs.m_CardShoe = {args...};
    }

    struct Hand {
    private:
        std::vector<struct Cards> m_Cards;
    public:
        explicit Hand(size_t max_card_per_hand = 11); // reserve 10

        [[nodiscard]] const std::vector<struct Cards> &LookAtCards() const;

        void MakeSecret(size_t i) const;
        void UnSecret(size_t i) const;

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

    bool operator>(const Hand&, const Hand &);
    bool operator<(const Hand&, const Hand &);

    bool operator>(const Hand&, int);
    bool operator<(const Hand&, int);

    bool operator==(const Hand&, const Hand &);
    bool operator==(const Hand&, int);

    bool operator!=(const Hand&, const Hand &);
    bool operator!=(const Hand&, int);

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

        Cards(CardPrice, CardSuit, bool);
        Cards(const Cards &);
        Cards(Cards &&) = default;
        Cards & operator= (const Cards &) = default;
        Cards & operator= (Cards&&) = default;

        void secret(bool) const;

        // для консоли
        static inline std::map<GameCard::Cards::CardPrice, std::string> m_value{
                {GameCard::Cards::CardPrice::ACE, "A"},
                {GameCard::Cards::CardPrice::KING, "K"},
                {GameCard::Cards::CardPrice::QUEEN, "Q"},
                {GameCard::Cards::CardPrice::JACK, "J"}

        };
        static inline std::map<GameCard::Cards::CardSuit, std::string> m_suit{
                {GameCard::Cards::CardSuit::HEARTS, "♥"},
                {GameCard::Cards::CardSuit::SPADES, "♠"},
                {GameCard::Cards::CardSuit::DIAMONDS, "♦"},
                {GameCard::Cards::CardSuit::CLUBS, "♣"}
        };

        CardPrice price;
        CardSuit suit;
        mutable bool is_secret;

        explicit operator std::string() const;
    };


}

#endif //BLACKJACK_CARDSTACK_H