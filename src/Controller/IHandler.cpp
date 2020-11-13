#include "IHandler.h"

#include <iostream>

void DealerHandlers::DealerableHandler::GiveCard(Controller::IDealer * dealer, Actors::IPlayer * player) {
    auto card = dealer->GetCard();
    player->SetCard(card);
    Event new_card(Event::DealerResponse::GIVECARD, card);
    dealer->HandleEvent(new_card);
    if (player->ShowHand().total() > BLACKJACK) {
        Event lose(Event::DealerResponse::LOSE, std::string("You Lose: " + std::to_string(dealer->GetBet())));
        dealer->MakeBet((-1) * dealer->GetBet());
        dealer->HandleEvent(lose);
        SwapPlayer(dealer, player);
    } else if (player->ShowHand().total() == BLACKJACK) {
        SwapPlayer(dealer, player);
    }
}

void DealerHandlers::DealerableHandler::SwapPlayer(Controller::IDealer * dealer, [[maybe_unused]] Actors::IPlayer * player) {
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

void DealerHandlers::DealerableHandler::GiveDoubleDown(Controller::IDealer * dealer, Actors::IPlayer * player) {
    if (player->ShowHand().GetSize() == 2) {
        dealer->MakeBet(dealer->GetBet() * 2);
        dealer->SetPlayer(player, dealer->GetBet() * 2);
        Event bet(Event::DealerResponse::MAKEBET, dealer->GetBet());
        dealer->HandleEvent(bet);
        auto card = dealer->GetCard();
        player->SetCard(card);
        Event new_card(Event::DealerResponse::GIVECARD, card);
        dealer->HandleEvent(new_card);
        if (player->ShowHand().total() > BLACKJACK) {
            Event lose(Event::DealerResponse::LOSE, std::string("You Lose: " + std::to_string(dealer->GetBet())));
            dealer->MakeBet((-1) * dealer->GetBet());
            dealer->HandleEvent(lose);
        }
        SwapPlayer(dealer, player);
    } else YOU_CANT_DO_IT
}


void DealerHandlers::BetableHandler::TakeBet(Controller::IDealer * dealer, Actors::IPlayer * player, int bet) {
    if (bet > Controller::IDealer::min && bet < Controller::IDealer::max) {
        dealer->MakeBet(bet);
        dealer->SetPlayer(player, bet);
        Event bet_(Event::DealerResponse::MAKEBET, dealer->GetBet());
        dealer->HandleEvent(bet_);
        SwapPlayer(dealer, player);
    } else {
        std::cout << "INVALID BET" << std::endl;
    }
}

void DealerHandlers::BetableHandler::SwapPlayer(Controller::IDealer * dealer, [[maybe_unused]] Actors::IPlayer * player) {
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

void DealerHandlers::DistributionHandler::NewRound(Controller::IDealer * dealer, [[maybe_unused]] Actors::IPlayer * player) {
    while (!dealer->IsPlayerDealer()){
        dealer->Next();
    }
    SwapPlayer(dealer, player);
    while (!dealer->IsPlayerDealer()){

        auto card1 = dealer->GetCard();
        player->SetCard(card1);
        Event new_card1(Event::DealerResponse::GIVECARD, card1);
        dealer->HandleEvent(new_card1);

        auto card2 = dealer->GetCard();
        player->SetCard(card2);
        Event new_card2(Event::DealerResponse::GIVECARD, card2);
        dealer->HandleEvent(new_card2);

        dealer->Next();
    }
    dealer->set_current(Controller::IDealer::states::DEALERABLE);
    Event swap_state(Event::DealerResponse::STATE, std::string());
    dealer->HandleEvent(swap_state);

    SwapPlayer(dealer, player);
}

void DealerHandlers::DistributionHandler::SwapPlayer(Controller::IDealer * dealer, [[maybe_unused]] Actors::IPlayer * player) {
    if (dealer->IsPlayerDealer())
        dealer->Reset();
    dealer->Next();
    Event swap(Event::DealerResponse::SWAPPLAYER, std::string());
    dealer->HandleEvent(swap);
}

void DealerHandlers::PlayableHandler::GiveCard(Controller::IDealer * dealer, Actors::IPlayer * player) {
    while (!dealer->IsPlayerDealer()){
        dealer->Next();
    }
    while (player->ShowHand().total() < DEALERBORDER){
        auto card = dealer->GetCard();
        player->SetCard(card);
        Event new_card(Event::DealerResponse::GIVECARD, card);
        dealer->HandleEvent(new_card);
    }
    SwapPlayer(dealer, player);
    dealer->PlayOut(player->ShowHand().total());
}

void DealerHandlers::PlayableHandler::SwapPlayer(Controller::IDealer * dealer, [[maybe_unused]] Actors::IPlayer * player) {
    if (dealer->IsPlayerDealer())
        dealer->Reset();
    dealer->Next();
    Event swap(Event::DealerResponse::SWAPPLAYER, std::string());
    dealer->HandleEvent(swap);
}

// TODO мб стоит засунуть номера и ники в класс плеера, и вообще делегировать ему основную работу
void DealerHandlers::PlayableHandler::PlayOut(Controller::IDealer * dealer, Actors::IPlayer * player, int dealer_total) {
    while (!dealer->IsPlayerDealer()) {
        if (player->ShowHand().total() == dealer_total){
            Event draw(Event::DealerResponse::DRAW, std::string("Draw, you get back: " + std::to_string(dealer->GetBet())));
            dealer->MakeBet(0);
            dealer->HandleEvent(draw);
        } else if (player->ShowHand().total() > dealer_total && dealer_total < BLACKJACK){
            Event win(Event::DealerResponse::WIN, std::string("You win: " + std::to_string(dealer->GetBet())));
            dealer->MakeBet(dealer->GetBet());
            dealer->HandleEvent(win);
        } else {
            Event win(Event::DealerResponse::LOSE, std::string("You lose: " + std::to_string(dealer->GetBet())));
            dealer->MakeBet(dealer->GetBet());
            dealer->HandleEvent(win);
        }
        SwapPlayer(dealer, player);
    }
    dealer->set_current(Controller::IDealer::states::BETABLE);

    dealer->TimeToShuffle();
    Event restart(Event::DealerResponse::RESTART, std::string("restart"));
    dealer->HandleEvent(restart);

    SwapPlayer(dealer, player);
}
