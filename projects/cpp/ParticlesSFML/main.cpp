#include <SFML/Graphics.hpp>

#include "particleSystem.h"

int WinMain()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Particles", sf::Style::None);
	ParticleSystem particles(10000);
	sf::Clock clock;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::KeyPressed)
				if (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::Q)
					window.close();
		}
		sf::Vector2i mouse = sf::Mouse::getPosition(window);
		particles.setEmitter(window.mapPixelToCoords(mouse));
		sf::Time elapsed = clock.restart();
		particles.update(elapsed);
		window.clear(sf::Color::Transparent);
		window.draw(particles);
		window.display();
	}
	return 0;
}