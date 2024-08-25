#pragma once

#include "board.h"
#include <vector>

std::vector<Move> order_moves(Position &p, std::vector<Move> moves);

Move search(Position &p, int depth);
