#pragma once

#include <cstdint>

struct Move;
struct Position;

uint64_t perft(int depth, Position &p);

void perft_up_to(int depth, Position p);
