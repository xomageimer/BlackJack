#ifndef BLACKJACK_COMPUTERPLAYER_H
#define BLACKJACK_COMPUTERPLAYER_H

#include <memory>

#include "IPlayer.h"
#include "OutputManager.h"

namespace Actors {
    struct ComputerPlayer : public IPlayer {
        std::shared_ptr<AI_view> eyes;

    };
}


#endif //BLACKJACK_COMPUTERPLAYER_H
