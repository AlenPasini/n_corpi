// #include "n_corpi.hpp"

// #ifndef PF_N_CORPI_HPP. // provvisorio
// #define PF_N_CORPI_HPP

#include <SFML/Graphics.hpp>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

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

Vector operator+(Vector const &v1, Vector const &v2) {
  auto result{v1};
  return result += v2;
}

Vector operator-(Vector const &v1, Vector const &v2) {
  auto result{v1};
  return result -= v2;
}

bool operator==(Vector const &v1, Vector const &v2) {
  return v1.x == v2.x && v1.y == v2.y;
}

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
  Vector get_r() const { return r_; }
  double get_k() const { return k_; }
  Vector get_v() const { return v_; }
  Vector get_a() const { return a_; }
  Vector get_a_fut() const { return a_fut_; }
  double get_m() const { return m_; }
  int get_id() const { return id_; }
  // deve essere const perché altrimenti non me la fa paragonare
  // nell'operatore ==

  void r_t() {
    r_.x += v_.x * dt + 0.5 * a_.x * dt * dt;
    r_.y += v_.y * dt + 0.5 * a_.y * dt * dt;
  }

  void v_t() {
    v_.x += 0.5 * (a_.x + a_fut_.x) * dt;
    v_.y += 0.5 * (a_.y + a_fut_.y) * dt;
  }

  void add_id(int id) { id_ = id; }

  void a_t(double a_t_x, double a_t_y) { a_fut_ = {a_t_x, a_t_y}; }

  void set_a_(Vector a_t_fut) { a_ = a_t_fut; }

  void set_dt(double new_dt) { dt = new_dt; }
};

bool operator==(Body const &b1, Body const &b2) {
  return b1.get_id() == b2.get_id();
}

class Universe {
 private:
  std::vector<Body> u_{};
  std::vector<sf::CircleShape> circles_{};
  std::vector<sf::Color> colors_{sf::Color::Red,     sf::Color::Green,
                                 sf::Color::Blue,    sf::Color::Yellow,
                                 sf::Color::Magenta, sf::Color::Cyan};

  double K_0{0.};
  double U_0{0.};
  double E_0{0.};
  double P_0{0.};
  double L_0{0.};

  double K_;
  double U_;
  double E_;
  double P_;
  double L_;

  double G;
  double dt;
  double eps{pow(10., -12.)};
  double scale_;

 public:
  Universe(double G, double dt, double scale) : G{G}, dt{dt}, scale_{scale} {}

  int size() const { return u_.size(); }

  void set_dt(double dt_input) { dt = dt_input; }

  void set_G(double new_G) { G = new_G; }

  void set_scale(double new_scale) { scale_ = new_scale; }

  void add(Body &b) {
    u_.push_back(b);
    u_.back().add_id(u_.size() - 1);

    K_0 += 0.5 * b.get_m() *
           (pow(b.get_v().get_x(), 2) + pow(b.get_v().get_y(), 2));

    P_0 += b.get_m() *
           std::sqrt((pow(b.get_v().get_x(), 2) + pow(b.get_v().get_y(), 2)));

    L_0 += b.get_m() * (b.get_v().get_y() * b.get_r().get_x() -
                        b.get_v().get_x() * b.get_r().get_y());

    sf::CircleShape circle(b.get_k() / scale_);
    circle.setOrigin(10., 10.);
    circle.setPosition(b.get_r().get_x() / scale_, b.get_r().get_y() / scale_);
    circle.setFillColor(colors_[u_.size() - 1]);

    /*
    int color_id;
    

    if (u_.back().get_id() <= this->n_colors() - 1) {
      color_id = u_.back().get_id();
    }

    else {
      color_id = u_.back().get_id() % n_colors();
    }

    circle.setFillColor(colors_[color_id]);
    */

    circle.setFillColor(colors_[u_.back().get_id()]);
    circles_.push_back(circle);
  }

  std::vector<sf::CircleShape> &get_circles() { return circles_; }

  Body get_body(int id) { return u_[id]; }

  double get_eps() const { return eps; }
  double get_G() const { return G; }
  double get_dt() const { return dt; }
  double get_scale() const { return scale_; }

  double get_K_0() const { return K_0; }
  double get_U_0() const { return U_0; }
  double get_E_0() const { return E_0; }

  double get_K_() const { return K_; }
  double get_U_() const { return U_; }
  double get_E_() const { return E_; }

  int n_colors() const { return colors_.size(); }

  void u_a_t(Body &b) {
    double a_t_x = 0.0;
    double a_t_y = 0.0;

    for (std::size_t j{0}; j < u_.size(); ++j) {
      if (b == u_[j]) {
      } else {
        Vector distance{b.get_r() - u_[j].get_r()};

        double num_x{get_G() * u_[j].get_m() * distance.get_x()};
        double num_y{get_G() * u_[j].get_m() * distance.get_y()};

        double denom{pow(
            distance.norm() * distance.norm() + get_eps() * get_eps(), 1.5)};

        a_t_x -= num_x / denom;
        a_t_y -= num_y / denom;
      }
    }

    b.a_t(a_t_x, a_t_y);
  }

  void r_t_complete() {
    for (std::size_t i{0}; i < u_.size(); ++i) {
      u_[i].r_t();
    }
  }

  void u_a_t_complete() {
    for (std::size_t i{0}; i < u_.size(); ++i) {
      u_a_t(u_[i]);
    }
  }

  void u_a_v_complete() {
    for (std::size_t i{0}; i < u_.size(); ++i) {
      u_[i].v_t();
    }
  }

  void K_t() {
    K_ = 0.;
    for (std::size_t j{0}; j < u_.size(); ++j) {
      K_ += 0.5 * u_[j].get_m() *
            (pow(u_[j].get_v().get_x(), 2) + pow(u_[j].get_v().get_y(), 2));
    }
  }

  void P_t() {
    P_ = 0.;
    for (std::size_t j{0}; j < u_.size(); ++j) {
      P_ += u_[j].get_m() * std::sqrt((pow(u_[j].get_v().get_x(), 2) +
                                       pow(u_[j].get_v().get_y(), 2)));
    }
  }

  void L_t() {
    L_ = 0.;
    for (std::size_t j{0}; j < u_.size(); ++j) {
      L_ += u_[j].get_m() * (u_[j].get_v().get_y() * u_[j].get_r().get_x() -
                             u_[j].get_v().get_x() * u_[j].get_r().get_y());
    }
  }

  double U_t() {
    double U{};
    for (std::size_t i{0}; i + 1 < u_.size(); ++i) {
      for (std::size_t j{i + 1}; j < u_.size(); ++j) {
        double num = get_G() * u_[i].get_m() * u_[j].get_m();

        double denom =
            std::sqrt(pow(u_[i].get_r().get_x() - u_[j].get_r().get_x(), 2) +
                      pow(u_[i].get_r().get_y() - u_[j].get_r().get_y(), 2));

        U -= num / denom;
      }
    }

    return U;
  }

  void check_collisions() {
    Vector r_n{0., 0.};
    Vector v_n{0., 0.};
    double m_n{0};
    double k_n{0};
    int n{1};
    std::size_t c_1{0};
    std::size_t c_2{0};
    while (n != 0) {
      n = 0;
      for (std::size_t i{0}; i + 1 < u_.size(); ++i) {
        for (std::size_t j{i + 1}; j < u_.size(); ++j) {
          Vector distance = u_[i].get_r() - u_[j].get_r();
          if (distance.norm() <= u_[i].get_k() + u_[j].get_k())

          /*
                    if (std::abs(std::sqrt(pow(u_[i].get_r().get_x(), 2) +
                                           pow(u_[i].get_r().get_y(), 2)) -
                                 std::sqrt(pow(u_[j].get_r().get_x(), 2) +
                                           pow(u_[j].get_r().get_y(), 2))) <=
                        u_[i].get_k() + u_[j].get_k())
          */
          {
            c_1 = i;
            c_2 = j - 1;
            m_n = u_[i].get_m() + u_[j].get_m();

            r_n.x = (u_[i].get_m() * u_[i].get_r().get_x() +
                     u_[j].get_m() * u_[j].get_r().get_x()) /
                    m_n;
            r_n.y = (u_[i].get_m() * u_[i].get_r().get_y() +
                     u_[j].get_m() * u_[j].get_r().get_y()) /
                    m_n;
            v_n.x = (u_[i].get_m() * u_[i].get_v().get_x() +
                     u_[j].get_m() * u_[j].get_v().get_x()) /
                    m_n;
            v_n.y = (u_[i].get_m() * u_[i].get_v().get_y() +
                     u_[j].get_m() * u_[j].get_v().get_y()) /
                    m_n;
            k_n = u_[i].get_k() + u_[j].get_k();

            u_.erase(u_.begin() + j);
            // erase necessita di un iteratore, non un indice

            n = 1;
            break;
          }
        }
        if (n == 1) {
          u_.erase(u_.begin() + i);
          break;
        }
      }

      if (n == 1) {
        Body d{r_n, v_n, m_n, k_n};
        this->add(d);  // da controllare
        for (std::size_t i{c_1}; i < c_2; ++i) {
          u_[i].add_id(u_[i].get_id() - 1);
        }
        for (std::size_t i = c_2; i + 1 < u_.size(); ++i) {
          u_[i].add_id(u_[i].get_id() - 2);
        }
        u_[u_.size() - 1].add_id(u_.size() - 1);

        this->set_a_0();
      }
    }
  }

  void set_U_0() {
    U_0 = this->U_t();
    E_0 = K_0 + U_0;
  }

  void set_energies() {
    K_ = K_0;
    U_ = U_0;
    E_ = E_0;
  }

  void set_a_0() {
    this->u_a_t_complete();
    for (std::size_t j{0}; j < u_.size(); ++j) {
      u_[j].set_a_(u_[j].get_a_fut());
    }
  }

  void simulation_steps(int steps) {
    for (std::size_t j{0}; j < u_.size(); ++j) {
      u_[j].set_dt(dt);
    }
    for (int i{0}; i < steps; ++i) {
      this->r_t_complete();  // non mi ricordo assolutamente se si fa così...
      this->u_a_t_complete();
      this->u_a_v_complete();
      this->K_t();
      U_ = this->U_t();
      E_ = K_ + U_;

      for (std::size_t j{0}; j < u_.size(); ++j) {
        u_[j].set_a_(u_[j].get_a_fut());
      }
    }
  }

  void single_simulation_step() {
    for (std::size_t j{0}; j < u_.size(); ++j) {
      u_[j].set_dt(dt);
    }
    this->check_collisions();
    this->r_t_complete();  // non mi ricordo assolutamente se si fa così...
    this->u_a_t_complete();
    this->u_a_v_complete();
    this->K_t();
    U_ = this->U_t();
    E_ = K_ + U_;

    for (std::size_t j{0}; j < u_.size(); ++j) {
      u_[j].set_a_(u_[j].get_a_fut());
    }
  }

  void single_simulation_step_back() {
    dt = -dt;
    this->single_simulation_step();
    dt = -dt;
  }

  void reset(int iterations) {
    if (iterations >= 0) {
      dt = -dt;
      for (int i{0}; i < iterations; ++i) {
        this->single_simulation_step();
      }
      dt = -dt;
    }

    else {
      for (int i{0}; i < -iterations; ++i) {
        this->single_simulation_step();
      }
    }
  }

  void new_config(double new_G, double new_dt, double new_scale) {
    this->set_G(new_G);
    this->set_dt(new_dt);
    this->set_scale(new_scale);
    K_0 = 0.;
    U_0 = 0.;
    E_0 = 0.;

    u_.clear();
    circles_.clear();
  }

  void update_graphics() {
    for (std::size_t j{0}; j < u_.size(); ++j) {
      circles_[j].setPosition(u_[j].get_r().get_x() / scale_,
                              -u_[j].get_r().get_y() / scale_);
    }
  }
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

  sf::Text legend_0_title{"Intial energetic conditions", times, 20};
  legend_0_title.setPosition(-w / 2 + 10., -h / 2 + 5.);
  text.push_back(legend_0_title);

  sf::Text K_0{"K = " + std::to_string(u.get_K_0()) + "J", times, 16};
  K_0.setPosition(-w / 2 + 10., -h / 2 + 35.);
  text.push_back(K_0);

  sf::Text U_0{"U = " + std::to_string(u.get_U_0()) + "J", times, 16};
  U_0.setPosition(-w / 2 + 10., -h / 2 + 55.);
  text.push_back(U_0);

  sf::Text E_0{"E = " + std::to_string(u.get_E_0()) + "J", times, 16};
  E_0.setPosition(-w / 2 + 10., -h / 2 + 75.);
  text.push_back(E_0);

  return text;
}

std::vector<sf::Text> current_legend_setting(double w, double h,
                                             sf::Font const &times,
                                             Universe const &u) {
  std::vector<sf::Text> text;

  sf::Text legend_title{"Current energetic conditions", times, 20};
  legend_title.setPosition(-w / 2 + 10., -h / 2 + 110.);
  text.push_back(legend_title);

  sf::Text K{"K = " + std::to_string(u.get_K_()) + "J", times, 16};
  K.setPosition(-w / 2 + 10., -h / 2 + 140.);
  text.push_back(K);

  sf::Text U{"U = " + std::to_string(u.get_U_()) + "J", times, 16};
  U.setPosition(-w / 2 + 10., -h / 2 + 160.);
  text.push_back(U);

  sf::Text E{"E = " + std::to_string(u.get_E_()) + "J", times, 16};
  E.setPosition(-w / 2 + 10., -h / 2 + 180.);
  text.push_back(E);

  sf::Text dE{"E_0 - E = " + std::to_string(u.get_E_0() - u.get_E_()) + "J",
              times, 16};
  dE.setPosition(-w / 2 + 10., -h / 2 + 220.);
  text.push_back(dE);

  return text;
}

std::vector<sf::Text> u_informations(double w, double h, sf::Font const &times,
                                     int n_iterations, Universe const &u) {
  std::vector<sf::Text> info;

  sf::Text iterations{"Iterations: " + std::to_string(n_iterations), times, 20};
  iterations.setPosition(w / 2 - 150., -h / 2 + 5.);
  info.push_back(iterations);

  sf::Text G{"G = " + std::to_string(u.get_G()), times, 20};
  G.setPosition(w / 2 - 150., -h / 2 + 25.);
  info.push_back(G);

  sf::Text dt{"dt = " + std::to_string(u.get_dt()), times, 20};
  dt.setPosition(w / 2 - 150., -h / 2 + 50.);
  info.push_back(dt);

  sf::Text scale{"scale = " + std::to_string(u.get_scale()), times, 20};
  scale.setPosition(w / 2 - 150., -h / 2 + 75.);
  info.push_back(scale);

  sf::Text bodies{"# bodies = " + std::to_string(u.size()), times, 20};
  bodies.setPosition(w / 2 - 150., -h / 2 + 100.);
  info.push_back(bodies);

  return info;
}

std::vector<sf::Text> configuration_text_setting(
    sf::Font const &times, std::vector<sf::RectangleShape> buttons) {
  std::vector<sf::Text> confs_text;

  sf::Text conf1{"First configuration", times, 20};
  confs_text.push_back(conf1);

  sf::Text conf2{"Second configuration", times, 20};
  confs_text.push_back(conf2);

  sf::Text conf3{"Third configuration", times, 20};
  confs_text.push_back(conf3);

  sf::Text conf4{"Fourth configuration", times, 20};
  confs_text.push_back(conf4);

  sf::Text conf5{"Fifth configuration", times, 20};
  confs_text.push_back(conf5);

  sf::Text conf6{"Sixth configuration", times, 20};
  confs_text.push_back(conf6);

  for (int i{0}; i < 6; ++i) {
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
  b1.setPosition(-790., 0.);
  conf_buttons.push_back(b1);

  sf::RectangleShape b2;
  b2.setSize(sf::Vector2f(200.f, 40.f));
  b2.setFillColor(sf::Color::Blue);
  b2.setPosition(-790., 50.);
  conf_buttons.push_back(b2);

  sf::RectangleShape b3;
  b3.setSize(sf::Vector2f(200.f, 40.f));
  b3.setFillColor(sf::Color::Blue);
  b3.setPosition(-790., 100.);
  conf_buttons.push_back(b3);

  sf::RectangleShape b4;
  b4.setSize(sf::Vector2f(200.f, 40.f));
  b4.setFillColor(sf::Color::Blue);
  b4.setPosition(-790., 150.);
  conf_buttons.push_back(b4);

  sf::RectangleShape b5;
  b5.setSize(sf::Vector2f(200.f, 40.f));
  b5.setFillColor(sf::Color::Blue);
  b5.setPosition(-790., 200.);
  conf_buttons.push_back(b5);

  sf::RectangleShape b6;
  b6.setSize(sf::Vector2f(200.f, 40.f));
  b6.setFillColor(sf::Color::Blue);
  b6.setPosition(-790., 250.);
  conf_buttons.push_back(b6);

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
// #endif