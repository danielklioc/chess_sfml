/*
void engine_move(int n)
{
	sf::Clock clock;
	std::string str = "";
	Vector2f oldPosition, newPosition;

	while (clock.getElapsedTime().asSeconds() < 4.0f)
	{
	}

	str = getNextMove(position);
	oldPosition = toCoord(str[0], str[1]);
	newPosition = toCoord(str[2], str[3]);

	for (int i = 0; i < 32; i++)
		if (figures[i].getPosition() == oldPosition)
			n = i;
}

void engineMoveAnimation(int n, Vector2f oldPosition, Vector2f newPosition)
{
	// Animation of move
	for (int k = 0; k < 50; k++)
	{
		Vector2f p = newPosition - oldPosition;
		figures[n].move(p.x / 50, p.y / 50);
		//window.draw(sBoard);

		for (int i = 0; i < 32; i++)
		{
		}
		//window.draw(figures[i]);

	//window.draw(figures[n]);
	//window.display();
	}
	//clock.restart();
}

class Gui
{
	Event event;

	enum eventType
	{
		CLOSED,
		KEY_PRESSED,
		PREVIOUS_MOVE,
		MOUSE_BUTTON_PRESSED,
		LEFT_MOUSE_BUTTON_PRESSED,
		BUTTON_RELEASED,
		LEFT_MOUSE_BUTTON_RELEASED,
		ENGINE_MOVE,
		PLAYER_MOVE
	};

public:
	Gui(Event e) : event(e)
	{
	}

	~Gui()
	{
	}

	void GuiState()
	{
		if (event.type == Event::Closed)
		{
		}

	}

};

class Game
{
	int width = 454;
	int height = 453;
	VideoMode mode1(width,height); //, "The Chess Game! (press: RIGHT for engine move, LEFT for previous move)";
	//VideoMode(454, 453), "The Chess Game! (press: RIGHT for engine move, LEFT for previous move)";
public:
	Game() {}

	~Game() {}

	void startGame()
	{
		RenderWindow window(mode1, "The Chess Game! (press: RIGHT for engine move, LEFT for previous move)");//VideoMode(454, 453), "The Chess! (press SPACE)");

		while (window.isOpen())
		{

		}
	}
};*/