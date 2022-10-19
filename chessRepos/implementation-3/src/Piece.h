/** @class Piece
* Abstract class Piece
*/
#ifndef PIECE_H_INCLUDED
#define PIECE_H_INCLUDED
#include<vector>
using namespace std;
class Board;
class Piece {
protected:
    char color, type;
public:
    /** Returns all the valid moves for the piece on a given square
    * @param y file (column) letter of the chosen square
    * @param x rank (row) number of the chosen square
    * @param board  the chessboard
    * @return a vector containing all the squares to which the chosen piece may move (disregarding special rules)
    */
    virtual vector< pair<char, int> > validMoves(char y, int x, Board& board) const = 0;
    /** Returns a character which describes the chosen piece
    * @return A character identifying the chosen piece. Lowercase for white, uppercase for black.
    */
    virtual char print() const { return type; }
    /** Returns a character which describes the chosen piece, disregarding color.
    * @return a character identifying the chosen piece.
    */
    virtual char getType() const = 0;
    virtual char getColor() const { return color; }
    /** Lists all valid moves for a piece that can move in a straight line.
    * This method is not overloaded because it is used both by the rook and the queen.
    * @param y file (column) letter of the chosen square
    * @param x rank (row) number of the chosen square
    * @param board the chessboard
    * @param moves the vector into which the valid moves are stored
    */
    void rookMoves(const char y, const int x, Board& board, vector< pair<char, int> >& moves) const;
    /** Lists all valid moves for a piece that can move diagonally.
    * This method is not overloaded because it is used both by the bishop and the queen.
    * @param y file (column) letter of the chosen square
    * @param x rank (row) number of the chosen square
    * @param board the chessboard
    * @param moves the vector into which the valid moves are stored
    */
    void bishopMoves(const char y, const int x, Board& board, vector< pair<char, int> >& moves) const;
    /** constructor
    * @param newColor the color of the new piece
    */
    Piece(char newColor): color(newColor){}
    virtual ~Piece(){}
    virtual bool getMoved() const { return true; }
    /** Sets the attribute 'moved' to true.
    * If the piece in question is a rook or a king, this means they cannot participate in castling.
    */
    virtual void setMoved() {}

};

#endif // PIECE_H_INCLUDED
