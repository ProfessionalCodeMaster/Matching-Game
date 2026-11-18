#include "Card.h"

#include <string>

Card::Card(char symbol)
	: symbol_(symbol)
{
	shape_.setFillColor(sf::Color(235, 239, 248));
	shape_.setOutlineColor(sf::Color(60, 75, 110));
	shape_.setOutlineThickness(-3.f);
}

void Card::flip() noexcept
{
	faceUp_ = !faceUp_;
}

bool Card::isFaceUp() const noexcept
{
	return faceUp_;
}

void Card::setPosition(sf::Vector2f position)
{
	shape_.setPosition(position);
}

bool Card::contains(sf::Vector2f point) const
{
	return shape_.getGlobalBounds().contains(point);
}

void Card::draw(sf::RenderTarget& target, const sf::Font& font) const
{
	target.draw(shape_);

	sf::Text label(font, std::string(1, faceUp_ ? symbol_ : '?'), 72);
	label.setFillColor(sf::Color(35, 45, 70));
	const sf::FloatRect textBounds = label.getLocalBounds();
	label.setOrigin(textBounds.position + textBounds.size / 2.f);
	label.setPosition(shape_.getPosition() + shape_.getSize() / 2.f);
	target.draw(label);
}
