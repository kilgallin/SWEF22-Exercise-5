#include "solver.hpp"
#include <vector>

#define PRINT_SOLVER cout << indent << "[SOLVER]: " 
#define VERBOSE_SOLVER 0
#define PRUNING 1
#define ORDERING 1
#define LEVEL 5
#define EXTRA_LEVEL 2
#define SAVE_GRAPH 0 

using namespace std ; 

int Solver::explore(int levels_to_explore, int alpha, int beta) {
	// initalise the score and best move to be found
	int score = game->is_white_to_play() ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max() ; 
	int best_score = score ; 

	// this condition is true at the first call of the function stack
	// we only care about the best 'motion' at the first stack. The rest of the time, we care about the best 'score' !
	if (levels_to_explore == level)
		Motion best_move ; 

	// Create the container for valid moves
	// containers allow for faster sorting
	// (each container contains the move of certain quality)
	std::array<std::vector<Motion>, 4> moves ; 
	for (auto& container: moves) {
		container.reserve(500) ; 
	}
	
	// get all the valid moves 
	game->get_all_valid_moves(moves) ; 	

	// start the exploration of the valid moves
	bool cutoff_happened = false ; 
	for (auto& container: moves) {
		// check if the cutoff flag happened
		if (cutoff_happened) 
			break ;

		for (auto& m: container) {
			#if SAVE_GRAPH == 1
			// first, find the indent to use. 
			// the indent is a function of 'levels to explore'
			int depth = level - levels_to_explore ; 
			for (int i=0 ; i < depth ; i++) {
				file << "    " ; 
			}

			file << m ;
			// file << ", white: " << game->is_white_to_play() ;
			file << (game->is_white_to_play() ? ", white" : ", black")  ;
			file << ", n_moves:" << number_of_moves ;
			file << "\n" ; 
			#endif 

			// first, make the move happen
			game->move_piece(m, true) ; 
			number_of_moves++; 

			// Let's find the score of this move
			if (game->is_finished()) {
				score = game->score() ; 
			} else if (levels_to_explore <= 0) {
				if ( m.is_extra_move() && (levels_to_explore > -EXTRA_LEVEL) ) {
					// then we can continue a little bit longer
					score = explore(levels_to_explore-1, alpha, beta) ; 
				} else {
					score = game->score() ; 
				}
			} else {
				// continue the recursion
				score = explore(levels_to_explore-1, alpha, beta) ; 
			}
			
			#if SAVE_GRAPH == 1
			for (int i=0 ; i < depth ; i++) {
				file << "    " ; 
			}
			file << "score: " << score ;
			file << "\n" ; 
			#endif 

			// And now that we have the score, we have to move back of 1 step
			game->resest_one_move() ; 

			// Best move selection 
			// - MinMax Search 
			// - Alpha-beta pruning 
			// - move ordering 

			// Black is playing when game->is_white_to_play() == false !
			// The move was already reversed so the logic is corrected
			if (game->is_white_to_play()) { 
				// white
				if (score > best_score) {
					best_score = score ; 
					if (levels_to_explore == level)
						best_move = m ; 
				}
				if (PRUNING) {
					if (score > beta) {
						// beta = the maximum score that black player is assured of
						cutoff_happened = true ; 
						break ; 
					}
					if (score > alpha) {
						alpha = score ; 
					}
				}
			} else {
				// Black tries to minimise the overall score
				if (score < best_score) {
					best_score = score ; 
					if (levels_to_explore == level)
						best_move = m ; 
				}
				if (PRUNING) {
					if (score < alpha) {
						// alpha = minimum score that white player is assured of
						cutoff_happened = true ; 
						break ; 
					}
					if (score < beta) {
						beta = score ; 
					}
				}
			}
		}
	}

	// Let's just find that we have reached back the optimisatin line 
	if (levels_to_explore == level) {
		this->best_move = best_move ; 
	} 



	return best_score ; 
}

Motion Solver::find_best_move() {
	this->level = LEVEL-1 ; 
	this->number_of_moves = 0 ; 
	// Eventually we can write the graph to file system 
	#if SAVE_GRAPH == 1
	file.open("../graph.txt") ;
	file << "NEW MOVE\n" ; 
	#endif

	auto t1 = std::chrono::high_resolution_clock::now() ; 
	shared_ptr<Motion> m ; 
	int score = this->explore(level, -1000000, 1000000) ; 
	m = make_shared<Motion>(this->best_move); 
	auto t2 = std::chrono::high_resolution_clock::now() ; 
	auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);

	cout << "\nSolver found best move, with level = " << LEVEL << endl ; 
	cout << *m << endl ; 
	cout << "Current score estimation: " << score << endl ; 
	cout << "Number of moves: " <<  number_of_moves << endl ; 
	cout << "Duration: " << ms_int.count() << " ms\n";
	return *m; 

	// eventually close the file
	#if SAVE_GRAPH == 1
	file.close() ; 
	#endif
}

void Solver::play_game() {
	game->print_board() ; 
	while (true) {
		if (game->is_white_to_play()) {
			// Make the player play a game
			bool success = game->prompt_for_a_move() ; 
			cout << "WHITE TO PLAY? " << game->is_white_to_play() << endl ; 
		} else {
			// Make the computer play
			Motion m = find_best_move() ; 
			game->move_piece(m, false) ; 
			game->print_board() ; 
		}
	}
}
