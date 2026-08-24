#include <SFML/Graphics.hpp>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

namespace nc
{
  struct Vector
  {
    double x;
    double y;

    Vector &operator+=(Vector const &v)
    {
      x += v.x;
      y += v.y;
      return *this;
    }

    Vector &operator-=(Vector const &v)
    {
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

  class Body
  {
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

  class Universe
  {
  private:
    std::vector<Body> u_{};
    std::vector<sf::CircleShape> circles_{};
    std::vector<sf::Color> colors_{sf::Color::Red, sf::Color::Green,
                                   sf::Color::Blue, sf::Color::Yellow,
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

  sf::Text space_instructions(sf::Font const &times);

  sf::Text ctrlE_instructions(sf::Font const &times);

  sf::Text arrows_instructions(sf::Font const &times);

  std::vector<sf::Text> intial_legend_setting(double w, double h,
                                              sf::Font const &times,
                                              Universe const &u);

  std::vector<sf::Text> current_legend_setting(double w, double h,
                                               sf::Font const &times,
                                               Universe const &u);

  std::vector<sf::Text> u_informations(double w, double h, sf::Font const &times,
                                       int n_iterations, Universe const &u);

  std::vector<sf::Text> configuration_text_setting(
      sf::Font const &times, std::vector<sf::RectangleShape> buttons,
      std::vector<std::string> conf_titles);

  std::vector<sf::RectangleShape> configuration_button_setting();

  std::vector<double> run_conf(std::string conf_title);

}  // namespace nc