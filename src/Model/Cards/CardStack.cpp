#include "CardStack.h"

#include <iostream>

std::ostream& operator<<(std::ostream& os, const GameCard::Cards& sc)
{
    os << static_cast<int>(sc.price) << ' ' << static_cast<int>(sc.suit);
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
    std::random_device rd;
    std::mt19937 mersenne(rd());

    m_CardShoe.splice(m_CardShoe.begin(), m_goneCards);

    auto it = m_CardShoe.begin();

    for (size_t i = 0; i < m_CardShoe.size(); i++) {
        m_CardShoe.splice(m_CardShoe.begin()++, m_CardShoe, (std::advance(it, (mersenne() % (m_CardShoe.size() - 1))), it));
        auto it = m_CardShoe.begin();
    }

     std::cout << "m_CardShoe: " << m_CardShoe << "\n";
}

GameCard::Cards::CardPrice& operator++(GameCard::Cards::CardPrice & cp){
    using Cards = GameCard::Cards::CardPrice;
    return (cp == Cards::KING) ? cp = Cards::ACE : cp = static_cast<Cards>(static_cast<int>(cp) + 1);
}

GameCard::Cards::CardSuit& operator++(GameCard::Cards::CardSuit & cp){
    using Cards = GameCard::Cards::CardSuit;
    return (cp == Cards::HEARTS) ? cp = Cards::SPADES : cp = static_cast<Cards>(static_cast<int>(cp) + 1);
}

void GameCard::GenerateCardPack(GameCard::CardStack & cs) {
    for (auto i = GameCard::Cards::CardSuit::SPADES; i <= GameCard::Cards::CardSuit::HEARTS; ++i){
        for (auto j = GameCard::Cards::CardPrice::ACE; j <= GameCard::Cards::CardPrice::KING; ++j){
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

GameCard::CardStack::CardStack(size_t count_of_card_stacks) {
    for (size_t i = 0; i < count_of_card_stacks; i++){
        GenerateCardPack(*this);
    }
}

GameCard::Hand::Hand(size_t max_card_per_hand) : m_Cards (max_card_per_hand){}

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

    for (auto & i : LookAtCards()){
        h_price += (i.price > Cards::CardPrice::TEN) ? static_cast<int>(i.price) : 10;
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

