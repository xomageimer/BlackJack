#undef BOOST_ASIO_ENABLE_HANDLER_TRACKING

#include "OutputManager.h"
#include "TCP_Player_Client.h"

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 3)
        {
            std::cerr << "Usage: chat_client <host> <port>\n";
            return 1;
        }

        boost::asio::io_service io_service;

        tcp::resolver resolver(io_service);
        auto endpoint_iterator = resolver.resolve({ argv[1], argv[2]});

        std::shared_ptr<OutputManager> manager = std::make_shared<OutputManager>();
        manager->subscribe("Logger", std::make_shared<ConsoleLogger>(std::cout));

        TCP_Player_Client c(1'000, io_service, endpoint_iterator);
        c.SetManager(manager);

        std::thread t([&io_service](){ io_service.run(); });

        c.SetName();

        while (true) {
            c.Process();
        }

        c.close();
        t.join();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}