#include "IActor.h"

void Actors::OfflinePlayer::Hit(const GameCard::Cards & card) {
    m_hand.SetNewCard(card);
}

void Actors::OfflinePlayer::DoubleDown(const GameCard::Cards & card1, const GameCard::Cards & card2) {
    m_hand.SetNewCard(card1);
    m_hand.SetNewCard(card2);
}

void Actors::OfflinePlayer::GetResult(double fract) {
    bank += fract;
}

const GameCard::Hand & Actors::OfflinePlayer::ShowHand() const {
    return m_hand;
}

double Actors::OfflinePlayer::GetPlayerCost() const {
    return bank;
}
