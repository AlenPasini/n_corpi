#include <SFML/Graphics.hpp>
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

#include "n_corpi.cpp"

int main() {

  // Costruzione del canvas e posizionamento di (0,0) nel centro del canvas
  double l{1600.};
  double h{800.};

  sf::RenderWindow window(sf::VideoMode(l, h), "Universe");

  sf::View view(sf::Vector2f(0.f, 0.f), sf::Vector2f(l, h));

  window.setView(view);

  
  // Caricamento del font da file
  sf::Font times;

  times.loadFromFile("times.ttf");
  if (!times.loadFromFile("times.ttf")) {
    std::cerr << "Couldn't load font" << '\n';
  }

  // Caricamento dei corpi nell'Universo e setting dell'Universo
  Universe u{};

  Body b0({-0.97000436, 0.24308753}, {0.4662036850, 0.4323657300}, 1., 1.);
  u.add(b0);

  Body b1({0.97000436, -0.24308753}, {0.4662036850, 0.4323657300}, 1., 1.);
  u.add(b1);

  Body b2({0., 0.}, {-0.93240737, -0.86473146}, 1., 1.);
  u.add(b2);

  u.set_U_0();
  u.set_a_0();

  // Apertura del canvas e setting per l'utilizzo dei tasti
  bool running{false};

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

    // Creazione "istruzioni" (e rettangolo che lo incapsula, posizionato al centro)
    sf::Text instructions {"Press SPACE to start / stop the simulation", times};
    sf::FloatRect bounds = instructions.getLocalBounds();
    instructions.setOrigin((bounds.left + bounds.width) / 2.,
                   (bounds.top + bounds.height) / 2.);
    instructions.setPosition(0, -340.);
    window.draw(instructions);

    // Creazione legenda dati energia
    sf::Text legend_0_title {"Intial energetic conditions", times, 20};
    legend_0_title.setPosition(-l/2 + 10., -h/2 + 5.);
    window.draw(legend_0_title);

    sf::Text K_0 {"K = ", times, 16};
    K_0.setPosition(-l/2 + 10., -h/2 + 30.);
    window.draw(K_0);

    sf::Text U_0 {"U = ", times, 16};
    U_0.setPosition(-l/2 + 10., -h/2 + 50.);
    window.draw(U_0);

    sf::Text E_0 {"E = ", times, 16};
    E_0.setPosition(-l/2 + 10., -h/2 + 70.);
    window.draw(E_0);
    

    // Calcolo di una simulazione, aggiornamento grafica e disegno corpi
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
