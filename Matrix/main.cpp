#include <SFML\Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <Windows.h>
#include "matrix.h"

#define WIDTH 1920
#define HEIGHT 1080
#define Row 960
#define Col 540
#define GAP 50

#define Cell_Color sf::Color::Yellow
#define Background_Color sf::Color::Black

//#include "game.h"
#include "game-old.h"

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

int main(int argv, char **argc)
{
	sf::Text text;
	text.setFillColor(sf::Color::Red);
	bool pause = true;
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Philo", sf::Style::Fullscreen);
	Game game(&window, Cell_Color);
	Matrix::Matrix<Row, Col, Cell> matrix(Cell::DEAD);
	for (int i = 1; i < matrix.row(); i++)
	{
		for (int j = 1; j < matrix.column(); j++)
		{
			if (rand() % GAP == 0)
			{
				matrix(i, j) = Cell::ALIVE;
			}
		}
	}
	game.start(matrix);
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) pause = !pause;
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
				{
					for (int i = 1; i < matrix.row(); i++)
					{
						for (int j = 1; j < matrix.column(); j++)
						{
							if (rand() % GAP == 0)
							{
								matrix(i, j) = Cell::ALIVE;
							}
							else
							{
								matrix(i, j) = Cell::DEAD;
							}
						}
					}
					game.start(matrix);
				}
				break;
			}
		}
		std::cout << "Lives : " << game.getLives() << std::endl;
		if(!pause) game.nextStep();
		if (pause) std::cout << "Pause" << std::endl;
		text.setString(std::to_string(game.getLives()));
		window.clear(Background_Color);
		game.display();
		window.draw(text);
		window.display();
	}
	return 0;
}

