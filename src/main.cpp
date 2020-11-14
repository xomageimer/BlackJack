#include <iostream>

#include "BlackJack.h"

int main() {
    using namespace std;

//    GameCard::CardStack stack(std::make_shared<GameCard::Mersenne_Generator>());
//    stack.GenNewStacks();
//    GameCard::Hand hand;
//    hand.SetNewCard(stack.GetCard());
//    hand.SetNewCard(stack.GetCard());
//
//    hand.MakeSecret(1);
//
//    for (auto & i : hand.LookAtCards()){
//        std::cout << std::string(i) << ' ';
//    }
//
//    hand.UnSecret(1);
//
//    for (auto & i : hand.LookAtCards()){
//        std::cout << std::string(i) << ' ';
//    }

    std::shared_ptr<Actors::OfflinePlayer> player1(new Actors::OfflinePlayer(1000));
    std::shared_ptr<Controller::SimpleDealer> dealer(new Controller::SimpleDealer(1000));

    BlackJack::Instance().PlayerConnect("Player1", player1);
    BlackJack::Instance().SetDealer(dealer);

    BlackJack::Instance().Run(cin, cout);

    return 0;
}