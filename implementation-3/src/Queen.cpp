#include<iostream>
#include<vector>
#include"Piece.h"
#include"Queen.h"
#include"Board.h"
using namespace std;

Queen::Queen(char newColor): Piece(newColor) {
    if(newColor == 'b')
        type = 'Q';
    else
        type = 'q';
}

vector< pair<char, int> > Queen::validMoves(const char y, const int x, Board& board)const {
    vector< pair<char, int> > moves;
    bishopMoves(y, x, board, moves);
    rookMoves(y, x, board, moves);
    return moves;
}

