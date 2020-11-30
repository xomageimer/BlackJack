#ifndef BLACKJACK_TCP_PLAYER_CLIENT_H
#define BLACKJACK_TCP_PLAYER_CLIENT_H

#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>

#include "Actors/IPlayer.h"

using boost::asio::ip::tcp;

typedef std::deque<std::string> chat_message_queue;

class player_client
{
public:
    player_client(boost::asio::io_service& io_service,
                tcp::resolver::iterator endpoint_iterator)
            : io_service_(io_service),
              socket_(io_service)
    {
        do_connect(endpoint_iterator);
    }

    void write(std::string& msg)
    {
        msg += "\r\n\r\n";
        io_service_.post(
            [this, msg]()
            {
                bool write_in_progress = !write_msgs_.empty();
                write_msgs_.push_back(msg);
                if (!write_in_progress)
                {
                    do_write();
                }
            });
    }

    void close()
    {
        io_service_.post([this]() { socket_.close(); });
    }

private:
    void do_connect(tcp::resolver::iterator endpoint_iterator)
    {
        boost::asio::async_connect(socket_, endpoint_iterator,
               [this](boost::system::error_code ec, tcp::resolver::iterator)
               {
                   if (!ec)
                   {
                       do_read_body();
                   }
               });
    }

    void do_read_body()
    {
        boost::asio::async_read_until(socket_,
              read_msg_,
              "\r\n\r\n",
              [this](boost::system::error_code ec, std::size_t /*length*/)
              {
                  if (!ec)
                  {
                      std::string myString;
                      std::istream(&read_msg_) >> myString;
                      std::cout << myString << std::endl;
                      std::cout << "\n";
                      do_read_body();
                  }
                  else
                  {
                      socket_.close();
                  }
              });
    }

    void do_write()
    {
        boost::asio::async_write(socket_,
             boost::asio::buffer(write_msgs_.front().data(),
                                 write_msgs_.front().length()),
             [this](boost::system::error_code ec, std::size_t /*length*/)
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
                     socket_.close();
                 }
             });
    }

private:
    boost::asio::io_service& io_service_;
    tcp::socket socket_;
    boost::asio::streambuf read_msg_;
    chat_message_queue write_msgs_;
};


struct TCP_Player_Client :  public player_client {
protected:

public:
    explicit TCP_Player_Client(int bank, boost::asio::io_service& io_service,
                               tcp::resolver::iterator endpoint_iterator) : player_client(io_service, endpoint_iterator) {}

};


#endif //BLACKJACK_TCP_PLAYER_CLIENT_H
