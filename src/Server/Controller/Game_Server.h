#ifndef BLACKJACK_GAME_SERVER_H
#define BLACKJACK_GAME_SERVER_H

#include <utility>

#include "json_message.h"

#include "Server/Controller/Dealer.h"
#include "Game_Room.h"

using boost::asio::ip::tcp;

struct Game_Session : public player_participant,
                        public std::enable_shared_from_this<Game_Session>{
public:
    Game_Session(tcp::socket socket, Game_Room& room, std::shared_ptr<Controller::IDealer> ur_dealer)
            : socket_(std::move(socket)),
              room_(room), dealer(ur_dealer) {
        dealer->SetRoom(&room_);
    }

    void start()
    {
        room_.join(shared_from_this());

        do_read_header();
    }

    void deliver(const json_message& msg) override
    {
        bool write_in_progress = !write_msgs_.empty();
        write_msgs_.push_back(msg);
        if (!write_in_progress)
        {
            do_write();
        }
    }

private:
    void do_read_header()
    {
        auto self(shared_from_this());
        boost::asio::async_read(socket_,
        boost::asio::buffer(read_msg_.data(), json_message::header_length),
        [this, self](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (!ec && read_msg_.decode_header())
            {
                do_read_body();
            }
            else
            {
                room_.leave(shared_from_this());
            }
        });
    }

    void do_read_body()
    {
        auto self(shared_from_this());
        boost::asio::async_read(socket_,
        boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
        [this, self](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (!ec)
            {
                //room_.deliver(read_msg_, current);
                dealer->Maker(std::string(read_msg_.body(), read_msg_.body_length()));
                //std::cout << std::string(read_msg_.body(), read_msg_.body_length()) << std::endl;
                do_read_header();
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
         boost::asio::buffer(write_msgs_.front().data(),
                             write_msgs_.front().length()),
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
    json_message read_msg_;
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
