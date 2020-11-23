#include <iostream>

#include "BlackJack.h"

int main() {
    using namespace std;

//    std::shared_ptr<Actors::IPlayer> player1(new Actors::ConsoleOfflinePlayer (1000));
//    std::shared_ptr<Actors::IPlayer> player2(new Actors::ConsoleOfflinePlayer (700));
    std::shared_ptr<Actors::IPlayer> player3(new Controller::SimpleDealer(1500));
    std::shared_ptr<Controller::SimpleDealer> dealer(new Controller::SimpleDealer(1'000'000));

//    BlackJack::Instance().PlayerConnect("Player1", player1);
//    BlackJack::Instance().PlayerConnect("Player2", player2);
    BlackJack::Instance().PlayerConnect("Player3", player3);
    BlackJack::Instance().SetDealer(dealer);

    BlackJack::Instance().Run(cout);

    return 0;
}