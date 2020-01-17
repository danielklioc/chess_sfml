/*
 * author: Daniel Klioc
 * brief: Chess game with GUI using SFML librarie and UCI interface
 * 
 * TO do:
 * 1. UCI protocol using boost pipes and process
 * 2. control who is turn now: human or engine, W or B
 * 3. End of game
 * 4. legal move for each figure
 * 5. separate backend and gui logic 
 * 6. En passant
 * 7. Checkmate
 * 8. animation easing
 *
*/
#define CONNECTOR_OPTION 1
#define UCI_OPTINO 0

#include <SFML/Graphics.hpp>
#include <time.h>
#include "Connector.hpp"
#include <optional>
#include <iostream>

using namespace sf;

int size = 56;
Vector2f offset(28,28);

Sprite figures[32]; //figures
std::string position="";

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

/*
 * convert figure position from vector to board representation
 */
std::string toChessNote(Vector2f p)
{
    std::string s = "";
    s += char(p.x/size+97);
    s += char(7-p.y/size+49);
    return s;
}

/*
 * move figure to coordinated
 */
Vector2f toCoord(char a,char b)
{
   int x = int(a) - 97;
   int y = 7-int(b)+49;
   return Vector2f(x*size,y*size);
}

/*
 * check if figure have been defeated
 * remove from board
 */
void move(std::string str)
{
    Vector2f oldPos = toCoord(str[0],str[1]);
    Vector2f newPos = toCoord(str[2],str[3]);

    for(int i=0;i<32;i++)
     if (figures[i].getPosition()==newPos) figures[i].setPosition(-100,-100);
        
    for(int i=0;i<32;i++)
        if (figures[i].getPosition()==oldPos) figures[i].setPosition(newPos);

    //castling       //if the king didn't move
    if (str=="e1g1") if (position.find("e1")==-1) move("h1f1"); 
    if (str=="e8g8") if (position.find("e8")==-1) move("h8f8");
    if (str=="e1c1") if (position.find("e1")==-1) move("a1d1");
    if (str=="e8c8") if (position.find("e8")==-1) move("a8d8");
}

/*
 * Calculate figures rectangule size
 * Set position of each figure
 */
void loadPosition()
{
    int k=0;
    for(int i=0;i<8;i++)
    for(int j=0;j<8;j++)
     {
       int n = board[i][j];
       if (!n) continue;
       int x = abs(n)-1;
       int y = n>0?1:0;
       figures[k].setTextureRect( IntRect(size*x,size*y,size,size) );
       figures[k].setPosition(size*j,size*i);
       k++;
     }

    for(int i = 0; i<position.length(); i+=5)
        move(position.substr(i,4));
}

struct NewMove {
	int i;
	std::string from;
	std::string to;
};

NewMove event_move_started(const int figure, const Vector2f& position) {
	const auto start_note = toChessNote(position);
	return NewMove{
		figure,
		start_note,
		""
	};
}

void event_move_finished(NewMove& move, const int figure, const Vector2f& position) {
	const auto end_note = toChessNote(position);
	move.to = end_note;
}


void engine_move()
{

}

int main()
{
    RenderWindow window(VideoMode(454, 453), "The Chess! (press SPACE)");

#ifdef CONNECTOR_OPTION
	ConnectToEngine("src/stockfish.exe");
#elif UCI_OPTINO
	//do nothing
#endif // DEBUG

    

    Texture t1,t2;
    t1.loadFromFile("images/figures.png"); 
    t2.loadFromFile("images/board0.png");
    sf::Clock clock;

    bool isMove = false;
    float dx = 0, dy = 0;
    int n = 0;

    Sprite s(t1);
    Sprite sBoard(t2);

    Vector2f oldPos, newPos;
    std::string str;

    for(int i=0;i<32;i++) figures[i].setTexture(t1);

    loadPosition();


    while(window.isOpen())
    {
        Vector2i pos= Mouse::getPosition(window);

        Event e;
        while(window.pollEvent(e))
        {
            if(e.type == Event::Closed)
                window.close();

            //move back
            if(e.type == Event::KeyPressed)
                if(e.key.code == Keyboard::BackSpace )
                {
                    position.erase(position.length() - 6,5);
                    loadPosition();
                }
            // drag and drop figures
			// start
			//inmove
			//finished. legal move
            if(e.type == Event::MouseButtonPressed)
                if(e.key.code == Mouse::Left)
                    for(int i=0;i<32;i++)
                        if(figures[i].getGlobalBounds().contains(pos.x,pos.y))
                        {
                            isMove = true;

                            n = i;
                            dx = pos.x - figures[i].getPosition().x;
                            dy = pos.y - figures[i].getPosition().y;
                            oldPos = figures[i].getPosition();
                        }

			if (e.type == Event::MouseButtonReleased)
			{
				if (e.key.code == Mouse::Left)
				{
					isMove = false;
					Vector2f p = figures[n].getPosition() + Vector2f(size / 2, size / 2);
					Vector2f newPos = Vector2f(size*int(p.x / size), size*int(p.y / size));
					str = toChessNote(oldPos) + toChessNote(newPos);
					move(str);
					position += str + " ";
					std::cout << str << std::endl;
					figures[n].setPosition(newPos);
				}

				while (clock.getElapsedTime().asSeconds() < 4.0f)
				{
				}

				str = getNextMove(position);
				oldPos = toCoord(str[0], str[1]);
				newPos = toCoord(str[2], str[3]);

				for (int i = 0; i < 32; i++)
					if (figures[i].getPosition() == oldPos)
						n = i;

				// Animation of move
				for (int k = 0; k < 50; k++)
				{
					//if( figures[n].getPosition() != newPos)
					//{
					Vector2f p = newPos - oldPos;
					figures[n].move(p.x / 50, p.y / 50);
					window.draw(sBoard);

					for (int i = 0; i < 32; i++)
						window.draw(figures[i]);

					window.draw(figures[n]);
					window.display();
				}
				//clock.restart();
			}
        }

        // computer move
        if(Keyboard::isKeyPressed(Keyboard::Space))
        {
            str = getNextMove(position);
            oldPos = toCoord(str[0],str[1]);
            newPos = toCoord(str[2],str[3]);

            for(int i=0; i<32; i++)
                if (figures[i].getPosition() == oldPos)
                    n=i;

            // Animation of move
            for(int k=0;k<50;k++)
            {
                //if( figures[n].getPosition() != newPos)
                //{
                    Vector2f p = newPos - oldPos;
                    figures[n].move(p.x/50, p.y/50);
                    window.draw(sBoard);

                    for(int i=0;i<32;i++)
                        window.draw(figures[i]);

                    window.draw(figures[n]);
                    window.display();
                //}
                //else
                   // break;
            }


           move(str);
           position+=str+" ";
           figures[n].setPosition(newPos);

        }

		//inMove figure
        if (isMove) figures[n].setPosition(pos.x-dx, pos.y-dy);

        // clear window
        window.clear();
        // draw chess board
        window.draw(sBoard);
        // draw chess figures
        for(int i =0; i<32; i++) window.draw(figures[i]);
        window.display();
    }
}
