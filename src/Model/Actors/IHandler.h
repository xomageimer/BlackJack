#ifndef BLACKJACK_IHANDLER_H
#define BLACKJACK_IHANDLER_H

#include <memory>
#include <cassert>

#include "Events.h"
#include "Cards/CardStack.h"

namespace Actors {
    struct IDealer;
    struct IActor;
}
struct IController;

// TODO чекать ход!

namespace DealerHandler {
    struct IHandler {
    protected:
        std::shared_ptr<IController> controller;
    public:
        virtual ~IHandler() = default;

        virtual void SetController(std::shared_ptr<IController>);

        virtual void GiveCard(std::shared_ptr<Actors::IDealer>){
            throw std::logic_error("");
        }

        virtual void SwapPlayer(std::shared_ptr<Actors::IDealer>){
            throw std::logic_error("");
        }

        virtual void PlayOut(std::shared_ptr<Actors::IDealer>, std::shared_ptr<Actors::IActor> player_dealer){
            throw std::logic_error("");
        }

        virtual void NewRound(std::shared_ptr<Actors::IDealer>){
            throw std::logic_error("");
        }

        virtual void BlackJackCheck(std::shared_ptr<Actors::IActor>){
            throw std::logic_error("");
        }

        virtual void TakeBet(std::shared_ptr<Actors::IDealer>, double) {
            throw std::logic_error("");
        }

        virtual void GiveDoubleDown(std::shared_ptr<Actors::IDealer>) {
            throw std::logic_error("");
        }

        virtual void Hit(std::shared_ptr<Actors::IActor>, const GameCard::Cards & card) {
            throw std::logic_error("");
        }

        virtual void ShowHand(std::shared_ptr<Actors::IActor>){
            throw std::logic_error("");
        }

    };

    // TODO тут должен быть ответный запрос в контроллер
    struct DealerableHandler : public IHandler {
    public:
        void GiveCard(std::shared_ptr<Actors::IDealer>) override;

        void SwapPlayer(std::shared_ptr<Actors::IDealer>) override;

        void PlayOut(std::shared_ptr<Actors::IDealer>, std::shared_ptr<Actors::IActor> player_dealer) override;

        void NewRound(std::shared_ptr<Actors::IDealer>) override;

        void TakeBet(std::shared_ptr<Actors::IDealer>, double) override;

        void GiveDoubleDown(std::shared_ptr<Actors::IDealer>) override;
    };

    struct BetableHandler : public IHandler {
    public:
        void GiveCard(std::shared_ptr<Actors::IDealer>) override;
        void SwapPlayer(std::shared_ptr<Actors::IDealer>) override;

        void TakeBet(std::shared_ptr<Actors::IDealer>, double) override;

        void GiveDoubleDown(std::shared_ptr<Actors::IDealer>) override;
    };

    struct PlayableHandler : public IHandler{
    public:
        void PlayOut(std::shared_ptr<Actors::IDealer>, std::shared_ptr<Actors::IActor> player_dealer) override;

        void SwapPlayer(std::shared_ptr<Actors::IDealer>) override;

        void NewRound(std::shared_ptr<Actors::IDealer>) override;

        void BlackJackCheck(std::shared_ptr<Actors::IActor>) override;

        void Hit(std::shared_ptr<Actors::IActor>, const GameCard::Cards & card) override;
    };

    // Состояние злопамятного диллера
    struct RevengefulHandler : public DealerableHandler {

    };
}

#endif //BLACKJACK_IHANDLER_H
