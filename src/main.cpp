#include <SFML/Graphics.hpp>
#include <time.h>
#include "Connector.hpp"
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

std::string postion = "";

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


int main()
{
    //RenderWindow window(VideoMode(594, 594), "The Chess! (press SPACE)");
    RenderWindow window(VideoMode(454, 453), "The Chess! (press SPACE)");

    Texture t1,t2;
    t1.loadFromFile("images/figures.png"); 
    t2.loadFromFile("images/board0.png");
    //t2.loadFromFile("images/bandw_chessboard.png");

    Sprite s(t1);
    Sprite sBoard(t2);

    Vector2f oldPos, newPos;
    std::string str;

    for(int i=0;i<32;i++) figures[i].setTexture(t1);

    loadPosition();

    bool isMove = false;
    float dx = 0, dy = 0;
    int n = 0;

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

            if(e.type == Event::MouseButtonReleased)
                if(e.key.code == Mouse::Left)
                {
                    isMove = false;
                    Vector2f p = figures[n].getPosition() + Vector2f(size/2,size/2);
                    Vector2f newPos = Vector2f( size*int(p.x/size), size*int(p.y/size));
                    str = toChessNote(oldPos) + toChessNote(newPos);
                    move(str);
                    position += str + " ";
                    std::cout<<str<<std::endl;
                    figures[n].setPosition(newPos);
                }
        }

        if (isMove) figures[n].setPosition(pos.x-dx, pos.y-dy);

        window.clear();
        window.draw(sBoard);
        for(int i =0; i<32; i++) window.draw(figures[i]);
        window.display();
    }
}
