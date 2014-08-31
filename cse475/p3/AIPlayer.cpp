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

#include "GameBoard.hpp"
#include "AIPlayer.hpp"

/**
 *  When scoring moves, this is the benefit of taking a corner
 */
#define CORNER_VALUE 3

/**
 *  Constructor for AIPlayer
 */
AIPlayer::AIPlayer(GameBoard* _game, int _player)
    : player(_player), game(_game)
{ }

/**
 *  Determine the quality of a move, by giving it a score
 *
 *  This is a pretty weak AI function... we could do a lot better with
 *  lookahead...
 */
int AIPlayer::rate_move(int x, int y) const
{
    // return if coordinates are off the board
    if (x >= game->get_size() || y >= game->get_size() || x < 0 || y < 0)
        return 0;

    // return if slot is not empty
    if (game->get_slot_status(x,y) != 0)
        return 0;

    // return if move invalid
    if (game->is_valid_move(x,y,player) == 0)
        return 0;

    // greedy heuristic
    int temp = game->flip_count(x,y,player);

    // favor corners that we take
    if ((x == 0) || (x == (game->get_size() - 1)))
        temp += CORNER_VALUE;
    if ((y == 0) || (y == (game->get_size() - 1)))
        temp += CORNER_VALUE;

    // discourage giving corner to opponent
    if ((x == 1) || (x == (game->get_size() - 2)))
        temp -= CORNER_VALUE;
    if ((y == 1) || (y == (game->get_size() - 2)))
        temp -= CORNER_VALUE;

    // return our score, or 1, whichever is greater
    return (temp > 0) ? temp : 1;
}

/**
 *  Use brute force to evaluate every possible move, and find the best one.
 *  Once the best move is found, play it.
 */
void AIPlayer::play()
{
    int x = 0,
        y = 0,
        cur_max = 0,
        best_x = 0,
        best_y = 0,
        temp = 0,
        rand_n = 0;

    srand(time(NULL));

    for (x = 0; x < game->get_size(); x++) {
        for (y = 0; y < game->get_size(); y++) {
            temp = rate_move(x, y);
            // if this move is better, choose it
            if (temp > cur_max) {
                cur_max = temp;
                best_x = x;
                best_y = y;
            }
            // otherwise flip a coin to make a decision
            else if (temp == cur_max) {
                rand_n = rand() % 2;
                if (rand_n  == 1) {
                    best_x = x;
                    best_y = y;
                }
            }
        }
    }
    game->make_move(best_x,best_y,player);
}
