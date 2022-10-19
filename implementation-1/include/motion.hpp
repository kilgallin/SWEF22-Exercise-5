#pragma once

#include "piece.hpp"
#include "enums.hpp"
#include <vector>

#define PRINT_MOTION std::cout << "[MOTION.hpp]: "

// Move quality: lower is better
enum MotionQuality{
	GoodCapture = 0,
	Capture = 1, 
	Exchange = 2, 
	NoCapture = 3,
}; 


/* A Motion is one evolution of the game: one piece which moves.
 * This class contains several helper function designed to constraint the motion to only legal ones.
 * They are several kinds of motion: move, capture, promote
 *
 *
 * For each kind of piece, we define
 * - the range of attack: all the attacked squared by the piece
 * - pawn are the only piece that have a ability to move where they don't attack...
 */

class Motion 
{
	private: 
		// Returns true if a target vector is found inside a list of vectors
		static bool find_in_vec(std::vector<std::vector<int>> vec, std::vector<int> to_find) {
			for (auto& v: vec) {
				if (v == to_find) {
					return true ; 
				}
			}
			return false ; 
		}

	public:
		// dx & dy can be used to analyse the kind of motion being used
		int dx = 0; 
		int dy = 0; 

		// bool valid_move = false ; 
		bool promotion = false ; 
		bool castling = false ; 
		MotionQuality quality = NoCapture ; 

		Position from_pos ; 
		Position to_pos ; 

		// Ranges of attack are defined as static members of the class Motion
		// (In C++, it's the onyl way to do it)

		static const std::vector<std::vector<int>>& pawn_range() {
			static const std::vector<std::vector<int>> tmp = {{1,1}, {-1,1}, {0,1}, {0,2}} ; 
			return tmp ; 
		} 

		static const std::vector<std::vector<int>>& pawn_range_black() {
			static const std::vector<std::vector<int>> tmp = {{1,-1}, {-1,-1}, {0,-1}, {0,-2}} ; 
			return tmp ; 
		} 

		static const std::vector<std::vector<int>>& king_moves() {
			static const std::vector<std::vector<int>> tmp = {
				{0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1},
				// castling
				{2,0}, {-2, 0}
			} ; 
			return tmp ; 
		} 

		static const std::vector<std::vector<int>>& knight_moves() {
			static const std::vector<std::vector<int>> tmp = {
				{1,2}, {-1,2}, {1,-2}, {-1,-2}, 
				{2,1}, {-2,1}, {2,-1}, {-2,-1}, 
			} ; 
			return tmp ; 
		} 

		static const std::vector<std::vector<int>>& rook_ranges() {
			static const std::vector<std::vector<int>> tmp = {
				{1, 0}, {2,0}, {3,0}, {4,0}, {5,0}, {6,0}, {7, 0}, 
				{0,1}, {0,2}, {0,3}, {0,4}, {0,5}, {0,6}, {0,7},
				{-1, 0}, {-2,0}, {-3,0}, {-4,0}, {-5,0}, {-6,0}, {-7, 0}, 
				{0,-1}, {0,-2}, {0,-3}, {0,-4}, {0,-5}, {0,-6}, {0,-7},
			} ; 
			return tmp ; 
		} 

		static const std::vector<std::vector<int>>& bishop_moves() {
			static const std::vector<std::vector<int>> tmp = {
				{1, 1}, {2,2}, {3,3}, {4,4}, {5,5}, {6, 6}, {7, 7}, 
				{1, -1}, {2,-2}, {3,-3}, {4,-4}, {5,-5}, {6, -6}, {7, -7}, 
				{-1, 1}, {-2,2}, {-3,3}, {-4,4}, {-5,5}, {-6, 6}, {-7, 7}, 
				{-1, -1}, {-2,-2}, {-3,-3}, {-4,-4}, {-5,-5}, {-6,- 6}, {-7, -7}
			} ; 
			return tmp ; 
		} 

		static const std::vector<std::vector<int>>& queen_moves() {
			static const std::vector<std::vector<int>> tmp = {
				{1, 1}, {2, 2}, {3,3}, {4,4}, {5,5}, {6, 6}, {7, 7}, 
				{1, -1}, {2,-2}, {3,-3}, {4,-4}, {5,-5}, {6, -6}, {7, -7}, 
				{-1, 1}, {-2,2}, {-3,3}, {-4,4}, {-5,5}, {-6, 6}, {-7, 7}, 
				{-1, -1}, {-2,-2}, {-3,-3}, {-4,-4}, {-5,-5}, {-6,- 6}, {-7, -7},
				{1, 0}, {2,0}, {3,0}, {4,0}, {5,0}, {6,0}, {7, 0}, 
				{0,1}, {0,2}, {0,3}, {0,4}, {0,5}, {0,6}, {0,7},
				{-1, 0}, {-2,0}, {-3,0}, {-4,0}, {-5,0}, {-6,0}, {-7, 0}, 
				{0,-1}, {0,-2}, {0,-3}, {0,-4}, {0,-5}, {0,-6}, {0,-7},
			} ; 
			return tmp ; 
		} 

		// A static function to get the correct range of motion
		static const std::vector<std::vector<int>>& get_range_of_attack(PieceType type, Color c) {
			switch (type) {
				case Pawn: 
					if (c == White) {
						return Motion::pawn_range() ; 
					} else {
						return Motion::pawn_range_black() ; 
					}
					break ; 
				case Bishop: return Motion::bishop_moves() ; break ;
				case Rook: return Motion::rook_ranges() ; break ; 
				case Knight: return Motion::knight_moves() ; break ; 
				case Queen: return Motion::queen_moves() ; break ; 
				case King: return Motion::king_moves() ; break ; 
				default: 
					PRINT_MOTION << "ERROR: asked for a piece, no range of attack defined" << std::endl ; 
					return Motion::pawn_range() ; 
			}
		}

		// Constructors

		Motion() {}

		Motion(Position from_pos, Position to_pos) {
			// Sanity check for impossible motions
			dx = to_pos.x - from_pos.x ;
			dy = to_pos.y - from_pos.y ;
			this->from_pos = from_pos ; 
			this->to_pos = to_pos ; 
		}

		Motion(const Position& from_pos, const std::vector<int>& move) {
			this->from_pos = from_pos ; 
			dx = move[0]; 
			dy = move[1]; 
			this->to_pos = Position(from_pos.x + dx, from_pos.y + dy) ; 
		}

		// Use this constructor to copy a motion 
		Motion(const Motion& m) : Motion(m.from_pos, m.to_pos) {
			// this->valid_move = m.valid_move ; 
			this->quality = m.quality ; 
			this->promotion = m.promotion ; 
			this->castling = m.castling ; 
		}

		//void set_checked() {
			//valid_move = true ; 
		//}

		bool is_extra_move() const {
			return promotion || (quality != NoCapture) ; 
		}

		// Validity check 

		bool is_in_board() const {
			return (to_pos.x < 8 && to_pos.x >= 0 && to_pos.y < 8 && to_pos.y >= 0) ; 
		}

		// Returns true if the motion is within possible range and respects the boundary of the map
		bool intrinsic_validity(PieceType type, Color color) const {
			bool is_valid = is_in_board() ; 
			if (is_valid) {
				// Check that the motion is allowed
				const auto& vec = Motion::get_range_of_attack(type, color);
				is_valid = find_in_vec(vec, {dx, dy}); 
			} 
			return is_valid  ;
		}

		// Returns a position that describes the direction of the motion
		// This direction is used 
		Position get_orientation() {
			return Position((dx > 0) ? 1 : (dx == 0) ? 0 : -1, (dy > 0) ? 1 : (dy == 0) ? 0 : -1 ) ; 
		}
		
		// Sorting vectors of motions can be done with this operator
		bool operator < (const Motion& m) const {
			return quality > m.quality ; 
		}

		// Functions for Debug

		friend std::ostream& operator<<(std::ostream& os, const Motion& m) {
			os << m.from_pos << " to " << m.to_pos  ; 
			return os ; 
		} 



} ; 


