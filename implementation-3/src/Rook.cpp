#include<iostream>
#include<vector>
#include<algorithm>
#include<fstream>
#include"Piece.h"
#include"Rook.h"
#include"Board.h"
using namespace std;

Rook ::Rook (char newColor): Piece(newColor) {
    if(newColor == 'b')
        type = 'R';
    else
        type = 'r';
    moved = false;
}

vector< pair<char, int> > Rook::validMoves(const char y, const int x, Board& board)const {
    vector< pair<char, int> > moves;
    rookMoves(y, x, board, moves);
    return moves;
}
