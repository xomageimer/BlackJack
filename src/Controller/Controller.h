#ifndef BLACKJACK_CONTROLLER_H
#define BLACKJACK_CONTROLLER_H

#include <map>
#include <unordered_map>
#include <memory>
#include <typeinfo>
#include <Actors/IHandler.h>

#include "Actors/IActor.h"
#include "Actors/Dealer.h"
#include "OutputManager.h"

struct IController {
public:
    virtual void HandleEvent(const Event & event) = 0;
};

struct RelationshipController : public IController{
private:
    std::shared_ptr<OutputManager> om;

    std::shared_ptr<Actors::IDealer> dealer;
    std::shared_ptr<Actors::IActor> player_dealer;

    std::map<std::string, std::pair<std::shared_ptr<Actors::IActor>, double>> players;

    std::unordered_map<DealerHandler::DealerLogic, std::shared_ptr<DealerHandler::IHandler>> Control_Logic;

    std::vector<size_t> vacancy {};
    std::vector<std::string> queue {};

    DealerHandler::DealerLogic current_logic;
    size_t current_number = 0;
    std::shared_ptr<Actors::IActor> current_player;
    double * current_bet;
public:
    RelationshipController();

    void HandleEvent(const Event & event) override;

    void NextHandler();

    void SubscribeHandlers(const DealerHandler::DealerLogic & logic, std::shared_ptr<DealerHandler::IHandler>);

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
        RestartGame();
    }
    template <typename T>
    inline constexpr void SubscribeDealer(T new_dealer){
        throw std::logic_error("\nERROR; FROM SUBSCRIBE DEALER METHOD; FILE: " + std::string(__FILE__) + "; ON LINE: " + std::to_string(__LINE__ - 1) + ";\nARGUMENT IS NOT INHERITED FROM IDealer AND/OR IActor;");
    }


    bool UnSubscribePlayer(const std::string & player_nickname);

    void RequestCard();
    void RequestStop();
    void RequestBet(const Event &);
    void RequestDoubleCard();

    void OutWarn(const Event &);

    void MakeBet(const Event &);
    void ChangePlayer(const Event &);
    void GiveCards(const Event &);
    void Result();
    void SetResult(const Event &);
    void RestartGame();

    void Output();
};

//struct ViewController : public IController {
//public:
//    void HandleEvent(const Event & event) override;
//
//    void ActionWrite();
//};


#endif //BLACKJACK_CONTROLLER_H
