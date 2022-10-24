/** @class Queen
* class Queen
*/
#ifndef QUEEN_H_INCLUDED
#define QUEEN_H_INCLUDED
#include<vector>
using namespace std;
class Queen : public Piece {
public:
    /** Constructor
    * @param newColor the color of the new piece
    */
    Queen (char newColor);
    vector< pair<char, int> > validMoves(const char y, const int x, Board& board) const;
    virtual char getType() const { return 'q'; }
};


#endif // QUEEN_H_INCLUDED
