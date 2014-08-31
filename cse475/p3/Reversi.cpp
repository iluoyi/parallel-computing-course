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
#include <cstdio>
#include <unistd.h>

#include "GameBoard.hpp"
#include "AIPlayer.hpp"
#include "AIPlayer1.hpp"
#include "AIPlayer2.hpp"
#include "AIPlayer3.hpp"
#include "timing.hpp"

using namespace std;

/**
 *  Function prototypes
 */
void play(int size, int mode, bool quiet);

/**
 *  Print basic usage
 */
void usage()
{
    printf("Options: -l [int] -m [1-3] -s[int] -h\n");
	printf("  -a:  AI mode\n");
    printf("       0 - AI without lookahead\n");
    printf("       1 - AI with lookahead for task#1\n");
    printf("       2 - AI with lookahead for task#2\n");
	printf("       3 - AI with lookahead for task#3\n");
	printf("  -l:  AI searching/lookahead depth\n");
    printf("  -s:  size of the board\n");
    printf("  -m:  game mode\n");
    printf("       1 - two human players\n");
    printf("       2 - one human player\n");
    printf("       3 - zero human players\n");
    printf("  -q:  quiet mode\n");
    printf("  -h:  this message\n\n");
    printf("Note: you'll probably want to run with -m 3 -q, and vary -s\n");
}

/**
 * command-line args are "-s" for board size, and "-m" for mode
 */
int main(int argc, char * argv[])
{
	int aiMode = 1; // the one from task#1 is the default one
	int depth = 1;
    int boardsize = -1;
    int mode = -1;
    bool quiet = false;

    // parse the command-line options
    int opt;
    while ((opt = getopt(argc, argv, "a:l:s:m:hq")) != -1) {
        switch (opt) {
		  case 'a': aiMode 	  = atoi(optarg); break;
		  case 'l': depth	  = atoi(optarg); break;
          case 's': boardsize = atoi(optarg); break;
          case 'm': mode      = atoi(optarg); break;
          case 'q': quiet     = true;         break;
          case 'h': usage();                  return 0;
        }
    }
	
	// validate the AI mode
	if (aiMode < 0 || aiMode > 3) {
		usage();
		return 0;
	} else 
		printf("You select the AI mode: %d\n", aiMode);
	
	if (aiMode != 0) {
		// validate lookahead depth
		if (depth > 0)
			 printf("AI searching depth is: %d\n", depth);
		else {
			usage();
			return 0;
		}
	} else {
		printf("You select the AI without any lookahead.\n");
	}

    // validate board size
    if (boardsize > 0)
        printf("Board size is: %d x %d\n", boardsize, boardsize);
    else {
        usage();
        return 0;
    }

    // validate mode
    if (mode < 1 || mode > 3) {
        usage();
        return 0;
    }

    // validate quiet
    if (mode == 1)
        quiet = false;

    // create game object
    GameBoard game(boardsize);

    // create any AI agents
    AIPlayer* ai1 = NULL;
    AIPlayer* ai2 = NULL;
	
	if (aiMode == 0) {
		if (mode == 2) {
			ai2 = new AIPlayer(&game,2);
		}
		if (mode == 3) {
			ai1 = new AIPlayer(&game,1);
			ai2 = new AIPlayer(&game,2);
		}
	} else if (aiMode == 2){
		if (mode == 2) {
			ai2 = new AIPlayer2(&game,2,1, depth);
		}
		if (mode == 3) {
			ai1 = new AIPlayer2(&game,1,2, depth);
			ai2 = new AIPlayer2(&game,2,1, depth);
		}
	} else if (aiMode == 3){
		if (mode == 2) {
			ai2 = new AIPlayer3(&game,2,1, depth);
		}
		if (mode == 3) {
			ai1 = new AIPlayer3(&game,1,2, depth);
			ai2 = new AIPlayer3(&game,2,1, depth);
		}
	} else {
		if (mode == 2) {
			ai2 = new AIPlayer1(&game,2,1, depth);
		}
		if (mode == 3) {
			ai1 = new AIPlayer1(&game,1,2, depth);
			ai2 = new AIPlayer1(&game,2,1, depth);
		}
	}
 
    // start main game loop
    if (!quiet) {
        printf("Player 1's turn:\n");
        game.print_board();
    }
	
	uint64_t seqstart = getElapsedTime();
    while (game.get_status() != 0) {
        // get player 1 move, display new state, check for victory
        if (ai1 != NULL)
            ai1->play();
        else
            game.get_move(1);
        if (!quiet) {
            game.print_board();
            game.print_scores();
        }
        if (game.check_win(1) == true)
            break;
        // repeat procedure for player 2
        if (ai2 != NULL)
            ai2->play();
        else
            game.get_move(2);
        if (!quiet) {
            game.print_board();
            game.print_scores();
        }
        if (game.check_win(2) == true)
            break;
    }
	uint64_t seqstop = getElapsedTime();
	
    // Print final stats
    if (game.get_status() == 0) {
        game.print_board();
        printf("Game over !\n");
        printf("Player 1 score: %d\n",game.get_score(1));
        printf("Player 2 score: %d\n",game.get_score(2));
        int winner = game.determine_winner();
        if (winner == 0)
            printf("Player 1 and Player 2 have TIED.\n");
        else
            printf("Player %d WINS!\n",winner);
			
		cout << "Time elapsed:\t" << (seqstop - seqstart) << " (ns)" << endl;
    }
    return 0;
}
