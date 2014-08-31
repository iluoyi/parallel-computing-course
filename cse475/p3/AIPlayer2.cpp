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
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"

#include "GameBoard.hpp"
#include "AIPlayer2.hpp"

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
AIPlayer2::AIPlayer2(GameBoard* _game, int _player, int _opponent, int _depth) : AIPlayer(_game, _player) { 
	opponent = _opponent;
	depth = _depth;
}


/**
 *  Determine the quality of a move, by giving it a score
 *
 */
int AIPlayer2::rate_move(GameBoard* theGame, int x, int y, int thePlayer) const
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

int AIPlayer2::do_switch(int thePlayer) {
	if (thePlayer == player)
		return opponent;
	else
		return player;
}

bool AIPlayer2::is_move_candicate(GameBoard* theGame, int x, int y) {
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

int AIPlayer2::alphaBetaSearch(GameBoard* theGame, int alpha, int beta, int l, int thePlayer) {
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

struct Pos {
		int x;
		int y;
		Pos(int _x, int _y): x(_x), y(_y) {}
};
	
/**
 *  This struct can also be a class. This is useful for TBB implementation
 */
struct AISearch {
	vector<Pos> * moveList;
	int* mList;
	AIPlayer2 * agent;
	GameBoard * theGame;
	int alpha, beta, depth, player;
	
	AISearch(vector<Pos> * _moveList, int* _mList, GameBoard* _theGame, int _alpha, int _beta, AIPlayer2 * _agent): 
		moveList(_moveList), theGame(_theGame), mList(_mList), alpha(_alpha), beta(_beta), agent(_agent) { }
	
	void operator() (const blocked_range<int>& r) const {
		for ( int i = r.begin(); i != r.end(); i++ ) { // the algorithm keeps the same as the sequential implementation
			int x = moveList->at(i).x;
			int y = moveList->at(i).y;
			
			// move one step by the player and evaluate all following available steps
			GameBoard nextGame(theGame);
			nextGame.make_move(x, y, agent->player);
			
			mList[i] = -(agent->alphaBetaSearch(&nextGame, alpha, beta, agent->depth-1, agent->do_switch(agent->player)));
		}
	}
};

/**
 *  Use brute force to evaluate every possible move, and find the best one.
 *  Once the best move is found, play it.
 */
void AIPlayer2::play() {
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
			//add this move to the candidate list
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
		// for (int i = 0; i < moveList->size(); i ++){
			// int x = moveList->at(i).x;
			// int y = moveList->at(i).y;
		
			// GameBoard nextGame(game);
			// // move one step by the player and evaluate all following available steps by opponent
			// nextGame.make_move(x, y, player); 
			// val = -alphaBetaSearch(&nextGame, alpha, beta, depth-1, do_switch(player));
			// //backtrack
					
			// if (val > cur_max) {
				// cur_max = val;
				// best_x = x;
				// best_y = y;
			// }
		// }
		
		int* mList = new int[moveList->size()];
		parallel_for(blocked_range<int>(0, moveList->size()), AISearch(moveList, mList, game, alpha, beta, this), 
					auto_partitioner()); // TBB's loop parallelization
			
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
