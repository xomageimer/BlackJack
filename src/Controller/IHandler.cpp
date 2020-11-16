#include "IHandler.h"

#include <iostream>

#define CURRENT_PLAYER dealer->getPlayer().first
#define CURRENT_BET dealer->getPlayer().second
#define CURRENT_DEALER dealer->getDealerPlayer()
#define STACK dealer->m_stack

using EVENT = Event::DealerResponse;
using CONTROLLER = Controller::IDealer::states;

void DealerHandlers::BetHandler::serveBet(Controller::IDealer * dealer) {
    Event cur_state_for_player (EVENT::STATE, std::string("Current State is BET for " + std::to_string(dealer->cursor) + ""));
    dealer->general_view_manager->notify(cur_state_for_player.GetData<std::string>()); // отобразить кнопки для текущего игрока

    auto response = CURRENT_PLAYER->Bet(); // для графики он может проверять на клик по кнопке

    if (response.Response == EVENT::MAKEBET){
        CURRENT_BET = response.GetData<int>();
        dealer->general_view_manager->notify(std::string("Player " + std::to_string(dealer->cursor) + " bet: " + std::to_string(response.GetData<int>())));

        if (++dealer->cursor == dealer->m_players.size())
            dealer->set_current(CONTROLLER::ROUND_SERVANT);
    }
}

void DealerHandlers::RoundHandler::serveRound(Controller::IDealer * dealer) {
    Event cur_state(EVENT::STATE, std::string("Current State is ROUNDED for all"));
    dealer->general_view_manager->notify(cur_state.GetData<std::string>());

    auto card1 = STACK->GetCard();
    CURRENT_DEALER->SetCard(card1);

    // потом для графики можно будет подменить на полный Event
    Event take_card1(EVENT::GIVECARD, std::string("Dealer take a card: " + std::string(card1)));
    dealer->general_view_manager->notify(take_card1.GetData<std::string>());

    auto card2 = STACK->GetCard();
    CURRENT_DEALER->SetCard(card2);

    CURRENT_DEALER->ShowHand().MakeSecret(1);

    Event take_card2(EVENT::GIVECARD, std::string("Dealer take a card: #SECRET#"));
    dealer->general_view_manager->notify(take_card2.GetData<std::string>());

    size_t i = 0;
    for (auto &[player, bet] : dealer->m_players) {
        auto p_card1 = STACK->GetCard();
        player->SetCard(p_card1);

        Event take_p_card1(EVENT::GIVECARD,
                           std::string("Player " + std::to_string(i) + " take a card: " + std::string(p_card1)));
        dealer->general_view_manager->notify(take_p_card1.GetData<std::string>());

        auto p_card2 = STACK->GetCard();
        player->SetCard(p_card2);

        Event take_p_card2(EVENT::GIVECARD,
                           std::string("Player " + std::to_string(i) + " take a card: " + std::string(p_card2)));
        dealer->general_view_manager->notify(take_p_card2.GetData<std::string>());
        i++;
    }

    if (CURRENT_DEALER->ShowHand().total() >= 10) {
        dealer->cursor = 0;
        dealer->set_current(CONTROLLER::DEAL_SERVANT);
    } else {
        dealer->cursor = 0;
        dealer->set_current(CONTROLLER::MOVE_SERVANT);
    }

    //TODO показать карты все участнико, потом убрать при графике
    std::string dealer_cards;
    for (auto & cards : CURRENT_DEALER->ShowHand().LookAtCards()){
        dealer_cards += std::string(cards) + " ";
    }
    dealer_cards += " = " + std::to_string(CURRENT_DEALER->ShowHand().total());
    dealer->general_view_manager->notify(std::string("Dealer have: " + dealer_cards));

    size_t j = 0;
    for(auto & [player, bet] : dealer->m_players) {
        std::string player_cards;
        for (auto &cards : player->ShowHand().LookAtCards()) {
            player_cards += std::string(cards) + " ";
        }
        player_cards += " = " + std::to_string(player->ShowHand().total());
        dealer->general_view_manager->notify(std::string("Player " + std::to_string(j) + " have: " + player_cards));
        j++;
    }
}

void DealerHandlers::MoveHandler::serveMove(Controller::IDealer * dealer) {
    Event cur_state_for_player(EVENT::STATE,
                               std::string("Current State is MOVE for " + std::to_string(dealer->cursor) + ""));
    dealer->general_view_manager->notify(
            cur_state_for_player.GetData<std::string>()); // отобразить кнопки для текущего игрока

    if (CURRENT_PLAYER->ShowHand().total() < BLACKJACK) {
        auto response = CURRENT_PLAYER->Move(); // для графики он может проверять на клик по кнопке
        if (response.Response == EVENT::GIVECARD) {
            auto card = STACK->GetCard();
            CURRENT_PLAYER->SetCard(card);

            Event take_p_card1(EVENT::GIVECARD, std::string(
                    "Player " + std::to_string(dealer->cursor) + " take a card: " + std::string(card)));
            dealer->general_view_manager->notify(take_p_card1.GetData<std::string>());

            //TODO показать все карты, убрать при графике
            std::string dealer_cards;
            for (auto & cards : CURRENT_DEALER->ShowHand().LookAtCards()){
                dealer_cards += std::string(cards) + " ";
            }
            dealer_cards += " = " + std::to_string(CURRENT_DEALER->ShowHand().total());
            dealer->general_view_manager->notify(std::string("Dealer have: " + dealer_cards));

            size_t j = 0;
            for(auto & [player, bet] : dealer->m_players) {
                std::string player_cards;
                for (auto &cards : player->ShowHand().LookAtCards()) {
                    player_cards += std::string(cards) + " ";
                }
                player_cards += " = " + std::to_string(player->ShowHand().total());
                dealer->general_view_manager->notify(std::string("Player " + std::to_string(j) + " have: " + player_cards));
                j++;
            }
            // TODO КОНЕЦ

            if (CURRENT_PLAYER->ShowHand().total() > BLACKJACK){
                // тут также можно вычитать из бд где лежат деньги
                CURRENT_PLAYER->GetRoundResult((-1) * CURRENT_BET);
                CURRENT_DEALER->GetRoundResult(CURRENT_BET);

                Event lose(EVENT::LOSE, std::string(
                        "Player " + std::to_string(dealer->cursor) + " lose: " + std::to_string(CURRENT_BET)));
                dealer->general_view_manager->notify(lose.GetData<std::string>());

                dealer->AFKCurrentPlayer();
            }
        }
        else if (response.Response == EVENT::SWAPPLAYER){
            if (++dealer->cursor == dealer->m_players.size())
                dealer->set_current(CONTROLLER::YOURSELF_SERVANT);
        }
        else if (response.Response == EVENT::DOUBLEDOWN){
            if (CURRENT_PLAYER->ShowHand().GetSize() == 2){
                auto card = STACK->GetCard();
                CURRENT_PLAYER->SetCard(card);

                Event take_p_card1(EVENT::GIVECARD, std::string(
                        "Player " + std::to_string(dealer->cursor) + " take a card: " + std::string(card)));
                dealer->general_view_manager->notify(take_p_card1.GetData<std::string>());

                CURRENT_BET *= 2;
                Event bet(EVENT::MAKEBET, CURRENT_BET);
                dealer->general_view_manager->notify(std::string("Player " + std::to_string(dealer->cursor) + " doubled his bet: " + std::to_string(bet.GetData<int>())));
            } else {
                Event warn(EVENT::WARN, std::string("YOU CANT DO IT"));
                dealer->general_view_manager->notify(warn.GetData<std::string>());
            }
        } else {
            Event warn(EVENT::WARN, std::string("YOU CANT DO IT"));
            dealer->general_view_manager->notify(warn.GetData<std::string>());
        }
    } else {
        if (++dealer->cursor == dealer->m_players.size())
            dealer->set_current(CONTROLLER::YOURSELF_SERVANT);
    }

}


void DealerHandlers::PlayoutHandler::servePlayout(Controller::IDealer * dealer) {
    Event cur_state(EVENT::STATE, std::string("Current State is PlayOut for all"));
    dealer->general_view_manager->notify(cur_state.GetData<std::string>());

    CURRENT_DEALER->ShowHand().UnSecret(1);

    //TODO показать карты все участнико, потом убрать при графике
    std::string dealer_cards;
    for (auto & cards : CURRENT_DEALER->ShowHand().LookAtCards()){
        dealer_cards += std::string(cards) + " ";
    }
    dealer_cards += " = " + std::to_string(CURRENT_DEALER->ShowHand().total());
    dealer->general_view_manager->notify(std::string("Dealer have: " + dealer_cards));

    size_t j = 0;
    for(auto & [player, bet] : dealer->m_players) {
        std::string player_cards;
        for (auto &cards : player->ShowHand().LookAtCards()) {
            player_cards += std::string(cards) + " ";
        }
        player_cards += " = " + std::to_string(player->ShowHand().total());
        dealer->general_view_manager->notify(std::string("Player " + std::to_string(j) + " have: " + player_cards));
        j++;
    }
    // TODO конец
    size_t p = 0;
    for (auto & [player, bet] : dealer->m_players){
        if (player->ShowHand().total() == BLACKJACK && player->ShowHand().GetSize() == 2 && CURRENT_DEALER->ShowHand().total() != BLACKJACK) {
            player->GetRoundResult(static_cast<int>(static_cast<double>(bet) * WinFactor));
            CURRENT_DEALER->GetRoundResult((-1) * static_cast<int>(static_cast<double>(bet) * WinFactor));

            Event won(EVENT::WIN, std::string(
                    "Player " + std::to_string(p++) + " get BlackJack in 2 card, win: " + std::to_string(bet + static_cast<int>(static_cast<double>(bet) * WinFactor))));
            dealer->general_view_manager->notify(won.GetData<std::string>());
        }
        else if ((player->ShowHand().total() > CURRENT_DEALER->ShowHand().total() && player->ShowHand().total() <= BLACKJACK) || (player->ShowHand().total() <= BLACKJACK && CURRENT_DEALER->ShowHand().total() > BLACKJACK)){
            player->GetRoundResult(bet);
            CURRENT_DEALER->GetRoundResult((-1) * bet);

            Event won(EVENT::WIN, std::string(
                    "Player " + std::to_string(p++) + " win: " + std::to_string(bet * 2)));
            dealer->general_view_manager->notify(won.GetData<std::string>());
        } else if (player->ShowHand().total() < CURRENT_DEALER->ShowHand().total() && CURRENT_DEALER->ShowHand().total() <= BLACKJACK){
            player->GetRoundResult((-1) * bet);
            CURRENT_DEALER->GetRoundResult(bet);

            Event lose(EVENT::LOSE, std::string(
                    "Player " + std::to_string(p++) + " lose: " + std::to_string(bet)));
            dealer->general_view_manager->notify(lose.GetData<std::string>());
        } else if (player->ShowHand().total() == CURRENT_DEALER->ShowHand().total() && CURRENT_DEALER->ShowHand().total() <= BLACKJACK){
            player->GetRoundResult(0);
            CURRENT_DEALER->GetRoundResult(0);

            Event draw(EVENT::DRAW, std::string(
                    "Player " + std::to_string(p++) + " draw with dealer, You get back: " + std::to_string(bet)));
            dealer->general_view_manager->notify(draw.GetData<std::string>());
        } else {p++;}
        player->ClearHand();
    }

    CURRENT_DEALER->ClearHand();
    dealer->cursor = 0;
    dealer->m_stack->TimeToShuffle();
    dealer->set_current(CONTROLLER::BET_SERVANT);
}

void DealerHandlers::PlayingHandler::serveYourself(Controller::IDealer * dealer) {
    Event cur_state(EVENT::STATE, std::string("Current State is DealerMoves for all"));
    dealer->general_view_manager->notify(cur_state.GetData<std::string>());

    CURRENT_DEALER->ShowHand().UnSecret(1);

    if (CURRENT_DEALER->ShowHand().total() == BLACKJACK && CURRENT_DEALER->ShowHand().GetSize() == 2) {
        // TODO показать карты, убрать при графике
        std::string dealer_cards;
        for (auto & cards : CURRENT_DEALER->ShowHand().LookAtCards()){
            dealer_cards += std::string(cards) + " ";
        }
        dealer->general_view_manager->notify(std::string("Dealer have BlackJack: " + dealer_cards));

        for (auto & [player, bet] : dealer->m_players){
            if (dealer->insurances.at(player)){
                player->GetRoundResult(0);
                CURRENT_DEALER->GetRoundResult(0);

                Event draw(EVENT::DRAW, std::string(
                        "Player " + std::to_string(dealer->cursor) + " save his bet, You get back: " + std::to_string(bet)));
                dealer->general_view_manager->notify(draw.GetData<std::string>());
            } else {
                player->GetRoundResult((-1) * bet);
                CURRENT_DEALER->GetRoundResult(bet);

                Event lose(EVENT::LOSE, std::string(
                        "Player " + std::to_string(dealer->cursor) + " lose: " + std::to_string(bet)));
                dealer->general_view_manager->notify(lose.GetData<std::string>());
            }
        }

        dealer->set_current(CONTROLLER::BET_SERVANT);
        dealer->cursor = 0;

    } else {
        while (CURRENT_DEALER->ShowHand().total() < DEALERBORDER) {
            auto card = STACK->GetCard();
            CURRENT_DEALER->SetCard(card);

            // потом для графики можно будет подменить на полный Event
            Event take_card(EVENT::GIVECARD, std::string("Dealer take a card: " + std::string(card)));
            dealer->general_view_manager->notify(take_card.GetData<std::string>());
        }

        dealer->set_current(CONTROLLER::PLAYOUT_SERVANT);
        dealer->cursor = 0;
    }
}

void DealerHandlers::DealHandler::serveMove(Controller::IDealer * dealer) {
    Event cur_state_for_player(EVENT::STATE,
                               std::string("Current State is DEAL for " + std::to_string(dealer->cursor) + ""));
    dealer->general_view_manager->notify(
            cur_state_for_player.GetData<std::string>());

    auto response = CURRENT_PLAYER->Answer();

    dealer->insurances[CURRENT_PLAYER] = response.Response == EVENT::YES;

    if (++dealer->cursor == dealer->m_players.size()) {
        CURRENT_DEALER->ShowHand().UnSecret(1);
        auto is_blackJack = CURRENT_DEALER->ShowHand().total();
        CURRENT_DEALER->ShowHand().MakeSecret(1);
        if (is_blackJack == BLACKJACK) {
            dealer->set_current(CONTROLLER::PLAYOUT_SERVANT);
        } else {
            size_t i = 0;
            for (auto & [player, bet] : dealer->m_players){
                if (dealer->insurances.at(player)){
                    player->GetRoundResult((-1) * bet / 2);
                    CURRENT_DEALER->GetRoundResult(bet / 2);

                    Event lose(EVENT::LOSE, std::string(
                            "Player " + std::to_string(i) + " insurance lost: " + std::to_string(bet / 2)));
                    dealer->general_view_manager->notify(lose.GetData<std::string>());
                }
                i++;
            }
            dealer->insurances.clear();
            dealer->cursor = 0;
            dealer->set_current(CONTROLLER::MOVE_SERVANT);
        }
    }
}