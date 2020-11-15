#include <iostream>

#include "BlackJack.h"

int main() {
    using namespace std;

    std::shared_ptr<Actors::IPlayer> player1(new Actors::ConsoleOfflinePlayer (1000));
    std::shared_ptr<Controller::SimpleDealer> dealer(new Controller::SimpleDealer(1000));

    BlackJack::Instance().PlayerConnect("Player1", player1);
    BlackJack::Instance().SetDealer(dealer);

    BlackJack::Instance().Run(cout);

    return 0;
}