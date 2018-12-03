// File:            player.h
// Module:          Player
// Dependencies:    Audio, Intrument, App, Pattern, Player, Volume
// Description:     Manages audio buffer

#ifndef __PLAYER_H
#define __PLAYER_H

#include <stdint.h>

#define Player_BufferSize 8000
#define Player_NumTracks 16

void Player_Init(void);
void Player_Start(void);
void Player_Stop(void);
void Player_CurrStep(void);

#endif
