#ifndef BLACKJACK_GAMEGROUND_H
#define BLACKJACK_GAMEGROUND_H

// TODO хранить всё об игре (плееры, диллеры, чей ход)
//  а также контроллеры

#include "Actors/IActor.h"
#include "Dealer.h"
#include "OutputManager.h"

const size_t MAX_PLAYER_COUNT = 7;

struct GameGround {
private:
    std::shared_ptr<OutputManager> om;

    std::shared_ptr<Actors::IDealer> dealer;
    std::shared_ptr<Actors::IActor> player_dealer;

    std::map<std::string, std::shared_ptr<Actors::IActor>> players;
    std::vector<int> bets;

    std::map<std::string, std::shared_ptr<Actors::IActor>> AFK_players;

    std::vector<size_t> vacancy {};
    std::vector<std::string> queue {};

    size_t current_number = 0;
    std::shared_ptr<Actors::IActor> current_player;

public:
    explicit GameGround (std::shared_ptr<OutputManager> output);

    void SetViewManager(std::shared_ptr<ILogger>);

    bool SubscribePlayer(std::string player_nickname, std::shared_ptr<Actors::IActor> new_player);
    template <typename T>
    inline constexpr auto SubscribeDealer(std::shared_ptr<T> new_dealer) noexcept ->
    decltype (
    std::declval<T>().GiveCard(),
            std::declval<T>().ShowHand(),
            void()
    ){
        dealer = new_dealer;
        player_dealer = new_dealer;
        players.emplace(std::piecewise_construct, std::forward_as_tuple("dealer"), std::forward_as_tuple(player_dealer));
    }
    template <typename T>
    inline constexpr void SubscribeDealer([[maybe_unused]] T new_dealer){
        throw std::logic_error("\nERROR; FROM SUBSCRIBE DEALER METHOD; FILE: " + std::string(__FILE__) + "; ON LINE: " + std::to_string(__LINE__ - 1) + ";\nARGUMENT IS NOT INHERITED FROM IDealer AND/OR IActor;");
    }

    bool UnSubscribePlayer(const std::string & player_nickname);

    void CurrentState(const Event &); // для графики

    void ChangePlayer(const Event &);

    void Output();
    void Destroy();

    void Result();
};


#endif //BLACKJACK_GAMEGROUND_H
