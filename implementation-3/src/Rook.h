/** @class Rook
* class Rook
*/
#ifndef ROOK_H_INCLUDED
#define ROOK_H_INCLUDED
#include<vector>
using namespace std;

class Rook : public Piece {
    bool moved;
public:
    /** Constructor
    * @param newColor the color of the new piece
    */
    Rook(char newColor);
    vector< pair<char, int> > validMoves(const char y, const int x, Board& board) const;
    virtual bool getMoved() const { return moved; }
    virtual void setMoved() { moved = true; }
    virtual char getType() const { return 'r'; }

};
#endif // ROOK_H_INCLUDED
