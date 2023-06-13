#include <iostream>
#include <cmath>

#include <SFML/Graphics.hpp>
#include <SFML/system.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

class Particle : public sf::CircleShape
{
public:
	Particle(float radius)
	{
		//Szczerze, nie wiem po co nam ten konstruktor, skoro mamy ten poniżej :)
		this->setRadius(radius);
		this->setOrigin(radius, radius);
		float x = std::rand() % 600;
		float y = std::rand() % 600;
		sf::Vector2f vector(x, y);
		this->setPosition(vector);
		this->setFillColor(sf::Color::White);
		sf::Vector2f vec(this->speed, this->speed);
		this->set_speed(vec);
	}

	Particle(float radius, const sf::Vector2f& possition)
	{
		this->setRadius(radius);
		this->setOrigin(radius, radius);
		this->setPosition(possition);
		this->setFillColor(sf::Color::White);
		sf::Vector2f vec(this->speed, this->speed);
		this->set_speed(vec);
	}

	sf::Vector2f get_speed() { return this->velocity; }
	sf::Vector2f set_speed(sf::Vector2f& vec) { this->velocity.x = vec.x; this->velocity.y = vec.y; return vec; }

private:
	sf::Vector2f velocity; // dodałem od siebie, na pierwszy rzut mózgiem nie przyszedł mi lepszy pomysł jak inaczej to rozwiązać
	float speed = 0.1;
};

class PhysicsHandler
{
public:
	void handle_Collision(Particle& p1, Particle& p2) 
	{
		sf::Vector2f diff = p1.getPosition() - p2.getPosition();
		float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
		if (distance <= p1.getRadius() + p2.getRadius())
		{
			sf::Vector2f vec1 = p1.get_speed();
			sf::Vector2f vec2 = p2.get_speed();
			p1.set_speed(vec2);
			p2.set_speed(vec1);
		}
	}

	void handle_Window_Bounds_Collision(Particle& p)
	{
		if (p.getPosition().x > 600 || p.getPosition().x < 0)
		{
			sf::Vector2f arg;
			arg.x = -p.get_speed().x;
			p.set_speed(arg);
		}
		else if (p.getPosition().y > 600 || p.getPosition().y < 0)
		{
			sf::Vector2f arg;
			arg.y = -1.f * p.get_speed().y;
			p.set_speed(arg);
		}
	}
};

class Simulation
{
public:

	Simulation(int n, float radius)
	{
		this->window.create(sf::VideoMode(600, 600), "Simulation");
		
		for (int i = 0; i < n; ++i)
		{
			float x = std::rand() % 600;
			float y = std::rand() % 600;
			sf::Vector2f vector(x, y);
			Particle p(radius, vector);
			this->particles.push_back(p);
		}
	}

	void run()
	{
		while (this->window.isOpen())
		{
			sf::Event event;
			while (this->window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					this->window.close();
				}
			}

			for (size_t i = 0; i < this->particles.size(); ++i)
			{
				for (size_t j = 1; j < this->particles.size(); ++j)
				{
					this->physics.handle_Collision(this->particles[i], this->particles[j]);
				}
				this->physics.handle_Window_Bounds_Collision(this->particles[i]);
				this->particles[i].move(this->particles[i].get_speed());
				this->window.draw(this->particles[i]);
			}
			this->window.display();
			this->window.clear();
		}
	}

private:
	PhysicsHandler physics;
	std::vector<Particle> particles;
	sf::RenderWindow window;
};

int main()
{
	Simulation s(10, 20.f);
	s.run();
	return 0;
}