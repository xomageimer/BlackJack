#include <iostream>
#include "BlackJack.h"

void BlackJack::Run(int argc, char* argv[]) {
    using boost::asio::ip::tcp;

    boost::asio::io_service io_service;

    std::list<Game_Server> servers;
    for (int i = 1; i < argc; ++i)
    {
        tcp::endpoint endpoint(tcp::v4(), std::atoi(argv[i]));
        servers.emplace_back(io_service, endpoint, dealer);
    }

    io_service.run();

//    _grounds->Execute();
}