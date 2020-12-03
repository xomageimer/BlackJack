#include "CardStack.h"

#include <iostream>
#include <utility>

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

    for (size_t j = 0; j < card_stack_count; j++) {
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
    }

    // std::cout << "m_CardShoe: " << m_CardShoe << "\n";
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
            cs.m_CardShoe.emplace_back(GameCard::Cards{j, i, false});
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

bool GameCard::CardStack::TimeToShuffle() {
    if ((2 * (m_CardShoe.size() + m_goneCards.size()) / 3) <= m_goneCards.size()) {
        GenNewStacks();
        std::cout << "SHUFFLE" << std::endl;
        return true;
    }
    return false;
}

size_t GameCard::CardStack::GoneCardsSize() const {
    return m_goneCards.size();
}

size_t GameCard::CardStack::CardShoeSize() const {
    return m_CardShoe.size();
}

GameCard::Hand::Hand(size_t max_card_per_hand) {m_Cards.reserve(max_card_per_hand);}

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
        if (!i.is_secret) {
            if (i.price != Cards::CardPrice::ACE)
                h_price += (i.price <= Cards::CardPrice::_10) ? static_cast<int>(i.price) : 10;
            else
                h_price += 11;
        }
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

GameCard::Cards GameCard::FromStr(std::string price, std::string suit, bool is_open) {
    GameCard::Cards::CardPrice p;
    GameCard::Cards::CardSuit s;
    if (price == "2") p = GameCard::Cards::CardPrice::_2;
    else if (price == "3") p = GameCard::Cards::CardPrice::_3;
    else if (price == "4") p = GameCard::Cards::CardPrice::_4;
    else if (price == "5") p = GameCard::Cards::CardPrice::_5;
    else if (price == "6") p = GameCard::Cards::CardPrice::_6;
    else if (price == "7") p = GameCard::Cards::CardPrice::_7;
    else if (price == "8") p = GameCard::Cards::CardPrice::_8;
    else if (price == "9") p = GameCard::Cards::CardPrice::_9;
    else if (price == "10") p = GameCard::Cards::CardPrice::_10;
    else if (price == "Ace") p = GameCard::Cards::CardPrice::ACE;
    else if (price == "King") p = GameCard::Cards::CardPrice::KING;
    else if (price == "Queen") p = GameCard::Cards::CardPrice::QUEEN;
    else if (price == "Jack") p = GameCard::Cards::CardPrice::JACK;

    if (suit == "Hearts") s = GameCard::Cards::CardSuit::HEARTS;
    else if (suit == "Spades") s = GameCard::Cards::CardSuit::SPADES;
    else if (suit == "Clubs") s = GameCard::Cards::CardSuit::CLUBS;
    else if (suit == "Diamonds") s = GameCard::Cards::CardSuit::DIAMONDS;

    return GameCard::Cards(p, s, is_open);
}

void GameCard::Hand::MakeSecret(size_t i) const {
    m_Cards.at(i).secret(true);
}

void GameCard::Hand::UnSecret(size_t i) const {
    m_Cards.at(i).secret(false);
}

nlohmann::json GameCard::Hand::Serialize() {
    nlohmann::json j;
    for (auto card : m_Cards){
        j.push_back(card.Serialize());
    }
    return j;
}

GameCard::Mersenne_Generator::Mersenne_Generator() : mersenne(std::random_device()()) {}

size_t GameCard::Mersenne_Generator::seed(size_t seed_) {
    auto rand = mersenne() % (seed_ - 1);
    return rand;
}

GameCard::Cards::operator std::string() const {
    std::string out;
    if (!is_secret) {
        auto o_value = m_value.find(this->price);
        auto o_suit = m_suit.find(this->suit);
        out = (((o_value == m_value.end())
                            ? std::to_string(static_cast<int>(this->price))
                            : o_value->second) + o_suit->second);
    } else {
        out = "#SECRET#";
    }
    return out;
}

void GameCard::Cards::secret(bool b) const {
    is_secret = b;
}

GameCard::Cards::Cards(const Cards & card) {
    price = card.price;
    suit = card.suit;
    is_secret = card.is_secret;
}

GameCard::Cards::Cards(GameCard::Cards::CardPrice _price, GameCard::Cards::CardSuit _suit, bool _secr) {
    price = _price;
    suit = _suit;
    is_secret = _secr;
}

nlohmann::json GameCard::Cards::Serialize() const{
    nlohmann::json j;
    j["isOpen"] = !is_secret;
    j["value"] = m_value.at(price);
    j["suit"] = m_suit.at(suit);

    return j;
}