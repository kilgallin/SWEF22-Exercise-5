#include<iostream>
#include<vector>
#include"Piece.h"
#include"King.h"
#include"Board.h"
using namespace std;

King::King(char newColor): Piece(newColor) {
    if(newColor == 'b')
        type = 'K';
    else
        type = 'k';
    moved = false;
}



vector< pair<char, int> > King::validMoves(const char y, const int x, Board& board)const{
    vector< pair<char, int> > moves, validatedMoves;
    if(board.at(y, x + 1) == NULL || board.at(y, x + 1)->getColor() != color)
        moves.push_back(make_pair(y, x + 1));
    if(board.at(y + 1, x + 1) == NULL || board.at(y + 1, x + 1)->getColor() != color)
        moves.push_back(make_pair(y + 1, x + 1));
    if(board.at(y + 1, x) == NULL || board.at(y + 1, x)->getColor() != color)
        moves.push_back(make_pair(y + 1, x));
    if(board.at(y + 1, x - 1) == NULL || board.at(y + 1, x - 1)->getColor() != color)
        moves.push_back(make_pair(y + 1, x - 1));
    if(board.at(y, x - 1) == NULL || board.at(y, x - 1)->getColor() != color)
        moves.push_back(make_pair(y, x - 1));
    if(board.at(y - 1, x - 1) == NULL || board.at(y - 1, x - 1)->getColor() != color)
        moves.push_back(make_pair(y - 1, x - 1));
    if(board.at(y - 1, x) == NULL || board.at(y - 1, x)->getColor() != color)
        moves.push_back(make_pair(y - 1, x));
    if(board.at(y - 1, x + 1) == NULL || board.at(y - 1, x + 1)->getColor() != color)
        moves.push_back(make_pair(y - 1, x + 1));

    for(unsigned int i = 0; i < moves.size(); i++){
        if(moves[i].first <= 'h' && moves[i].first >= 'a' && moves[i].second <= 8 && moves[i].second >= 1)
            validatedMoves.push_back(moves[i]);
    }
    return validatedMoves;
}
