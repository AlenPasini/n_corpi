#include <SFML/Graphics.hpp>
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

#include "n_corpi.cpp"

int main() {
  Universe u{};

  double l{1600.};
  double h{800.};

  sf::RenderWindow window(sf::VideoMode(l, h), "Universe");

  sf::View view(sf::Vector2f(0.f, 0.f), sf::Vector2f(l, h));

  window.setView(view);

  std::vector<sf::CircleShape> bodies;

  sf::Font times;
  times.loadFromFile("times.ttf");
  sf::Text text;

  bool running{false};

  Body b0({-0.97000436, 0.24308753}, {0.4662036850, 0.4323657300}, 1., 1.);
  u.add(b0);

  Body b1({0.97000436, -0.24308753}, {0.4662036850, 0.4323657300}, 1., 1.);
  u.add(b1);

  Body b2({0., 0.}, {-0.93240737, -0.86473146}, 1., 1.);
  u.add(b2);

  u.set_U_0();
  u.set_a_0();

  while (window.isOpen()) {
    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window.close();

      if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Space) {
          running = !running;
        }
      }
    }

    window.clear();

    text.setFont(times);
    text.setString("Press SPACE to start / stop the simulation");
    text.setPosition(0, -200.);
    window.draw(text);

    if (running) {
      u.single_simulation_step();
    }

    u.update_graphics();

    for (int i{0}; i < u.size(); ++i) {
      window.draw(u.get_circles()[i]);
    }

    window.display();
  }
}
