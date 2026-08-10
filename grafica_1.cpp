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

  std::vector<sf::CircleShape> bodies;

  Body b0({-0.97000436, 0.24308753}, {0.4662036850, 0.4323657300}, 1., 1.);
  u.add(b0);

  Body b1({0.97000436, -0.24308753}, {0.4662036850, 0.4323657300}, 1., 1.);
  u.add(b1);

  Body b2({0., 0.}, {-0.93240737, -0.86473146}, 1., 1.);
  u.add(b2);

  u.set_U_0();
  u.set_a_0();

  while (window.isOpen()) {

    u.single_simulation_step();

    u.update_graphics();

    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window.close();
    }

    window.clear();

    for (int i{0}; i < u.size(); ++i) {
      window.draw(u.get_circles()[i]);
    }

    window.display();
  }
}
