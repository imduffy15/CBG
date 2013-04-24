/// ConnectFour Game.
/// @author Darren Brogan

#include "ConnectFour.h"
#include <algorithm>

using namespace std;
using std::max;

ConnectFour::ConnectFour():Game(2,7,6) {
  state = 0;
  int amountOfPieceTypes = 1;
  int maxAmountOfPlayerPieces = 21;
  columnHeight.resize(columns);
  vector <string> player1PieceTypes(amountOfPieceTypes);
  vector <string> player2PieceTypes(amountOfPieceTypes);
  player1PieceTypes[0] = FYELLOW "○";
  player2PieceTypes[0] = FWHITE "○";

  players[0] = new Player(amountOfPieceTypes,
                          player1PieceTypes,maxAmountOfPlayerPieces);

  players[1] = new Player(amountOfPieceTypes,
                          player2PieceTypes,maxAmountOfPlayerPieces);


  string start = FBLUE "| ";
  string end = " " RESET;

  for(int y=0; y<rows; y++) {
    for(int x=0; x<columns; x++) {
      grid[y][x] = Square(1, start, end,amountOfPlayers,
                          Coordinate(x, y));
    }
  }
}

/// Function to print out the board.
void ConnectFour::drawScreen() const {
  clearScreen();
  cout << "Player " << (currentPlayer+1) << " it is your go\n";
  cout << "\n ";
  for(int i=1; i<=columns; i++) cout << "  " << i << " ";
  cout << "\n";
  cout << FBLUE " +---+---+---+---+---+---+---+" RESET << "\n";
  // Print out each square of the board
  for(int y=0; y<rows; y++) {
    std::cout << " ";
    for(int x=0; x<columns; x++) {
      // Print out what's inside each square
      cout << grid[y][x].getStart();
      cout << grid[y][x];
      cout << grid[y][x].getEnd();
    }
    cout << FBLUE "|" RESET << "\n";
    cout << FBLUE " +---+---+---+---+---+---+---+" RESET << "\n";
  }
  cout << "\n";
}

/// Overloaded function to to return weather or not the game
/// is over or not.
int ConnectFour::isOver() const {
  return state;
}

int ConnectFour::isOver(const Square& current) const {
  if(fourInRow(current)) return 1;
  else if (isDraw()) return 2;
  else return 0;
}

/// Function to get the max amount of pieces in a line around the piece added
/// Returns weather or not the number is greater than 3, signifying a win
bool ConnectFour::fourInRow(const Square& current) const {
  int numPiecesInRow = max(
    // Take away one on each becuase current square is tested twice
    max((checkNext(current,0,1) + checkNext(current,0,-1) -1),
        (checkNext(current,1,1) + checkNext(current,-1,-1)-1)),
    max((checkNext(current,1,0) + checkNext(current,-1,0) -1),
        (checkNext(current,1,-1) + checkNext(current,-1,1)-1))
  );
  return(numPiecesInRow > 3);
}

/// Recursive function to check the next square to see if it has a piece
/// to see if it's owned by current player.
int ConnectFour::checkNext(const Square& current,int yOffset,int xOffset) const {
  if(current.hasPieceOwnedBy(currentPlayer)) {
    if(isLegal(current, yOffset, xOffset)) {
      Coordinate currentPos = current.getPosition();
      // Get the next square in the row
      Square& next = grid[currentPos.y + yOffset][currentPos.x + xOffset];
      // Return 1 for the current piece plus check the next square for piece.
      return 1 + checkNext(next, yOffset, xOffset);
    }
    // If the next square isn't legal just return one for the current square
    return 1;
  }
  return 0;
}

/// Function to test if the next square is legal.
bool ConnectFour::isLegal(const Square& current,int yOffset,int xOffset) const {
  Coordinate currentPos = current.getPosition();
  // Ensure that the next square lies inside the bounds of the board
  return((currentPos.x + xOffset < columns)&&(currentPos.x + xOffset >= 0))
  &&(currentPos.y + yOffset < rows)&&(currentPos.y + yOffset >= 0)
  &&(yOffset != 0 || xOffset != 0);
}

/// Function to request a move from the user and read it in
/// Pass move to executeMove().
bool ConnectFour::getMove() {
  int x;
  bool validInput = false;
  do {
    cout << "\nType in the X coordinate of the column you would like to "
    << "add your piece to:\n";
    cin >> x;
    // Decrement x so the inputed value corresponds to how the board is
    // laid out.
    x = x - 1;
    // Check that input is a numeric value.
    if(cin.fail()) {
      cout << "\nYou entered a non numeric value, try again\n";
      cin.clear();
      cin.ignore(1000,'\n');
      continue;
    }
    // Ensure that the number inputted lies inside the bounds of the board.
    if(x >= columns || x < 0) {
      cout << "\nYour input fell out of the bounds of the board\n";
    } else if(columnHeight[x] >= rows) {
      cout << "\nDestination column is full\n";
    } else validInput = true;
    // Loop until correct number in inputed.
  } while(!validInput);
  // pass inputed value to executeMove() function.
  executeMove(x);
  // Change currentPlayer to the next player.
  currentPlayer = (currentPlayer + 1) % 2;
  return true;
}

/// Function to take a column and move a piece to the next
/// available slot in that column.
bool ConnectFour::executeMove(int x) {
  // Add one to the number of items in desired column.
  columnHeight[x]++;
  // Get the y destination of the piece to be added bytaking the
  // amount of pieces in the column from the total
  int y = rows - columnHeight[x];
  if(players[currentPlayer]->hasRoomForPiece()) {
    grid[y][x].addPiece(currentPlayer, players[currentPlayer]->addPiece());
  }
  // Update the state variable
  state = isOver(grid[y][x]);
  return true;
}

bool ConnectFour::isDraw() const {
  return(players[0]->getAmountOfPieces()+players[1]->getAmountOfPieces() == 42);
}
/// Deconstructor for Connect Four
ConnectFour::~ConnectFour() {
  columnHeight.clear();
}
