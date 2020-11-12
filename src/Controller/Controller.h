#ifndef BLACKJACK_CONTROLLER_H
#define BLACKJACK_CONTROLLER_H

#include <map>
#include <memory>
#include <typeinfo>

#include "Actors/Events.h"

struct GameGround;

// TODO расписать состояния контроллера
//  можно хранить в контроллере всё о view и вводе, а остальное засунуть в GameGround

struct IController {
public:
    enum class DealerLogic : int {
        BETABLE,
        DEALERABLE,
        PLAYABLE,
        DISTRIB
    };
protected:
    GameGround * m_facade;
public:
    IController (GameGround * grnd);

    virtual ~IController() = default;

    virtual void HandleEvent(const Event & event) = 0;
    virtual void NextState(const DealerLogic &);
};

struct BetableController : public IController {
    BetableController(GameGround *pGround) : IController(pGround) {}

    void HandleEvent(const Event & event) override;
};

struct DistributionController : public IController {
    DistributionController(GameGround *pGround) : IController(pGround) {}

    void HandleEvent(const Event & event) override;
};

struct DealerableController : public IController {
    DealerableController(GameGround *pGround) : IController(pGround) {}

    void HandleEvent(const Event & event) override;
};

struct PlayeableController : public IController {
private:
    int count = 0;
public:
    PlayeableController(GameGround *pGround) : IController(pGround) {}

    void HandleEvent(const Event & event) override;
};


#endif //BLACKJACK_CONTROLLER_H
