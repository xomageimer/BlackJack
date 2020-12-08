#include "BlackJack.h"

int main(int argc, char* argv[]) {
    using namespace std;
    try
    {
        if (argc < 2)
        {
            std::cerr << "Usage: chat_server <port> [<port> ...]\n";
            return 1;
        }

        std::shared_ptr<Controller::SimpleDealer> dealer(new Controller::SimpleDealer(1'000'000));
        BlackJack::Instance().SetDealer(dealer);
        BlackJack::Instance().Run(argc, argv);
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}