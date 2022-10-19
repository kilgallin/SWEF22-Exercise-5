#include <iostream>
#include <sstream>
#include <vector>
#include <deque>
#include <set>
#include <unordered_set>

#include "chessgame.hpp"
#include "piece.hpp"
#include "solver.hpp"
#include "gui.h"

using namespace std ; 

void pointer_test() {
	int a,b,c ;
	a = 10 ; 
	b = 100 ; 
	c = 1000 ; 

	int* array[3] = {} ; 
	array[0] = nullptr ; 
	array[1] = nullptr ; 
	array[2] = nullptr ; 
	cout << "array: " << array[0] << ", " << array[1] << ", " << array[2] << endl ; 

	// p0 becomes a copy of array[0]
	// and therefore the array is not modified...
	int* p0 = array[0] ; 
	p0 = &a ; 
	cout << "pointer value: " << p0 << endl ; 
	cout << "array: " << array[0] << ", " << array[1] << ", " << array[2] << endl ; 

	cout << "Changing directly:  " ; 
	array[0] = &a ; 
	cout << array[0] << ", " << array[1] << ", " << array[2] << endl ; 

	// so let's do it with a reference
	// and now it's gonna work ! 
	int* &p1 = array[1] ; 
	p1 = &b ; 
	cout << "array: " << array[0] << ", " << array[1] << ", " << array[2] << endl ; 
}

void split_test() {
	auto cams = "cam0, cam1" ; 

	vector<string> strings;
	istringstream f(cams);
	string s;    
	while (getline(f, s, ',')) {
		cout << s << "  " <<  (int) s.back() - (int) '0' << endl;

		strings.push_back(s);
	}
}

void array_test() {
	// these are all the allowed move for white pawns
	// But there is a ton of extra rules to add 
	// - that there is no piece in the path of visited 
	// - that the capture is valid: there has to be a piece from the other color
	// - that the pawn does not leave the board
	// For black pawns, the array can be obtained by flipping the y values
	int array[4][2] = {{0,1}, {0,2}, {1,1}, {-1,1}} ; 
	cout << "move: " << array[1][0] << ", " << array[1][1]<< endl ; 

	// But we could also use something else
	vector<vector<int>> vec = {{0,1}, {0,2}, {1,1}, {-1,1}} ; 
	cout << vec.at(0).at(0) << endl ; 
	cout << "move: " << vec[1][0] << ", " << vec[1][1]<< endl ; 
}

void print_matric(const std::array<std::array<int,8>, 8> &  mat) {
	cout << endl ; 
	for (const auto& vec: mat) {
		for (const auto& m: vec) {
			cout << m << " , " ; 
		}
		cout << endl ; 
	}
}

void matrix_test() {
	std::array<std::array<int,8>, 8>  mat {}; 
	print_matric(mat) ; 
	mat[0][0] = 1 ; 
	print_matric(mat) ; 
	mat = {} ; 
	print_matric(mat);

}

void print_vector(vector<Motion>& vec) {
	for (const auto& m: vec) {
		cout << &m << " - valid ? " << endl ; 
	}
}


void test_motion_sorting() {
	Motion m1 = Motion() ; 
	Motion m2 = Motion() ; 
	Motion m3 = Motion() ; 
	m1.quality = NoCapture ;
	m2.quality = Capture ; 
	m3.quality = GoodCapture ; 
	std::deque<Motion> moves ; 
	moves.push_back(m1) ; 
	moves.push_back(m2) ; 
	moves.push_back(m3) ; 
	for (const auto& move: moves ) {
		cout << move.quality ; 
	}
	cout << endl ; 
	std::sort(moves.begin(), moves.end()) ; 
	for (const auto& move: moves ) {
		cout << move.quality ; 
	}
	cout << endl ; 
}

void gui_game(Chessgame& game, Solver& solver, bool play_with_solver) {
	GUI gui(&game) ; 
	bool piece_selected = false ; 
	bool solver_has_to_play = false ; 
	Position p1 ; 
    sf::RenderWindow window(sf::VideoMode(768,512), "Chess", sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);
    while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){

			// Here, we can ask the solver for the best move
			if (play_with_solver && solver_has_to_play) {
				Motion m = solver.find_best_move() ; 
				gui.set_start_position(m.from_pos) ; 
				gui.set_end_position(m.to_pos) ; 
				game.move_piece(m, false) ; 
				solver_has_to_play = false ; 
			}

            if(event.type == sf::Event::Closed)
                window.close();

			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Key::A) {
					game.print_attacked_pieces() ;
				}
				if (event.key.code == sf::Keyboard::Key::R) {
					cout << "Resetting 1 move ! " << endl;
					game.resest_one_move() ;
				}
			}

            if(event.type == sf::Event::MouseButtonPressed){
                if(event.mouseButton.button == sf::Mouse::Left){
					int x = event.mouseButton.x ;
					int y = event.mouseButton.y ;
					int x_pos = (x / 64) ; 
					int y_pos = 7 - (y / 64) ; 
					// logic to select a piece
					if (! piece_selected) {
						// try to select the piece at this given position 
						const auto& piece = game.get_piece_ref(x_pos, y_pos) ; 
						if (piece != nullptr) {
							cout << "There is a piece there: " << *piece << endl ; 
							piece_selected = true ; 
							p1 = Position(x_pos, y_pos)  ;
							gui.set_start_position(p1) ; 
						} else {
							cout << "No piece to move" << endl ; 
							piece_selected = false ; 
							gui.reset_selected_positions() ; 
						}
					} else {
						// it means we want to move the piece there ; 
						auto p2 = Position(x_pos, y_pos) ; 

						// now we can try to make the move
						Motion m = Motion(p1, p2) ; 
						bool success = game.check_and_move(m) ; 

						if (success) {
							gui.set_end_position(p2) ; 
							solver_has_to_play = true ; 
						} else {
							cout << "ERROR: wrong motion, try again." << endl ; 
							gui.reset_selected_positions() ; 
						}
						piece_selected = false ; 
					}
                } 
            }
        }

        window.draw(gui);
        window.display(); 
	}
}

void test_int_to_letter() {
	static constexpr char letters[8] = {'a', 'b', 'c'} ;
	cout << letters[0] << endl ; 
}

int main(int argc, char** argv)
{
	//test_motion_sorting() ; 
	//test_check_moves(game) ; 
	// matrix_test() ; 
	// test_int_to_letter();

	Chessgame game ; 
	Solver solver = Solver(&game); 
	gui_game(game, solver, true) ; 
	
	//solver.play_game() ; 
	//game.prompt_for_moves();
	
	return 0 ; 
}
