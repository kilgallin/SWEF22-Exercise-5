#include<iostream>
#include<vector>
#include"Piece.h"
#include"Board.h"
using namespace std;

class Board;

void Piece::rookMoves(const char y, const int x, Board& board, vector< pair<char, int> >& moves) const{
    char i = y;
    int j = x;
    while(j > 1 && (board.at(i, j-1) == NULL || board.at(i, j-1)->getColor() != color) ){
        j--;
        moves.push_back(make_pair(i, j));
        if(board.at(i, j) != NULL && board.at(i, j)->getColor() != color)
            break;
    }
    j = x;
    while(j < 8 && (board.at(i, j+1) == NULL || board.at(i, j+1)->getColor() != color) ){
        j++;
        moves.push_back(make_pair(i, j));
        if(board.at(i, j) != NULL && board.at(i, j)->getColor() != color)
            break;
    }
    j = x;
    while(i > 'a' && (board.at(i-1, j) == NULL || board.at(i-1, j)->getColor() != color) ){
        i--;
        moves.push_back(make_pair(i, j));
        if(board.at(i, j) != NULL && board.at(i, j)->getColor() != color)
            break;
    }
    i = y;
    while(i < 'h' && (board.at(i+1, j) == NULL || board.at(i+1, j)->getColor() != color) ){
        i++;
        moves.push_back(make_pair(i, j));
        if(board.at(i, j) != NULL && board.at(i, j)->getColor() != color)
            break;
    }
}

void Piece::bishopMoves(const char y, const int x, Board& board, vector< pair<char, int> >& moves) const{
    char i = y;
    int j = x;

    int iter[4][2] = {{-1, -1}, {1, 1}, {-1, 1}, {1, -1}};

    for(int k = 0; k < 4; k++){
        while(j >= 1 && i >= 'a' && j <= 8 && i <= 'h' && (board.at(i+iter[k][0], j+iter[k][1]) == NULL || board.at(i+iter[k][0], j+iter[k][1])->getColor() != color) ){
            i+=iter[k][0];
            j+=iter[k][1];
            if(j >= 1 && i >= 'a' && j <= 8 && i <= 'h'){
                moves.push_back(make_pair(i, j));
            }
            if(board.at(i, j) != NULL && board.at(i, j)->getColor() != color)
                break;
        }
        j = x;
        i = y;
    }

}
