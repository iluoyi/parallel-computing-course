/*
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// Original Author      : Keunhong Park
// Original Source      : https://github.com/void42/Reversi

#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <time.h>
#include <vector>
#include "tbb/task_group.h"

#include "GameBoard.hpp"
#include "AIPlayer3.hpp"

using namespace tbb;
using std::cout;
using std::endl;
using std::vector;

/**
 *  When scoring moves, this is the benefit of taking a corner
 */
#define CORNER_VALUE 3

/**
 *  Constructor for AIPlayer
 */
AIPlayer3::AIPlayer3(GameBoard* _game, int _player, int _opponent, int _depth) : AIPlayer(_game, _player) { 
	opponent = _opponent;
	depth = _depth;
}


/**
 *  Determine the quality of a move, by giving it a score
 *
 */
int AIPlayer3::rate_move(GameBoard* theGame, int x, int y, int thePlayer) const
{
    // greedy heuristic
    int temp = theGame->flip_count(x,y,thePlayer);

    // favor corners that we take
    if ((x == 0) || (x == (theGame->get_size() - 1)))
        temp += CORNER_VALUE;
    if ((y == 0) || (y == (theGame->get_size() - 1)))
        temp += CORNER_VALUE;

    // discourage giving corner to opponent
    if ((x == 1) || (x == (theGame->get_size() - 2)))
        temp -= CORNER_VALUE;
    if ((y == 1) || (y == (theGame->get_size() - 2)))
        temp -= CORNER_VALUE;

    // return our score, or 1, whichever is greater
    return (temp > 0) ? temp : 1;
}

int AIPlayer3::do_switch(int thePlayer) {
	if (thePlayer == player)
		return opponent;
	else
		return player;
}

bool AIPlayer3::is_move_candicate(GameBoard* theGame, int x, int y) {
	// if coordinates are off the board
	if (x >= theGame->get_size() || y >= theGame->get_size() || x < 0 || y < 0)
		return false;
	// if slot is not empty
	if (theGame->get_slot_status(x,y) != 0)
		return false;
	// return if move invalid
	if (theGame->is_valid_move(x,y,player) == 0)
		return false;
	return true;
}

int AIPlayer3::alphaBetaSearch(GameBoard* theGame, int alpha, int beta, int l, int thePlayer) {
	int best = -32767; // a very small number
	int val = 0; 
	
	if (l <= 1) {
		for (int x = 0; x < theGame->get_size(); x++) {
			for (int y = 0; y < theGame->get_size(); y++) {
				if (!is_move_candicate(theGame, x, y))
					continue;
				
				if (thePlayer == player)
					val = rate_move(theGame, x, y, player);
				else
					val = -rate_move(theGame, x, y, opponent);
					
				if (val > best) {
					best = val;
				}
			}
		}	
	} 
	else {
		 for (int x = 0; x < theGame->get_size(); x++) {
			for (int y = 0; y < theGame->get_size(); y++) {
				if (!is_move_candicate(theGame, x, y))
					continue;
			
				GameBoard nextGame(theGame);
				// move one step by the player and evaluate all following available steps
				nextGame.make_move(x, y, thePlayer); 
				val = -alphaBetaSearch(&nextGame, -beta, -alpha, l-1, do_switch(thePlayer));
				//backtrack

				if (val >= beta) {
					return val;
				}
			
				if (val > best) {
					best = val;
					if (val > alpha) {
						alpha = val;
					}
				}
			}
		}
	}
	return best;
}

/**
 *  Use brute force to evaluate every possible move, and find the best one.
 *  Once the best move is found, play it.
 */
void AIPlayer3::play() {
	struct Pos {
		int x;
		int y;
		Pos(int _x, int _y): x(_x), y(_y) {}
	};
	
    int val = 0,
		cur_max = -32767,
        best_x = 0,
        best_y = 0,
		beta = 32767,
		alpha = -32767;
	vector<Pos> * moveList = new vector<Pos>();
	
	for (int x = 0; x < game->get_size(); x++) {
		for (int y = 0; y < game->get_size(); y++) {
			if (!is_move_candicate(game, x, y))
				continue;
			//else, add this move to the candidate list
			Pos pos(x, y);
			moveList->push_back(pos);
		}
	}
	
	if (depth <= 1) {
		for (int i = 0; i < moveList->size(); i ++){
			int x = moveList->at(i).x;
			int y = moveList->at(i).y;
			val = rate_move(game, x, y, player);
			if (val > cur_max) {
				cur_max = val;
				best_x = x;
				best_y = y;
			}
		}
	} else {
		task_group group;
		int* mList = new int[moveList->size()];
		
		for (int i = 0; i < moveList->size(); i ++){
			int x = moveList->at(i).x;
			int y = moveList->at(i).y;
		
			// move one step by the player and evaluate all following available steps by opponent
			GameBoard nextGame(game);
			nextGame.make_move(x, y, player); 
			group.run([=, &nextGame]{mList[i] = -alphaBetaSearch(&nextGame, alpha, beta, depth-1, do_switch(player));});
		}
		group.wait();
		
		for (int i = 0; i < moveList->size(); i ++) {
			if (mList[i] > cur_max) {
				cur_max = mList[i];
				best_x = moveList->at(i).x;
				best_y = moveList->at(i).y;
			}
		}
	}
	
    game->make_move(best_x,best_y,player);
}
