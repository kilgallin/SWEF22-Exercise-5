/** @class Bishop
* class Bishop
*/
#ifndef BISHOP_H_INCLUDED
#define BISHOP_H_INCLUDED
#include<vector>
using namespace std;
class Bishop : public Piece {
public:
    /** Constructor
    * @param newColor the color of the new piece
    */
    Bishop(char newColor);
    virtual vector< pair<char, int> > validMoves(const char y, const int x, Board& board) const;
    virtual char getType() const { return 'b'; }
};


#endif // BISHOP_H_INCLUDED
