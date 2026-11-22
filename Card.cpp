#include "Card.h"

#include <algorithm>
#include <cmath>
#include <numbers>
#include <string>

namespace
{
	constexpr float cardWidth = 140.f;
	constexpr float cardHeight = 180.f;
	constexpr float cornerRadius = 16.f;
	constexpr std::size_t pointsPerCorner = 9;
}

Card::Card(char symbol)
	: symbol_(symbol)
{
	shape_.setPointCount(4 * pointsPerCorner);
	const sf::Vector2f centers[] = {
		{ cardWidth - cornerRadius, cornerRadius },
		{ cardWidth - cornerRadius, cardHeight - cornerRadius },
		{ cornerRadius, cardHeight - cornerRadius },
		{ cornerRadius, cornerRadius }
	};
	for (std::size_t corner = 0; corner < 4; ++corner)
	{
		for (std::size_t point = 0; point < pointsPerCorner; ++point)
		{
			const float angle = (-0.5f + static_cast<float>(corner) * 0.5f +
				0.5f * static_cast<float>(point) / static_cast<float>(pointsPerCorner - 1)) * std::numbers::pi_v<float>;
			shape_.setPoint(corner * pointsPerCorner + point,
				centers[corner] + sf::Vector2f(std::cos(angle), std::sin(angle)) * cornerRadius);
		}
	}
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

bool Card::matches(const Card& other) const noexcept
{
	return symbol_ == other.symbol_;
}

void Card::setPosition(sf::Vector2f position)
{
	shape_.setPosition(position);
}

bool Card::contains(sf::Vector2f point) const
{
	const sf::Vector2f local = point - shape_.getPosition();
	if (local.x < 0.f || local.x >= cardWidth || local.y < 0.f || local.y >= cardHeight)
	{
		return false;
	}
	const float nearestX = std::clamp(local.x, cornerRadius, cardWidth - cornerRadius);
	const float nearestY = std::clamp(local.y, cornerRadius, cardHeight - cornerRadius);
	const float dx = local.x - nearestX;
	const float dy = local.y - nearestY;
	return dx * dx + dy * dy <= cornerRadius * cornerRadius;
}

void Card::draw(sf::RenderTarget& target, const sf::Font& font) const
{
	target.draw(shape_);

	sf::Text label(font, std::string(1, faceUp_ ? symbol_ : '?'), 72);
	label.setFillColor(sf::Color(35, 45, 70));
	const sf::FloatRect textBounds = label.getLocalBounds();
	label.setOrigin(textBounds.position + textBounds.size / 2.f);
	label.setPosition(shape_.getPosition() + sf::Vector2f(cardWidth / 2.f, cardHeight / 2.f));
	target.draw(label);
}
