#include "Card.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#include <algorithm>
#include <array>
#include <filesystem>
#include <iostream>
#include <optional>
#include <random>
#include <string>
#include <vector>

namespace
{
    void centerText(sf::Text& text, sf::Vector2f center)
    {
        const sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(bounds.position + bounds.size / 2.f);
        text.setPosition(center);
    }
}

int main()
{
	// Locate the Windows font directory and load the Segoe UI font.
    std::array<wchar_t, MAX_PATH> windowsDirectory{};
    const UINT length = GetWindowsDirectoryW(windowsDirectory.data(), static_cast<UINT>(windowsDirectory.size()));
    if (length == 0 || length >= windowsDirectory.size())
    {
        std::cerr << "Could not locate the Windows font directory.\n";
        return 1;
    }

	// Load the Segoe UI font from the Windows font directory.
    sf::Font font;
    const auto fontPath = std::filesystem::path(windowsDirectory.data()) / L"Fonts" / L"segoeui.ttf";
    if (!font.openFromFile(fontPath))
    {
        std::cerr << "Could not load the Windows Segoe UI font.\n";
        return 1;
    }

	constexpr std::array<char, 4> symbols{ 'A', 'B', 'C', 'D' };
	constexpr std::size_t cardsPerRow = 4;
	constexpr int startingTries = 3;
	std::vector<Card> cards;
	std::random_device randomDevice;
	std::mt19937 randomEngine(randomDevice());
	int triesLeft = startingTries;
	std::size_t matchedPairs = 0;
	std::optional<std::size_t> firstCard;
	std::optional<std::size_t> secondCard;
	bool gameOver = false;
	bool won = false;
	sf::Clock mismatchClock;

	const auto resetGame = [&]()
	{
		cards.clear();
		cards.reserve(symbols.size() * 2);
		for (const char symbol : symbols)
		{
			cards.emplace_back(symbol);
			cards.emplace_back(symbol);
		}
		std::shuffle(cards.begin(), cards.end(), randomEngine);
		for (std::size_t i = 0; i < cards.size(); ++i)
		{
			cards[i].setPosition({ 60.f + 180.f * static_cast<float>(i % cardsPerRow),
								   80.f + 200.f * static_cast<float>(i / cardsPerRow) });
		}
		triesLeft = startingTries;
		matchedPairs = 0;
		firstCard.reset();
		secondCard.reset();
		gameOver = false;
		won = false;
	};
	resetGame();

	sf::RenderWindow window(sf::VideoMode({ 800, 500 }), "Matching Game",
							sf::Style::Titlebar | sf::Style::Close);
	window.setFramerateLimit(60);

	sf::RectangleShape overlay({ 800.f, 500.f });
	overlay.setFillColor(sf::Color(0, 0, 0, 180));
	sf::RectangleShape dialog({ 340.f, 200.f });
	dialog.setPosition({ 230.f, 150.f });
	dialog.setFillColor(sf::Color(50, 65, 95));
	sf::RectangleShape retryButton({ 160.f, 55.f });
	retryButton.setPosition({ 320.f, 255.f });
	retryButton.setFillColor(sf::Color(235, 239, 248));
	sf::Text retryText(font, "Retry", 28);
	retryText.setFillColor(sf::Color(35, 45, 70));
	centerText(retryText, { 400.f, 282.5f });

	while (window.isOpen())
	{
		while (const auto event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}
			else if (const auto* click = event->getIf<sf::Event::MouseButtonPressed>())
			{
				if (click->button != sf::Mouse::Button::Left)
				{
					continue;
				}
				const sf::Vector2f point = window.mapPixelToCoords(click->position);
				if (gameOver || won)
				{
					if (retryButton.getGlobalBounds().contains(point))
					{
						resetGame();
					}
				}
				else if (!secondCard)
				{
					for (std::size_t i = 0; i < cards.size(); ++i)
					{
						if (!cards[i].isFaceUp() && cards[i].contains(point))
						{
							cards[i].flip();
							if (!firstCard)
							{
								firstCard = i;
							}
							else if (cards[*firstCard].matches(cards[i]))
							{
								firstCard.reset();
								++matchedPairs;
								won = matchedPairs == symbols.size();
							}
							else
							{
								secondCard = i;
								--triesLeft;
								mismatchClock.restart();
							}
							break;
						}
					}
				}
			}
		}

		if (!window.isOpen())
		{
			break;
		}
		if (secondCard && mismatchClock.getElapsedTime() >= sf::seconds(1.f))
		{
			cards[*firstCard].flip();
			cards[*secondCard].flip();
			firstCard.reset();
			secondCard.reset();
			gameOver = triesLeft == 0;
		}

		window.clear(sf::Color(30, 40, 60));
		sf::Text triesText(font, "Tries Left: " + std::to_string(triesLeft), 28);
		triesText.setFillColor(sf::Color::White);
		triesText.setPosition({ 20.f, 15.f });
		window.draw(triesText);
		for (const Card& card : cards)
		{
			card.draw(window, font);
		}
		if (gameOver || won)
		{
			window.draw(overlay);
			window.draw(dialog);
			sf::Text message(font, gameOver ? "Game Over" : "You Win!", 36);
			message.setFillColor(sf::Color::White);
			centerText(message, { 400.f, 205.f });
			window.draw(message);
			window.draw(retryButton);
			window.draw(retryText);
		}
		window.display();
	}

	return 0;
}
