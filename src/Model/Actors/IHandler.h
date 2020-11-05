#ifndef BLACKJACK_IHANDLER_H
#define BLACKJACK_IHANDLER_H

#include <memory>
#include <cassert>

#include "Controller.h"
#include "IActor.h"
#include "Dealer.h"

// TODO чекать ход!

namespace DealerHandler {
    struct IHandler {
    protected:
        std::shared_ptr<IController> controller;
    public:

        virtual void SetController(std::shared_ptr<IController>);

        virtual void GiveCard(std::shared_ptr<Actors::IDealer>) = 0;

        virtual void SwapPlayer(std::shared_ptr<Actors::IDealer>) = 0;

        virtual void PlayOut(std::shared_ptr<Actors::IDealer>, std::shared_ptr<Actors::IActor> player_dealer) = 0;

        virtual void NewRound(std::shared_ptr<Actors::IDealer>) = 0;

        virtual void TakeBet(std::shared_ptr<Actors::IDealer>, double) = 0;

        virtual void GiveDoubleDown(std::shared_ptr<Actors::IDealer>) = 0;

        virtual void BlackJackCheck(std::shared_ptr<Actors::IActor>) = 0;

        virtual void Hit(std::shared_ptr<Actors::IActor>, const GameCard::Cards & card) = 0;

        virtual void DoubleDown(std::shared_ptr<Actors::IActor>) = 0;

        virtual void GetResult(std::shared_ptr<Actors::IActor>) = 0;

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

        void BlackJackCheck(std::shared_ptr<Actors::IActor>) override{
            throw std::bad_exception();
        }

        void Hit(std::shared_ptr<Actors::IActor>, const GameCard::Cards & card) override{
            throw std::bad_exception();
        }

        void DoubleDown(std::shared_ptr<Actors::IActor>) override{
            throw std::bad_exception();
        }

        void GetResult(std::shared_ptr<Actors::IActor>) override{
            throw std::bad_exception();
        }

    };

    struct BetableHandler : public IHandler {
    public:
        void GiveCard(std::shared_ptr<Actors::IDealer>) override{
            Event event(Event::Type::WARN, std::string("You have not placed a bet!"));
            controller->HandleEvent(event);
        }

        void SwapPlayer(std::shared_ptr<Actors::IDealer>) override;

        void PlayOut(std::shared_ptr<Actors::IDealer>, std::shared_ptr<Actors::IActor> player_dealer) override{
            Event event(Event::Type::WARN, std::string("You have not placed a bet!"));
            controller->HandleEvent(event);
        }

        void NewRound(std::shared_ptr<Actors::IDealer>) override{
            Event event(Event::Type::WARN, std::string("You have not placed a bet!"));
            controller->HandleEvent(event);
        }

        void TakeBet(std::shared_ptr<Actors::IDealer>, double) override;

        void BlackJackCheck(std::shared_ptr<Actors::IActor>) override{
            throw std::bad_exception();
        }

        void GiveDoubleDown(std::shared_ptr<Actors::IDealer>) override{
            Event event(Event::Type::WARN, std::string("You have not placed a bet!"));
            controller->HandleEvent(event);
        }

        void Hit(std::shared_ptr<Actors::IActor>, const GameCard::Cards & card) override{
            throw std::bad_exception();
        }

        void DoubleDown(std::shared_ptr<Actors::IActor>) override{
            throw std::bad_exception();
        }

        void GetResult(std::shared_ptr<Actors::IActor>) override{
            throw std::bad_exception();
        }

    };

    struct PlayableHandler : public IHandler{
    public:
        void GiveCard(std::shared_ptr<Actors::IDealer>) override{
            throw std::bad_exception();
        }

        void SwapPlayer(std::shared_ptr<Actors::IDealer>) override{
            throw std::bad_exception();
        }

        void PlayOut(std::shared_ptr<Actors::IDealer>, std::shared_ptr<Actors::IActor> player_dealer) override{
            throw std::bad_exception();
        }

        void NewRound(std::shared_ptr<Actors::IDealer>) override{
            throw std::bad_exception();
        }

        void BlackJackCheck(std::shared_ptr<Actors::IActor>) override;

        void TakeBet(std::shared_ptr<Actors::IDealer>, double) override{
            throw std::bad_exception();
        }

        void GiveDoubleDown(std::shared_ptr<Actors::IDealer>) override{
            throw std::bad_exception();
        }

        void Hit(std::shared_ptr<Actors::IActor>, const GameCard::Cards & card) override;

        void DoubleDown(std::shared_ptr<Actors::IActor>) override{
            throw std::bad_exception();
        }

        void GetResult(std::shared_ptr<Actors::IActor>) override{
            throw std::bad_exception();
        }

    };
}

#endif //BLACKJACK_IHANDLER_H
