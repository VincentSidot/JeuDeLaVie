#include <SFML\Graphics.hpp>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <Windows.h>
#include "matrix.h"

#define WIDTH 1920
#define HEIGHT 1080
#define K 5
#define Row 1920 / K
#define Col 1080 / K
#define GAP 30

#define Cell_Color sf::Color::Black
#define Background_Color sf::Color::White

//#include "game.h"
#include "game-old.h"

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#define MAX_CYCLE 10000
#define START_PAUSE true



int main(int argv, char **argc)
{
	bool stop = false;
	bool pause = START_PAUSE;
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
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Jeu de la vie", sf::Style::Fullscreen);
	Game game(&window);
	Matrix::Matrix<Row, Col, Cell> matrix(Cell::Cell());
	for (int i = 1; i < matrix.row(); i++)
	{
		for (int j = 1; j < matrix.column(); j++)
		{
			if (rand() % GAP == 0)
			{
				matrix(i, j).setAlive(true);
				matrix(i, j).setColor(randomColor());
			}
		}
	}
	game.start(matrix);
	while (window.isOpen())
	{
		if (game.getLives() >= MAX_CYCLE) stop = true;
		else stop = false;
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
					game.start(matrix);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
				{
					for (int i = 1; i < matrix.row(); i++)
					{
						for (int j = 1; j < matrix.column(); j++)
						{
							if (rand() % GAP == 0)
							{
								matrix(i, j).setAlive(true);
								matrix(i, j).setColor(randomColor());
							}
							else
							{
								matrix(i, j).setAlive(false);
							}
						}
					}
					game.start(matrix);
				}
				break;
			default:
				break;
			}
		}
		std::cout << "Lives : " << game.getLives() << std::endl;
		if(!pause && !stop) game.nextStep();
		if (pause) std::cout << "Pause" << std::endl;
		double speed;
		if (time != GetTickCount64() && !pause)
			speed = 1000 / (GetTickCount64() - time);
		else
			speed = 0;
		time = GetTickCount64();
		if (GetTickCount64() - time2 > 250)
		{
			time2 = GetTickCount64();
			std::stringstream ss;
			ss << " Cycles : " << game.getLives() << std::endl << " Speed : " << speed << " Cycle/Second";
			text.setString(ss.str());
		}
		window.clear(Background_Color);
		game.display();
		window.draw(text);
		window.display();
	}
	return 0;
}

