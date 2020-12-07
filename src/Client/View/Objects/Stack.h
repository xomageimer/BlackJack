#ifndef BLACKJACK_STACK_H
#define BLACKJACK_STACK_H

#include "Objects/Table.h"

struct Stack : public Table {
    Stack(glm::vec2 position, glm::vec2 size) : Table(position, size) {}
};


#endif //BLACKJACK_STACK_H
