#include "CardStack.h"

#include <iostream>
#include <utility>

// TODO туз должен быть 1 или 11

std::ostream& operator<<(std::ostream& os, const GameCard::Cards& sc)
{
    os << static_cast<int>(sc.price) << ' ' << static_cast<int>(sc.suit) << ';';
    return os;
}

std::ostream& operator<<(std::ostream& os, const std::list<GameCard::Cards>& list)
{
    for (auto &i : list) {
        os << " " << i;
    }
    return os;
}

void GameCard::CardStack::GenNewStacks() {
    m_CardShoe.splice(m_CardShoe.begin(), m_goneCards);

    auto it = prev(m_CardShoe.end());
    auto sec_it = std::next(m_CardShoe.begin(), _gen->seed(m_CardShoe.size()));
    auto sec_tmp = sec_it;

    for (size_t i = 0; i < m_CardShoe.size(); i++) {
        sec_tmp = std::next(it);

        m_CardShoe.splice(it, m_CardShoe, sec_it);
        m_CardShoe.splice(sec_tmp, m_CardShoe, it);

        sec_it = std::next(m_CardShoe.begin(), _gen->seed(m_CardShoe.size()));
        it = std::prev(prev(m_CardShoe.end()), i);
    }

  //   std::cout << "m_CardShoe: " << m_CardShoe << "\n";
}

GameCard::Cards::CardPrice& operator++(GameCard::Cards::CardPrice & cp){
    using Cards = GameCard::Cards::CardPrice;
    return (cp == Cards::KING) ? cp = Cards::STOPPER : cp = static_cast<Cards>(static_cast<int>(cp) + 1);
}

GameCard::Cards::CardSuit& operator++(GameCard::Cards::CardSuit & cp){
    using Cards = GameCard::Cards::CardSuit;
    return (cp == Cards::HEARTS) ? cp = Cards::STOPPER : cp = static_cast<Cards>(static_cast<int>(cp) + 1);
}

void GameCard::GenerateCardPack(GameCard::CardStack & cs) {
    for (auto i = GameCard::Cards::CardSuit::SPADES; i != GameCard::Cards::CardSuit::STOPPER; ++i){
        for (auto j = GameCard::Cards::CardPrice::ACE; j != GameCard::Cards::CardPrice::STOPPER; ++j){
            cs.m_CardShoe.emplace_back(GameCard::Cards{j, i});
        }
    }
}

GameCard::Cards GameCard::CardStack::GetCard() {
    this->m_goneCards.push_back(this->m_CardShoe.front());
    GameCard::Cards tmp = this->m_CardShoe.front();
    this->m_CardShoe.pop_front();

    return tmp;
}

GameCard::CardStack::CardStack(std::shared_ptr<Generator> gen, size_t count_of_card_stacks) : card_stack_count(count_of_card_stacks), _gen(std::move(gen)) {
    for (size_t i = 0; i < count_of_card_stacks; i++){
        GenerateCardPack(*this);
    }
}

void GameCard::CardStack::TimeToShuffle() {
    if ((2 * (m_CardShoe.size() + m_goneCards.size()) / 3) <= m_goneCards.size())
        GenNewStacks();
}

size_t GameCard::CardStack::GoneCardsSize() const {
    return m_goneCards.size();
}

size_t GameCard::CardStack::CardShoeSize() const {
    return m_CardShoe.size();
}

GameCard::Hand::Hand(size_t max_card_per_hand) : m_Cards (max_card_per_hand), is_open(max_card_per_hand, true){}

const std::vector<GameCard::Cards> &GameCard::Hand::LookAtCards() const {
    return this->m_Cards;
}

void GameCard::Hand::SetNewCard(GameCard::Cards card) {
    this->m_Cards.emplace_back(card);
}

bool GameCard::operator>(const GameCard::Hand & h1, const GameCard::Hand & h2) {
    return h1.total() > h2.total();
}

bool GameCard::operator<(const GameCard::Hand & h1, const GameCard::Hand & h2) {
    return h2 > h1;
}

bool GameCard::operator>(const GameCard::Hand & h1, int val) {
    return h1.total() > val;
}

bool GameCard::operator<(const GameCard::Hand & h1, int val) {
    return val > h1.total();
}

int GameCard::Hand::total() const {
    int h_price = 0;
    int ACE_counts = 0;

    for (auto & i : LookAtCards()){
        ACE_counts += (i.price == Cards::CardPrice::ACE) ? 1 : 0;
    }

    for (auto & i : LookAtCards()){
        if (i.price != Cards::CardPrice::ACE)
            h_price += (i.price > Cards::CardPrice::TEN) ? static_cast<int>(i.price) : 10;
        else
            h_price += 11;
    }

    while (h_price > 21 && ACE_counts > 0) {
        h_price -= 11;
        h_price += 1;
        --ACE_counts;
    }

    return h_price;
}

void GameCard::Hand::Clear() {
    this->m_Cards.clear();
}

bool GameCard::operator==(const GameCard::Hand & h1, int val) {
    return h1.total() == val;
}

bool GameCard::operator==(const GameCard::Hand & h1, const GameCard::Hand & h2) {
    return h1.total() == h2.total();
}

size_t GameCard::Hand::GetSize() const {
    return m_Cards.size();
}

bool GameCard::operator!=(const GameCard::Hand & h1, const GameCard::Hand & h2) {
    return h1.total() != h2.total();;
}

bool GameCard::operator!=(const GameCard::Hand & h, int val) {
    return h.total() != val;
}

void GameCard::Hand::MakeSecret(size_t i) {
    is_open[i] = false;
}

void GameCard::Hand::UnSecret(size_t i) {
    is_open[i] = true;
}

GameCard::Mersenne_Generator::Mersenne_Generator() : mersenne(std::random_device()()) {}

size_t GameCard::Mersenne_Generator::seed(size_t seed_) {
    auto rand = mersenne() % (seed_ - 1);
    return rand;
}
