#pragma once
#include "matrix.h"
#include <vector>
#include <initializer_list>

typedef enum Cell
{
	DEAD, ALIVE
}Cell;

sf::Color randomColor() { return sf::Color(rand() % 255, rand() % 255, rand() % 255, 255); }


void dispMatrix(Matrix::Matrix<Row, Col, Cell> const &matrix, sf::RenderTarget *screen, sf::Color color)
{
	sf::Vector2u screen_size = screen->getSize();
	for (int i = 1; i <= matrix.row(); i++)
		for (int j = 1; j <= matrix.column(); j++)
		{
			if (matrix(i, j) == Cell::ALIVE)
			{
				sf::VertexArray vertex(sf::PrimitiveType::Quads, 4);
				sf::Vector2f position;
				//First point
				position.x = (i - 1)*(screen_size.x / matrix.row());
				position.y = (j - 1)*(screen_size.y / matrix.column());
				vertex[0] = sf::Vertex(position, color);
				//Second point
				position.x = (i - 1)*(screen_size.x / matrix.row());
				position.y = j*(screen_size.y / matrix.column());
				vertex[1] = sf::Vertex(position, color);
				//Third point
				position.x = i*(screen_size.x / matrix.row());
				position.y = j*(screen_size.y / matrix.column());
				vertex[2] = sf::Vertex(position, color);
				//Last point
				position.x = i*(screen_size.x / matrix.row());
				position.y = (j - 1)*(screen_size.y / matrix.column());
				vertex[3] = sf::Vertex(position, color);
				//
				screen->draw(vertex);
			}
		}
	return;
}

class Game
{
public:
	Game(sf::RenderTarget *target,sf::Color color) : m_matrix(Cell::DEAD)
	{
		m_color = color;
		m_target = target;
		lives = 0;
	}
	void start(const std::initializer_list<Cell> &liste)
	{
		m_matrix.fill(liste);
		lives = 0;
		return;
	}
	void start(const Matrix::Matrix<Row,Col,Cell> const &m)
	{
		m_matrix = m;
		lives = 0;
		return;
	}
	void nextStep()
	{
		lives++;
		for (int i = 1; i <= m_matrix.row(); i++)
		{
			for (int j = 1; j <= m_matrix.column(); j++)
			{
				if (m_matrix(i, j) == Cell::ALIVE)
				{
					if (countCell(sf::Vector2u(i, j)) == 2 || countCell(sf::Vector2u(i, j)) == 3)
					{
						m_matrix(i, j) = Cell::ALIVE;
					}
					else
					{
						m_matrix(i, j) = Cell::DEAD;
					}
				}
				else
				{
					if (countCell(sf::Vector2u(i, j)) == 3)
					{
						m_matrix(i, j) = Cell::ALIVE;
					}
					else
					{
						m_matrix(i, j) = Cell::DEAD;
					}
				}
			}
		}
	}
	size_t getLives() const { return lives; }
	inline void display() const { dispMatrix(m_matrix, m_target,m_color); }
	inline Matrix::Matrix< Row, Col, Cell> matrix() const { return m_matrix; }
private:
	size_t countCell(sf::Vector2u pos)
	{
		size_t rep = 0;
		for(int i = -1; i<=1; i++)
			for (int j = -1; j <= 1; j++)
			{
				if (pos.x + i > 0 && pos.x + i <= m_matrix.row() && pos.y + j > 0 && pos.y + j <= m_matrix.column())
				{
					if (m_matrix(pos.x + i, pos.y + j) == Cell::ALIVE)
						rep++;
				}
			}
		return rep;
	}
	Matrix::Matrix < Row, Col, Cell> m_matrix;
	sf::RenderTarget *m_target;
	sf::Color m_color;
	size_t lives;

};
