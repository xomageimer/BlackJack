#ifndef BLACKJACK_IHANDLER_H
#define BLACKJACK_IHANDLER_H

#include "Controller/Dealer.h"
#include "Actors/IPlayer.h"

#define DEFAULT {               \
    throw std::logic_error(""); \
}                               \

#define YOU_CANT_DO_IT {                                                    \
    Event warn(Event::DealerResponse::WARN, std::string("YOU CANT DO IT")); \
    dealer->HandleEvent(warn);                                              \
}                                                                           \

namespace DealerHandlers {
    struct IDealerHandler {
        virtual ~IDealerHandler() = default;
        virtual void TakeBet(Controller::IDealer *, Actors::IPlayer *, [[maybe_unused]] int bet) DEFAULT;
        virtual void GiveCard(Controller::IDealer *, Actors::IPlayer *) DEFAULT;
        virtual void SwapPlayer(Controller::IDealer *, Actors::IPlayer *) DEFAULT;
        virtual void PlayOut(Controller::IDealer *, Actors::IPlayer *) DEFAULT;
        virtual void NewRound(Controller::IDealer *, Actors::IPlayer *) DEFAULT;
        virtual void GiveDoubleDown(Controller::IDealer *, Actors::IPlayer *) DEFAULT;
    };

    struct DealerableHandler : public IDealerHandler{
        void GiveCard(Controller::IDealer *, Actors::IPlayer *) override;
        void SwapPlayer(Controller::IDealer *, Actors::IPlayer *) override;
        void GiveDoubleDown(Controller::IDealer *, Actors::IPlayer *) override;
    };

    struct BetableHandler : public IDealerHandler{
        void TakeBet(Controller::IDealer *, Actors::IPlayer *, int bet) override;
        void SwapPlayer(Controller::IDealer *, Actors::IPlayer *) override;

        void GiveCard(Controller::IDealer * dealer, [[maybe_unused]]Actors::IPlayer *) override YOU_CANT_DO_IT;
        void GiveDoubleDown(Controller::IDealer * dealer, [[maybe_unused]]Actors::IPlayer *) override YOU_CANT_DO_IT;
    };

    struct DistributionHandler : public IDealerHandler{
        void NewRound(Controller::IDealer *, Actors::IPlayer *) override;
        void SwapPlayer(Controller::IDealer *, Actors::IPlayer *) override;
    };

    struct PlayableHandler : public IDealerHandler{
        void GiveCard(Controller::IDealer *, Actors::IPlayer *) override;
        void SwapPlayer(Controller::IDealer *, Actors::IPlayer *) override;

        void PlayOut(Controller::IDealer *, Actors::IPlayer *) override;
    };
}


#endif //BLACKJACK_IHANDLER_H
