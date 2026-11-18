#include "Card.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#include <algorithm>
#include <array>
#include <filesystem>
#include <iostream>
#include <random>
#include <vector>

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

	// Create a vector of 8 cards, two copies of each of the 4 symbols.
    constexpr std::array<char, 4> symbols{ 'A', 'B', 'C', 'D' };
    std::vector<Card> cards;
    cards.reserve(symbols.size() * 2);
    for (const char symbol : symbols)
    {
        cards.emplace_back(symbol);
        cards.emplace_back(symbol);
    }

	// Shuffle the cards and position them in a 4x2 grid.
    std::random_device randomDevice;
    std::mt19937 randomEngine(randomDevice());
    std::shuffle(cards.begin(), cards.end(), randomEngine);

	// Position the cards in a 4x2 grid with margins and spacing.
    constexpr std::size_t cardsPerRow = 4;
    constexpr float margin = 40.f;
    constexpr float spacing = 180.f;
    for (std::size_t i = 0; i < cards.size(); ++i)
    {
        cards[i].setPosition({ margin + spacing * static_cast<float>(i % cardsPerRow),
                               margin + spacing * static_cast<float>(i / cardsPerRow) });
    }

	// Create the SFML window and run the main event loop.
    sf::RenderWindow window(sf::VideoMode({ 780, 420 }), "Matching Game",
                            sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);
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
                if (click->button == sf::Mouse::Button::Left)
                {
                    const sf::Vector2f point = window.mapPixelToCoords(click->position);
                    for (Card& card : cards)
                    {
                        if (card.contains(point))
                        {
                            card.flip();
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
        window.clear(sf::Color(30, 40, 60));
        for (const Card& card : cards)
        {
            card.draw(window, font);
        }
        window.display();
    }

    return 0;
}
