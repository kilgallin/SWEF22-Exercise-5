#include<iostream>
#include<vector>
#include"Piece.h"
#include"Pawn.h"
#include"Board.h"
using namespace std;

Pawn::Pawn(char newColor): Piece(newColor) {
    if(newColor == 'b')
        type = 'P';
    else
        type = 'p';
}

vector< pair<char, int> > Pawn::validMoves(const char y, const int x, Board& board)const{
    vector< pair<char, int> > moves;
    if ((color == 'w' && x != 8) || (color == 'b' && x != 1)){
        if(color == 'w' && board.at(y, x + 1) == NULL)
            moves.push_back(make_pair(y, x+1));
        if(color == 'b' && board.at(y, x - 1) == NULL)
            moves.push_back(make_pair(y, x-1));
        if(color == 'w' && x == 2 && board.at(y, x + 1) == NULL && board.at(y, x + 2) == NULL)
            moves.push_back(make_pair(y, x+2));
        if(color == 'b' && x == 7 && board.at(y, x - 1) == NULL && board.at(y, x - 2) == NULL)
            moves.push_back(make_pair(y, x-2));
        if(color == 'w' && board.at(y - 1, x + 1) != NULL && board.at(y - 1, x + 1)->getColor() == 'b')
            moves.push_back(make_pair(y-1, x+1));
        if(color == 'w' && board.at(y + 1, x + 1) != NULL && board.at(y + 1, x + 1)->getColor() == 'b')
            moves.push_back(make_pair(y+1, x+1));
        if(color == 'b' && board.at(y - 1, x - 1) != NULL && board.at(y - 1, x - 1)->getColor() == 'w')
            moves.push_back(make_pair(y-1, x-1));
        if(color == 'b' && board.at(y + 1, x - 1) != NULL && board.at(y + 1, x - 1)->getColor() == 'w')
            moves.push_back(make_pair(y+1, x-1));
    }
    return moves;

}
