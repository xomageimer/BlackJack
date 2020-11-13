#include "IActor.h"

const int BLACKJACK = 21;

void Actors::OfflinePlayer::Hit(const GameCard::Cards & card) {
    SetCard(card);
}

void Actors::OfflinePlayer::GetResult(int fract) {
    m_bank += fract;
}

const GameCard::Hand & Actors::OfflinePlayer::ShowHand() const {
    return m_hand;
}

int Actors::OfflinePlayer::GetPlayerCost() const {
    return m_bank;
}

bool Actors::OfflinePlayer::BlackJackCheck() const {
    return m_hand == BLACKJACK;
}

void Actors::OfflinePlayer::SetCard(const GameCard::Cards & card) {
    m_hand.SetNewCard(card);
}

Actors::OfflinePlayer::OfflinePlayer(int bank) : m_bank(bank) {}
