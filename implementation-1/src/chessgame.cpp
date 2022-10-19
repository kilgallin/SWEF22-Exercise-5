#include "chessgame.hpp"

#define VERBOSE_GAME 0

using namespace std ; 

void Chessgame::print_board() const {
	cout << endl ; 
	std::cout << "     a   b   c   d   e   f   g   h   " << std::endl ; 
	for (int i=7; i>=0; i--) {
		std::cout << "   +---+---+---+---+---+---+---+---+" << std::endl ; 
		std::cout << i+1 << "  | " ; 
		for (int j=0; j<H; j++) {
			// get a pointer to the piece
			auto p = get_piece(j, i) ; 
			if (p != nullptr) {
				// If we're here it means that there is a piece ! 
				std::cout << p->piece_char(); 
			} else {
				std::cout << " " ; 
			}
			std::cout << " | " ; 
		}
		std::cout << std::endl ; 
	}
	std::cout << "   +---+---+---+---+---+---+---+---+" << std::endl ; 
	std::cout << "     a   b   c   d   e   f   g   h   " << std::endl ; 
	cout << "score = " << score() << endl ; 
	cout << endl ; 
}

void Chessgame::print_all_pieces() const {
	cout << endl << "All pieces on bard" << endl ;
	cout << "---------------------" << endl ; 
	for (int i=7; i>=0; i--) {
		for (int j=0; j<H; j++) {
			auto p = get_piece(j, i) ; 
			if (p != nullptr) {
				// If we're here it means that there is a piece ! 
				bool is_white = p->get_color() == White ; 
				string color = is_white ? "white" : "black" ;  
				cout << " - " << *p << " of " << color << endl ;				
			}
		}
	}
}

int Chessgame::score() const {
	int score = 0 ; 
	// 1. Score of the pieces
	for (const auto& p: pieces) {
		if (p != nullptr) {
			// 1. Absolute value of the piece
			score += p->get_score() ; 
		}
	}
	// 2. Score of the attacked squares
	int score_update_white = 0 ; 
	for (const auto& row: white_attacked_pieces) {
		for (const auto& p: row) {
			score +=  p ; 
		}
	}
	for (const auto& row: black_attacked_pieces) {
		for (const auto& p: row) {
			score -=  p ; 
		}
	}
	return score ; 
}

// returns the pieces that can be moved for next turn
// Given the desired color 
std::vector<Piece*> Chessgame::pieces_to_move(const Color& c) const {
	vector<Piece*> to_move ; 
	to_move.reserve(32) ; 
	for (const auto& p: pieces) {
		if (p != nullptr && p->get_color() == c) {
			to_move.push_back(p) ; 
		}
	}
	return to_move ; 
} 


// the arrays to fill is passed by reference 
void Chessgame::get_all_valid_moves(std::array<std::vector<Motion>, 4>& moves) const { 
	for (const auto& p: pieces) {
		// make sure to have only the pieces of the right color
		if (p == nullptr || p->get_color() != get_current_color()) continue ; 

		// Go through all possible displacement and check validity
		for (const auto& displacement: Motion::get_range_of_attack(p->get_type(), p->get_color())) {
			Motion m = Motion(p->get_pos(), displacement) ; 

			// 1. Check if the move is valid.
			bool is_valid = this->check_move(m) ; 

			// 2. If the move is valid, appends it to the correct container
			if (is_valid) {
				moves[m.quality].push_back(m) ; 
			} 

		}	
	}
} 

void Chessgame::get_all_valid_moves_other_player(std::vector<Motion>& moves) const {
	for (const auto& p: pieces) {
		// make sure to have only the pieces of the right color
		if (p == nullptr || p->get_color() == get_current_color()) continue ; 

		// Go through all possible displacement and check validity
		for (const auto& displacement: Motion::get_range_of_attack(p->get_type(), p->get_color())) {
			Motion m = Motion(p->get_pos(), displacement) ; 

			// 1. Check if the move is valid.
			bool is_valid = this->check_move(m) ; 

			// 2. If the move is valid, appends it to the correct container
			if (is_valid) {
				moves.push_back(m) ; 
			} 

		}	
	}

}

void Chessgame::reset_moves() {
	while (moves_buffer.size() > 0) {
		this->resest_one_move() ; 
	} 
}

bool Chessgame::check_move(Motion& m, bool check_intrinsinc) const {
	// 1. to start with, check that it's in the board
	bool is_valid = m.is_in_board() ; 

	if (!is_valid) {
		return false ; 
	}

	// get the references to the piece pointers
	Piece* const& p1 = get_piece_ref(m.from_pos) ; 
	Piece* const& p2 = get_piece_ref(m.to_pos) ; 

	if (p1 != nullptr) {
		PieceType p = p1->get_type() ; 
		Color c = p1->get_color() ; 

		// a. check the color order is correct 
		// (do not check for the solver...)
		if (check_intrinsinc && ((c == White && !white_to_play) || (c == Black && white_to_play) )){
			// then this move can't be accepted.
			is_valid = false ; 
			cout << "ERROR: Wrong color was asked to move !" << endl ; 
		}

		// b. Is the move part of the range ? 
		// (do not check for the solver...)
		if (check_intrinsinc && is_valid) {
			is_valid = m.intrinsic_validity(p, c); 
		}

		// When the solver works, it start from here

		// c. if there is a piece at the last cell, check that it's from the other color 
		// ==> CAPTURE
		if (is_valid && p2 != nullptr) {
			if (p2->get_color() == c) {
				if (VERBOSE_GAME) {
					cout << "ERROR: you can't go where a piece of your team is already !" << endl ; 
				}
				is_valid = false ; 
			} else {
				// It's a capture...
				// 1. good capture: small piece capture big one
				// 2. exchange capture: same value exchanged 
				// 3. capture: big value captures smaller one
				// 4. no capture at all	
				int s1 = abs(p1->get_score()); 
				int s2 = abs(p2->get_score()) ; 
				if (s1 < s2) {
					m.quality = GoodCapture ; 
				} else if (s1 == s2) {
					m.quality = Exchange ; 
				} else {
					m.quality = Capture ; 
				}
			}
		} 

		// d. (only for pawn move): 
		// - if it's a diagonal move, is there a piece to capture ?
		// - if it's not a diagonal move, is there no piece to capture ? 
		// - make sure that jumps are valid only for the first move of the game
		if (is_valid && p == Pawn) {
			if (m.dx != 0) {
				// ==> it's a pawn capture move
				// check that there is a piece of another color at the destination
				if (! (p2 != nullptr && p2->get_color() != c)) {
					// not valid diagonal move ! 
					is_valid = false ; 
				}
			} else {
				// not diagonal move, it must NOT be a capture
				if (p2 != nullptr) {
					// It means there is a piece there
					// not valid vald move
					is_valid = false ; 
				}
			}
			if (m.dy == 2 && c == White && m.from_pos.y != 1 ) {
				// White pawn jump not valid
				is_valid = false ; 
			} else if (m.dy == -2 && c == Black && m.from_pos.y != 6) {
				// White pawn jump not valid
				is_valid = false ; 
			}
		}

		// e. (everything unless knight) check that there is no piece in the path of visited cells, except the last cell
		// Go through all the cases with this orientation increment and check that there is no piece in between 
		if (is_valid && p != Knight) {
			// get the orientation 
			Position orientation = m.get_orientation();
			Position tmp = m.from_pos ; // it's a copy
			// we know that we will exit the while loop because the implementation is valdiated
			while (true) {
				tmp += orientation ; 
				if (tmp == m.to_pos) {
					break ; 
				} else {
					// Check that there is no piece 
					if ( get_piece_ref(tmp) != nullptr) {
						// It means that there is a problem ... a piece is in between ! 
						is_valid = false ; 
						break ; 
					}
				}
			}
		}

		// f. castling 
		if (is_valid && p == King && (m.dx == 2 || m.dx == -2) ) {
			// it's a castling move... 
			// a. check that the player did not castled already
			if ((c == White && white_has_castled) || (c == Black && black_has_castled)) {
				// then, refuse the move
				is_valid = false ; 
			} else if (m.from_pos.x != 4) {
				// the king has to be in proper position
				is_valid = false ; 
			} else {
				// compute the increment to find the rook 
				Position rook_before = {m.dx == 2 ? 7 : 0, m.to_pos.y} ; 

				// check that there is a rook that can be moved...
				Piece* const& p3 = get_piece_ref(rook_before) ; 
				if (p3 == nullptr || (p3->get_type() != Rook)) {
					//cout << "Not a rook, the castling is not valid" << endl ; 
					is_valid = false ; 
				} else {

					// Finally, check that none of the attacked pieces is attacked.
					const auto& attacked_pieces = white_to_play ? black_attacked_pieces : white_attacked_pieces ; 
					if (attacked_pieces[m.dx == 2 ? 6 : 2][m.to_pos.y] > 0) {
						is_valid = false ; 
					} else if (attacked_pieces[m.dx == 2 ? 5 : 3][m.to_pos.y] > 0) {
						is_valid = false ; 
					} else if (attacked_pieces[4][m.to_pos.y] > 0) {
						is_valid = false ; 
					} else {
						// If you arrive here, it means that the castling is valid !
						m.castling = true ; 
					}
						
				}
			}
		}

	} else {
		return false ; 
	}

	return is_valid ; 
} 

// This is the function in charge to move a piece. 
// Be careful, the function does not check if the move is valid.
void Chessgame::move_piece(Motion& m, bool keep_track) {
	Piece* &p1 = get_piece_ref(m.from_pos) ; 
	Piece* &p2 = get_piece_ref(m.to_pos) ; 

	if (p2 != nullptr && p2->get_type() == King) {
		// It means the game is finished...
		this->finished = true ; 
	}

	// Pawn promotion here 
	if (p1->get_type() == Pawn && (m.to_pos.y == 7 || m.to_pos.y == 0)) {
		// it's a promotion
		p1->promote() ; 
		m.promotion = true ; 
	}

	// Castling effect 
	if (m.castling) {
		// Moving the rook
		// compute the increment to find the rook 
		Position rook_before = {m.dx == 2 ? 7 : 0, m.to_pos.y} ; 
		Position rook_after = {m.dx == 2 ? 5 : 3, m.to_pos.y} ; 
		Piece*& rook1 = get_piece_ref(rook_before) ; 
		Piece*& rook2 = get_piece_ref(rook_after) ; 

		// from logic, we know that there wont be any pieces where the rook is moves
		// hence, we can just make the switch
		rook1->set_pos(rook_after) ; 
		rook2 = &*rook1 ; 
		rook1 = nullptr ;  

		if (white_to_play) {
			//cout << "WHITE CASTLES" << endl ; 
			white_has_castled = true ; 
			
		} else {
			//cout << "BLACK CASTLES" << endl ; 
			black_has_castled = true ; 
		}
	}

	if (keep_track) {
		// If keep track is on, it means we can step back
		this->moves_buffer.push_back({m, p2}) ; 
	}

	white_to_play = !white_to_play ; 
	p1->set_pos(m.to_pos) ; 
	p2 = &*p1 ; 
	p1 = nullptr ; 

	// Now that the move is done, we can update the array of attacked pieces
	// We have to update the arrays for both the players
	update_attacked_pieces(); 
} 

// when this function is called, the valid moves array for both players are going to be filled
void Chessgame::update_attacked_pieces() {
	// First thing: empty the arrays
	white_attacked_pieces = {} ; 
	black_attacked_pieces = {} ; 

	// Then, go through all the pieces and find the permitted motions
	for (const auto& p: pieces) {
		if (p == nullptr) continue ; 

		// Go through all possible displacement and check validity
		for (const auto& displacement: Motion::get_range_of_attack(p->get_type(), p->get_color())) {
			Motion m = Motion(p->get_pos(), displacement) ; 

			// 1. Check if the move is valid.
			bool is_valid = this->check_move(m) ; 

			// 2. If the move is valid, appends it to the correct container
			if (is_valid) {
				if (p->get_color() == White) {
					white_attacked_pieces[m.to_pos.x][m.to_pos.y] += 1 ; 
				} else {
					black_attacked_pieces[m.to_pos.x][m.to_pos.y] += 1 ; 
				}
			} 

		}	
	}
}

void Chessgame::print_attacked_pieces() const {
	cout << "[DEBUG]: printing valid moves" << endl ; 
	cout << "white: " << endl ; 
	for (const auto& vec: white_attacked_pieces) {
		for (const auto& m: vec) {
			cout << m << " , " ; 
		}
		cout << endl ; 
	}
	cout << "black: " << endl ; 
	for (const auto& vec: black_attacked_pieces) {
		for (const auto& m: vec) {
			cout << m << " , " ; 
		}
		cout << endl ; 
	}
	cout << endl ; 
}




void Chessgame::resest_one_move() {
	const auto& last_move = moves_buffer.at(moves_buffer.size()-1) ; 
	Motion m = std::get<0>(last_move) ; 
	Piece* lost_piece = std::get<1>(last_move) ; 
	// p1 = pointer to the Piece in the array, before the move
	// p1 has to be the nullptr ; Else, it means that there is something wrong
	Piece* &p1 = get_piece_ref(m.from_pos) ; 
	// p2 = pointer to the Piece after the move
	// p2 can't be the nullptr, and it needs to be placed back into p2
	Piece* &p2 = get_piece_ref(m.to_pos) ; 
	p2->set_pos(m.from_pos) ; 
	p1 = &*p2 ; 
	
	// but to reverse p2, we have to be careful ! It's not necesraly the nullptr 
	if (lost_piece != nullptr) {
		// we just have to put it back like so 
		p2 = &*lost_piece ; 
		if (p2->get_type() == King) {
			this->finished = false ; 
		}
		m.quality = NoCapture ; 
	} else {
		p2 = nullptr ; 
	}

	// Reverse the pawn promotion too
	if (m.promotion) {
		// it's a promotion
		p1->unpromote() ; 
		m.quality = NoCapture ; 
	}

	// Castling reverse
	if (m.castling) {
		// Reverse the rook 
		// we need to get the position of the rook before and after the moves
		Position rook_before = {m.dx == 2 ? 7 : 0, m.to_pos.y} ; 
		Position rook_after = {m.dx == 2 ? 5 : 3, m.to_pos.y} ; 
		Piece*& rook1 = get_piece_ref(rook_before) ; 
		Piece*& rook2 = get_piece_ref(rook_after) ; 
		rook2->set_pos(rook_before) ; 
		rook1 = &*rook2 ; 
		rook2 = nullptr ; 


		if (!white_to_play) {
			//cout << "White has not castled anymore" << endl ; 
			white_has_castled = false ; 
		} else {
			//cout << "black has not castled anymore" << endl ; 
			black_has_castled = false ; 
		}
		update_attacked_pieces
(); 
	}

	// TODO: return to the previous array of attacked pieces

	// last thing is to swap the turn
	white_to_play = !white_to_play ; 

	// and we can remove this from here
	moves_buffer.pop_back() ; 
}

/* Function to be called after a player has moved. It will look at all the possible moves this player can do 
 * (this player is now the 'other player' because the flag white_to_play has been switched) and update the array of pieces
 * it is attacking.
 */
void Chessgame::fill_attacked_pieces() {
	// Create the container for valid moves
	// (each container contains the move of certain quality)
	std::vector<Motion> moves ; 
	moves.reserve(500) ; 
	
	// get all the valid moves (this include the valid castling moves)
	get_all_valid_moves_other_player(moves) ; 

	// find the attacked pieces by the player at this turn.
	// If it's black to play, this show all of the cases that black is attacking
	// During next turn of the evaluation, the function 'check_move' will be using this array
	// NOTE: the array have to be initialised
	// TODO: reset the array
	auto& array_to_fill = white_to_play ? black_attacked_pieces : white_attacked_pieces ; 
	for (const auto& valid_move: moves) {
		array_to_fill[valid_move.to_pos.x][valid_move.to_pos.y] += 1 ; 
	}
}

// This is the function called by human player to move pieces
// Additional rule checks are done (second argument of the check_move function) 
// Also, the arrays of attacked pieces are changed
bool Chessgame::check_and_move(Motion& m) {
	bool is_valid = check_move(m, true) ; 
	cout << is_valid << endl ; 
	if (is_valid) {
		move_piece(m, true) ; 

		// Now that the move is done, we fill the attacked pieces
		//fill_attacked_pieces();

	}
	return is_valid ; 
} 

// user is asked to play a single move
bool Chessgame::prompt_for_a_move() {
	std::string move ; 
	bool success ; 
	cout << "What move ? " << endl ; 
	cin >> move ; 

	// let's parse the move 
	// 1. split by the ,
	vector<string> strings;
	istringstream f(move);
	string s;    
	while (getline(f, s, ',')) {
		strings.push_back(s);
	}

	if (strings.size() == 1 && strings.at(0) == "r") {
		reset_moves() ; 
		print_all_pieces() ; 
		return true ; 
	}

	if (strings.size() != 2) {
		cout << "ERROR: wrong number of pieces" << endl ; 
	} 

	try {
		auto p1 = Position(strings.at(0)) ; 
		auto p2 = Position(strings.at(1)) ; 

		// now we can try to make the move
		Motion m = Motion(p1, p2) ; 
		success = check_and_move(m) ; 

		if (success) {
			print_board() ; 
			print_number_pieces() ; 
		}

		return success ; 
	}
	catch (const std::exception& ex)
	{
		cout << "ERROR: position could not be parsed" << endl ; 
		std::cout << ex.what();
		return false ; 
	}

}

// HELPER FUNCTIONS; NOT REALLY INTERESTING

void Chessgame::create_normal_chessboard() {
	// add the pawns
	for (int i=0; i<8; i++) {
		add_new_piece(White, Pawn, i, 1) ; 
		add_new_piece(Black, Pawn, i, 6) ; 
	}

	// add White the pieces
	add_new_piece(White, Rook, 0, 0) ; 
	add_new_piece(White, Rook, 7, 0) ; 
	add_new_piece(White, Knight, 1, 0) ; 
	add_new_piece(White, Knight, 6, 0) ; 
	add_new_piece(White, Bishop, 2, 0) ; 
	add_new_piece(White, Bishop, 5, 0) ; 
	add_new_piece(White, King, 4, 0) ; 
	add_new_piece(White, Queen, 3, 0) ; 

	// add Black the pieces
	add_new_piece(Black, Rook, 0, 7) ; 
	add_new_piece(Black, Rook, 7, 7) ; 
	add_new_piece(Black, Knight, 1, 7) ; 
	add_new_piece(Black, Knight, 6, 7) ; 
	add_new_piece(Black, Bishop, 2, 7) ; 
	add_new_piece(Black, Bishop, 5, 7) ; 
	add_new_piece(Black, King, 4, 7) ; 
	add_new_piece(Black, Queen, 3, 7) ; 
}

void Chessgame::create_test1() {
	add_new_piece(White, Pawn, 0, 1) ; 
	add_new_piece(White, Pawn, 5, 1) ; 
	add_new_piece(Black, Pawn, 1, 6) ; 
	add_new_piece(Black, Pawn, 5, 6) ; 
}

// Test the promotion 
// The rook has to capture the pawn and not the Knight
void Chessgame::create_test_promotion() {
	add_new_piece(White, Pawn, 1, 5) ; 
	add_new_piece(White, Rook, 6, 2) ; 
	add_new_piece(White, King, 0, 0) ; 

	add_new_piece(Black, Rook, 6, 2) ; 
	add_new_piece(Black, Pawn, 5, 3) ; 
	add_new_piece(Black, King, 7, 5) ; 
}

void Chessgame::create_test_castling() {
	add_new_piece(White, Pawn, 1, 5) ; 
	add_new_piece(White, Rook, 0, 0) ; 
	add_new_piece(White, Rook, 7, 0) ; 
	add_new_piece(White, King, 4, 0) ; 
	add_new_piece(White, Pawn, 5, 1) ; 

	add_new_piece(Black, Rook, 6, 2) ; 
	add_new_piece(Black, Pawn, 5, 3) ; 
	add_new_piece(Black, King, 7, 5) ; 
}

Chessgame::Chessgame() {
	std::cout << "Creating Board" << std::endl ; 
	this->moves_buffer.reserve(10) ; 

	for (int i=0; i<64; i++) {
		pieces[i] = nullptr ; 
	}

	create_normal_chessboard() ; 
	//create_test_promotion() ; 
	// create_test_castling() ; 


	// TODO: initialise the array of attacked pieces

	update_attacked_pieces(); 
} 

void Chessgame::print_number_pieces() const {
	int tmp = 0 ; 
	for (const auto& p: pieces) {
		if (p != nullptr) {
			tmp ++ ; 
		}
	}
	cout << " Number of pieces: " << tmp << ", current score = " << score() << endl ; 
}

void Chessgame::print_moves_buffer() const {
	cout << "\nList of previous motion" << endl ; 
	for (const auto& tmp: moves_buffer) {
		cout << " - Move:  " << std::get<0>(tmp) ; 
		if ((std::get<1>(tmp) == nullptr)) {
			cout << " (no capture)" << endl ; 
		} else {
			cout << " 	capture of : " << std::get<1>(tmp)->get_type() << endl ;
		}
	}
}

// User is asked to play all the moves
void Chessgame::prompt_for_moves() {
	while (true) {
		prompt_for_a_move() ; 
		print_board();
	}
}

