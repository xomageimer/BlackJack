#include <iostream>
#include <iostream>


#include "BlackJack.h"


int main() {
    using namespace std;

    std::shared_ptr<RelationshipController> controller(new RelationshipController());
    std::shared_ptr<Actors::OfflinePlayer> player1(new Actors::OfflinePlayer(1000.f));
    std::shared_ptr<Actors::SimpleDealer> dealer(new Actors::SimpleDealer(controller, 1000.f));

    BlackJack::Instance().SetController(controller);
    BlackJack::Instance().PlayerConnect("Player1", player1);
    BlackJack::Instance().SetDealer(dealer);

    BlackJack::Instance().Run(cin, cout);

    return 0;
}
