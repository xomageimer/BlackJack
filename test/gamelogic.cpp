#include <gtest/gtest.h>

#include <sstream>
#include <fstream>

#include "BlackJack.h"

using namespace std;

#define price GameCard::Cards::CardPrice
#define suit GameCard::Cards::CardSuit

TEST (One_Player, test_insurances_1){
    stringstream input {
        "BET 30\n"
        "YES\n"
        "BET 20\n"
    };
    stringstream answer;
    GameCard::TestCardStack fixed_stack(std::make_shared<GameCard::Mersenne_Generator>());
    std::shared_ptr<GameCard::ICardStack> fs (&fixed_stack);
    std::shared_ptr<Actors::IPlayer> player1(new Actors::ConsoleOfflinePlayer (1000, input));
    std::shared_ptr<Controller::SimpleDealer> dealer(new Controller::SimpleDealer(1'000'000, fs));

    BlackJack::Instance().PlayerConnect("Player1", player1);
    BlackJack::Instance().SetDealer(dealer);

    // TODO убрать все кроме превых 4 карт
    GameCard::GenerateCardPack(fixed_stack,
            GameCard::Cards(price::ACE, suit::CLUBS, false),
            GameCard::Cards(price::QUEEN, suit::CLUBS, false),
            GameCard::Cards(price::TWO, suit::CLUBS, false),
            GameCard::Cards(price::THREE, suit::CLUBS, false),
            GameCard::Cards(price::SEVEN, suit::CLUBS, false),
            GameCard::Cards(price::EIGHT, suit::CLUBS, false),
            GameCard::Cards(price::NINE, suit::CLUBS, false),
            GameCard::Cards(price::KING, suit::CLUBS, false),
            GameCard::Cards(price::SIX, suit::CLUBS, false)
            );

    stringstream expected {
            "Current State is BET for 0\n"
            "Player 0 bet: 30\n"
            "Current State is ROUNDED for all\n"
            "Dealer take a card: A♣\n"
            "Dealer take a card: #SECRET#\n"
            "Player 0 take a card: 2♣\n"
            "Player 0 take a card: 3♣\n"
            "Dealer have: A♣ #SECRET#  = 11\n"
            "Player 0 have: 2♣ 3♣  = 5\n"
            "Current State is DEAL for 0\n"
            "Current State is DealerMoves for all\n"
            "Dealer have BlackJack: A♣ Q♣ \n"
            "Player 0 save his bet, You get back: 30\n"
            "Current State is BET for 0\n"
            "Player 0 bet: 20\n"
    };

    BlackJack::Instance().Run(answer, 5);


    EXPECT_EQ(answer.str(), expected.str());
    ASSERT_EQ(player1->GetPlayerCost(), 1'000);
    ASSERT_EQ(dealer->GetPlayerCost(), 1'000'000);
}

TEST (One_Player, test_insurances_2){
    stringstream input {
            "BET 30\n"
            "YES\n"
            "HIT\n"
    };
    stringstream answer;
    GameCard::TestCardStack fixed_stack(std::make_shared<GameCard::Mersenne_Generator>());
    std::shared_ptr<GameCard::ICardStack> fs (&fixed_stack);
    std::shared_ptr<Actors::IPlayer> player1(new Actors::ConsoleOfflinePlayer (1000, input));
    std::shared_ptr<Controller::SimpleDealer> dealer(new Controller::SimpleDealer(1'000'000, fs));

    BlackJack::Instance().PlayerConnect("Player1", player1);
    BlackJack::Instance().SetDealer(dealer);

    // TODO убрать все кроме превых 4 карт
    GameCard::GenerateCardPack(fixed_stack,
            GameCard::Cards(price::ACE, suit::CLUBS, false),
            GameCard::Cards(price::ACE, suit::HEARTS, false),
            GameCard::Cards(price::TWO, suit::CLUBS, false),
            GameCard::Cards(price::THREE, suit::CLUBS, false),
            GameCard::Cards(price::SEVEN, suit::CLUBS, false),
            GameCard::Cards(price::EIGHT, suit::CLUBS, false),
            GameCard::Cards(price::NINE, suit::CLUBS, false),
            GameCard::Cards(price::KING, suit::CLUBS, false),
            GameCard::Cards(price::SIX, suit::CLUBS, false)
    );

    stringstream expected {
            "Current State is BET for 0\n"
            "Player 0 bet: 30\n"
            "Current State is ROUNDED for all\n"
            "Dealer take a card: A♣\n"
            "Dealer take a card: #SECRET#\n"
            "Player 0 take a card: 2♣\n"
            "Player 0 take a card: 3♣\n"
            "Dealer have: A♣ #SECRET#  = 11\n"
            "Player 0 have: 2♣ 3♣  = 5\n"
            "Current State is DEAL for 0\n"
            "Player 0 insurance lost: 15\n"
            "Current State is MOVE for 0\n"
            "Player 0 take a card: 7♣\n"
            "Dealer have: A♣ #SECRET#  = 11\n"
            "Player 0 have: 2♣ 3♣ 7♣  = 12\n"
    };

    BlackJack::Instance().Run(answer, 4);


    EXPECT_EQ(answer.str(), expected.str());
    ASSERT_EQ(player1->GetPlayerCost(), 985);
    ASSERT_EQ(dealer->GetPlayerCost(), 1'000'015);
}

TEST (One_Player, test_game_1){
    stringstream input {
            "BET 30\n"
            "HIT\n"
            "HIT\n"
    };
    stringstream answer;
    GameCard::TestCardStack fixed_stack(std::make_shared<GameCard::Mersenne_Generator>());
    std::shared_ptr<GameCard::ICardStack> fs (&fixed_stack);
    std::shared_ptr<Actors::IPlayer> player1(new Actors::ConsoleOfflinePlayer (1000, input));
    std::shared_ptr<Controller::SimpleDealer> dealer(new Controller::SimpleDealer(1'000'000, fs));

    BlackJack::Instance().PlayerConnect("Player1", player1);
    BlackJack::Instance().SetDealer(dealer);

    // TODO убрать все кроме превых 4 карт
    GameCard::GenerateCardPack(fixed_stack,
                               GameCard::Cards(price::THREE, suit::CLUBS, false),
                               GameCard::Cards(price::ACE, suit::HEARTS, false),
                               GameCard::Cards(price::TWO, suit::CLUBS, false),
                               GameCard::Cards(price::THREE, suit::CLUBS, false),
                               GameCard::Cards(price::ACE, suit::CLUBS, false),
                               GameCard::Cards(price::EIGHT, suit::CLUBS, false),
                               GameCard::Cards(price::NINE, suit::CLUBS, false),
                               GameCard::Cards(price::KING, suit::CLUBS, false),
                               GameCard::Cards(price::SIX, suit::CLUBS, false)
    );

    stringstream expected {
            "Current State is BET for 0\n"
            "Player 0 bet: 30\n"
            "Current State is ROUNDED for all\n"
            "Dealer take a card: 3♣\n"
            "Dealer take a card: #SECRET#\n"
            "Player 0 take a card: 2♣\n"
            "Player 0 take a card: 3♣\n"
            "Dealer have: 3♣ #SECRET#  = 3\n"
            "Player 0 have: 2♣ 3♣  = 5\n"
            "Current State is MOVE for 0\n"
            "Player 0 take a card: A♣\n"
            "Dealer have: 3♣ #SECRET#  = 3\n"
            "Player 0 have: 2♣ 3♣ A♣  = 16\n"
            "Current State is MOVE for 0\n"
            "Player 0 take a card: 8♣\n"
            "Dealer have: 3♣ #SECRET#  = 3\n"
            "Player 0 have: 2♣ 3♣ A♣ 8♣  = 14\n"
            "Current State is MOVE for 0\n"
            "Player 0 take a card: 9♣\n"
            "Dealer have: 3♣ #SECRET#  = 3\n"
            "Player 0 have: 2♣ 3♣ A♣ 8♣ 9♣  = 23\n"
            "Player 0 lose: 30\n"
            "Current State is MOVE for 0\n"
    };

    BlackJack::Instance().Run(answer, 6);


    EXPECT_EQ(answer.str(), expected.str());
    ASSERT_EQ(player1->GetPlayerCost(), 970);
    ASSERT_EQ(dealer->GetPlayerCost(), 1'000'030);
}

TEST (One_Player, test_game_2){
    stringstream input {
            "BET 30\n"
            "HIT\n"
            "STAND\n"
    };
    stringstream answer;
    GameCard::TestCardStack fixed_stack(std::make_shared<GameCard::Mersenne_Generator>());
    std::shared_ptr<GameCard::ICardStack> fs (&fixed_stack);
    std::shared_ptr<Actors::IPlayer> player1(new Actors::ConsoleOfflinePlayer (1000, input));
    std::shared_ptr<Controller::SimpleDealer> dealer(new Controller::SimpleDealer(1'000'000, fs));

    BlackJack::Instance().PlayerConnect("Player1", player1);
    BlackJack::Instance().SetDealer(dealer);

    // TODO убрать все кроме превых 4 карт
    GameCard::GenerateCardPack(fixed_stack,
                               GameCard::Cards(price::THREE, suit::CLUBS, false),
                               GameCard::Cards(price::ACE, suit::HEARTS, false),
                               GameCard::Cards(price::TWO, suit::CLUBS, false),
                               GameCard::Cards(price::THREE, suit::CLUBS, false),
                               GameCard::Cards(price::ACE, suit::CLUBS, false),
                               GameCard::Cards(price::EIGHT, suit::CLUBS, false),
                               GameCard::Cards(price::TEN, suit::CLUBS, false),
                               GameCard::Cards(price::KING, suit::CLUBS, false),
                               GameCard::Cards(price::SIX, suit::CLUBS, false)
    );

    stringstream expected {
            "Current State is BET for 0\n"
            "Player 0 bet: 30\n"
            "Current State is ROUNDED for all\n"
            "Dealer take a card: 3♣\n"
            "Dealer take a card: #SECRET#\n"
            "Player 0 take a card: 2♣\n"
            "Player 0 take a card: 3♣\n"
            "Dealer have: 3♣ #SECRET#  = 3\n"
            "Player 0 have: 2♣ 3♣  = 5\n"
            "Current State is MOVE for 0\n"
            "Player 0 take a card: A♣\n"
            "Dealer have: 3♣ #SECRET#  = 3\n"
            "Player 0 have: 2♣ 3♣ A♣  = 16\n"
            "Current State is MOVE for 0\n"
            "Current State is DealerMoves for all\n"
            "Dealer take a card: 8♣\n"
            "Dealer take a card: 10♣\n"
            "Current State is PlayOut for all\n"
            "Dealer have: 3♣ A♥ 8♣ 10♣  = 22\n"
            "Player 0 have: 2♣ 3♣ A♣  = 16\n"
            "Player 0 win: 60\n"
    };

    BlackJack::Instance().Run(answer, 6);


    EXPECT_EQ(answer.str(), expected.str());
    ASSERT_EQ(player1->GetPlayerCost(), 1'030);
    ASSERT_EQ(dealer->GetPlayerCost(), 999'970);
}

//#include <chrono>
//
//struct TimeChecker {
//public:
//    explicit TimeChecker(std::string mes = "") : message (mes){
//        start = std::chrono::steady_clock::now();
//    }
//
//    ~TimeChecker(){
//        auto end = std::chrono::steady_clock::now();
//        std::cout << std::string(message + " ") << double(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) / 1000 << " seconds" << std::endl;
//    }
//
//private:
//    std::string message;
//    std::chrono::steady_clock::time_point start;
//};

TEST (One_Player, test_time){
    std::vector<string> input {"BET 30\n", "HIT\n", "STAND\n"};

    stringstream inp;

    size_t count = 5'000;

    for (size_t i = 0; i < count; i++){
        inp << input[i % 3];
    }

    stringstream bucket;

    std::shared_ptr<Actors::IPlayer> player1(new Actors::ConsoleOfflinePlayer (1000, inp));
    std::shared_ptr<Controller::SimpleDealer> dealer(new Controller::SimpleDealer(1'000'000));

    BlackJack::Instance().PlayerConnect("Player1", player1);
    BlackJack::Instance().SetDealer(dealer);

    BlackJack::Instance().Run(bucket, count);

    ASSERT_EQ(1, 1);
}