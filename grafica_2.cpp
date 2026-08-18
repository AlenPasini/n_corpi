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

  Universe u{1., 0.005, 5.e-3};

  Body b0({-0.97000436, 0.24308753}, {0.4662036850, 0.4323657300}, 1., 0.05);
  u.add(b0);

  Body b1({0.97000436, -0.24308753}, {0.4662036850, 0.4323657300}, 1., 0.05);
  u.add(b1);

  Body b2({0., 0.}, {-0.93240737, -0.86473146}, 1., 0.05);
  u.add(b2);

  u.set_U_0();
  u.set_a_0();
  u.set_energies();

  auto legend = intial_legend_setting(w, h, times, u);
  auto legend_current = current_legend_setting(w, h, times, u);
  auto buttons = configuration_button_setting();
  auto conf_text = configuration_text_setting(times, buttons);
  auto info = u_informations(w, h, times, 0, u);

  // Apertura del canvas e setting per l'utilizzo dei tasti
  bool running{false};
  int clicked{0};
  std::vector<double> pars;

  std::vector<std::string> conf_titles{
      "First configuration",  "Second configuration", "Third configuration",
      "Fourth configuration", "Fifth configuration",  "Sixth configuration",
      "Seventh configuration"};

  while (window.isOpen()) {
    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window.close();

      if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2f mouseWorld = window.mapPixelToCoords(
            sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

        if (event.mouseButton.button == sf::Mouse::Left) {
          for (int i{0}; i < 7; ++i) {
            if (buttons[i].getGlobalBounds().contains(mouseWorld) &&
                clicked != i) {
              clicked = i;
              pars = run_conf(conf_titles[i]);

              u.new_config(pars[0], pars[1], pars[2]);

              std::size_t n_bodies{(pars.size() - 3) / 6};

              for (std::size_t j{0}; j < n_bodies; ++j) {
                Body b({pars[3 + 6 * j], pars[4 + 6 * j]},
                       {pars[5 + 6 * j], pars[6 + 6 * j]}, pars[7 + 6 * j],
                       pars[8 + 6 * j]);

                u.add(b);
              }

              u.set_U_0();
              u.set_a_0();
              u.set_energies();
              iterations = 0;

              legend = intial_legend_setting(w, h, times, u);
              legend_current = current_legend_setting(w, h, times, u);
            }
          }
        }
      }

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
          pars = run_conf(conf_titles[clicked]);

          u.new_config(pars[0], pars[1], pars[2]);

          std::size_t n_bodies{(pars.size() - 3) / 6};

          for (std::size_t j{0}; j < n_bodies; ++j) {
            Body b({pars[3 + 6 * j], pars[4 + 6 * j]},
                   {pars[5 + 6 * j], pars[6 + 6 * j]}, pars[7 + 6 * j],
                   pars[8 + 6 * j]);

            u.add(b);
          }

          u.set_U_0();
          u.set_a_0();
          u.set_energies();
          iterations = 0;

          legend_current = current_legend_setting(w, h, times, u);
        }
      }
    }

    window.clear();

    window.draw(space_instructions(times));
    window.draw(ctrlE_instructions(times));

    info[0].setString("Iterations: " + std::to_string(iterations));
    info[4].setString("# bodies: " + std::to_string(u.size()));
    for (size_t i{0}; i < info.size(); ++i) {
      window.draw(info[i]);
    }

    if (running == false) {
      window.draw(arrows_instructions(times));
    }

    for (size_t i{0}; i < legend.size(); ++i) {
      window.draw(legend[i]);
    }

    legend_current[1].setString("K = " + std::to_string(u.get_K_()) + "J");
    legend_current[2].setString("U = " + std::to_string(u.get_U_()) + "J");
    legend_current[3].setString("E = " + std::to_string(u.get_E_()) + "J");
    legend_current[4].setString("P = " + std::to_string(u.get_P_()) + "J");
    legend_current[5].setString("L = " + std::to_string(u.get_L_()) + "J");
    legend_current[6].setString(
        "E_0 - E = " + std::to_string(u.get_E_0() - u.get_E_()) + "J");
    legend_current[7].setString(
        "P_0 - P = " + std::to_string(u.get_P_0() - u.get_P_()) + "J");
    legend_current[8].setString(
        "L_0 - L = " + std::to_string(u.get_L_0() - u.get_L_()) + "J");

    for (size_t i{0}; i < legend_current.size(); ++i) {
      window.draw(legend_current[i]);
    }

    buttons[clicked].setFillColor(sf::Color::Yellow);
    conf_text[clicked].setFillColor(sf::Color::Black);

    for (int i{0}; i < 7; ++i) {
      if (clicked != i) {
        buttons[i].setFillColor(sf::Color::Blue);
        conf_text[i].setFillColor(sf::Color::White);
      }
    }

    for (size_t i{0}; i < buttons.size(); ++i) {
      window.draw(buttons[i]);
      window.draw(conf_text[i]);
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
