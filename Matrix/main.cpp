#include <SFML\Graphics.hpp>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <Windows.h>
#include "matrix.h"

#define WIDTH 1920
#define HEIGHT 1080
#define Row 480
#define Col 270
#define GAP 25

#define Cell_Color sf::Color::Black
#define Background_Color sf::Color::White

//#include "game.h"
#include "game-old.h"

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

int main(int argv, char **argc)
{
	unsigned long long time = GetTickCount64();
	unsigned long long time2 = GetTickCount64();
	sf::Font font;
	if (!font.loadFromFile("C:\\Projects\\Matrix\\Release\\font.ttf"))
	{
		// erreur...
		exit(EXIT_FAILURE);
	}
	sf::Text text;
	text.setFont(font); 
	text.setCharacterSize(48);
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
		double speed;
		if (time != GetTickCount64() && !pause)
			speed = 1000 / (GetTickCount64() - time);
		else
			speed = 0;
		time = GetTickCount64();
		if (GetTickCount64() - time2 > 500)
		{
			time2 = GetTickCount64();
			std::stringstream ss;
			ss << "Cycles : " << game.getLives() << std::endl << "Speed : " << speed << " Cycle/Second";
			text.setString(ss.str());
		}
		window.clear(Background_Color);
		window.draw(text);
		game.display();
		window.display();
	}
	return 0;
}

