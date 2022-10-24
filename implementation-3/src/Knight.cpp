#include<iostream>
#include<vector>
#include"Piece.h"
#include"Knight.h"
#include"Board.h"
using namespace std;

Knight::Knight (char newColor): Piece(newColor) {
    if(newColor == 'b')
        type = 'N';
    else
        type = 'n';
}

vector< pair<char, int> > Knight::validMoves(const char y, const int x, Board& board)const {
    vector< pair<char, int> > moves, validatedMoves;
    if(board.at(y+1, x+2) == NULL || board.at(y+1, x+2)->getColor() != color)
        moves.push_back(make_pair(y+1, x+2));
    if(board.at(y+1, x-2) == NULL || board.at(y+1, x-2)->getColor() != color)
        moves.push_back(make_pair(y+1, x-2));
    if(board.at(y-1, x+2) == NULL || board.at(y-1, x+2)->getColor() != color)
        moves.push_back(make_pair(y-1, x+2));
    if(board.at(y-1, x-2) == NULL || board.at(y-1, x-2)->getColor() != color)
        moves.push_back(make_pair(y-1, x-2));
    if(board.at(y+2, x+1) == NULL || board.at(y+2, x+1)->getColor() != color)
        moves.push_back(make_pair(y+2, x+1));
    if(board.at(y+2, x-1) == NULL || board.at(y+2, x-1)->getColor() != color)
        moves.push_back(make_pair(y+2, x-1));
    if(board.at(y-2, x+1) == NULL || board.at(y-2, x+1)->getColor() != color)
        moves.push_back(make_pair(y-2, x+1));
    if(board.at(y-2, x-1) == NULL || board.at(y-2, x-1)->getColor() != color)
        moves.push_back(make_pair(y-2, x-1));

    for(unsigned int i = 0; i < moves.size(); i++){
        if(moves[i].first <= 'h' && moves[i].first >= 'a' && moves[i].second <= 8 && moves[i].second >= 1)
            validatedMoves.push_back(moves[i]);
    }

    return validatedMoves;
}
