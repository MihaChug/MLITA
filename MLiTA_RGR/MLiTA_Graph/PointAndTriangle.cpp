#include "stdafx.h"
#include "PointAndTriangle.h"

using namespace std;
using namespace sf;

CPointAndTriangle::CPointAndTriangle(std::ifstream & fin)
	:circle(sf::CircleShape(2))
{
	ReadInputFile(fin);
	Font font;
	font.loadFromFile("res/ARCADECLASSIC.ttf");
	text = Text("", font, 35);
	text.setColor(Color::Yellow);
	text.setPosition(10, 400);
}

int CPointAndTriangle::Area(sf::Vector2i & a, sf::Vector2i & b, sf::Vector2i & c)
{
	return abs((b.y - a.y)*(c.x - a.x) - (b.x - a.x)*(c.y - a.y));
}

bool CPointAndTriangle::CheckPoint()
{
	int areaOfTreangle = Area(m_trianglePoints[0], m_trianglePoints[1], m_trianglePoints[2]);
	int	firstAreaWithPoint = Area(m_trianglePoints[0], m_trianglePoints[1], m_point);
	int	secondAreaWithPoint = Area(m_trianglePoints[1], m_trianglePoints[2], m_point);
	int	thirdAreaWithPoint = Area(m_trianglePoints[0], m_trianglePoints[2], m_point);

	if (firstAreaWithPoint + secondAreaWithPoint + thirdAreaWithPoint == areaOfTreangle)
		return true;
	else
		return false;

}

void CPointAndTriangle::Visualisation()
{
	circle.setFillColor(sf::Color::Black);
	circle.setPosition((float)m_point.x + 298, (float)m_point.y + 298);

	convex.setFillColor(sf::Color::Green);
	convex.setPointCount(3);

	convex.setPoint(0, sf::Vector2f((float)m_trianglePoints[0].x + 300, (float)m_trianglePoints[0].y + 300));
	convex.setPoint(1, sf::Vector2f((float)m_trianglePoints[1].x + 300, (float)m_trianglePoints[1].y + 300));
	convex.setPoint(2, sf::Vector2f((float)m_trianglePoints[2].x + 300, (float)m_trianglePoints[2].y + 300));


	if (CheckPoint())
	{
		text.setString("Point in triangle ");
	}
	else
	{
		text.setString("Point not in triangle");
	}
}

void CPointAndTriangle::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	sf::Vertex line1[] =
	{
		sf::Vertex(sf::Vector2f(300, 0),sf::Color::Black),
		sf::Vertex(sf::Vector2f(300, 600),sf::Color::Black)
	};
	sf::Vertex line2[] =
	{
		sf::Vertex(sf::Vector2f(0, 300),sf::Color::Black),
		sf::Vertex(sf::Vector2f(600, 300),sf::Color::Black)
	};
	render.draw(line1, 2, sf::Lines, states);
	render.draw(line2, 2, sf::Lines, states);
	render.draw(convex, states);
	render.draw(circle, states);
}

void CPointAndTriangle::ReadInputFile(std::ifstream & fin)
{
	int x, y;
	for (int i = 0; i < 3; i++)
	{
		fin >> x >> y;
		m_trianglePoints.push_back({ x, y });
	}
	fin >> x >> y;
	m_point = { x, y };

}