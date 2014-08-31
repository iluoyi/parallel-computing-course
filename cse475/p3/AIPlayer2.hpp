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

#include "AIPlayer.hpp"

#ifndef AIPLAYER2_HPP__
#define AIPLAYER2_HPP__

class AIPlayer2 : public AIPlayer
{
  public:
	/**
     *  ID of the opponent
     */
    int opponent;
	
	/**
	 *  AI searching depth
	 */
	int depth;
	
	/**
     *  The utility function to switch the player
     */
	int do_switch(int thePlayer);
	
	/**
     *  The utility function to check the candidate move
     */
	bool is_move_candicate(GameBoard* theGame, int x, int y);
	
	/**
     *  The AI function via alpha_beta searching
     */
	int alphaBetaSearch(GameBoard* theGame, int alpha, int beta, int l, int thePlayer);
	
    /**
     *  Internal method to score a move
     */
    virtual int rate_move(GameBoard* theGame, int x, int y, int thePlayer) const;

    /**
     *  Constructor just stores a reference to the game, and stores the player
     *  ID
     */
    AIPlayer2(GameBoard* game, int player, int opponent, int depth);
	
	 /**
     *  Tell the player to find the best move and make it
     */
    virtual void play();
};

#endif // AIPLAYER2_HPP__
