/** @class Board
* Board representation and gameplay implementation
*/
#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED

class Board {
    /** the representation of the chessboard */
    Piece * board[8][8];
    /** the square to which a pawn may move after performing en passant capture */
    pair<char, int> epSquare;
    static const int MOVE = 1, QUIT = 2, LOAD = 3, LOAD_AND_MOVE = 4, RESIGN = 5;
    /** the color of the player who is to move*/
    char color;
    bool isCheck, isMate;
    /** Releases the resources allocated by an instance of this class.
    * Used by the destructor and when loading a file.
    */
    void deleteBoard();
    /** Sets up a new chessboard in the initial configuration.
    */
    void newBoard();
    /** Creates a new chessboard. Used for loading from a file.
    * @param state Contains the ditribution of the pieces for the new chessboard
    */
    void makeBoard(const string state);
    /** Saves the current state of the chessboard into a file.
    * @param filename the name of the file where the game should be saved
    * @return true if the game was successfully saved, false otherwise
    */
    bool save (const char * filename) const;
    /** Loads a saved game from a file.
    * @param filename the name of the file from which the game should be loaded
    * @return the content of the file from which the game was loaded
    */
    string load (string filename);
    /** Saves the current state of the chessboard to a string. Used when saving to a file.
    * @return a string in which the current state of the chessboard is encoded
    */
    string saveToString (bool changeColor) const;
   /** Promotes a pawn after traversing the board.
    * @param toy file letter of the destination square
    * @param tox rank number of the destination square
    * @param promoteTo the piece to which the pawn should be promoted
    * @param playerColor the pawn's color
    */
     void promotion(const char toy, const int tox, char& promoteTo, const char playerColor);

public:
    bool getIsCheck() { return isCheck; }
    bool getIsMate() { return isMate; }
    string getColorName();
    char getColor() const { return color; }
    /**Displays information about check and ends the game when a stalemate or a checkmate occurs.
    * @param isLocal true if the game is being played locally, false otherwise
    * @return true if the game has ended, flase otherwise
    */
    bool endTurn(const bool isLocal);
    void setColor(char newColor){ color = newColor; }
    /**When playing over the network, sends data about the players actions to the opponent.
    * @param state the current state of the game
    * @param loadedGame When the player loads a game, this string describes the state of the chessboard.
    * @param fromy file letter of the origin square of the piece which the player moved
    * @param fromx rank number of the origin square of the piece which the player moved
    * @param toy file letter of the destination square of the piece which the player moved
    * @param tox rank number of the destination square of the piece which the player moved
    * @param promote When the player promotes a pawn, this variable contains the name of the promoted piece
    * @param currSock descriptor for the socket
    */
    int sendData(const int& state, const string& loadedGame, const char fromy, const int fromx, const char toy,
                  const int tox, char& promote, const int currSock) const;
    /** Enables access to the pieces on the board using standard chess notation.
    * @param y file (column) letter
    * @param x rank (row) number
    * @return pointer to the piece located on the chosen square
    */
    Piece * at (const char y, const int x) const;
    /**  Checks if the player is in check.
    * @param playerColor the color of the player
    * @return true if the player is in check, false if not
    */
    bool check(const char playerColor) const;
    /** Checks if the player has any valid moves left.
    * @param playerColor the color of the player
    * @return true if the player has any valid moves, false if not
    */
    bool mate(const char playerColor);
    Board();
    ~Board();
    /** Returns the color of the other player
    * @param playerColor the color of one of the players
    * @return 'b' for white, 'w' for black
    */
    char other (const char playerColor) const;
     /** Asks the user if they want to load a saved game and handles the loading.
    * @param state the current state of the game, used when playing over the network
    * @param loadedGame If the player wants to load a new game, this string will contain the state of the chessboard.
    * @param isLocal false if the game is being played over the network, true otherwise
    */
    void startGame(int& state, string& loadedGame, bool isLocal);
    /** When playing over the network, this method receives data describing the opponent's actions.
    * @param state the current state of the game
    * @param currSock descriptor for the socket
    * @param listenSock descriptor for the accepted socket (if running as server)
    * @param isLocal false if the game is being played over the network, true otherwise
    * @param isSrv true if running as server, false otherwise
    * @param loadedGame If the opponent loaded a game, this string will contain the current state of the chessboard
    * @return 2 if the connection failed, 1 if the game has ended, 0 otherwise
    */
    int receiveData(int& state, const int currSock, const int listenSock, const bool isLocal, const bool isSrv, string& loadedGame);
    /** Parses input from the player and performes the appropriate action (e.g. moves a piece or saves the game).
    * @param state the current state of the game
    * @param promote When the player promotes a pawn, this variable contains the name of the promoted piece
    * @param loadedGame If the player loaded a game and moved immediately afterwards, this string will contain the state of the chessboard.
    * @param fromy file letter of the origin square of the piece which the player moved
    * @param fromx rank number of the origin square of the piece which the player moved
    * @param toy file letter of the destination square of the piece which the player moved
    * @param tox rank number of the destination square of the piece which the player moved
    * @param firstTime true if the player is making their first move, false otherwise
    * @return 0 on success, 1 on failure
    */
    int getInput(int& state, char& promote, string& loadedGame,
                  char& fromy, int& fromx, char& toy, int& tox, const bool firstTime);
    /** Moves a piece on the board.
    * @param fromy file letter of the origin square of the piece which the player moved
    * @param fromx rank number of the origin square of the piece which the player moved
    * @param toy file letter of the destination square of the piece which the player moved
    * @param tox rank number of the destination square of the piece which the player moved
    * @param playerColor color of the player who moved the piece
    * @return true if the move was valid and successfully performed, false otherwise
    */
    bool movePiece (const char fromy, const int fromx, const char toy, const int tox, const char playerColor);
    /** Returns the graphical representation of the chessboard
    * @return a string containing a representation of the board
    */
    string print() const;
};


#endif // BOARD_H_INCLUDED
