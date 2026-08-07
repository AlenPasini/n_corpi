#include <SFML/Graphics.hpp>
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

#include "n_corpi.cpp"

int main() {
  Universe u{};

  sf::RenderWindow window(sf::VideoMode(1600, 800), "Universe");

  sf::View view(sf::Vector2f(0.f, 0.f), sf::Vector2f(1600.f, 800.f));

  window.setView(view);

  sf::Clock clock;

  std::vector<sf::CircleShape> bodies;

  /*
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
  }

  file.close();
  */

  Body b0({0., 0.}, {0., 0.}, 1. * pow(10, 10));
  u.add(b0);

  Body b1({4 * pow(10, 7), 5.*pow(10,6)}, {-1.*pow(10,6), 0.}, 1. * pow(10, 14));
  u.add(b1);

  Body b2({-4 * pow(10, 7), 0.}, {1.*pow(10,6), 0.}, 1. * pow(10, 10));
  u.add(b2);

  u.set_U_0();

  // int n_step{1000};

  while (window.isOpen()) {
    float dt = clock.restart().asSeconds();

    // EVENTS
    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window.close();
    }

    // UPDATE

    u.simulation_graphics(dt);

    // DRAW
    window.clear();

    for (int i{0}; i < u.size(); ++i) {
      window.draw(u.get_circles()[i]);
    }

    window.display();
  }
}
