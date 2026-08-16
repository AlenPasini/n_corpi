#include <SFML/Graphics.hpp>
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

#include "n_corpi.cpp"
// #include "graphic_setting.cpp"

int main() {
  // Costruzione del canvas e posizionamento di (0,0) nel centro del canvas
  double w{1600.};
  double h{800.};

  int iterations{0};

  sf::RenderWindow window(sf::VideoMode(w, h), "Universe");

  sf::View view(sf::Vector2f(0.f, 0.f), sf::Vector2f(w, h));

  window.setView(view);

  // Caricamento del font da file
  sf::Font times;

  times.loadFromFile("times.ttf");
  if (!times.loadFromFile("times.ttf")) {
    std::cerr << "Couldn't load font" << '\n';
  }

  // Caricamento dei corpi nell'Universo e setting dell'Universo
  Universe u{1., 0.005, 1e-2};

  Body sole(    {0.0, 0.0},   {0.0, 0.0},    100.0,   0.15); // Sole (al centro)
Body mercurio({1.5, 0.0},   {0.0, 8.16},   0.0001,  0.03); // Mercurio
Body venere(  {3.0, 0.0},   {0.0, 5.77},   0.005,   0.05); // Venere
Body terra(   {5.0, 0.0},   {0.0, 4.47},   0.006,   0.06); // Terra
Body marte(   {8.0, 0.0},   {0.0, 3.53},   0.0006,  0.04); // Marte

  
  u.add(sole);     // ID 0
  u.add(mercurio); // ID 1
  u.add(venere);   // ID 2
  u.add(terra);    // ID 3
  u.add(marte);    // ID 4
/*
  Body b0({-9.1, 0.0}, {0.0, -0.0095}, 10.0,   0.2); // Stella
Body b1({90.9, 0.0},  {0.0,  0.0954}, 1.0,    0.2); // Pianeta (Lontanissimo a x = 90.9!)
Body b2({58.7, 0.0},  {0.0,  0.0616}, 0.0001, 0.2);     // Satellite L1 (Raggio 0.1, è a x = 25.0)

  u.add(b0);
  u.add(b1); 
  u.add(b2); 

  
  Body b0({-1., -1.}, {0., 0.}, 1., 0.1);
  u.add(b0);

  Body b1({1., 1.}, {0., 0.}, 1., 0.1);
  u.add(b1);
  

  */
 /*
  Body b0({-0.97000436, 0.24308753}, {0.4662036850, 0.4323657300}, 1., 1.);
  u.add(b0);

  Body b1({0.97000436, -0.24308753}, {0.4662036850, 0.4323657300}, 1., 1.);
  u.add(b1);

  Body b2({0., 0.}, {-0.93240737, -0.86473146}, 1., 1.);
  u.add(b2);
  */

  /*Body b0({-1., -1.}, {0., 0.}, 1., 0.1);
  u.add(b0);

  Body b1({1., 1.}, {0., 0.}, 1., 0.1);
  u.add(b1);

  Body b2({0., 2.}, {1., 0.}, 0.2, 0.1);
  u.add(b2);
  */

  u.set_U_0();
  u.set_a_0();
  u.set_energies();

  auto legend = intial_legend_setting(w, h, times, u);
  auto legend_current = current_legend_setting(w, h, times, u);

  // Apertura del canvas e setting per l'utilizzo dei tasti
  bool running{false};

  while (window.isOpen()) {
    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window.close();

      if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Space) {
          if (running == false) {
            running = true;
          }

          else {
            running = false;
          }
        }

        if (running == false) {
          if (event.key.code == sf::Keyboard::Right) {
            u.single_simulation_step();
            iterations += 1;

            for (int i{0}; i < u.size(); ++i) {
              window.draw(u.get_circles()[i]);
            }
          }

          if (event.key.code == sf::Keyboard::Left) {
            u.single_simulation_step_back();
            iterations -= 1;

            for (int i{0}; i < u.size(); ++i) {
              window.draw(u.get_circles()[i]);
            }
          }
        }
        if (event.key.code == sf::Keyboard::E && event.key.control) {
          u.reset(iterations);
          u.update_graphics();
          iterations = 0;
        }
      }
    }

    window.clear();

    window.draw(space_instructions(times));
    window.draw(ctrlE_instructions(times));
    window.draw(iterations_title(w, h, times, iterations));

    if (running == false) {
      window.draw(arrows_instructions(times));
    }

    for (size_t i{0}; i < legend.size(); ++i) {
      window.draw(legend[i]);
    }

    legend_current[1].setString("K = " + std::to_string(u.get_K_()) + "J");
    legend_current[2].setString("U = " + std::to_string(u.get_U_()) + "J");
    legend_current[3].setString("E = " + std::to_string(u.get_E_()) + "J");
    legend_current[4].setString(
        "E_0 - E = " + std::to_string(u.get_E_0() - u.get_E_()) + "J");

    for (size_t i{0}; i < legend_current.size(); ++i) {
      window.draw(legend_current[i]);
    }

    // Calcolo di una simulazione, aggiornamento grafica e disegno corpi
    if (running == true) {
      u.single_simulation_step();
      iterations += 1;
    }

    u.update_graphics();

    for (int i{0}; i < u.size(); ++i) {
      window.draw(u.get_circles()[i]);
    }

    window.display();
  }
}
