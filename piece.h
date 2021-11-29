// Kunling Yang, 20912628
// Last Modified at 0230, 20211123
// Abstract Base Class of Piece

#ifndef __PIECE_H__
#define __PIECE_H__

#include <memory>
#include "posn.h"
#include <vector>

class Piece {
    // side = 1 means the bottom player, side == 2 means the top player
    int side;
    char type;
    bool moved;
  public:
    Piece( const int &side, const char &type );
    int getSide();
    char getType();
    bool &isMoved();
    // require: the input position must be on the board
    virtual bool isValidMove( Posn *original, Posn *end, 
      std::vector<std::vector<Piece *>> &board ) = 0;
    virtual ~Piece();
};


#endif
