#pragma once
#include <memory>
#include <iostream>
#include <vector>
#include <deque>
#include <sstream>
#include <tuple>

#include "piece.hpp"
#include "motion.hpp"
#include "gui.h"

#define H 8
#define VERBOSE 1

class Chessgame {

	private: 
        Piece* pieces[64] =  {}; 

		bool white_to_play = true ; 
		bool finished = false ; 

		bool white_has_castled = false ; 
		bool black_has_castled = false ; 

		std::vector<std::tuple<Motion, Piece*>> moves_buffer ; 

		// not sure if I need these guys...
		std::array<std::array<int,8>, 8>  white_attacked_pieces {}; 
		std::array<std::array<int,8>, 8>  black_attacked_pieces {}; 

		// TODO: remove these two properties, not going to be helpful
		// std::array<std::vector<Motion>, 4> valid_moves_white ; 
		// std::array<std::vector<Motion>, 4> valid_moves_black ; 

		void create_normal_chessboard() ; 
		void create_test1() ; 
		void create_test_promotion() ; 
		void create_test_castling() ; 

	public: 


		Chessgame() ; 

        ~Chessgame() {
            if (VERBOSE)
                std::cout << "Deleting board" << std::endl ; 

            for (int i=0; i<64; i++) {
                delete pieces[i] ; 
            }
        }

		void print_board() const ;

		void print_all_pieces() const ;

		void print_number_pieces() const ;

		void print_moves_buffer() const ; 

		// When this function is called, the stack of previous moves
		// will be recursively visited and reverted
		void reset_moves() ; 

		// reset the last move only
		void resest_one_move() ; 

		// This function verifies that a move is valid 
		// NOTE: This function does not make the move happen, use the move_piece function afterward if desired
		// If you are exploring several moves, It requires that the previous move have been set.
		// INPUTS
		// - check_intrinsinc: if 'true', some properties of the motion are verified. 
		// 		but it is not always necessary, for instance the solver will only ask for valid moves
		// - compute_move_quality: if 'true', motions qualities are computed
		bool check_move(Motion& m, bool check_intrinsinc = false) const ; 
		
		// returns all the valid motions for the current position
		// The moves are sorted by importance
		void get_all_valid_moves(std::array<std::vector<Motion>, 4>& moves) const ; 

		void get_all_valid_moves_other_player(std::vector<Motion>& moves) const ; 

		void update_attacked_pieces() ; 

		// debugging function
		void print_attacked_pieces() const ; 

		// returns the set of pieces to move
		std::vector<Piece*> pieces_to_move(const Color& c) const ; 

		// This function makes a move happen, without making any verification 
		// WARNING: the move HAS TO BE verified first.
		// If keep track is true, a stack of pieces is kept in memory to remember which were the pieces captured 
		// in the previous moves.
		void move_piece(Motion& m, bool keep_track = false) ; 

		void fill_attacked_pieces() ; 



		// FUNCTIONS to play a game

		// This function checks that a move is valid, and if so, makes it happen.
		bool check_and_move(Motion& m) ; 

		bool prompt_for_a_move() ; 

		void prompt_for_moves() ; 

		// Functions to access the pieces on board

        Piece*& get_piece_ref(int i, int j) {
            return pieces[i + j * 8] ; 
        }

        Piece* get_piece(int i, int j) const {
            return pieces[i + j * 8] ; 
        }

        Piece*& get_piece_ref(const Position& p) {
            return pieces[p.x + p.y * 8] ; 
        }

        Piece* const& get_piece_ref(const Position& p) const {
            return pieces[p.x + p.y * 8] ; 
        }

        Piece* get_piece(const Position& p) const {
            return pieces[p.x + p.y * 8] ; 
        }

		Piece*& get_piece_ref(int i) {
			return pieces[i] ; 
		}

		// Functions to access some information about the game

		Color get_current_color() const {
			if (white_to_play)
				return White; 
			else
				return Black;
		}

		bool is_white_to_play() const {
			return white_to_play ; 
		}

		bool is_finished() const {
			return finished ; 
		}

		// Same functions but using position object
		
		// returns the score for white player
		int score() const ; 

		void add_new_piece(Color c, PieceType t, int i, int j) {
			this->add_new_piece(c, t, Position(i,j))  ;
		}

		void add_new_piece(Color c, PieceType t, Position pos) {
            Piece*& p = get_piece_ref(pos) ; 
            auto pawn = new Piece(c, t, pos.x, pos.y) ; 
            p = pawn ; 
		} ; 

};
