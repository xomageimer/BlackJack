#include "Controller.h"
#include "Actors/GameGround.h"


IController::IController(GameGround * grnd) : m_facade(grnd) {}

void IController::NextState(const IController::DealerLogic & logic) {
    m_facade->SetState(logic);
}

void BetableController::HandleEvent(const Event &event) {
    switch (event.type){
        case Event::Type::BET:
            m_facade->IssuingBet(event);
            break;
        case Event::Type::MAKEBET:
            m_facade->MakeBet(event);
            m_facade->Output();
            break;
        case Event::Type::SWAPPLAYER:
            m_facade->ChangePlayer(event);
            m_facade->Output();
            // TODO обнулить таймер
            if (m_facade->is_around()) {
                NextState(IController::DealerLogic::DISTRIB);
                m_facade->Destroy();
            }
            break;
        // TODO event (от таймера) отказ от участия если вовремя не получается ставка и перенос в AFK
        case Event::Type::WARN :
        default:
            m_facade->OutWarn(event);
    }
}

void DistributionController::HandleEvent(const Event &event) {
    switch (event.type){
        case Event::Type::NEWROUND :
            m_facade->IssuingCards();
            m_facade->IssuingCards();
            if (!m_facade->is_around()) {
                m_facade->NewRound();
                m_facade->Output();
            } else {
                NextState(DealerLogic::DEALERABLE);
                m_facade->Destroy();
            }
            break;
        case Event::Type::GIVECARD :
            m_facade->GiveCards(event);
            break;
        case Event::Type::SWAPPLAYER:
            m_facade->ChangePlayer(event);
            m_facade->Output();
            break;
        default:
            m_facade->Output();
    }
}

void DealerableController::HandleEvent(const Event &event) {
    switch (event.type){
        case Event::Type::HIT :
            m_facade->IssuingCards();
            break;
        case Event::Type::STAND :
            m_facade->IssuingStop();
            break;
        case Event::Type::DOUBLEDOWN :
            m_facade->IssuingDoubleDown();
            break;
        case Event::Type::LOSE :
        case Event::Type::WIN :
            m_facade->SetResult(event);
            m_facade->Output();
            break;
        case Event::Type::GIVECARD :
            m_facade->GiveCards(event);
            m_facade->Output();
            break;
        case Event::Type::SWAPPLAYER :
            m_facade->ChangePlayer(event);
            if (m_facade->is_around()) {
                NextState(IController::DealerLogic::PLAYABLE);
                m_facade->Destroy();
            }
            m_facade->Output();
            break;
        case Event::Type::PLAYOUT :
            if (m_facade->is_around()){
                m_facade->TimeToShuffle();
                NextState(IController::DealerLogic::BETABLE);
            } else {
                m_facade->Result(); // дальше играет диллер, он должен стать плеером, выполнить свой скрипт, затем стать снова диллером и подвести итоги
                m_facade->Output();
            }
            break;
        case Event::Type::WARN :
        default:
            m_facade->OutWarn(event);
            break;
    }
}

void PlayeableController::HandleEvent(const Event &event) {
    switch (event.type) {
        case Event::Type::STAND :
            m_facade->IssuingStop();
            break;
        case Event::Type::SWAPPLAYER :
            if (count++ > 0) {
                NextState(IController::DealerLogic::DEALERABLE);
                m_facade->Result();
            } else {
                m_facade->IssuingCards();
                count = 0;
            }
            m_facade->Destroy();
            break;
        case Event::Type::GIVECARD :
            m_facade->GiveCards(event);
            m_facade->Output();
            break;
        case Event::Type::WARN :
        default:
            m_facade->OutWarn(event);
            break;
    }
}