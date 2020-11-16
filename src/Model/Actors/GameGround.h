#ifndef BLACKJACK_GAMEGROUND_H
#define BLACKJACK_GAMEGROUND_H

// TODO хранить всё об игре (плееры, диллеры, чей ход)
//  а также контроллеры

#include "Controller/Dealer.h"
#include "Actors/IPlayer.h"
#include "OutputManager.h"

const size_t MAX_PLAYER_COUNT = 7;

// Некий медиатор/декоратор над машиной состояний

struct GameGround {
private:
    std::shared_ptr<OutputManager> om;

    std::shared_ptr<Controller::IDealer> dealer;
    std::shared_ptr<Actors::IPlayer> player_dealer;

    std::map<std::string, std::shared_ptr<Actors::IPlayer>> players;

    std::map<std::string, std::shared_ptr<Actors::IPlayer>> AFK_players;
    std::map<std::string, int> AFK_players_limited;


    std::vector<size_t> vacancy {};
    std::vector<std::string> queue {};

    size_t current_number = 0;
    std::shared_ptr<Actors::IPlayer> current_player;
public:
    explicit GameGround (std::shared_ptr<OutputManager> output);

    void SetViewManager(std::shared_ptr<ILogger>);
    bool SubscribePlayer(std::string player_nickname, std::shared_ptr<Actors::IPlayer> new_player);
    template <typename T>
    inline constexpr auto SubscribeDealer(std::shared_ptr<T> new_dealer) noexcept ->
    decltype (
            Controller::Is_Inherited_v<T, Controller::IDealer>,
            Controller::Is_Inherited_v<T, Actors::IPlayer>,

            void()
    ){
        dealer = new_dealer;
        player_dealer = new_dealer;
        dealer->SetPlayerDealer(player_dealer);
    }
    template <typename T>
    inline constexpr void SubscribeDealer([[maybe_unused]] T new_dealer){
        throw std::logic_error("\nERROR; FROM SUBSCRIBE DEALER METHOD; FILE: " + std::string(__FILE__) + "; ON LINE: " + std::to_string(__LINE__ - 1) + ";\nARGUMENT IS NOT INHERITED FROM IDealer AND/OR IActor;");
    }
    bool UnSubscribePlayer(const std::string &player_nickname);

    void Execute();

    void Output();
    void Destroy();
};


#endif //BLACKJACK_GAMEGROUND_H
