#include <SFML/Graphics.hpp>
#include "SquareGrid.h"

//main function
//sets program up and handles events and drawing
int main()
{
	sf::RenderWindow window(sf::VideoMode(200, 200), "COP5570 Term Project");
	sf::Color colors[2] = {sf::Color(0, 0, 0), sf::Color(255, 255, 255)};
	SquareGrid* grid = new SquareGrid(&window, 64, 64, colors);
	int counter = -1;

	for (int x = 0; x < 64; ++x)
	{
		for (int y = 0; y < 64; ++y)
		{
			grid->setIntToGrid(1, x, y);
		}
	}

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		counter = (counter + 1) % 8192;
		grid->setIntToGrid(counter / 4096, (counter / 64) % 64, counter % 64);

		window.clear();
		grid->draw();
		window.display();
	}

	return 0;
}