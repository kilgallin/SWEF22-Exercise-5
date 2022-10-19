/** @class King
* class King
*/
#ifndef KING_H_INCLUDED
#define KING_H_INCLUDED
#include<vector>
using namespace std;

class King : public Piece {
    bool moved;
public:
    /** Constructor
    * @param newColor the color of the new piece
    */
    King(char newColor);
    vector< pair<char, int> > validMoves(const char y, const int x, Board& board) const;
    virtual bool getMoved() const { return moved; }
    virtual void setMoved() { moved = true; }
    virtual char getType() const { return 'k'; }
};


#endif // KING_H_INCLUDED
