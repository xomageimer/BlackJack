#ifndef BLACKJACK_GAME_ROOM_H
#define BLACKJACK_GAME_ROOM_H

#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include <boost/asio.hpp>

#include "Server/Controller/Dealer.h"
#include "Actors/IPlayer.h"

using boost::asio::ip::tcp;

const size_t MAX_PLAYER_COUNT = 7;

typedef std::deque<json_message> json_message_queue ;

struct player_participant
{
protected:
    std::string name = "Player";
    int my_id = 0;
public:
    void set_id(int id){
        my_id = id;
    }
    int get_id() const {
        return my_id;
    }
    virtual ~player_participant() {}
    virtual void deliver(const json_message& msg) = 0;
};

typedef std::shared_ptr<player_participant> player_participant_ptr;

struct Game_Room {
private:
    std::map<std::string, std::shared_ptr<Actors::IPlayer>> players;
    std::vector<std::string> queue {};
public:

    bool SubscribePlayer(std::string player_nickname, std::shared_ptr<Actors::IPlayer> new_player);
    bool UnSubscribePlayer(const std::string &player_nickname);

    void join( player_participant_ptr participant)
    {
        participant->set_id(count);
        participants_.emplace(count++, participant);
        for (auto msg: recent_msgs_)
            participant->deliver(msg);
    }

    void leave( player_participant_ptr participant)
    {
        auto id = participant->get_id();
        participants_.erase(participant->get_id());
    }

    void deliver(const json_message& msg, int num)
    {
        recent_msgs_.push_back(msg);
        while (recent_msgs_.size() > max_recent_msgs)
            recent_msgs_.pop_front();

        participants_[num]->deliver(msg);
    }

    void deliver(const json_message& msg)
    {
        recent_msgs_.push_back(msg);
        while (recent_msgs_.size() > max_recent_msgs)
            recent_msgs_.pop_front();

        for (auto participant: participants_)
          participant.second->deliver(msg);

    }

private:
    std::unordered_map<int, player_participant_ptr> participants_;
    enum { max_recent_msgs = 100 };
    json_message_queue recent_msgs_;

    int count = 0;
};


#endif //BLACKJACK_GAME_ROOM_H
