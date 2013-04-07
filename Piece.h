#ifndef PIECE_H
#define PIECE_H

#include <string>
#include "Coordinate.h"

class Player;
class Piece {
    friend std::ostream& operator<<(std::ostream& out, const Piece& piece);
    
    protected:
      int           type;
      Coordinate    position;
      
    public:
      Piece();
      Piece(Player* owner);
      Piece(Player* owner, Coordinate position);
      Coordinate getCoordinate();
      int getType();
      bool setType(int type);
      Player* owner;
};

#endif
