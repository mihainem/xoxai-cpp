#ifndef AI_H
#define AI_H
#include "Cell.h"
#include "Game.h"

/*  self.ai_agility_time = 1 # second
        self.ai_smartness = 80 # 80 % smartness
        self.rand_num = 100 */
class AI
{
public:
    AI(); 
    Cell get_ai_move(Game& board,float player_time=-1);
    void reset_ai();

private:        
    bool ai_should_play_smart(int wins=0, int losses=0, float player_think_time = 0);

    float ai_agility_time = 1; // second
    int ai_smartness = 80; // 80 % smartness
    int rand_num = 100;
};
#endif
