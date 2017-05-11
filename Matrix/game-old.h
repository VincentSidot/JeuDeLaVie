#pragma once
#include "matrix.h"
#include <vector>
#include <initializer_list>

typedef struct Adj
{
	size_t count;
	sf::Color color;
}ADJ,*PADJ;

inline sf::Color getMostUsed(std::vector<ADJ> const &v)
{
	int max = 0;
	for (auto &color : v)
	{
		if(color.count > max) max = color.count;
	}
	std::vector<size_t> index;
	for (int i = 0; i < v.size(); i++)
	{
		if (v[i].count == max)
			index.push_back(i);
	}
	if (index.size() == 0)
		return v[rand() % v.size()].color;
	return v[index[rand() % index.size()] % v.size()].color;
}


inline sf::Color randomColor() { return sf::Color(rand() % 255, rand() % 255, rand() % 255, 255); }
class Cell
{
public:
	Cell() : m_isAlive(false)
	{}
	inline sf::Color color() const { return m_color; }
	inline bool isAlive() const { return m_isAlive; }
	inline void setColor(const sf::Color &color) { m_color = color; return; }
	inline void setAlive(const bool &isAlive) { m_isAlive = isAlive; return; }

protected:
	sf::Color m_color;
	bool m_isAlive;

};

inline void dispMatrix(Matrix::Matrix<Row, Col, Cell> const &matrix, sf::RenderTarget *screen)
{
	sf::Vector2u screen_size = screen->getSize();
	for (int i = 1; i <= matrix.row(); i++)
		for (int j = 1; j <= matrix.column(); j++)
		{
			if (matrix(i, j).isAlive())
			{
				sf::VertexArray vertex(sf::PrimitiveType::Quads, 4);
				sf::Vector2f position;
				//First point
				position.x = (i - 1)*(screen_size.x / matrix.row());
				position.y = (j - 1)*(screen_size.y / matrix.column());
				vertex[0] = sf::Vertex(position, matrix(i, j).color());
				//Second point
				position.x = (i - 1)*(screen_size.x / matrix.row());
				position.y = j*(screen_size.y / matrix.column());
				vertex[1] = sf::Vertex(position, matrix(i, j).color());
				//Third point
				position.x = i*(screen_size.x / matrix.row());
				position.y = j*(screen_size.y / matrix.column());
				vertex[2] = sf::Vertex(position, matrix(i, j).color());
				//Last point
				position.x = i*(screen_size.x / matrix.row());
				position.y = (j - 1)*(screen_size.y / matrix.column());
				vertex[3] = sf::Vertex(position, matrix(i, j).color());
				//
				screen->draw(vertex);
			}
		}
	return;
}

class Game
{
public:
	Game(sf::RenderTarget *target) : m_matrix(Cell())
	{
		m_target = target;
		lives = 0;
	}
	inline void start(const std::initializer_list<Cell> &liste)
	{
		m_matrix.fill(liste);
		lives = 0;
		nextStep();
		return;
	}
	inline void start(const Matrix::Matrix<Row,Col,Cell> const &m)
	{
		m_matrix = m;
		lives = 0;
		nextStep();
		return;
	}
	inline void nextStep()
	{
		lives++;
		for (int i = 1; i <= m_matrix.row(); i++)
		{
			for (int j = 1; j <= m_matrix.column(); j++)
			{
				ADJ adj = countCell(sf::Vector2u(i, j));
				if (m_matrix(i, j).isAlive())
				{
					if (adj.count == 2 || adj.count == 3)
					{
						m_matrix(i, j).setAlive(true);
					}
					else
					{
						m_matrix(i, j).setAlive(false);
					}
				}
				else
				{
					if (adj.count == 3)
					{
						m_matrix(i, j).setAlive(true);
						m_matrix(i, j).setColor(adj.color);
					}
					else
					{
						m_matrix(i, j).setAlive(false);
					}
				}
			}
		}
	}
	inline size_t getLives() const { return lives; }
	inline void display() const { dispMatrix(m_matrix, m_target); }
	inline Matrix::Matrix< Row, Col, Cell> matrix() const { return m_matrix; }
private:
	inline ADJ countCell(sf::Vector2u pos)
	{
		size_t rep = 0;
		std::vector<ADJ> color;
		for(int i = -1; i<=1; i++)
			for (int j = -1; j <= 1; j++)
			{
				if (pos.x + i > 0 && pos.x + i <= m_matrix.row() && pos.y + j > 0 && pos.y + j <= m_matrix.column())
				{
					if (m_matrix(pos.x + i, pos.y + j).isAlive())
					{
						rep++;
						if(color.size() == 0)
							color.push_back({ 1,m_matrix(pos.x + i, pos.y + j).color() });
						else
						{
							bool test = false;
							for (auto &c : color)
							{
								if (c.color == m_matrix(pos.x + i, pos.y + j).color())
								{
									test = true;
									c.count++;
									break;
								}
							}
							if (!test)
							{
								color.push_back({ 1,m_matrix(pos.x + i, pos.y + j).color() });
							}
						}
					}
				}
			}
		if (rep > 0)
			return{ rep,getMostUsed(color) };
		else
			return{ 0,sf::Color::Black };
	}
	Matrix::Matrix < Row, Col, Cell> m_matrix;
	sf::RenderTarget *m_target;
	size_t lives;

};
