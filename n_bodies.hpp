#ifndef PF_N_BODIES_HPP
#define PF_N_BODIES_HPP

#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>

namespace nb {
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

  double norm() const { return std::sqrt(x * x + y * y); }
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
  std::size_t id_;
  double dt_{0.005};

  sf::VertexArray trail;
  int steps{0};

 public:
  Body(Vector r, Vector v, double m, double k) : r_{r}, v_{v}, m_{m}, k_{k} {}

  Vector get_r() const;
  double get_k() const;
  Vector get_v() const;
  Vector get_a() const;
  Vector get_a_fut() const;
  double get_m() const;
  std::size_t get_id() const;

  sf::VertexArray &get_trail();

  void r_t();

  void v_t();

  void add_id(std::size_t id);

  void a_t(double a_t_x, double a_t_y);

  void set_a_(Vector const &a_t_fut);

  void set_dt(double new_dt);

  void update_trail(sf::CircleShape circle);
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

  double dE_;
  double dP_;
  double dL_;

  double G_;
  double dt_;
  double scale_;

 public:
  Universe(double G, double dt, double scale) : G_{G}, dt_{dt}, scale_{scale} {}

  std::size_t size() const;

  void set_dt(double dt_input);

  void set_G(double new_G);

  void set_scale(double new_scale);

  void add(Body &b);

  std::vector<sf::CircleShape> &get_circles();

  Body get_body(std::size_t id) const;

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

  double get_dE_() const;
  double get_dP_() const;
  double get_dL_() const;

  std::size_t n_colors() const;

  void u_a_t(Body &b);

  void r_t_complete();

  void u_a_t_complete();

  void u_a_v_complete();

  double K_t() const;

  double U_t() const;

  Vector P_t() const;

  double L_t() const;

  void set_differences();

  void check_collisions();

  void set_universe_0();

  void set_energies();

  void update_variables();

  void set_a_0();

  void single_simulation_step();

  void single_simulation_step_back();

  void new_config(double new_G, double new_dt, double new_scale);

  void update_graphics();
};

std::string scient(double value);

sf::Text space_instructions(sf::Font const &times);

sf::Text altR_instructions(sf::Font const &times);

sf::Text arrows_instructions(sf::Font const &times);

std::vector<sf::Text> radii_disclaimer(sf::Font const &times);

std::vector<sf::Text> intial_legend_setting(unsigned int w, unsigned int h,
                                            sf::Font const &times,
                                            Universe const &u);

std::vector<sf::Text> current_legend_setting(unsigned int w, unsigned int h,
                                             sf::Font const &times,
                                             Universe const &u);

std::vector<sf::Text> u_informations(unsigned int w, unsigned int h,
                                     sf::Font const &times, int n_iterations,
                                     Universe const &u);

std::vector<sf::Text> configuration_text_setting(
    sf::Font const &times, std::vector<sf::RectangleShape> buttons,
    std::vector<std::string> conf_titles);

std::vector<sf::RectangleShape> configuration_button_setting();

std::vector<double> run_conf(std::string conf_title);

} 
#endif