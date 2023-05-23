// Purpose: Source file for AI class
#include <iostream>
#include "AI.h"
#include "Game.h"
    
    AI::AI()
    {
        this->ai_agility_time = 1; // second
        this->ai_smartness = 80; // 80 % smartness
        this->rand_num = 100; 
    } 
        
    
    void AI::reset_ai()
    {
        this->rand_num = rand() % 100;
    }

    bool AI::ai_should_play_smart(int wins=0, int losses=0, float player_think_time = 0)
    {
        if (player_think_time <= 0)
        {
            player_think_time = this->ai_agility_time;
        }
            
        player_think_time = std::min(player_think_time, this->ai_agility_time);

        //make sure the division by total will not result in DivisionByZeroException
        int total = std::max(wins + losses, 1);

        //ai should take in consideration wins and loses after a minimum of 5 plays
        if (total < 5)
        {
            wins = losses = 0;
        }
        
        //from the current smartness fa
        int multiplier = this->ai_smartness / 2;

        int ai_score = this->ai_smartness - (this->ai_agility_time - player_think_time ) * (multiplier / this->ai_agility_time) + (wins - losses) * (multiplier / total);
        
        std::cout << "Computer's score would be: " << ai_score << " knowing we have " << wins << " wins and " << losses << " losses, while random number is: " << this->rand_num << std::endl;
        return this->rand_num <= ai_score;
    }

    Cell AI::get_ai_move(Game& board,float player_time=-1)
    {
        std::cout << "history: " << board.history << std::endl;
        std::cout << "received player time: " << player_time << std::endl;
        char player = 'X';
        char enemy = 'O';
        int wins = std::count(board.history.begin(), board.history.end(), player);
        int losses = std::count(board.history.begin(), board.history.end(), enemy);
        if (this->ai_should_play_smart(wins, losses, player_time))
        {
            std::cout << "Computer is playing smart!" << std::endl;
            return board.next_best_move(enemy, player);
        }
        return board.wrong_move(enemy);
    }
