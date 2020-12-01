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

#include "Actors/IPlayer.h"

namespace Controller{
    struct IDealer;
}

using boost::asio::ip::tcp;

const size_t MAX_PLAYER_COUNT = 7;

typedef std::deque<std::string> json_message_queue ;

struct player_participant
{
protected:
    std::string name = "Player";
    int my_id = 0;
public:
    void set_id(int id){
        my_id = id;
    }
    [[nodiscard]] int get_id() const {
        return my_id;
    }

    void set_name(const std::string & new_name){
        name = new_name;
    }
    [[nodiscard]] std::string get_name() const {
        return name;
    }

    virtual ~player_participant() = default;
    virtual void deliver(const std::string& msg) = 0;
};

typedef std::shared_ptr<player_participant> player_participant_ptr;

struct Game_Room {
private:
    std::shared_ptr<Controller::IDealer> dealer;

    std::map<std::string, std::shared_ptr<Actors::IPlayer>> players;
    std::vector<std::string> queue {};
    std::vector<int> vacancy;
public:

    bool SubscribePlayer(std::string player_nickname, std::shared_ptr<Actors::IPlayer> new_player);
    bool UnSubscribePlayer(const std::string &player_nickname);

    void NewRound();

    void Notify_result();

    void SetDealer(std::shared_ptr<Controller::IDealer> dealer);

    void join( player_participant_ptr participant)
    {
        if (queue.size() < MAX_PLAYER_COUNT) {
            if (!vacancy.empty()) {
                participant->set_id(vacancy.back());
                participants_.emplace(vacancy.back(), participant);
                vacancy.pop_back();
            } else {
                participant->set_id(count);
                participants_.emplace(count++, participant);
            }
            SubscribePlayer(participant->get_name(), std::make_shared<Actors::Player>(1'000));
            for (auto msg: recent_msgs_)
                participant->deliver(msg);
        }
    }

    void leave( player_participant_ptr participant)
    {
        if (!queue.empty()) {
            UnSubscribePlayer(participant->get_name());
            participants_.erase(participant->get_id());
        }
    }

    void deliver(const std::string& msg, int num)
    {
        recent_msgs_.push_back(msg);
        while (recent_msgs_.size() > max_recent_msgs)
            recent_msgs_.pop_front();

        participants_[num]->deliver(msg + "\r\n\r\n");
    }

    void deliver(const std::string& msg)
    {
        recent_msgs_.push_back(msg);
        while (recent_msgs_.size() > max_recent_msgs)
            recent_msgs_.pop_front();

        for (auto participant: participants_)
          participant.second->deliver(msg + "\r\n\r\n");
    }

private:
    std::unordered_map<int, player_participant_ptr> participants_;
    enum { max_recent_msgs = 100 };
    json_message_queue recent_msgs_;

    int count = 0;
};


#endif //BLACKJACK_GAME_ROOM_H
