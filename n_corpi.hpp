#include <SFML/Graphics.hpp>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>


namespace nc
{
  struct Vector {
  double x;
  double y;

  Vector &operator+=(Vector const &v) {
    x += v.x;
    y += v.y;
    return *this;
  }

  Vector &operator-=(Vector const &v) {
    x -= v.x;
    y -= v.y;
    return *this;
  }

  double get_x() const { return x; }
  double get_y() const { return y; }

  double norm() const { return sqrt(x * x + y * y); }
};

Vector operator+(Vector const &v1, Vector const &v2);

Vector operator-(Vector const &v1, Vector const &v2);

bool operator==(Vector const &v1, Vector const &v2);

class Body {
 private:
  Vector r_;
  Vector v_;
  Vector a_{0., 0.};
  Vector a_fut_{0., 0.};
  double m_;
  double k_;
  int id_{-1};
  double dt{0.005};

 public:
  Body(Vector r, Vector v, double m, double k) : r_{r}, v_{v}, m_{m}, k_{k} {}
  Vector get_r() const;
  double get_k() const;
  Vector get_v() const;
  Vector get_a() const;
  Vector get_a_fut() const;
  double get_m() const;
  int get_id() const;
  // deve essere const perché altrimenti non me la fa paragonare
  // nell'operatore ==

  void r_t();

  void v_t();

  void add_id(int id);

  void a_t(double a_t_x, double a_t_y);

  void set_a_(Vector a_t_fut);

  void set_dt(double new_dt);
};

bool operator==(Body const &b1, Body const &b2);

class Universe {
 private:
  std::vector<Body> u_{};
  std::vector<sf::CircleShape> circles_{};
  std::vector<sf::Color> colors_{sf::Color::Red,     sf::Color::Green,
                                 sf::Color::Blue,    sf::Color::Yellow,
                                 sf::Color::Magenta, sf::Color::Cyan};

  double K_0;
  double U_0;
  double E_0;
  Vector P_0;
  double L_0;

  double K_;
  double U_;
  double E_;
  Vector P_;
  double L_;

  double G;
  double dt;
  // double eps{pow(10., -12.)};
  // double eps{0.};
  double scale_;

 public:
  Universe(double G, double dt, double scale) : G{G}, dt{dt}, scale_{scale} {}

  int size() const;

  void set_dt(double dt_input);

  void set_G(double new_G);

  void set_scale(double new_scale);

  void add(Body &b);

  std::vector<sf::CircleShape> &get_circles();

  Body get_body(int id) const;

  // double get_eps() const { return eps; }
  double get_G() const;
  double get_dt() const;
  double get_scale() const;

  double get_K_0() const;
  double get_U_0() const;
  double get_E_0() const;
  Vector get_P_0() const;
  double get_L_0() const;

  double get_K_() const;
  double get_U_() const;
  double get_E_() const;
  Vector get_P_() const;
  double get_L_() const;

  int n_colors() const;

  void u_a_t(Body &b);

  void r_t_complete();

  void u_a_t_complete();

  void u_a_v_complete();

  double K_t();

  double U_t();

  Vector P_t();

  double L_t();

  void check_collisions();

  void set_universe_0();

  void set_energies();

  void update_variables();

  void set_a_0();

  void simulation_steps(int steps);

  void single_simulation_step();

  void single_simulation_step_back();

  void new_config(double new_G, double new_dt, double new_scale);

  void update_graphics();
};

sf::Text space_instructions(sf::Font const &times) {
  sf::Text instructions{"Press SPACE to start / stop the simulation", times};
  sf::FloatRect bounds = instructions.getLocalBounds();
  instructions.setOrigin((bounds.left + bounds.width) / 2.,
                         (bounds.top + bounds.height) / 2.);
  instructions.setPosition(0, -340.);

  return instructions;
}

sf::Text ctrlE_instructions(sf::Font const &times) {
  sf::Text instructions{
      "Press ctrl + E to reset the simulation to its starting configuration",
      times, 20};
  sf::FloatRect bounds = instructions.getLocalBounds();
  instructions.setOrigin((bounds.left + bounds.width) / 2.,
                         (bounds.top + bounds.height) / 2.);
  instructions.setPosition(0, 340.);

  return instructions;
}

sf::Text arrows_instructions(sf::Font const &times) {
  sf::Text instructions{
      "Press --> to move forward in the simulation, press <-- to move back",
      times, 20};
  sf::FloatRect bounds = instructions.getLocalBounds();
  instructions.setOrigin((bounds.left + bounds.width) / 2.,
                         (bounds.top + bounds.height) / 2.);
  instructions.setPosition(0, -310.);

  return instructions;
}

std::vector<sf::Text> intial_legend_setting(double w, double h,
                                            sf::Font const &times,
                                            Universe const &u) {
  std::vector<sf::Text> text;

  sf::Text legend_0_title{"Intial universe conditions", times, 20};
  legend_0_title.setPosition(-w / 2 + 10., -h / 2 + 5.);
  text.push_back(legend_0_title);

  sf::Text K_0{"K = " + std::to_string(u.get_K_0()) + " J", times, 16};
  K_0.setPosition(-w / 2 + 10., -h / 2 + 35.);
  text.push_back(K_0);

  sf::Text U_0{"U = " + std::to_string(u.get_U_0()) + " J", times, 16};
  U_0.setPosition(-w / 2 + 10., -h / 2 + 55.);
  text.push_back(U_0);

  sf::Text E_0{"E = " + std::to_string(u.get_E_0()) + " J", times, 16};
  E_0.setPosition(-w / 2 + 10., -h / 2 + 75.);
  text.push_back(E_0);

  sf::Text P_0{"P = (" + std::to_string(u.get_P_0().get_x()) + ", " +
                   std::to_string(u.get_P_0().get_y()) + " ) kg m s^-1",
               times, 16};
  P_0.setPosition(-w / 2 + 10., -h / 2 + 95.);
  text.push_back(P_0);

  sf::Text L_0{"L = " + std::to_string(u.get_L_0()) + " kg m^2 s^-1", times, 16};
  L_0.setPosition(-w / 2 + 10., -h / 2 + 115.);
  text.push_back(L_0);

  return text;
}

std::vector<sf::Text> current_legend_setting(double w, double h,
                                             sf::Font const &times,
                                             Universe const &u) {
  std::vector<sf::Text> text;

  sf::Text legend_title{"Current universe conditions", times, 20};
  legend_title.setPosition(-w / 2 + 10., -h / 2 + 150.);
  text.push_back(legend_title);

  sf::Text K{"K = " + std::to_string(u.get_K_()) + " J", times, 16};
  K.setPosition(-w / 2 + 10., -h / 2 + 180.);
  text.push_back(K);

  sf::Text U{"U = " + std::to_string(u.get_U_()) + " J", times, 16};
  U.setPosition(-w / 2 + 10., -h / 2 + 200.);
  text.push_back(U);

  sf::Text E{"E = " + std::to_string(u.get_E_()) + " J", times, 16};
  E.setPosition(-w / 2 + 10., -h / 2 + 220.);
  text.push_back(E);

  sf::Text P{"P = (" + std::to_string(u.get_P_0().get_x()) + ", " +
                 std::to_string(u.get_P_0().get_y()) + " ) kg m s^-1",
             times, 16};
  P.setPosition(-w / 2 + 10., -h / 2 + 240.);
  text.push_back(P);

  sf::Text L{"L = " + std::to_string(u.get_L_()) + " kg m^2 s^-1", times, 16};
  L.setPosition(-w / 2 + 10., -h / 2 + 260.);
  text.push_back(L);

  sf::Text dE{"E_0 - E = " + std::to_string(u.get_E_0() - u.get_E_()) + " J",
              times, 16};
  dE.setPosition(-w / 2 + 10., -h / 2 + 300.);
  text.push_back(dE);

  sf::Text dP{"P_0 - P = (" +
                  std::to_string((u.get_P_0() - u.get_P_()).get_x()) + ", " +
                  std::to_string((u.get_P_0() - u.get_P_()).get_y()) + ") kg m s^-1",
              times, 16};
  dP.setPosition(-w / 2 + 10., -h / 2 + 320.);
  text.push_back(dP);

  sf::Text dL{"L_0 - L = " + std::to_string(u.get_L_0() - u.get_L_()) + " kg m^2 s^-1",
              times, 16};
  dL.setPosition(-w / 2 + 10., -h / 2 + 340.);
  text.push_back(dL);

  return text;
}

std::vector<sf::Text> u_informations(double w, double h, sf::Font const &times,
                                     int n_iterations, Universe const &u) {
  std::vector<sf::Text> info;

  sf::Text iterations{"Iterations: " + std::to_string(n_iterations), times, 16};
  iterations.setPosition(w / 2 - 120., -h / 2 + 5.);
  info.push_back(iterations);

  sf::Text G{"G = " + std::to_string(u.get_G()), times, 16};
  G.setPosition(w / 2 - 120., -h / 2 + 25.);
  info.push_back(G);

  sf::Text dt{"dt = " + std::to_string(u.get_dt()), times, 16};
  dt.setPosition(w / 2 - 120., -h / 2 + 45.);
  info.push_back(dt);

  sf::Text scale{"scale = " + std::to_string(u.get_scale()), times, 16};
  scale.setPosition(w / 2 - 120., -h / 2 + 65.);
  info.push_back(scale);

  sf::Text bodies{"# bodies = " + std::to_string(u.size()), times, 16};
  bodies.setPosition(w / 2 - 120., -h / 2 + 85.);
  info.push_back(bodies);

  return info;
}

std::vector<sf::Text> configuration_text_setting(
    sf::Font const &times, std::vector<sf::RectangleShape> buttons,
    std::vector<std::string> conf_titles) {
  std::vector<sf::Text> confs_text;

  sf::Text conf1{conf_titles[0], times, 20};
  confs_text.push_back(conf1);

  sf::Text conf2{conf_titles[1], times, 20};
  confs_text.push_back(conf2);

  sf::Text conf3{conf_titles[2], times, 20};
  confs_text.push_back(conf3);

  sf::Text conf4{conf_titles[3], times, 20};
  confs_text.push_back(conf4);

  sf::Text conf5{conf_titles[4], times, 20};
  confs_text.push_back(conf5);

  sf::Text conf6{conf_titles[5], times, 20};
  confs_text.push_back(conf6);

  sf::Text conf7{conf_titles[6], times, 20};
  confs_text.push_back(conf7);

  for (int i{0}; i < 7; ++i) {
    sf::FloatRect tbounds = confs_text[i].getLocalBounds();

    // 2. Imposta l'origine al centro del testo (tenendo conto di left/top)
    confs_text[i].setOrigin(tbounds.left + tbounds.width / 2.f,
                            tbounds.top + tbounds.height / 2.f);

    // 3. Posiziona il testo al centro del rettangolo
    sf::FloatRect rectBounds = buttons[i].getGlobalBounds();
    confs_text[i].setPosition(rectBounds.left + rectBounds.width / 2.f,
                              rectBounds.top + rectBounds.height / 2.f);
  }

  return confs_text;
}

std::vector<sf::RectangleShape> configuration_button_setting() {
  std::vector<sf::RectangleShape> conf_buttons;

  sf::RectangleShape b1;
  b1.setSize(sf::Vector2f(200.f, 40.f));
  b1.setFillColor(sf::Color::Blue);
  b1.setPosition(-790., 50.);
  conf_buttons.push_back(b1);

  sf::RectangleShape b2;
  b2.setSize(sf::Vector2f(200.f, 40.f));
  b2.setFillColor(sf::Color::Blue);
  b2.setPosition(-790., 100.);
  conf_buttons.push_back(b2);

  sf::RectangleShape b3;
  b3.setSize(sf::Vector2f(200.f, 40.f));
  b3.setFillColor(sf::Color::Blue);
  b3.setPosition(-790., 150.);
  conf_buttons.push_back(b3);

  sf::RectangleShape b4;
  b4.setSize(sf::Vector2f(200.f, 40.f));
  b4.setFillColor(sf::Color::Blue);
  b4.setPosition(-790., 200.);
  conf_buttons.push_back(b4);

  sf::RectangleShape b5;
  b5.setSize(sf::Vector2f(200.f, 40.f));
  b5.setFillColor(sf::Color::Blue);
  b5.setPosition(-790., 250.);
  conf_buttons.push_back(b5);

  sf::RectangleShape b6;
  b6.setSize(sf::Vector2f(200.f, 40.f));
  b6.setFillColor(sf::Color::Blue);
  b6.setPosition(-790., 300.);
  conf_buttons.push_back(b6);

  sf::RectangleShape b7;
  b7.setSize(sf::Vector2f(200.f, 40.f));
  b7.setFillColor(sf::Color::Blue);
  b7.setPosition(-790., 350.);
  conf_buttons.push_back(b7);

  return conf_buttons;
}

std::vector<double> run_conf(std::string conf_title) {
  std::fstream file;
  file.open("configurations.txt");
  std::vector<double> all_parameters;
  std::string line;

  bool check = false;
  if (file.is_open()) {
    while (getline(file, line)) {
      if (line == conf_title) {
        check = true;
        continue;  // salta il resto di questa iterazione, passa direttamente
                   // alla successiva
      }

      if (check == true) {
        std::istringstream stream(line);

        std::string par;
        while (stream >> par) {
          if (par == "fine") {
            check = false;
            break;
          }
          all_parameters.push_back(std::stod(par));
        }
      }
    }

    file.close();
  }

  else {
    std::cout << "Errore: file non trovato" << '\n';
  }

  return all_parameters;
}

}