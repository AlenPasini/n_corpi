#include "n_bodies.hpp"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

int main() {
  unsigned int w{1600};
  unsigned int h{800};

  int iterations{0};

  sf::RenderWindow window(sf::VideoMode(w, h), "Universe");

  sf::View view(sf::Vector2f(0.f, 0.f),
                sf::Vector2f(static_cast<float>(w), static_cast<float>(h)));

  window.setView(view);

  std::vector<std::string> conf_titles{
      "Figure-8",           "Pulsating Hexagon",
      "Yarn Configuration", "Henon Configuration",
      "The Chase", "Earth & Moon",
      "Inner Solar System"};

  sf::Font times;

  times.loadFromFile("times.ttf");
  if (!times.loadFromFile("times.ttf")) {
    std::cerr << "Couldn't load font" << '\n';
  }

  nb::Universe u{1., 0.005, 5.e-3};

  nb::Body b0({-0.97000436, 0.24308753}, {0.4662036850, 0.4323657300}, 1.,
              0.05);
  u.add(b0);

  nb::Body b1({0.97000436, -0.24308753}, {0.4662036850, 0.4323657300}, 1.,
              0.05);
  u.add(b1);

  nb::Body b2({0., 0.}, {-0.93240737, -0.86473146}, 1., 0.05);
  u.add(b2);

  u.set_universe_0();
  u.set_a_0();

  auto disclamer = nb::radii_disclaimer(times);
  auto legend = nb::intial_legend_setting(w, h, times, u);
  auto legend_current = nb::current_legend_setting(w, h, times, u);
  auto buttons = nb::configuration_button_setting();
  auto conf_text = nb::configuration_text_setting(times, buttons, conf_titles);
  auto info = nb::u_informations(w, h, times, 0, u);

  bool running{false};
  std::size_t clicked{0};
  std::vector<double> pars;

  while (window.isOpen()) {
    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window.close();

      if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2f mouseWorld = window.mapPixelToCoords(
            sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

        if (event.mouseButton.button == sf::Mouse::Left) {
          for (std::size_t i{0}; i < 7; ++i) {
            if (buttons[i].getGlobalBounds().contains(mouseWorld) &&
                clicked != i) {
              clicked = i;
              pars = nb::run_conf(conf_titles[i]);

              u.new_config(pars[0], pars[1], pars[2]);

              auto n_bodies{(pars.size() - 3) / 6};

              for (std::size_t j{0}; j < n_bodies; ++j) {
                nb::Body b({pars[3 + 6 * j], pars[4 + 6 * j]},
                           {pars[5 + 6 * j], pars[6 + 6 * j]}, pars[7 + 6 * j],
                           pars[8 + 6 * j]);

                u.add(b);
              }

              u.set_universe_0();
              u.set_a_0();
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

            for (std::size_t i{0}; i < u.size(); ++i) {
              window.draw(u.get_circles()[i]);
            }
          }

          if (event.key.code == sf::Keyboard::Left) {
            u.single_simulation_step_back();
            iterations -= 1;

            for (std::size_t i{0}; i < u.size(); ++i) {
              window.draw(u.get_circles()[i]);
            }
          }
        }
        if (event.key.code == sf::Keyboard::R && event.key.alt) {
          pars = nb::run_conf(conf_titles[clicked]);

          u.new_config(pars[0], pars[1], pars[2]);

          auto n_bodies{(pars.size() - 3) / 6};

          for (std::size_t j{0}; j < n_bodies; ++j) {
            nb::Body b({pars[3 + 6 * j], pars[4 + 6 * j]},
                       {pars[5 + 6 * j], pars[6 + 6 * j]}, pars[7 + 6 * j],
                       pars[8 + 6 * j]);

            u.add(b);
          }

          u.set_universe_0();
          u.set_a_0();
          iterations = 0;

          legend_current = current_legend_setting(w, h, times, u);
        }
      }
    }

    window.clear();

    window.draw(nb::space_instructions(times));
    window.draw(nb::altR_instructions(times));

    info[0].setString("Iterations: " + std::to_string(iterations));
    info[4].setString("# bodies: " + std::to_string(u.size()));

    for (std::size_t i{0}; i < info.size(); ++i) {
      window.draw(info[i]);
    }

    if (running == false) {
      window.draw(nb::arrows_instructions(times));
    }

    for (std::size_t i{0}; i < legend.size(); ++i) {
      window.draw(legend[i]);
    }

    legend_current[1].setString("K = " + nb::scient(u.get_K_()) + " J");
    legend_current[2].setString("U = " + nb::scient(u.get_U_()) + " J");
    legend_current[3].setString("E = " + nb::scient(u.get_E_()) + " J");
    legend_current[4].setString("P = (" + nb::scient(u.get_P_0().get_x()) +
                                ", " + nb::scient(u.get_P_0().get_y()) +
                                " ) kg m s^-1");
    legend_current[5].setString("L = " + nb::scient(u.get_L_()) +
                                " kg m^2 s^-1");
    legend_current[6].setString("(E_0 - E) % = " + nb::scient(u.get_dE_()) +
                                " J");
    legend_current[7].setString("|P_0 - P| % = " + nb::scient(u.get_dP_()) +
                                " kg m s^-1");
    legend_current[8].setString("(L_0 - L) % = " + nb::scient(u.get_dL_()) +
                                " kg m^2 s^-1");

    for (std::size_t i{0}; i < legend_current.size(); ++i) {
      window.draw(legend_current[i]);
    }

    buttons[clicked].setFillColor(sf::Color::Yellow);
    conf_text[clicked].setFillColor(sf::Color::Black);

    for (std::size_t i{0}; i < 7; ++i) {
      if (clicked != i) {
        buttons[i].setFillColor(sf::Color::Blue);
        conf_text[i].setFillColor(sf::Color::White);
      }
    }

    for (std::size_t i{0}; i < buttons.size(); ++i) {
      window.draw(buttons[i]);
      window.draw(conf_text[i]);
    }

    if (clicked == 4) {
      for (std::size_t i{0}; i < disclamer.size(); ++i) {
        window.draw(disclamer[i]);
      }
    }

    if (running == true) {
      u.single_simulation_step();
      iterations += 1;
    }

    u.update_graphics();

    for (std::size_t i{0}; i < u.size(); ++i) {
      window.draw(u.get_circles()[i]);
    }

    window.display();
  }
}
