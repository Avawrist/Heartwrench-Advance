#include "game_state.h"

//////////////////////
// Struct GameState //
//////////////////////

GameState::GameState()
{
    current_displayed_level_currency = 0;
    current_displayed_world_currency = 0;
    current_level_currency           = 0;
    current_world_currency           = 0;
}

GameState::GameState(const GameState& other)
{
    current_displayed_level_currency = other.current_displayed_level_currency;
    current_displayed_world_currency = other.current_displayed_world_currency;
    current_level_currency           = other.current_level_currency;
    current_world_currency           = other.current_world_currency;
}

GameState::~GameState()
{

}

GameState& GameState::operator =(const GameState& other)
{
    current_displayed_level_currency = other.current_displayed_level_currency;
    current_displayed_world_currency = other.current_displayed_world_currency;
    current_level_currency           = other.current_level_currency;
    current_world_currency           = other.current_world_currency; 

    return *this;
}

void GameState::update()
{
    // Clamp Level & World Currency
    current_level_currency           = clamp(0, MAX_CURRENCY, current_level_currency);
    current_displayed_level_currency = clamp(0, MAX_CURRENCY, current_displayed_level_currency);

    current_world_currency           = clamp(0, MAX_CURRENCY, current_world_currency);
    current_displayed_world_currency = clamp(0, MAX_CURRENCY, current_displayed_world_currency);
        
    // Update Displayed Currency:
    if(current_displayed_level_currency < current_level_currency) {current_displayed_level_currency += CURRENCY_INCREMENT_UNIT;}
    if(current_displayed_world_currency < current_world_currency) {current_displayed_world_currency += CURRENCY_INCREMENT_UNIT;}
}