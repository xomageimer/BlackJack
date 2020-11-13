#include "IPlayer.h"

void Actors::OfflinePlayer::SetCard(const GameCard::Cards & card) {
    m_hand.SetNewCard(card);
}

bool Actors::OfflinePlayer::BlackJackCheck() const {
    return m_hand == BLACKJACK;
}

void Actors::OfflinePlayer::GetRoundResult(int bet) {
    m_bank += bet;
}

const GameCard::Hand &Actors::OfflinePlayer::ShowHand() const {
    return m_hand;
}

int Actors::OfflinePlayer::GetPlayerCost() const {
    return m_bank;
}

void Actors::OfflinePlayer::ClearHand() {
    m_hand.Clear();
}
