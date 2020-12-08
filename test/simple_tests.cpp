#include <gtest/gtest.h>

#include <sstream>
#include <fstream>

#include "BlackJack.h"

using namespace std;

TEST (cards, SecretFuncs){
    std::stringstream answer;
    GameCard::CardStack stack(std::make_shared<GameCard::Mersenne_Generator>());
    stack.GenNewStacks();
    GameCard::Hand hand;

    auto card1 = stack.GetCard();
    auto card2 = stack.GetCard();
    hand.SetNewCard(card1);
    hand.SetNewCard(card2);
    hand.MakeSecret(1);

    for (auto & i : hand.LookAtCards()){
        answer << std::string(i) << ' ';
    }

    std::stringstream expected {
            string(card1) + " #SECRET# "
    };
    EXPECT_EQ(answer.str(), expected.str());
}

TEST (cards, UnsecretFuncs){
    std::stringstream answer;
    GameCard::CardStack stack(std::make_shared<GameCard::Mersenne_Generator>());
    stack.GenNewStacks();
    GameCard::Hand hand;

    auto card1 = stack.GetCard();
    auto card2 = stack.GetCard();
    hand.SetNewCard(card1);
    hand.SetNewCard(card2);
    hand.MakeSecret(1);

    hand.UnSecret(1);

    for (auto & i : hand.LookAtCards()){
        answer << std::string(i) << ' ';
    }

    std::stringstream expected {
            string(card1) + " " + string(card2) + " "
    };
    EXPECT_EQ(answer.str(), expected.str());
}

TEST (cards, Operation_1){
    std::stringstream answer;

    GameCard::Hand hand1;
    GameCard::Hand hand2;

    GameCard::Cards card (GameCard::Cards::CardPrice::ACE, GameCard::Cards::CardSuit::CLUBS, false);

    hand1.SetNewCard(GameCard::Cards(GameCard::Cards::CardPrice::ACE, GameCard::Cards::CardSuit::CLUBS, false));
    hand2.SetNewCard(GameCard::Cards(GameCard::Cards::CardPrice::TWO, GameCard::Cards::CardSuit::HEARTS, false));

    answer << ((hand1 > hand2) ? "true" : "false");

    std::stringstream expected {
            "true"
    };
    EXPECT_EQ(answer.str(), expected.str());
}

TEST (cards, Operation_2){
    std::stringstream answer;

    GameCard::Hand hand1;
    GameCard::Hand hand2;

    hand1.SetNewCard(GameCard::Cards(GameCard::Cards::CardPrice::ACE, GameCard::Cards::CardSuit::CLUBS, false));
    hand2.SetNewCard(GameCard::Cards(GameCard::Cards::CardPrice::TWO, GameCard::Cards::CardSuit::HEARTS, false));

    answer << ((hand1 < hand2) ? "true" : "false");

    std::stringstream expected {
            "false"
    };
    EXPECT_EQ(answer.str(), expected.str());
}

TEST (cards, Operation_3){
    std::stringstream answer;

    GameCard::Hand hand1;
    GameCard::Hand hand2;

    hand1.SetNewCard(GameCard::Cards(GameCard::Cards::CardPrice::ACE, GameCard::Cards::CardSuit::CLUBS, false));
    hand2.SetNewCard(GameCard::Cards(GameCard::Cards::CardPrice::TWO, GameCard::Cards::CardSuit::HEARTS, false));
    hand2.SetNewCard(GameCard::Cards(GameCard::Cards::CardPrice::NINE, GameCard::Cards::CardSuit::DIAMONDS, false));

    answer << ((hand1 == hand2) ? "true" : "false");

    std::stringstream expected {
            "true"
    };
    EXPECT_EQ(answer.str(), expected.str());
}

TEST (cards, Operation_4){
    std::stringstream answer;

    GameCard::Hand hand;

    hand.SetNewCard(GameCard::Cards(GameCard::Cards::CardPrice::ACE, GameCard::Cards::CardSuit::CLUBS, false));
    hand.SetNewCard(GameCard::Cards(GameCard::Cards::CardPrice::JACK, GameCard::Cards::CardSuit::CLUBS, false));

    answer << ((hand == 21) ? "true" : "false");

    std::stringstream expected {
            "true"
    };
    EXPECT_EQ(answer.str(), expected.str());
}

TEST (cards, Operation_5){
    std::stringstream answer;

    GameCard::Hand hand;

    hand.SetNewCard(GameCard::Cards(GameCard::Cards::CardPrice::ACE, GameCard::Cards::CardSuit::CLUBS, false));
    hand.SetNewCard(GameCard::Cards(GameCard::Cards::CardPrice::JACK, GameCard::Cards::CardSuit::CLUBS, false));
    hand.SetNewCard(GameCard::Cards(GameCard::Cards::CardPrice::KING, GameCard::Cards::CardSuit::HEARTS, false));

    answer << ((hand == 21) ? "true" : "false");

    std::stringstream expected {
            "true"
    };
    EXPECT_EQ(answer.str(), expected.str());
}

TEST (cards, Operation_6_ACE_pair){
    std::stringstream answer;

    GameCard::Hand hand;

    hand.SetNewCard(GameCard::Cards(GameCard::Cards::CardPrice::ACE, GameCard::Cards::CardSuit::CLUBS, false));
    hand.SetNewCard(GameCard::Cards(GameCard::Cards::CardPrice::ACE, GameCard::Cards::CardSuit::CLUBS, false));

    answer << ((hand == 12) ? "true" : "false");

    std::stringstream expected {
            "true"
    };
    EXPECT_EQ(answer.str(), expected.str());
}