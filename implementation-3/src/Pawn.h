/** @class Pawn
* class Pawn
*/
#ifndef PAWN_H_INCLUDED
#define PAWN_H_INCLUDED
#include<vector>
using namespace std;
class Pawn : public Piece {
public:
    /** Constructor
    * @param newColor the color of the new piece
    */
    Pawn(char newColor);
     vector< pair<char, int> > validMoves(const char y, const int x, Board& board) const;
     virtual char getType() const { return 'p'; }
};


#endif // PAWN_H_INCLUDED
