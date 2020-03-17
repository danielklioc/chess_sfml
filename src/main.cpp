/*
 * author: Daniel Klioc
 * brief: Chess game with GUI using SFML librarie and UCI interface
 * 
 * TO do:
 * 1. UCI protocol using boost pipes and process
 * 2. control who is turn now: human or engine, W or B
 * 3. End of game
 * 4. legal move for each figure
 * 6. En passant
 * 7. Checkmate
 * 8. animation easing
 * 9. highlight then click on figure, to show legal move
 * 10. check if string holding previous moves is empty. at the moment it crashes game.
 *     in future, just block previousmove method
*/

#include <SFML/Graphics.hpp>
#include <time.h>
#include "Connector.hpp"
#include <iostream>
#include "Piece.h"

int boardSize = 56;
sf::Vector2f offset(28, 28);

sf::Sprite figures[32]; //Drawable representation of chess figure
std::string position="";

void castling(std::string strPos);
void move(std::string str);

// Standard 8x8 board representation with figures weights
int board[8][8] = 
    {-1,-2,-3,-4,-5,-3,-2,-1,
     -6,-6,-6,-6,-6,-6,-6,-6,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      6, 6, 6, 6, 6, 6, 6, 6,
      1, 2, 3, 4, 5, 3, 2, 1};

/** convert figure position from vector to board representation. */
std::string toChessNote(sf::Vector2f position)
{
    std::string s = "";
    s += char(position.x/boardSize+97);
    s += char(7 - position.y/boardSize+49);
    return s;
}

/** convert figure position from chess board representation to vector  */
sf::Vector2f toCoord(char a,char b)
{
   int x = int(a) - 97;
   int y = 7-int(b)+49;
   return sf::Vector2f(x*boardSize,y*boardSize);
}

void castling(std::string strPos)
{
	//castling       
	//if the king made first move
	if (strPos == "e1g1") if (position.find("e1") == -1) move("h1f1");
	if (strPos == "e8g8") if (position.find("e8") == -1) move("h8f8");
	if (strPos == "e1c1") if (position.find("e1") == -1) move("a1d1");
	if (strPos == "e8c8") if (position.find("e8") == -1) move("a8d8");
} 

void move(std::string str)
{
	sf::Vector2f oldPos = toCoord(str[0], str[1]);
	sf::Vector2f newPos = toCoord(str[2], str[3]);

	// check if figure have been atacked
	for (int i = 0; i < 32; i++)
		if (figures[i].getPosition() == newPos)
		{
			// remove from board
			figures[i].setPosition(-100, -100);
		}

	for (int i = 0; i < 32; i++)
		if (figures[i].getPosition() == oldPos) figures[i].setPosition(newPos);

	// castling
	castling(str);
}


class NewMove
{
public:
	NewMove() {}
	~NewMove(){}

	bool previousMove()
	{
		position.erase(position.length() - 6, 5);
		loadPosition();
		std::cout << position << std::endl;
		return true;
	}

	/*
	 * Calculate figures rectangule size
	 * Set position of each figure
	 */
	void loadPosition()
	{
		int k = 0;
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
			{
				int n = board[i][j];
				if (!n) continue;
				int x = abs(n) - 1;
				int y = n > 0 ? 1 : 0;
				figures[k].setTextureRect(sf::IntRect(boardSize*x, boardSize*y, boardSize, boardSize));
				figures[k].setPosition(boardSize*j, boardSize*i);
				k++;
			}

		// Takes previous made moves and updates board according them
		for (int i = 0; i < position.length(); i += 5)
			move(position.substr(i, 4));
	}

};

class ChessGame
{
	NewMove move;

public:
	// constructor
	ChessGame() 
	{
		move.loadPosition();
		ConnectToEngine("src/stockfish.exe");
	}

	//destructor
	~ChessGame() 
	{
		CloseConnection();
	}

};

class Gui
{
	
public:
	sf::Texture figuresTexture, boardTexture;
	sf::Sprite boardSprite;

	Gui() 
	{
		if (!figuresTexture.loadFromFile("images/figures.png"))
		{
			std::cout << "Error to load figures texture " << std::endl;
		}
		if (!boardTexture.loadFromFile("images/board2.png"))
		{
			std::cout << "Error to load board texture " << std::endl;
		}
		
		//Drawable representation of texture
		boardSprite.setTexture(boardTexture);
		for (int i = 0; i < 32; i++) figures[i].setTexture(figuresTexture);
	}

	~Gui() {}

private:
	int width0 = 454, height0 = 453; //for board0
	int width = 504, height = 504; // for board
	int width2 = 490, height2 = 506; //for board2
};



int main()
{
	sf::Vector2f oldPosition, newPosition;
	std::string str;
	ChessGame game;
	NewMove m;
	Gui gui;

	float dx = 0, dy = 0;
	int n = 0;
	int width0 = 490, height0 = 506; //for board2
	
	bool isMove = false;
	bool whiteMove = true;

	// (press: RIGHT for engine move, LEFT for previous move)");
	sf::RenderWindow window(sf::VideoMode(width0, height0), "The Chess Game!");

    while(window.isOpen())
    {
		sf::Vector2i pos= sf::Mouse::getPosition(window);
		sf::Event e;

		// pop event on top of event queue
		while (window.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
			{
				window.close();
			}

			//previous move
			if (e.type == sf::Event::KeyPressed)
				if (e.key.code == sf::Keyboard::Left)
				{
					whiteMove = m.previousMove();
				}
			// drag and drop figures
			// start
			//inmove
			//finished. legal move
			if (e.type == sf::Event::MouseButtonPressed)
				if (e.key.code == sf::Mouse::Left)
					if (whiteMove)
					{
						for (int i = 0; i < 32; i++)
							if (figures[i].getGlobalBounds().contains(pos.x, pos.y))
							{
								isMove = true;
								n = i;
								dx = pos.x - figures[i].getPosition().x;
								dy = pos.y - figures[i].getPosition().y;
								oldPosition = figures[i].getPosition();
							}
					}
			if (e.type == sf::Event::MouseButtonReleased)
			{
				if (e.key.code == sf::Mouse::Left)
				{
					isMove = false;
					sf::Vector2f p = figures[n].getPosition() + sf::Vector2f(boardSize / 2, boardSize / 2);
					sf::Vector2f newPos = sf::Vector2f(boardSize*int(p.x / boardSize), boardSize*int(p.y / boardSize));
					str = toChessNote(oldPosition) + toChessNote(newPos);
					move(str);
					position += str + " ";
					std::cout << str << std::endl;
					figures[n].setPosition(newPos);
				}
				whiteMove = false;
			}


			// computer move
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				if (!whiteMove)
				{
					str = getNextMove(position);
					oldPosition = toCoord(str[0], str[1]);
					newPosition = toCoord(str[2], str[3]);

					for (int i = 0; i < 32; i++)
						if (figures[i].getPosition() == oldPosition)
							n = i;

					// Animation of move
					for (int k = 0; k < 50; k++)
					{
						sf::Vector2f p = newPosition - oldPosition;
						figures[n].move(p.x / 50, p.y / 50);
						window.draw(gui.boardSprite);

						for (int i = 0; i < 32; i++)
							window.draw(figures[i]);

						window.draw(figures[n]);
						window.display();

					}
				}

				move(str);
				position += str + " ";
				figures[n].setPosition(newPosition);

				whiteMove = true;
			}
		}

		//draw figure on new position after player moved it
        if (isMove) figures[n].setPosition(pos.x-dx, pos.y-dy);

		// clear window
		window.clear();
		// draw chess board
		window.draw(gui.boardSprite);
		//for (int i = 0; i < 32; i++) figures[i].move(offset); // for board0
		// draw chess figures
		for (int i = 0; i < 32; i++)
		{
			window.draw(figures[i]);
		}
		window.draw(figures[n]);
		//for (int i = 0; i < 32; i++) figures[i].move(-offset);
		window.display();
    }

	return 0;
}
