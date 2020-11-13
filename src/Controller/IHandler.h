#ifndef BLACKJACK_IHANDLER_H
#define BLACKJACK_IHANDLER_H

#include <memory>
#include <cassert>

#include "Actors/Events.h"
#include "Cards/CardStack.h"

namespace Actors {
    struct IDealer;
    struct IActor;
}
struct GameGround;

namespace DealerHandler {
    enum class DealerLogic : int {
        BETABLE,
        DEALERABLE,
        PLAYABLE,
        DISTRIB
    };

    struct IHandler {
    protected:
        std::shared_ptr<GameGround> controller;
    public:
        virtual ~IHandler() = default;

        virtual void GiveCard([[maybe_unused]] Actors::IDealer *){
            throw std::logic_error("");
        }

        virtual void SwapPlayer([[maybe_unused]] Actors::IDealer *){
            throw std::logic_error("");
        }

        virtual void PlayOut([[maybe_unused]] Actors::IDealer *, [[maybe_unused]] Actors::IActor * player_dealer){
            throw std::logic_error("");
        }

        virtual void NewRound([[maybe_unused]] Actors::IDealer *, [[maybe_unused]] Actors::IActor *){
            throw std::logic_error("");
        }

        virtual void TakeBet([[maybe_unused]] Actors::IDealer *, double) {
            throw std::logic_error("");
        }

        virtual void GiveDoubleDown([[maybe_unused]] Actors::IDealer *) {
            throw std::logic_error("");
        }

        virtual void Hit([[maybe_unused]] Actors::IDealer  *,  [[maybe_unused]]  Actors::IActor * player_dealer, [[maybe_unused]] const GameCard::Cards & card) {
            throw std::logic_error("");
        }

        virtual void ShowHand([[maybe_unused]] Actors::IActor *){
            throw std::logic_error("");
        }

    };

    struct DealerableHandler : public IHandler {
    public:
        void GiveCard(Actors::IDealer *) override;

        void SwapPlayer(Actors::IDealer *) override;

        void PlayOut(Actors::IDealer *, Actors::IActor * player_dealer) override;

        void TakeBet([[maybe_unused]] Actors::IDealer *, [[maybe_unused]] double) override;

        void GiveDoubleDown(Actors::IDealer *) override;
    };

    struct BetableHandler : public IHandler {
    public:
        void GiveCard([[maybe_unused]] Actors::IDealer *) override;

        void SwapPlayer([[maybe_unused]] Actors::IDealer *) override;

        void TakeBet([[maybe_unused]] Actors::IDealer *, double) override;

        void GiveDoubleDown([[maybe_unused]] Actors::IDealer *) override;
    };

    struct PlayableHandler : public IHandler{
    public:
        void SwapPlayer(Actors::IDealer *) override;

        void GiveCard(Actors::IDealer *) override;

        void Hit(Actors::IDealer *,  Actors::IActor * player_dealer, const GameCard::Cards & card) override;
    };

    struct DistributionHandler : public IHandler {
        void Hit(Actors::IDealer *,  Actors::IActor * player_dealer, const GameCard::Cards & card) override;

        void NewRound(Actors::IDealer *, Actors::IActor *) override;

        void GiveCard(Actors::IDealer *) override;

        void SwapPlayer(Actors::IDealer *) override;
    };

    // Состояние злопамятного диллера
//    struct RevengefulHandler : public DealerableHandler {
//
//    };
}

#endif //BLACKJACK_IHANDLER_H
