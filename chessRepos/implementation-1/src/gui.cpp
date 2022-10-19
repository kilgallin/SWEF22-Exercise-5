#include "gui.h"


sf::Texture PieceTextures::blackKing = PieceTextures::loadTexture("../Textures/b_king.png");
sf::Texture PieceTextures::blackQueen = PieceTextures::loadTexture("../Textures/b_queen.png");
sf::Texture PieceTextures::blackRook = PieceTextures::loadTexture("../Textures/b_rook.png");
sf::Texture PieceTextures::blackKnight = PieceTextures::loadTexture("../Textures/b_knight.png");
sf::Texture PieceTextures::blackBishop = PieceTextures::loadTexture("../Textures/b_bishop.png");
sf::Texture PieceTextures::blackPawn = PieceTextures::loadTexture("../Textures/b_pawn.png");
sf::Texture PieceTextures::whiteKing = PieceTextures::loadTexture("../Textures/w_king.png");
sf::Texture PieceTextures::whiteQueen = PieceTextures::loadTexture("../Textures/w_queen.png");
sf::Texture PieceTextures::whiteRook = PieceTextures::loadTexture("../Textures/w_rook.png");
sf::Texture PieceTextures::whiteKnight = PieceTextures::loadTexture("../Textures/w_knight.png");
sf::Texture PieceTextures::whiteBishop = PieceTextures::loadTexture("../Textures/w_bishop.png");
sf::Texture PieceTextures::whitePawn = PieceTextures::loadTexture("../Textures/w_pawn.png");

using namespace std ; 

sf::Sprite GUI::createPiece(Position pos, Color color, PieceType type) const {
    sf::Sprite m_sprite = sf::Sprite();
    switch (type)
    {
        case King:
            m_sprite.setTexture(color==White ? PieceTextures::whiteKing : PieceTextures::blackKing);
            break;
        case Queen:
            m_sprite.setTexture(color==White ? PieceTextures::whiteQueen : PieceTextures::blackQueen);
            break;
        case Rook:
            m_sprite.setTexture(color==White ? PieceTextures::whiteRook : PieceTextures::blackRook);
            break;
        case Bishop:
            m_sprite.setTexture(color==White ? PieceTextures::whiteBishop : PieceTextures::blackBishop);
            break;
        case Knight:
            m_sprite.setTexture(color==White ? PieceTextures::whiteKnight : PieceTextures::blackKnight);
            break;
        case Pawn:
            m_sprite.setTexture(color==White ? PieceTextures::whitePawn : PieceTextures::blackPawn);
            break;
        default:
            std::cerr << "Error piece type does not exist.\n";
            break;
    }
	m_sprite.setPosition(sf::Vector2f(pos.x*64+9, 448 - pos.y*64+8)) ; 
	m_sprite.setScale(sf::Vector2f(0.375f,0.375f));
	return m_sprite ; 
}

GUI::GUI(Chessgame* game) {
	this->game = game ; 
	this->reset_selected_positions() ; 
	create_board() ; 
}

void GUI::create_board() {
    for(int i=0; i<8;i++){
        bool tmpColor = ((i % 2)==0)?true:false;
        for(int j=0; j<8;j++){
            squares[j + (i * 8)].setPosition(sf::Vector2f( j*64.f , i*64.f ));
            squares[j + (i * 8)].setSize(sf::Vector2f(64.f, 64.f));
			squares[j + (i * 8)].setFillColor(tmpColor ? color_white : color_black);
            tmpColor = !tmpColor;
        }
    }
}

void GUI::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	for(int i=0;i<64;i++){
		target.draw(squares[i]);
	}
	for(int i=0;i<64;i++){
		const auto& piece = game->get_piece_ref(i) ; 
		if (piece != nullptr) {
			sf::Sprite s = createPiece(piece->get_pos(), piece->get_color(), piece->get_type()) ; 
			target.draw(s) ; 
		}
	}
}
