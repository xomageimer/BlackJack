#ifndef BLACKJACK_TCP_PLAYER_CLIENT_H
#define BLACKJACK_TCP_PLAYER_CLIENT_H

#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>

#include "Actors/IPlayer.h"
#include "Client/View/OutputManager.h"

#include "Graphic/SpriteAnimator.h"
#include "Objects/Net_Player.h"
#include "Objects/Button.h"

#define DEFAULT {               \
    throw std::logic_error(""); \
}                               \

using boost::asio::ip::tcp;

typedef std::deque<std::string> message_queue;

#define BLACKJACK 21

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

    void write(const std::string& msg)
    {
        auto t_msg = msg + "\r\n\r\n";
        io_service_.post(
            [this, t_msg]()
            {
                bool write_in_progress = !write_msgs_.empty();
                write_msgs_.push_back(t_msg);
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

protected:
    void do_connect(tcp::resolver::iterator endpoint_iterator)
    {
        using namespace std::chrono_literals;
        boost::asio::async_connect(socket_, endpoint_iterator,
               [this](boost::system::error_code ec, tcp::resolver::iterator)
               {
                   if (!ec)
                   {
                       do_read_body();
                   }
                   std::this_thread::sleep_for(3s);
               });
    }

    virtual void do_read_body() = 0;

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

protected:
    boost::asio::io_service& io_service_;
    tcp::socket socket_;
    boost::asio::streambuf read_msg_;
    message_queue write_msgs_;
};


struct TCP_Player_Client : public player_client, public Actors::IPlayer,  public Net_Player{
protected:
    GameCard::Hand m_hand;
    int m_bank;
    bool authorize = false;

    std::shared_ptr<OutputManager> om;

    enum class states : int {
        BET,
        MOVE,
        DEAL,

        NOTHING
    };
    states cur_state = states::NOTHING;

    void Collect(const json & vec);

public:
    explicit TCP_Player_Client(int bank, boost::asio::io_service& io_service,
                               tcp::resolver::iterator endpoint_iterator) : player_client(io_service, std::move(endpoint_iterator)), Net_Player(glm::vec2{0.f, 0.f}, glm::vec2{0.f, 0.f}), m_bank(bank) {}

    void do_read_body() override
    {
        boost::asio::async_read_until(socket_,
              read_msg_,
              "\r\n\r\n",
              [this](boost::system::error_code ec, std::size_t bytes_transferred/*length*/)
              {
                  if (!ec)
                  {
                      std::string str{
                              boost::asio::buffers_begin(read_msg_.data()),
                                                         boost::asio::buffers_begin(read_msg_.data()) + bytes_transferred
                                                         - std::string("\r\n\r\n").size()};
                      read_msg_.consume(bytes_transferred);
                      Request(str);
                      do_read_body();
                  }
                  else
                  {
                      socket_.close();
                  }
              });
    }

    void Request(std::string str);

    void SetManager(std::shared_ptr<OutputManager> manager);

    void Process();

    void SetName(const std::string & name) override;

    inline bool Is_Authorize() const{
        return authorize;
    }

    void Move() override;
    void Bet() override;
    void Answer() override;

    void SetCard(const GameCard::Cards &) override;
    [[nodiscard]] bool BlackJackCheck() const override;
    void GetRoundResult(int) override;

    void ClearHand() override;

    [[nodiscard]] const GameCard::Hand &ShowHand() const override;
    [[nodiscard]] int GetPlayerCost() const override;

    void mouse_controller(GLFWwindow* window, double xpos, double ypos);
private:
    std::vector<Button> buttons;

    glm::vec2 mouse_position;
};


#endif //BLACKJACK_TCP_PLAYER_CLIENT_H
