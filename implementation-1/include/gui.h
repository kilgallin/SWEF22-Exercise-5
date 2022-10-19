#pragma once
#include <SFML/Graphics.hpp>
#include <array>

#include "chessgame.hpp"
#include "enums.hpp"

class Chessgame ; 

class PieceTextures{
	public:
		static sf::Texture blackKing;
		static sf::Texture blackQueen;
		static sf::Texture blackRook;
		static sf::Texture blackKnight;
		static sf::Texture blackBishop;
		static sf::Texture blackPawn;

		static sf::Texture whiteKing;
		static sf::Texture whiteQueen;
		static sf::Texture whiteRook;
		static sf::Texture whiteKnight;
		static sf::Texture whiteBishop;
		static sf::Texture whitePawn;

		static sf::Texture loadTexture(std::string str) {
			sf::Texture tmp;
			if (!tmp.loadFromFile(str))
				std::cout << "Error loading file\n";
			return tmp;
		}
};

class GUI: public sf::Drawable {
	private: 
		sf::Color color_white = sf::Color(215, 193, 163, 255); 
		sf::Color color_black = sf::Color(153, 104, 72, 255); 
		sf::Color color_selected = sf::Color(235, 213, 136); 

		// reference to the game
		std::array<sf::RectangleShape, 64> squares ;
		Chessgame* game ; 

		// Indexes of squares to draw with different color 
		int start_pos ; 
		int end_pos ; 

		// FUNCTIONS


		// Function to render to SFML window
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		// Put the initial sqaures
		void create_board();

		// Helper function used to get the proper piece
		sf::Sprite createPiece(Position pos, Color color, PieceType type) const ; 


		void change_squares_color() {
			for(int i=0; i<64; i++){
				bool tmpColor = (((i + i/8) % 2)==0) ? true:false;
				if (i == start_pos) {
					squares[i].setFillColor(color_selected);
				} else if (i == end_pos) {
					squares[i].setFillColor(color_selected);
				} else {
					squares[i].setFillColor(tmpColor ? color_white : color_black);
				}
			}

		}


	public:

		GUI(Chessgame* game) ; 

		void print_game() ; 

		void reset_selected_positions() {
			start_pos = -10 ; 
			end_pos = -10 ; 
			change_squares_color() ; 
		}

		void set_start_position(Position& p) {
			int index = p.x + 8* (7 - p.y); 
			start_pos = index ; 
			end_pos = -10 ; 
			change_squares_color() ; 
		}

		void set_end_position(Position& p) {
			int index = p.x + 8* (7 - p.y); 
			end_pos = index ; 
			change_squares_color() ; 
		}


} ; 

