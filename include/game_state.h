#ifndef GAME_STATE_H
#define GAME_STATE_H

// My Libs
#include "math.h"
#include "utility.h"

//////////////////////
// Struct GameState //
//////////////////////

#define MAX_CURRENCY            999
#define CURRENCY_INCREMENT_UNIT 1

struct GameState
{
    int32 current_displayed_level_currency;
    int32 current_displayed_world_currency;
    int32 current_level_currency;
    int32 current_world_currency;

    GameState();
	GameState(const GameState& other);
	~GameState();

	GameState& operator =(const GameState& other);

    void update();
};

#endif