#include "IHandler.h"

#include <iostream>

void DealerHandlers::DealerableHandler::GiveCard(Controller::IDealer * dealer, Actors::IPlayer * player) {
    if (player->ShowHand().total() > BLACKJACK)
        SwapPlayer(dealer, player);
    else {
        std::cout << "GIVE CARD" << std::endl;
    }
}

void DealerHandlers::DealerableHandler::SwapPlayer(Controller::IDealer * dealer, [[maybe_unused]] Actors::IPlayer * player) {
    dealer->Next();
    if (dealer->IsPlayerDealer()) {
        dealer->set_current(Controller::IDealer::states::PLAYABLE);
        dealer->GiveCard();
    }
}

void DealerHandlers::DealerableHandler::GiveDoubleDown(Controller::IDealer * dealer, Actors::IPlayer * player) {
    if (player->ShowHand().GetSize() == 2) {
        std::cout << "DOUBLEDOWN" << std::endl;
        SwapPlayer(dealer, player);
    } else YOU_CANT_DO_IT
}


void DealerHandlers::BetableHandler::TakeBet(Controller::IDealer * dealer, Actors::IPlayer * player, int bet) {
    if (bet > Controller::IDealer::min && bet < Controller::IDealer::max) {
        std::cout << "MAKE BET " + std::to_string(bet) << std::endl;
        dealer->MakeBet(bet);
        SwapPlayer(dealer, player);
    } else {
        std::cout << "INVALID BET" << std::endl;
    }
}

void DealerHandlers::BetableHandler::SwapPlayer(Controller::IDealer * dealer, [[maybe_unused]] Actors::IPlayer * player) {
    dealer->Next();
    if (dealer->IsPlayerDealer()) {
        dealer->set_current(Controller::IDealer::states::DISTRIBUTION);
        dealer->NewRound();
    }
}

void DealerHandlers::DistributionHandler::NewRound(Controller::IDealer * dealer, [[maybe_unused]] Actors::IPlayer * player) {
    while (!dealer->IsPlayerDealer()){
        dealer->Next();
    }
    std::cout << "Dealer take x 2 card" << std::endl;
    SwapPlayer(dealer, player);
    while (!dealer->IsPlayerDealer()){
        std::cout << "GIVE CARD x 2" << std::endl;
        dealer->Next();
    }
    dealer->set_current(Controller::IDealer::states::DEALERABLE);
    SwapPlayer(dealer, player);
}

void DealerHandlers::DistributionHandler::SwapPlayer(Controller::IDealer * dealer, [[maybe_unused]] Actors::IPlayer * player) {
    if (dealer->IsPlayerDealer())
        dealer->Reset();
    dealer->Next();
}

void DealerHandlers::PlayableHandler::GiveCard(Controller::IDealer * dealer, Actors::IPlayer * player) {
    while (!dealer->IsPlayerDealer()){
        dealer->Next();
    }
    std::cout << "Dealer take a card, becouse his xod!" << std::endl;

    SwapPlayer(dealer, player);
    dealer->PlayOut();
}

void DealerHandlers::PlayableHandler::SwapPlayer(Controller::IDealer * dealer, [[maybe_unused]] Actors::IPlayer * player) {
    if (dealer->IsPlayerDealer())
        dealer->Reset();
    dealer->Next();
}

// TODO мб стоит засунуть номера и ники в класс плеера, и вообще делегировать ему основную работу
void DealerHandlers::PlayableHandler::PlayOut(Controller::IDealer * dealer, Actors::IPlayer * player) {
    while (!dealer->IsPlayerDealer()) {
        std::cout << "END of GAME and make some results" << std::endl;
        SwapPlayer(dealer, player);
    }
    dealer->set_current(Controller::IDealer::states::BETABLE);
    dealer->TimeToShuffle();
    SwapPlayer(dealer, player);
}
