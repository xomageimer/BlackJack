#include "IPlayer.h"

void Actors::ConsoleOfflinePlayer::SetCard(const GameCard::Cards & card) {
    m_hand.SetNewCard(card);
}

bool Actors::ConsoleOfflinePlayer::BlackJackCheck() const {
    return m_hand == BLACKJACK;
}

void Actors::ConsoleOfflinePlayer::GetRoundResult(int bet) {
    m_bank += bet;
}

const GameCard::Hand &Actors::ConsoleOfflinePlayer::ShowHand() const {
    return m_hand;
}

int Actors::ConsoleOfflinePlayer::GetPlayerCost() const {
    return m_bank;
}

void Actors::ConsoleOfflinePlayer::ClearHand() {
    m_hand.Clear();
}
