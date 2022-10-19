#include<iostream>
#include<vector>
#include<algorithm>
#include<fstream>
#include"Piece.h"
#include"Bishop.h"
#include"Board.h"
using namespace std;

Bishop::Bishop(char newColor): Piece(newColor) {
    if(newColor == 'b')
        type = 'B';
    else
        type = 'b';
}

vector< pair<char, int> > Bishop::validMoves(const char y, const int x, Board& board)const {
    vector< pair<char, int> > moves;
    bishopMoves(y, x, board, moves);
    return moves;
}
