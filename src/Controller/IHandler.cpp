#include "IHandler.h"

#include <iostream>

void DealerHandlers::DealerableHandler::GiveCard(Controller::IDealer * dealer) {
    auto card = dealer->GetCard();
    dealer->GetPlayer()->SetCard(card);
    Event new_card(Event::DealerResponse::GIVECARD, card);
    dealer->HandleEvent(new_card);
    if (dealer->GetPlayer()->ShowHand().total() > BLACKJACK) {
        Event lose(Event::DealerResponse::LOSE, std::string("\t\t\tYou lose: " + std::to_string(dealer->GetBet())));
        dealer->MakeBet((-1) * dealer->GetBet());
        dealer->HandleEvent(lose);
        SwapPlayer(dealer);
    } else if (dealer->GetPlayer()->ShowHand().total() == BLACKJACK) {
        SwapPlayer(dealer);
    }
}

void DealerHandlers::DealerableHandler::SwapPlayer(Controller::IDealer * dealer) {
    dealer->Next();
    while (dealer->GetPlayer()->ShowHand().total() == BLACKJACK)
        dealer->Next();
    if (dealer->IsPlayerDealer()) {
        dealer->set_current(Controller::IDealer::states::PLAYABLE);
        Event swap_state(Event::DealerResponse::STATE, std::string());
        dealer->HandleEvent(swap_state);
        dealer->GiveCard();
    } else {
        Event swap(Event::DealerResponse::SWAPPLAYER, std::string());
        dealer->HandleEvent(swap);
    }
}

void DealerHandlers::DealerableHandler::GiveDoubleDown(Controller::IDealer * dealer) {
    std::cout << dealer->GetPlayer()->ShowHand().GetSize() << std::endl;
    if (dealer->GetPlayer()->ShowHand().GetSize() == 2) {

        dealer->MakeBet(dealer->GetBet() * 2);
        dealer->SetPlayer(dealer->GetPlayer(), dealer->GetBet() * 2);
        Event bet(Event::DealerResponse::MAKEBET, dealer->GetBet());
        dealer->HandleEvent(bet);

        auto card = dealer->GetCard();
        dealer->GetPlayer()->SetCard(card);
        Event new_card(Event::DealerResponse::GIVECARD, card);
        dealer->HandleEvent(new_card);

        if (dealer->GetPlayer()->ShowHand().total() > BLACKJACK) {
            Event lose(Event::DealerResponse::LOSE, std::string("\t\t\tYou Lose: " + std::to_string(dealer->GetBet())));
            dealer->MakeBet((-1) * dealer->GetBet());
            dealer->HandleEvent(lose);
        }
        SwapPlayer(dealer);
    } else YOU_CANT_DO_IT
}


void DealerHandlers::BetableHandler::TakeBet(Controller::IDealer * dealer, int bet) {
    if (bet > Controller::IDealer::min && bet < Controller::IDealer::max) {
        dealer->MakeBet(bet);
        dealer->SetPlayer(dealer->GetPlayer(), bet);
        Event bet_(Event::DealerResponse::MAKEBET, dealer->GetBet());
        dealer->HandleEvent(bet_);
        SwapPlayer(dealer);
    } else {
        std::cout << "INVALID BET" << std::endl;
    }
}

void DealerHandlers::BetableHandler::SwapPlayer(Controller::IDealer * dealer) {
    dealer->Next();
    if (dealer->IsPlayerDealer()) {
        dealer->set_current(Controller::IDealer::states::DISTRIBUTION);
        Event swap_state(Event::DealerResponse::STATE, std::string());
        dealer->HandleEvent(swap_state);
        dealer->NewRound();
    } else {
        Event swap(Event::DealerResponse::SWAPPLAYER, std::string());
        dealer->HandleEvent(swap);
    }
}

void DealerHandlers::DistributionHandler::NewRound(Controller::IDealer * dealer) {
    while (!dealer->IsPlayerDealer()){
        dealer->Next();
    }

    auto card1 = dealer->GetCard();
    dealer->GetPlayer()->SetCard(card1);
    Event new_card1(Event::DealerResponse::GIVECARD, card1);
    dealer->HandleEvent(new_card1);

    auto card2 = dealer->GetCard();
    dealer->GetPlayer()->SetCard(card2);
    Event new_card2(Event::DealerResponse::GIVECARD, card2);
    dealer->HandleEvent(new_card2);

    if (dealer->GetDealerHand().total() == BLACKJACK){
        dealer->set_current(Controller::IDealer::states::PLAYABLE);
        Event swap_state(Event::DealerResponse::STATE, std::string());
        dealer->HandleEvent(swap_state);
        SwapPlayer(dealer);
        dealer->PlayOut();
    } else {
        SwapPlayer(dealer);
        while (!dealer->IsPlayerDealer()) {

            auto card1 = dealer->GetCard();
            dealer->GetPlayer()->SetCard(card1);
            Event new_card1(Event::DealerResponse::GIVECARD, card1);
            dealer->HandleEvent(new_card1);

            auto card2 = dealer->GetCard();
            dealer->GetPlayer()->SetCard(card2);
            Event new_card2(Event::DealerResponse::GIVECARD, card2);
            dealer->HandleEvent(new_card2);

            dealer->Next();
        }
        dealer->set_current(Controller::IDealer::states::DEALERABLE);
        Event swap_state(Event::DealerResponse::STATE, std::string());
        dealer->HandleEvent(swap_state);

        SwapPlayer(dealer);
    }
}

void DealerHandlers::DistributionHandler::SwapPlayer(Controller::IDealer * dealer) {
    if (dealer->IsPlayerDealer())
        dealer->Reset();
    dealer->Next();
    Event swap(Event::DealerResponse::SWAPPLAYER, std::string());
    dealer->HandleEvent(swap);
}

void DealerHandlers::PlayableHandler::GiveCard(Controller::IDealer * dealer) {
    while (!dealer->IsPlayerDealer()){
        dealer->Next();
    }
    while (dealer->GetPlayer()->ShowHand().total() < DEALERBORDER){
        auto card = dealer->GetCard();
        dealer->GetPlayer()->SetCard(card);
        Event new_card(Event::DealerResponse::GIVECARD, card);
        dealer->HandleEvent(new_card);
    }
    SwapPlayer(dealer);
    dealer->PlayOut();
}

void DealerHandlers::PlayableHandler::SwapPlayer(Controller::IDealer * dealer) {
    if (dealer->IsPlayerDealer())
        dealer->Reset();
    dealer->Next();
    Event swap(Event::DealerResponse::SWAPPLAYER, std::string());
    dealer->HandleEvent(swap);
}

// TODO мб стоит засунуть номера и ники в класс плеера, и вообще делегировать ему основную работу
void DealerHandlers::PlayableHandler::PlayOut(Controller::IDealer * dealer) {
    while (!dealer->IsPlayerDealer()) {
        if (dealer->GetPlayer()->ShowHand().total() == dealer->GetDealerHand().total()){
            Event draw(Event::DealerResponse::DRAW, std::string("\t\t\tDraw, you get back: " + std::to_string(dealer->GetBet())));
            dealer->MakeBet(0);
            dealer->HandleEvent(draw);
        }
        else if ((dealer->GetPlayer()->ShowHand().total() > dealer->GetDealerHand().total() && dealer->GetDealerHand().total() <= BLACKJACK)
                    || (dealer->GetDealerHand().total() > BLACKJACK && dealer->GetPlayer()->ShowHand().total() <= BLACKJACK))
        {
            Event win(Event::DealerResponse::WIN, std::string("\t\t\tYou win: " + std::to_string(dealer->GetBet())));
            dealer->MakeBet(dealer->GetBet());
            dealer->HandleEvent(win);
        } else {
            Event lose(Event::DealerResponse::LOSE, std::string("\t\t\tYou lose: " + std::to_string(dealer->GetBet())));
            dealer->MakeBet((-1) * dealer->GetBet());
            dealer->HandleEvent(lose);
        }
        SwapPlayer(dealer);
    }
    dealer->set_current(Controller::IDealer::states::BETABLE);

    dealer->TimeToShuffle();
    Event restart(Event::DealerResponse::RESTART, std::string("restart"));
    dealer->HandleEvent(restart);
}