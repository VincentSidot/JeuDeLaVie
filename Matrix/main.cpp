#include <SFML\Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <Windows.h>
#include "matrix.h"

#define WIDTH 800
#define HEIGHT 600
#define Row 400
#define Col 300

#include "game.h"

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

int main(int argv, char **argc)
{
	bool pause = true;
	//srand(GetTickCount());
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Philo");
	Game game(&window);
	Matrix::Matrix<Row, Col, Cell> matrix(Cell::DEAD);
	for (int i = 1; i < matrix.row(); i++)
	{
		for (int j = 1; j < matrix.column(); j++)
		{
			if (rand() % 25 == 0)
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
				break;
			}
		}
		std::cout << "Lives : " << game.getLives() << std::endl;
		if(!pause) game.nextStep();
		window.clear(sf::Color::White);
		game.display();
		window.display();
	}
	return 0;
}

