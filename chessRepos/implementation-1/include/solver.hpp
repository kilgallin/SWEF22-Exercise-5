#pragma once 
#include "chessgame.hpp"
#include <memory>
#include <string>
#include <chrono>
#include <limits>

#include <iostream>
#include <fstream>


class Solver {
	private: 
		int level = 1 ;
		Chessgame* game ; 
		int number_of_moves = 0  ;
		Motion best_move ; 


		// Eventually, we can write the solver's graph into memory
		std::ofstream file ; 


		std::tuple<bool, int, std::shared_ptr<Motion>> find_optimisation_tree(int levels_to_explore, std::vector<Motion>& moves, int alpha, int beta) ; 

		// This will be the new optimisation function
		int explore(int levels_to_explore, int alpha, int beta) ; 
		

	public: 
		Solver(Chessgame* game) {
			this->game = game ; 
			remove("../graph.txt") ; 
		} 

		~Solver() {}

		Motion find_best_move() ; 

		void set_level(int level) {
			this->level = level ; 
		}

		void play_game() ; 

} ; 
