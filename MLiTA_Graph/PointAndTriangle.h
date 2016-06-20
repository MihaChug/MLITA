#pragma once
#include "stdafx.h"

class CPointAndTriangle : public sf::Drawable
{
public:
	CPointAndTriangle(std::ifstream & fin);
	bool CheckPoint();
	void Visualisation();
	void draw(sf::RenderTarget & render, sf::RenderStates states)const override;
private:
	void ReadInputFile(std::ifstream & fin);
	sf::Vector2i m_point;
	std::vector<sf::Vector2i> m_trianglePoints;
	int Area(sf::Vector2i & a, sf::Vector2i & b, sf::Vector2i & c);
	sf::ConvexShape convex;
	sf::CircleShape circle;
	sf::Text text;
};
