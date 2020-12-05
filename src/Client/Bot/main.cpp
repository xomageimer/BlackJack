#undef BOOST_ASIO_ENABLE_HANDLER_TRACKING

#include "OutputManager.h"
#include "Client/Bot/TCP_Bot_Client.h"

int main(int argc, char* argv[])
{

        if (argc != 4)
        {
            std::cerr << "Usage: chat_client <host> <port> <bot_name>\n";
            return 1;
        }

        boost::asio::io_service io_service;

        tcp::resolver resolver(io_service);
        auto endpoint_iterator = resolver.resolve({ argv[1], argv[2] });

        std::shared_ptr<OutputManager> manager = std::make_shared<OutputManager>();
        manager->subscribe("Logger", std::make_shared<ConsoleLogger>(std::cout));

        TCP_Bot_Client c(1'000, io_service, endpoint_iterator, argv[3]);
        c.SetManager(manager);

        std::thread t([&io_service](){ io_service.run(); });

//        std::string line;
//        for (std::string line; std::getline(std::cin, line);)
//        {

        c.SetName();

        while (true) {
            c.Process();
        }
//        }

        c.close();
        t.join();


    return 0;
}