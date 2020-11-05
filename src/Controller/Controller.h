#ifndef BLACKJACK_CONTROLLER_H
#define BLACKJACK_CONTROLLER_H

#include <map>
#include <memory>

#include "Actors/Dealer.h"
#include "Actors/IActor.h"
#include "Actors/Events.h"
#include "Cards/CardStack.h"
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

    std::shared_ptr<Actors::IActor> current_player;
    double & current_bet;
public:
    RelationshipController() = default;

    void HandleEvent(const Event & event) override;

    void SubscribePlayer(const std::string & player_nickname, std::shared_ptr<Actors::IActor> new_player);
    void SubscribeDealer(std::shared_ptr<Actors::IDealer> new_dealer);

    void UnSubscribePlayer(const std::string & player_nickname, std::shared_ptr<Actors::IActor> player);

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
    void RestartGame(const Event &);

    void Output();
};

struct ViewController : public IController {
public:
    void HandleEvent(const Event & event) override;

    void ActionWrite();
};


#endif //BLACKJACK_CONTROLLER_H
