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
#include <atomic>

#include "Actors/IPlayer.h"
#include "nlohmann/json.hpp"

namespace Controller{
    struct IDealer;
}

using json = nlohmann::json;

using boost::asio::ip::tcp;

const size_t MAX_PLAYER_COUNT = 7;

const size_t max_rounds = 1'000;

typedef std::deque<std::string> json_message_queue ;

struct player_participant
{
protected:
    bool is_Authorize = false;
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
        json j_name = json::parse(new_name);
        name = j_name["data"]["name"];
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

    std::map<int, std::shared_ptr<Actors::IPlayer>> players;
    std::vector<std::string> queue {};
    std::vector<int> vacancy;

    size_t rounds = 0;
public:

    bool SubscribePlayer(std::string player_nickname, std::shared_ptr<Actors::IPlayer> new_player, int id);
    bool UnSubscribePlayer(int player_id);

    void NewRound();

    void Notify_result();

    void SetDealer(std::shared_ptr<Controller::IDealer> dealer);

    void join( player_participant_ptr participant)
    {
        if (queue.size() <= MAX_PLAYER_COUNT) {
            if (!vacancy.empty()) {
                std::cerr << "NEW PLAYER " << vacancy.back() << std::endl;
                participant->set_id(vacancy.back());
                participants_.emplace(vacancy.back(), participant);
                vacancy.pop_back();
            } else {
                std::cerr << "NEW PLAYER " << count << std::endl;
                participant->set_id(count);
                participants_.emplace(count++, participant);
            }
        }
    }

    void leave( player_participant_ptr participant)
    {
        if (!queue.empty()) {
            UnSubscribePlayer(participant->get_id());
            participants_.erase(participant->get_id());
        }
    }

    void deliver(const std::string& msg, int num)
    {
        std::cerr << "SEND TO " << num << ": " << msg << std::endl << std::endl;
        participants_[num]->deliver(msg + "\r\n\r\n");
    }

    void deliver(const std::string& msg)
    {
        std::cerr << "SEND TO ALL: " << msg << std::endl << std::endl;
        for (auto participant: participants_)
          participant.second->deliver(msg + "\r\n\r\n");
    }

private:
    std::map<int, player_participant_ptr> participants_;
    enum { max_recent_msgs = 100 };
    json_message_queue recent_msgs_;

    std::atomic_int count = 0;
};


#endif //BLACKJACK_GAME_ROOM_H
