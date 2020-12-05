#ifndef BLACKJACK_TCP_BOT_CLIENT_H
#define BLACKJACK_TCP_BOT_CLIENT_H

#include <set>
#include "Client/Player/TCP_Player_Client.h"

struct TCP_Bot_Client : public player_client, public Actors::IPlayer {
protected:
    GameCard::Hand m_hand;
    GameCard::Hand dealer_hand;
    std::map<GameCard::Cards, int> round_cards;
    std::vector<int> my_banks;

    bool is_ace = false;

    std::map<GameCard::Cards::CardPrice, int> lefts_cards;

    inline static std::map<int, std::map<int, std::string>> tactic {
            { 2, { {4, "HIT"}, {5, "HIT"}, {6, "HIT"}, {7, "HIT"}, {8, "HIT"}, {9, "HIT"}, {10, "DOUBLEDOWN"}, {11, "DOUBLEDOWN"}, {12, "HIT"}, {13, "STAND"}, {14, "STAND"}, {15, "STAND"}, {16, "STAND"}, {17, "STAND"} } },
            {3, { {4, "HIT"}, {5, "HIT"}, {6, "HIT"}, {7, "HIT"}, {8, "HIT"}, {9, "DOUBLEDOWN"}, {10, "DOUBLEDOWN"}, {11, "DOUBLEDOWN"}, {12, "HIT"}, {13, "STAND"}, {14, "STAND"}, {15, "STAND"}, {16, "STAND"}, {17, "STAND"} } },
            {4, { {4, "HIT"}, {5, "HIT"}, {6, "HIT"}, {7, "HIT"}, {8, "HIT"}, {9, "DOUBLEDOWN"}, {10, "DOUBLEDOWN"}, {11, "DOUBLEDOWN"}, {12, "STAND"}, {13, "STAND"}, {14, "STAND"}, {15, "STAND"}, {16, "STAND"}, {17, "STAND"} } },
            {5, { {4, "HIT"}, {5, "HIT"}, {6, "HIT"}, {7, "HIT"}, {8, "HIT"}, {9, "DOUBLEDOWN"}, {10, "DOUBLEDOWN"}, {11, "DOUBLEDOWN"}, {12, "STAND"}, {13, "STAND"}, {14, "STAND"}, {15, "STAND"}, {16, "STAND"}, {17, "STAND"} } },
            {6, { {4, "HIT"}, {5, "HIT"}, {6, "HIT"}, {7, "HIT"}, {8, "HIT"}, {9, "DOUBLEDOWN"}, {10, "DOUBLEDOWN"}, {11, "DOUBLEDOWN"}, {12, "STAND"}, {13, "STAND"}, {14, "STAND"}, {15, "STAND"}, {16, "STAND"}, {17, "STAND"} } },
            {7, { {4, "HIT"}, {5, "HIT"}, {6, "HIT"}, {7, "HIT"}, {8, "HIT"}, {9, "HIT"}, {10, "DOUBLEDOWN"}, {11, "DOUBLEDOWN"}, {12, "HIT"}, {13, "HIT"}, {14, "HIT"}, {15, "HIT"}, {16, "HIT"}, {17, "STAND"} } },
            {8, { {4, "HIT"}, {5, "HIT"}, {6, "HIT"}, {7, "HIT"}, {8, "HIT"}, {9, "HIT"}, {10, "DOUBLEDOWN"}, {11, "DOUBLEDOWN"}, {12, "HIT"}, {13, "HIT"}, {14, "HIT"}, {15, "HIT"}, {16, "HIT"}, {17, "STAND"} } },
            {9, { {4, "HIT"}, {5, "HIT"}, {6, "HIT"}, {7, "HIT"}, {8, "HIT"}, {9, "HIT"}, {10, "DOUBLEDOWN"}, {11, "DOUBLEDOWN"}, {12, "HIT"}, {13, "HIT"}, {14, "HIT"}, {15, "HIT"}, {16, "HIT"}, {17, "STAND"}  } },
            {10, { {4, "HIT"}, {5, "HIT"}, {6, "HIT"}, {7, "HIT"}, {8, "HIT"}, {9, "HIT"}, {10, "HIT"}, {11, "DOUBLEDOWN"}, {12, "HIT"}, {13, "HIT"}, {14, "HIT"}, {15, "HIT"}, {16, "HIT"}, {17, "STAND"}  } },
            {11, { {4, "HIT"}, {5, "HIT"}, {6, "HIT"}, {7, "HIT"}, {8, "HIT"}, {9, "HIT"}, {10, "HIT"}, {11, "HIT"}, {12, "HIT"}, {13, "HIT"}, {14, "HIT"}, {15, "HIT"}, {16, "HIT"}, {17, "STAND"}  } },
    };

    inline static std::map<int, std::map<int, std::string>> tactic_with_ace {
            { 2, { {19, "STAND"}, {20, "STAND"}, {18, "STAND"}, {17, "HIT"}, {16, "HIT"}, {15, "HIT"}, {14, "HIT"}, {13, "HIT"} } },
            { 3, { {19, "STAND"}, {20, "STAND"}, {18, "DOUBLEDOWN"}, {17, "DOUBLEDOWN"}, {16, "HIT"}, {15, "HIT"}, {14, "HIT"}, {13, "HIT"}  } },
            { 4, {  {19, "STAND"}, {20, "STAND"}, {18, "DOUBLEDOWN"}, {17, "DOUBLEDOWN"}, {16, "DOUBLEDOWN"}, {15, "DOUBLEDOWN"}, {14, "HIT"}, {13, "HIT"}  } },
            { 5, { {19, "STAND"}, {20, "STAND"}, {18, "DOUBLEDOWN"}, {17, "DOUBLEDOWN"}, {16, "DOUBLEDOWN"}, {15, "DOUBLEDOWN"}, {14, "DOUBLEDOWN"}, {13, "DOUBLEDOWN"} } },
            { 6, { {19, "STAND"}, {20, "STAND"}, {18, "DOUBLEDOWN"}, {17, "DOUBLEDOWN"}, {16, "DOUBLEDOWN"}, {15, "DOUBLEDOWN"}, {14, "DOUBLEDOWN"}, {13, "DOUBLEDOWN"}  } },
            { 7, { {19, "STAND"}, {20, "STAND"}, {18, "STAND"}, {17, "HIT"}, {16, "HIT"}, {15, "HIT"}, {14, "HIT"}, {13, "HIT"}  } },
            { 8, { {19, "STAND"}, {20, "STAND"}, {18, "STAND"}, {17, "HIT"}, {16, "HIT"}, {15, "HIT"}, {14, "HIT"}, {13, "HIT"} } },
            { 9, { {19, "STAND"}, {20, "STAND"}, {18, "HIT"}, {17, "HIT"}, {16, "HIT"}, {15, "HIT"}, {14, "HIT"}, {13, "HIT"}  } },
            {10, {  {19, "STAND"}, {20, "STAND"}, {18, "HIT"}, {17, "HIT"}, {16, "HIT"}, {15, "HIT"}, {14, "HIT"}, {13, "HIT"}   } },
            {11, {  {19, "STAND"}, {20, "STAND"}, {18, "HIT"}, {17, "HIT"}, {16, "HIT"}, {15, "HIT"}, {14, "HIT"}, {13, "HIT"}   } }
    };

    std::shared_ptr<OutputManager> om;

    int m_bank;

    enum class states : int {
        BET,
        MOVE,
        DEAL,
        NOTHING
    };
    states cur_state = states::NOTHING;


    void Collect(const json & vec);
    void UpdateLeftCards();
    bool Mathematical_Expectation() const;

public:
    explicit TCP_Bot_Client(int bank, boost::asio::io_service& io_service,
                               tcp::resolver::iterator endpoint_iterator, const std::string & name) : player_client(io_service, endpoint_iterator), m_bank(bank) {
        Actors::IPlayer::SetName(name);
    }

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

    void SetName();

    void Move() override;
    void Bet() override;
    void Answer() override;

    void SetCard(const GameCard::Cards &) override;
    void SetDealerCard(const GameCard::Cards &);
    [[nodiscard]] bool BlackJackCheck() const override;
    void GetRoundResult(int) override;

    void ClearHand() override;

    [[nodiscard]] const GameCard::Hand &ShowHand() const override;
    [[nodiscard]] int GetPlayerCost() const override;

};


#endif //BLACKJACK_TCP_BOT_CLIENT_H
