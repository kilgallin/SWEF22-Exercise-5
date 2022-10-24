#pragma once
#include <iostream>
#include <string>

#include "position.hpp"
#include "motion.hpp"
#include "enums.hpp"

class Piece {
	PieceType type ; 
	Position pos ; 
	Color color;

	public: 
		Piece() ; 

		Piece(Color c, PieceType t, int x, int y) ; 

		Piece(Color c, PieceType t, Position p) ; 

		int get_score() const {
			int score ; 
			switch(type) {
				case Pawn: score = 10 ;break;
				case Knight: score = 30 ; break;
				case Bishop: score = 30 ; break;
				case Rook: score = 50 ; break;
				case Queen: score = 90 ; break;
				case King: score = 10000 ; break;
			}
			if (color == Black) {
				score = -score ; 
			}
			return score ; 
		}
		
		Position get_pos() const {
			return pos ; 
		}

		PieceType get_type() const {
			return type ; 
		}

		Color get_color() const {
			return color ; 
		}
		 
		void set_pos(const Position& p) {
			pos = p ; 
		}

		// Change this piece to become a Queen
		void promote() {
			type = Queen ; 
		}

		// Change this piece to be a Pawn 
		void unpromote() {
			type = Pawn ; 
		}

		/* Return a CHAR to represent the piece on the board */
		const char* piece_char()  const ;
		
		friend std::ostream& operator<<(std::ostream& os, const Piece& p) ; 

} ; 


