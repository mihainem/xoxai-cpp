#include <unordered_map>
#include <string>
#include <functional>
#include "Constants.h"
#include <nlohmann/json.hpp>
#include "Game.h"
#include "AI.h"
#include "Cell.h"
#include "Localizations.cpp"

using json = nlohmann::json;

/*
import ai
import json
import localizations
from game import Game
import duel
import asyncio
import constants as c

ai = ai.AI()
events = {}

def set_player_move(game, data):
    row,col = move = [int(data[c.ROW]), int(data[c.COL])]
    player_symbol = data[c.IS_FIRST_PLAYER] and 'X' or 'O'
    game.make_move(move, player_symbol)
    result = game.check_play_ended()
    return result or "Server set move to: " + str(move) + " index: " + str(col + row*3)

def get_ai_hint(game, data):
    player_symbol = data[c.IS_FIRST_PLAYER] and 'X' or 'O'
    player_think_time = data.get(c.PLAYER_THINK_TIME, -1)
    if game.is_board_full():
        game.reset()
        return "Invalid board state"
    else:
        row,col = ai.get_ai_move(game, player_think_time)
        return {c.ROW:row, c.COL:col}

def get_hint(game, data):
    player_symbol = data[c.IS_FIRST_PLAYER] and 'X' or 'O'
    enemy_symbol = player_symbol == 'X' and 'O' or 'X'
    if game.is_board_full():
        game.reset()
        return "Invalid board state"
    else:
        row,col = game.next_best_move(player_symbol, enemy_symbol)
        return {c.ROW:row, c.COL:col}
    
def undo_move(game, data):
    no_of_undos = int(data[c.NO_OF_UNDOS])
    for i in range(no_of_undos):
        game.undo_move()
    return f"Undid {no_of_undos} moves"

def reset_board(game):
    game.reset()
    ai.reset_ai()
    return "board is now reset"

def dispatch_event(data):
    return data

async def init_new_host(websocket, data):
    code = data[c.EVENT_DATA][c.CODE]
    await duel.start(websocket, code)

async def init_new_join(websocket, data):
    code = data[c.EVENT_DATA][c.CODE]
    await duel.join(websocket, code)

def duel_play(websocket, data):
    print(data)
    
    

events[c.SET_MOVE] = lambda game,data: set_player_move(game, data)
events[c.GET_AI_HINT] = lambda game,data: get_ai_hint(game, data)
events[c.GET_HINT] = lambda game,data: get_hint(game, data)
events[c.UNDO_MOVE] = lambda game,data: undo_move(game, data)
events[c.RESET_BOARD] = lambda game, data: reset_board(game)
events[c.GET_LANGUAGE] = lambda game, data: localizations.get_language_dict(data)
events[c.DISPATCH_EVENT] = lambda game, data: dispatch_event(data)
events[c.INIT_HOST] = lambda websocket, data: init_new_host(websocket, data)
events[c.INIT_JOIN] = lambda websocket, data: init_new_join(websocket, data)
events[c.DUEL_PLAY] = lambda websocket, data: duel_play(websocket, data)


games = {}

def try_handle_event(message, websocket):
    name = message["event-name"]
    content = message["event-data"]
    
    game = games.get(websocket.id, Game())
    games[websocket.id] = game
    message["event-data"] = events.get(name, lambda game,data: "Invalid event")(game, content)
    return message*/

// Define a type for event handlers
using EventHandler = std::function<json(Game&, const json&)>;
AI ai;

json set_player_move(Game& game, const json& data)
{
    // Set the player's move
    int row = data[Constants::ROW];
    int col = data[Constants::COL];
    char player_symbol = data[Constants::IS_FIRST_PLAYER] ? 'X' : 'O';
    game.make_move({row, col}, player_symbol);
    json result = game.check_play_ended();
    if (result != nullptr)
    {
        return result;
    }
    else
    {
        return "Server set move to: " + std::to_string(row) + " " + std::to_string(col);
    }
}

json get_ai_hint(Game& game, const json& data)
{
    // Get the AI's hint
    char player_symbol = data[Constants::IS_FIRST_PLAYER] ? 'X' : 'O';
    int player_think_time = data[Constants::PLAYER_THINK_TIME];
    if (game.is_board_full())
    {
        game.reset();
        return "Invalid board state";
    }
    else
    {
        Cell move = ai.get_ai_move(game.board, player_think_time);
        return {{Constants::ROW, move.row}, {Constants::COL, move.col}};
    }
}

json get_hint(Game& game, const json& data)
{
    // Get the hint
    char player_symbol = data[Constants::IS_FIRST_PLAYER] ? 'X' : 'O';
    char enemy_symbol = player_symbol == 'X' ? 'O' : 'X';
    if (game.is_board_full())
    {
        game.reset();
        return "Invalid board state";
    }
    else
    {
        Cell move = game.next_best_move(player_symbol, enemy_symbol);
        return {{Constants::ROW, move.row}, {Constants::COL, move.col}};
    }

}

json undo_move(Game& game, const json& data)
{
    // Undo the move
    int no_of_undos = data[Constants::NO_OF_UNDOS];
    for (int i = 0; i < no_of_undos; i++)
    {
        game.undo_move();
    }
    return "Undid " + std::to_string(no_of_undos) + " moves";
}

json reset_board(Game& game)
{
    // Reset the board
    game.reset();
    ai.reset_ai();
    return "board is now reset";
}


// Define an unordered_map to map event names to handlers
std::unordered_map<std::string, EventHandler> events = {
    {Constants::SET_MOVE, [](Game& game, const json& data) -> json {
        return set_player_move(game, data);
        //return json({});
    }},
    {Constants::GET_AI_HINT, [](Game& game, const json& data) -> json {
        return get_ai_hint(game, data);
        //return json({});
    }},
    {Constants::GET_HINT, [](Game& game, const json& data) -> json {
        return get_hint(game, data);
       // return json({});
    }},
    {Constants::UNDO_MOVE, [](Game& game, const json& data) -> json {
        return undo_move(game, data);
       // return json({});
    }},
    {Constants::RESET_BOARD, [](Game& game, const json& data) -> json {
        return reset_board(game);
       // return json({});
    }},
    {Constants::GET_LANGUAGE, [](Game& game, const json& data) -> json {
        return get_language(data);
       // return json({});
    }},
    {Constants::DISPATCH_EVENT, [](Game& game, const json& data) -> json {
        return data;
       // return json({});
    }},
    {Constants::INIT_HOST, [](Game& game, const json& data) -> json {
    
        return json({});
    }},
    {Constants::INIT_JOIN, [](Game& game, const json& data) -> json {
    
        return json({});
    }},
    {Constants::DUEL_PLAY, [](Game& game, const json& data) -> json {
    
        return json({});
    }}
  
    // ... add more events here
};

// Add a new event and handler
void add_event(const std::string& event_name, EventHandler handler) {
    events[event_name] = handler;
}

// Use an event handler
void handle_event(const std::string& event_name) {
    auto it = events.find(event_name);
    if (it != events.end()) {
        it->second();
    } else {
        // Handle unknown event
    }
}
