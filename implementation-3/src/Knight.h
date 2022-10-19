/** @class Knight
* class Knight
*/
#ifndef KNIGHT_H_INCLUDED
#define KNIGHT_H_INCLUDED
#include<vector>
using namespace std;
class Knight : public Piece {
public:
    /** Constructor
    * @param newColor the color of the new piece
    */
    Knight(char newColor);
    vector< pair<char, int> > validMoves(const char y, const int x, Board& board) const;
    virtual char getType() const { return 'n'; }
};


#endif // KNIGHT_H_INCLUDED
