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
        virtual void TakeBet(Controller::IDealer *, [[maybe_unused]] int bet) DEFAULT;
        virtual void GiveCard(Controller::IDealer *) DEFAULT;
        virtual void SwapPlayer(Controller::IDealer *) DEFAULT;
        virtual void PlayOut(Controller::IDealer *) DEFAULT;
        virtual void NewRound(Controller::IDealer *) DEFAULT;
        virtual void GiveDoubleDown(Controller::IDealer *) DEFAULT;
    };

    struct DealerableHandler : public IDealerHandler{
        void GiveCard(Controller::IDealer *) override;
        void SwapPlayer(Controller::IDealer *) override;
        void GiveDoubleDown(Controller::IDealer *) override;

        void TakeBet(Controller::IDealer * dealer, [[maybe_unused]] int bet) override YOU_CANT_DO_IT;
    };

    struct BetableHandler : public IDealerHandler{
        void TakeBet(Controller::IDealer *, int bet) override;
        void SwapPlayer(Controller::IDealer *) override;

        void GiveCard(Controller::IDealer * dealer) override YOU_CANT_DO_IT;
        void GiveDoubleDown(Controller::IDealer * dealer) override YOU_CANT_DO_IT;
    };

    struct DistributionHandler : public IDealerHandler{
        void NewRound(Controller::IDealer *) override;
        void SwapPlayer(Controller::IDealer *) override;
    };

    struct PlayableHandler : public IDealerHandler{
        void GiveCard(Controller::IDealer *) override;
        void SwapPlayer(Controller::IDealer *) override;

        void PlayOut(Controller::IDealer *) override;
    };
}


#endif //BLACKJACK_IHANDLER_H
