#include "piece.hpp"

using namespace std ; 

Piece::Piece(Color c, PieceType t, int x, int y) {
	color = c ; 
	type = t ; 
	auto position = Position() ; 
	position.x = x ; 
	position.y = y ; 
	pos = position ; 
}

Piece::Piece(Color c, PieceType t, Position p) {
	color = c ; 
	type = t ; 
	pos = p ; 
}

const char* Piece::piece_char()  const {
	if (color == White) {
		switch(type) {
			case PieceType::King: return "♔" ; 
			case PieceType::Queen: return "♕" ; 
			case PieceType::Bishop: return "♗" ;  
			case PieceType::Knight: return "♘";  
			case PieceType::Rook: return "♖" ;  
			case PieceType::Pawn: return "♙" ; 
			default: return "?" ; 
		}
	} else {
		switch(type) {
			case PieceType::King: return "♚" ; 
			case PieceType::Queen: return "♛" ; 
			case PieceType::Bishop: return "♝" ;  
			case PieceType::Knight: return "♞" ;
			case PieceType::Rook: return "♜" ;  
			case PieceType::Pawn: return "♟︎" ; 
			default: return "?" ; 
		}
	}
}


ostream& operator<<(ostream& os, const Piece& p){
	std::string type_str ; 
	switch(p.type) {
		case PieceType::King: type_str = "King" ; break ; 
		case PieceType::Queen: type_str = "Queen" ; break ;  
		case PieceType::Bishop: type_str = "Bishop" ;  break ; 
		case PieceType::Knight: type_str = "Knight" ;  break ; 
		case PieceType::Rook: type_str = "Rook" ;  break ; 
		case PieceType::Pawn: type_str = "Pawn" ; break ; 
		default: type_str = "What ?" ; 
	}
	os << type_str << " at pos: " << p.pos.x << ", " << p.pos.y ; 
	return os ; 
}

