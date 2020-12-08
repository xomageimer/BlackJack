#include "IHandler.h"

#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

#define LOBBY dealer->my_room

#define CURRENT_PLAYER dealer->getPlayer().first
#define CURRENT_DEALER dealer->getDealerPlayer()
#define STACK dealer->m_stack

using EVENT = Event::DealerResponse;
using CONTROLLER = Controller::IDealer::states;

void DealerHandlers::BetHandler::serveBet(Controller::IDealer * dealer) {
    json event;
    event["command"] = "Bet";
    event["data"] = {{"min", 10}, {"max", 100}};
    event["timeout"] = 600;

    LOBBY->deliver(event.dump(), dealer->cursor);
}

void DealerHandlers::RoundHandler::serveRound(Controller::IDealer * dealer) {
    auto card1 = STACK->GetCard();
    CURRENT_DEALER->SetCard(card1);

    auto card2 = STACK->GetCard();
    CURRENT_DEALER->SetCard(card2);

    CURRENT_DEALER->ShowHand().MakeSecret(1);

    dealer->Notify_about_dealer();

    size_t i = 0;
    for (auto &[player, bet] : dealer->m_players) {
        auto p_card1 = STACK->GetCard();
        player->SetCard(p_card1);

        auto p_card2 = STACK->GetCard();
        player->SetCard(p_card2);

        dealer->Notify_about_player(i);

        i++;
    }

    if (CURRENT_DEALER->ShowHand().total() >= 10) {
        dealer->cursor = 0;
        dealer->set_current(CONTROLLER::DEAL_SERVANT);
    } else {
        dealer->cursor = 0;
        dealer->set_current(CONTROLLER::MOVE_SERVANT);
    }
}

void DealerHandlers::MoveHandler::serveMove(Controller::IDealer * dealer) {
    bool next = true;
    while (CURRENT_PLAYER->ShowHand().total() >= BLACKJACK) {
        if (++dealer->cursor == dealer->m_players.size()) {
            dealer->set_current(CONTROLLER::YOURSELF_SERVANT);
            next = false;
        }
    }

    if (next) {
        json event;
        event["command"] = "RequestAction";
        event["data"];

        LOBBY->deliver(event.dump(), dealer->cursor);
    }
}


void DealerHandlers::PlayoutHandler::servePlayout(Controller::IDealer * dealer) {
    CURRENT_DEALER->ShowHand().UnSecret(1);

    dealer->Notify_about_dealer();
    for (size_t i = 0; i < dealer->m_players.size(); i++){
        dealer->Notify_about_player(i);
    }

    size_t p = 0;
    for (auto & [player, bet] : dealer->m_players){
        if (player->ShowHand().total() == BLACKJACK && player->ShowHand().GetSize() == 2 && CURRENT_DEALER->ShowHand().total() != BLACKJACK) {
            player->GetRoundResult(static_cast<int>(static_cast<double>(bet) * WinFactor));
            CURRENT_DEALER->GetRoundResult((-1) * static_cast<int>(static_cast<double>(bet) * WinFactor));
        }
        else if ((player->ShowHand().total() > CURRENT_DEALER->ShowHand().total() && player->ShowHand().total() <= BLACKJACK) || (player->ShowHand().total() <= BLACKJACK && CURRENT_DEALER->ShowHand().total() > BLACKJACK)){
            player->GetRoundResult(bet);
            CURRENT_DEALER->GetRoundResult((-1) * bet);
        } else if (player->ShowHand().total() < CURRENT_DEALER->ShowHand().total() && CURRENT_DEALER->ShowHand().total() <= BLACKJACK){
            player->GetRoundResult((-1) * bet);
            CURRENT_DEALER->GetRoundResult(bet);
        } else if (player->ShowHand().total() == CURRENT_DEALER->ShowHand().total() && CURRENT_DEALER->ShowHand().total() <= BLACKJACK){
            player->GetRoundResult(0);
            CURRENT_DEALER->GetRoundResult(0);
        } else {p++;}
        player->ClearHand();
    }

    CURRENT_DEALER->ClearHand();
    if (STACK->TimeToShuffle()) {
        json shuffle_event;
        shuffle_event["command"] = "Shuffle";
        LOBBY->deliver(shuffle_event.dump());
    }
    dealer->RestartDealer();
}

void DealerHandlers::PlayingHandler::serveYourself(Controller::IDealer * dealer) {
    CURRENT_DEALER->ShowHand().UnSecret(1);

    if (CURRENT_DEALER->ShowHand().total() == BLACKJACK && CURRENT_DEALER->ShowHand().GetSize() == 2) {

        dealer->Notify_about_dealer();

        size_t p = 0;
        for (auto & [player, bet] : dealer->m_players){
            auto it = dealer->insurances.find(player);
            if (it != dealer->insurances.end() && it->second){
                player->GetRoundResult(0);
                CURRENT_DEALER->GetRoundResult(0);
            } else {
                player->GetRoundResult((-1) * bet);
                CURRENT_DEALER->GetRoundResult(bet);
            }
            p++;
            player->ClearHand();
        }
        CURRENT_DEALER->ClearHand();

        dealer->RestartDealer();

    } else {
        while (CURRENT_DEALER->ShowHand().total() < DEALERBORDER) {
            auto card = STACK->GetCard();
            CURRENT_DEALER->SetCard(card);

            dealer->Notify_about_dealer();
        }

        dealer->set_current(CONTROLLER::PLAYOUT_SERVANT);
    }
}

void DealerHandlers::DealHandler::serveMove(Controller::IDealer * dealer) {
    json event;
    event["command"] = "Insurance";
    event["data"];

    LOBBY->deliver(event.dump(), dealer->cursor);
}