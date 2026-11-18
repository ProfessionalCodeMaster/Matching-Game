#pragma once

#include <SFML/Graphics.hpp>

class Card
{
public:
	explicit Card(char symbol);

	void flip() noexcept;
	bool isFaceUp() const noexcept;
	void setPosition(sf::Vector2f position);
	bool contains(sf::Vector2f point) const;
	void draw(sf::RenderTarget& target, const sf::Font& font) const;

private:
	char symbol_;
	bool faceUp_ = false;
	sf::RectangleShape shape_{{160.f, 160.f}};
};
