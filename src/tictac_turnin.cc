#include <tictac_support.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <limits>


/**
    moves_left: checks for available moves
    args:
      int [][3] bo: 3x3 array of ints representing the
      board state. The values of board are altered based
      on the move
    returns (bool):
      true or false if board a move can or cannot be made respectively
**/
bool moves_left(int bo[][3])
  {
    for(int i = 0; i < 3; i++)
    {
      for(int j = 0; j < 3; j++)
        {
          if(bo[i][j] == 0)
              return true;
        }
    }
    return false;
  }

/**
  evald: checks for wins and returns value based on state of board
  args:
    int pr: the state will be passed and referred to as
            pr or player in the case of the Game
  returns (int):
    10, -10, or 0 if the player won losses or ties on the move
**/
int evald(int board[][3], int pr)
  {
    for(int i = 0; i < 3; i++)
      {
        //if statement checking for horizontal wins
        if(board[i][0] == pr  && board[i][1] == pr && board[i][2] == pr)
          {
            if(pr == 1)
              return 10;
            else if (pr == -1)
              return -10;
          }
        //if statement checking for vertical wins
        if(board[0][i] == pr && board[1][i] == pr && board[2][i] == pr)
          {
            if(pr == 1)
              return 10;
            else if (pr == -1)
              return -10;
          }
      }
    //if statement checking for diagonal win top left to bottom right
    if(board[0][0] == pr && board[1][1] == pr && board[2][2] == pr)
      {
        if(pr == 1)
          return 10;
        else if (pr == -1)
          return -10;
      }
    //if statement checking for diagonal win bottom left to top right
    if(board[2][0] == pr && board[1][1] == pr && board[0][2] == pr)
      {
        if(pr == 1)
          return 10;
        else if (pr == -1)
          return -10;
      }
    return 0;
  }


int minimax(int board[][3], int d, bool alpha)
  {

    //determining whose turn it is
    int state = 0;
    for( int i = 0; i < 3; i++ )
      for( int j = 0; j < 3; j++ )
        state += board[i][j];

    state = -state;
    if( state == 0 )
      state = 1;
    //assigning reward for winning or losing
    int reward = evald(board,state);

    //if reward found, return it
    if(reward == 10 || reward == - 10)
      return reward;

    //if there are no moves left return 0
    if(moves_left(board)==false)
      return 0;
    int step = 0;
    //if maximizer's turn
    if(alpha)
      {
        int bestVal = -1000;
        for(int i = 0; i < 3; i++)
          {
            for(int j = 0; j < 3; j++)
              {
                //checking for empty space
                if(board[i][j] == 0)
                  {
                    //making move for maximizer
                    board[i][j] = 1;
                    //find the best valued move using recursion(minimax)
                    bestVal = std::max(bestVal, minimax(board,d+1,!alpha));
                    //retract move
                    board[i][j] = 0;
                    step+=1;
                  }
              }
          }
        return bestVal;
      }
    //it's the minimizers move
    else
      {
        int bestVal = 1000;
        for(int i = 0; i < 3; i++)
          {
            for(int j = 0; j < 3; j++)
              {
                if(board[i][j] == 0)
                  {
                    board[i][j] = -1;
                    bestVal = std::./min(bestVal, minimax(board,d+1,!alpha));
                    board[i][j] = 0;
                    step+=1;
                  }
              }
          }
          return bestVal;
      }
  }

/**
	make_move: takes a board state and makes a legal
	(hopefully optimal) move
	args:
		int [][3] board: 3x3 array of ints representing the
		board state. The values of board are altered based
		on the move
			0: empty
			1: X
		 -1: O

	returns (int):
		the number of steps it took to choose the best move
		(current implementation returns 1 by default, 0 if no move made)
**/

int make_move( int board[][3])
{
	int bestVal = -1000;
  int row;
  int col;
  int steps;
  int state = 0;
  for( int i = 0; i < 3; i++ )
    for( int j = 0; j < 3; j++ )
      state += board[i][j];

  state = -state;
  if( state == 0 )
    state = 1;
  if(moves_left(board) == false)
    return 0;
	// default behavior: find any unoccupied square and make the move
	for( int i = 0; i < 3; i++ )
  {
		for( int j = 0; j < 3; j++ )
    {
			// find an empty square
      //checking if space is empty and makes move in spot
			if( board[i][j] == 0 )
			{
        steps+=1;
        board[i][j] = 1;
        int playValue = minimax(board, 0 , false);
        board[i][j] = 0;
        if(playValue > bestVal)
          {
            row = i;
            col = j;
            bestVal = playValue;
          }
        }
      }
    }
				// that's the move
				printf( "player [%d] made move: [%d,%d]\n", state, row, col );
        //HOW TO MAKE MOVE IN FUNCTION
				board[row][col] = state;
				return steps;
}
