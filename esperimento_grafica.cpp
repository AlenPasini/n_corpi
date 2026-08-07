#include <SFML/Graphics.hpp>
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

#include "n_corpi.cpp"

int main() {
  Universe u{};

  double scale = 1.0e5;

  sf::RenderWindow window(sf::VideoMode(800, 600), "Universe");

  sf::View view(sf::Vector2f(0.f, 0.f), sf::Vector2f(800.f, 600.f));

  window.setView(view);

  std::vector<sf::CircleShape> bodies;

  std::ifstream file("configurations.txt");

  if (!file) {
    std::cerr << "Cannot open file" << '\n';
    return 1;
  }

  double r_x;
  double r_y;
  double v_x;
  double v_y;
  double m;

  while (file >> r_x >> r_y >> v_x >> v_y >> m) {
    Vector r{r_x, r_y};
    Vector v{v_x, v_y};
    Body b{r, v, m};

    u.add(b);

    sf::CircleShape circle(10.f);
    circle.setOrigin(10.f, 10.f);

    circle.setPosition(r_x/scale, r_y/scale);

    bodies.push_back(circle);
  }

  file.close();

  u.set_U_0();

  int n_step{1000};

  while (window.isOpen()) {
    // float dt = clock.restart().asSeconds();

    // EVENTS
    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window.close();
    }

    // UPDATE

    // DRAW
    window.clear();

    for (size_t i{0}; i < bodies.size(); ++i) {
      window.draw(bodies[i]);
    }

    window.display();
  }
}
