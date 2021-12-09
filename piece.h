// Kunling Yang, 20912628
// Last Modified at 0230, 20211123
// Abstract Base Class of Piece

#ifndef __PIECE_H__
#define __PIECE_H__

#include <memory>
#include "posn.h"
#include <vector>
#include "moveHistory.h"

class Piece {
    // side = 1 means the bottom player, side == 2 means the top player
    int side;
    char type;
    bool moved;
  public:
    Piece( const int &side, const char &type );
    int getSide();
    char getType();
    // return the reference of moved and then after the first move
    // the game will be able to change the moved status
    bool &isMoved();
    // return the reference of justMoved and then after the move
    // the game will be able to change the moved status
    bool &isJustMoved();
    // require: the input position must be on the board
    virtual bool isValidMove( Posn *original, Posn *end, 
      std::vector<std::vector<Piece *>> &board, MoveHistory *mh ) = 0;
    virtual ~Piece();
};


#endif
