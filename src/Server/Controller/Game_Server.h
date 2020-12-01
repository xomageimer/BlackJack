#ifndef BLACKJACK_GAME_SERVER_H
#define BLACKJACK_GAME_SERVER_H

#include <utility>

#include "Server/Controller/Dealer.h"
#include "Game_Room.h"

using boost::asio::ip::tcp;

struct Game_Session : public player_participant,
                        public std::enable_shared_from_this<Game_Session>{
public:
    Game_Session(tcp::socket socket, Game_Room& room, std::shared_ptr<Controller::IDealer> ur_dealer)
            : socket_(std::move(socket)),
              room_(room), dealer(std::move(ur_dealer)) {
        dealer->SetRoom(&room_);
        room_.SetDealer(dealer);
    }

    void start()
    {
        room_.join(shared_from_this());
        do_read_body();
    }

    void deliver(const std::string& msg) override
    {
        bool write_in_progress = !write_msgs_.empty();
        write_msgs_.push_back(msg);
        if (!write_in_progress)
        {
            do_write();
        }
    }

private:
    void do_read_body()
    {
        auto self(shared_from_this());
        boost::asio::async_read_until(socket_,
          read_msg_,
          "\r\n\r\n",
          [this, self](boost::system::error_code ec, std::size_t /*length*/)
          {
            if (!ec)
            {
                std::string str;
                std::istream(&read_msg_) >> str;
                if (!str.empty()) {
                    std::cerr << str << std::endl;
                    dealer->Maker(std::string(str));
                    dealer->Process();
                }
                do_read_body();
            }
            else
            {
                room_.leave(shared_from_this());
            }
        });
    }

    void do_write()
    {
        auto self(shared_from_this());
        boost::asio::async_write(socket_,
                                 boost::asio::buffer(write_msgs_.front()),
         [this, self](boost::system::error_code ec, std::size_t /*length*/)
         {
             if (!ec)
             {
                 write_msgs_.pop_front();
                 if (!write_msgs_.empty())
                 {
                     do_write();
                 }
             }
             else
             {
                 room_.leave(shared_from_this());
             }
         });
    }

    tcp::socket socket_;
    Game_Room& room_;
    boost::asio::streambuf read_msg_;
    json_message_queue write_msgs_;

    std::shared_ptr<Controller::IDealer> dealer;
};

struct Game_Server {
public:
    Game_Server(boost::asio::io_service& io_service,
                const tcp::endpoint& endpoint,
                std::shared_ptr<Controller::IDealer> dealer)
            : acceptor_(io_service, endpoint),
              socket_(io_service), dealer_(std::move(dealer))
    {
        do_accept();
    }
private:
    void do_accept()
    {
        acceptor_.async_accept(socket_,
           [this](boost::system::error_code ec)
           {
               if (!ec)
               {
                   std::make_shared<Game_Session>(std::move(socket_), room_, dealer_)->start();
               }

               do_accept();
           });
    }

    tcp::acceptor acceptor_;
    tcp::socket socket_;
    Game_Room room_;

    std::shared_ptr<Controller::IDealer> dealer_;
};

#endif //BLACKJACK_GAME_SERVER_H
