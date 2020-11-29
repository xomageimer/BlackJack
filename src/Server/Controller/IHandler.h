#ifndef BLACKJACK_IHANDLER_H
#define BLACKJACK_IHANDLER_H

#include "Server/Controller/Dealer.h"
#include "Actors/IPlayer.h"

#define DEFAULT {               \
    throw std::logic_error(""); \
}                               \

namespace DealerHandlers {
    struct IDealerHandler {
        virtual ~IDealerHandler() = default;
        virtual void serveMove(Controller::IDealer *) DEFAULT;
        virtual void serveRound(Controller::IDealer *) DEFAULT;
        virtual void serveBet(Controller::IDealer *) DEFAULT;
        virtual void servePlayout(Controller::IDealer *) DEFAULT;
        virtual void serveYourself(Controller::IDealer *) DEFAULT;
    };

    struct BetHandler : public IDealerHandler{
        void serveBet(Controller::IDealer *) override;
    };

    struct MoveHandler : public IDealerHandler{
        void serveMove(Controller::IDealer *) override;
    };

    struct PlayoutHandler : public IDealerHandler {
        void servePlayout(Controller::IDealer *) override;
    };

    struct PlayingHandler : public IDealerHandler {
        void serveYourself(Controller::IDealer *) override;
    };

    struct RoundHandler : public IDealerHandler {
        void serveRound(Controller::IDealer *) override;
    };

    struct DealHandler : public IDealerHandler {
        void serveMove(Controller::IDealer *) override;
    };

}


#endif //BLACKJACK_IHANDLER_H
