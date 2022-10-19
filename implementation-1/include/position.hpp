#pragma once

/* Position of a chess piece */ 
class Position 
{
	public: 
		int x ; 
		int y ; 

		Position(int x, int y) {
			this->x = x ; 
			this->y = y ; 
		}

		void print_pos() {
			std::cout << x << " - " << y << std::endl ; 
		}

		Position(std::string s) {
			if (s.size() != 2) {
				throw std::runtime_error("POSITION ERROR: string of wrong size") ; 
			}
			// Now let's parse the position 
			switch ((char) s[0]) {
				case 'a': this->x = 0 ; break ; 
				case 'b': this->x = 1 ; break ; 
				case 'c': this->x = 2 ; break ; 
				case 'd': this->x = 3 ; break ; 
				case 'e': this->x = 4 ; break ; 
				case 'f': this->x = 5 ; break ; 
				case 'g': this->x = 6 ; break ; 
				case 'h': this->x = 7 ; break ; 
				default:
					throw std::runtime_error("POSITION ERROR: wrong letter in position") ; 
			}

			switch ((char) s[1]) {
				case '1': this->y = 0 ; break ; 
				case '2': this->y = 1 ; break ; 
				case '3': this->y = 2 ; break ; 
				case '4': this->y = 3 ; break ; 
				case '5': this->y = 4 ; break ; 
				case '6': this->y = 5 ; break ; 
				case '7': this->y = 6 ; break ; 
				case '8': this->y = 7 ; break ; 
				default:
					throw std::runtime_error("POSITION ERROR: wrong number in position") ; 
			}
		}

		Position() {
			this->x = 0 ; 
			this->y = 0 ; 
		}

		friend std::ostream& operator<<(std::ostream& os, const Position& p) {
			static constexpr char letters[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'} ;
			// this is the new way to do it
			os << letters[p.x] << p.y+1 ;

			// this is the old way of doing it
			// os << "(" << p.x << ", " << p.y << ")"  ; 
			return os ; 
		} 

		Position& operator+=(const Position& p ){
			Position new_pos ; 
			this->x += p.x ; 
			this->y += p.y ; 
			return *this ; 
		}

		Position operator+(const Position& p) {
			Position pos ; 
			pos.x = this->x + p.x ; 
			pos.y = this->y + p.y ; 
			return pos ; 
		}

		bool operator==(const Position& p2) {
			return (this->x == p2.x && this->y == p2.y);
		}

		bool is_in_board() const {
			return (x < 8 && x >= 0 && y < 8 && y >= 0) ; 
		}

} ; 
