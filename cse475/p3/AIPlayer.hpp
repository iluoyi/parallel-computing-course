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

#ifndef AIPLAYER_HPP__
#define AIPLAYER_HPP__

class AIPlayer
{
	public:
    /**
     *  ID of the player
     */
    int player;

    /**
     *  Reference to game board
     */
    GameBoard* game;

    /**
     *  Internal method to score a move
     */
    virtual int rate_move(int x, int y) const;

    /**
     *  Constructor just stores a reference to the game, and stores the player
     *  ID
     */
    AIPlayer(GameBoard* game, int player);

    /**
     *  Tell the player to find the best move and make it
     */
    virtual void play();
};

#endif // AIPLAYER_HPP__
