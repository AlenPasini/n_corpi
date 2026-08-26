#include "n_bodies.hpp"

#ifndef PF_N_CORPI_HPP  // provvisorio
#define PF_N_CORPI_HPP

#include <SFML/Graphics.hpp>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

namespace nb {

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

Vector Body::get_r() const { return r_; }
double Body::get_k() const { return k_; }
Vector Body::get_v() const { return v_; }
Vector Body::get_a() const { return a_; }
Vector Body::get_a_fut() const { return a_fut_; }
double Body::get_m() const { return m_; }
std::size_t Body::get_id() const { return id_; }
// deve essere const perché altrimenti non me la fa paragonare
// nell'operatore ==

void Body::r_t() {
  r_.x += v_.x * dt_ + 0.5 * a_.x * dt_ * dt_;
  r_.y += v_.y * dt_ + 0.5 * a_.y * dt_ * dt_;
}

void Body::v_t() {
  v_.x += 0.5 * (a_.x + a_fut_.x) * dt_;
  v_.y += 0.5 * (a_.y + a_fut_.y) * dt_;
}

void Body::add_id(std::size_t id) { id_ = id; }

void Body::a_t(double a_t_x, double a_t_y) { a_fut_ = {a_t_x, a_t_y}; }

void Body::set_a_(Vector a_t_fut) { a_ = a_t_fut; }

void Body::set_dt(double new_dt) { dt_ = new_dt; }

bool operator==(Body const &b1, Body const &b2) {
  return b1.get_id() == b2.get_id();
}

std::size_t Universe::size() const { return u_.size(); }

void Universe::set_dt(double dt_input) { dt_ = dt_input; }

void Universe::set_G(double new_G) { G_ = new_G; }

void Universe::set_scale(double new_scale) { scale_ = new_scale; }

void Universe::add(Body &b) {
  u_.push_back(b);
  u_.back().add_id(u_.size() - 1);

  sf::CircleShape circle(static_cast<float>(b.get_k() / scale_));

  circle.setOrigin(static_cast<float>(b.get_k() / scale_),
                   static_cast<float>(b.get_k() / scale_));
  circle.setPosition(static_cast<float>(b.get_r().get_x() / scale_),
                     static_cast<float>(b.get_r().get_y() / scale_));

  std::size_t color_id;

  if (u_.back().get_id() <= this->n_colors() - 1) {
    color_id = u_.back().get_id();
  }

  else {
    color_id = u_.back().get_id() % n_colors();
  }

  circle.setFillColor(colors_[color_id]);

  circles_.push_back(circle);
}

std::vector<sf::CircleShape> &Universe::get_circles() { return circles_; }

Body Universe::get_body(std::size_t id) const { return u_[id]; }

// double get_eps() const { return eps; }
double Universe::get_G() const { return G_; }
double Universe::get_dt() const { return dt_; }
double Universe::get_scale() const { return scale_; }

double Universe::get_K_0() const { return K_0; }
double Universe::get_U_0() const { return U_0; }
double Universe::get_E_0() const { return E_0; }
Vector Universe::get_P_0() const { return P_0; }
double Universe::get_L_0() const { return L_0; }

double Universe::get_K_() const { return K_; }
double Universe::get_U_() const { return U_; }
double Universe::get_E_() const { return E_; }
Vector Universe::get_P_() const { return P_; }
double Universe::get_L_() const { return L_; }

double Universe::get_dE_() const { return dE_; }
double Universe::get_dP_() const { return dP_; }
double Universe::get_dL_() const { return dL_; }

std::size_t Universe::n_colors() const { return colors_.size(); }

void Universe::u_a_t(Body &b) {
  double a_t_x = 0.0;
  double a_t_y = 0.0;

  for (std::size_t j{0}; j < u_.size(); ++j) {
    if (b == u_[j]) {
    } else {
      Vector distance{b.get_r() - u_[j].get_r()};

      double num_x{get_G() * u_[j].get_m() * distance.get_x()};
      double num_y{get_G() * u_[j].get_m() * distance.get_y()};

      /*
      double denom{pow(
          distance.norm() * distance.norm() + get_eps() * get_eps(), 1.5)};
      */

      double denom{pow(distance.norm() * distance.norm(), 1.5)};

      a_t_x -= num_x / denom;
      a_t_y -= num_y / denom;
    }
  }

  b.a_t(a_t_x, a_t_y);
}

void Universe::r_t_complete() {
  for (std::size_t i{0}; i < u_.size(); ++i) {
    u_[i].r_t();
  }
}

void Universe::u_a_t_complete() {
  for (std::size_t i{0}; i < u_.size(); ++i) {
    u_a_t(u_[i]);
  }
}

void Universe::u_a_v_complete() {
  for (std::size_t i{0}; i < u_.size(); ++i) {
    u_[i].v_t();
  }
}

double Universe::K_t() {
  double K{0.};
  for (std::size_t j{0}; j < u_.size(); ++j) {
    K += 0.5 * u_[j].get_m() *
         (pow(u_[j].get_v().get_x(), 2) + pow(u_[j].get_v().get_y(), 2));
  }

  return K;
}

double Universe::U_t() {
  double U{0.};
  for (std::size_t i{0}; i + 1 < u_.size(); ++i) {
    for (std::size_t j{i + 1}; j < u_.size(); ++j) {
      double num = get_G() * u_[i].get_m() * u_[j].get_m();

      double denom =
          // (u_[i].get_r() - u_[j].get_r()).norm();
          std::sqrt(pow(u_[i].get_r().get_x() - u_[j].get_r().get_x(), 2.) +
                    pow(u_[i].get_r().get_y() - u_[j].get_r().get_y(), 2.));

      U -= num / denom;
    }
  }

  return U;
}

Vector Universe::P_t() {
  double p_x{0.};
  double p_y{0.};
  for (std::size_t j{0}; j < u_.size(); ++j) {
    p_x += u_[j].get_m() * u_[j].get_v().get_x();
    p_y += u_[j].get_m() * u_[j].get_v().get_y();
  }
  return {p_x, p_y};
}

double Universe::L_t() {
  double L{0.};
  for (std::size_t j{0}; j < u_.size(); ++j) {
    L += u_[j].get_m() * (u_[j].get_v().get_y() * u_[j].get_r().get_x() -
                          u_[j].get_v().get_x() * u_[j].get_r().get_y());
  }
  return L;
}

void Universe::check_collisions() {
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
        if (distance.norm() <= u_[i].get_k() + u_[j].get_k()) {
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

          u_.erase(u_.begin() + static_cast<int>(j));
          circles_.erase(circles_.begin() + static_cast<int>(j));
          // erase necessita di un iteratore, non un indice

          n = 1;
          break;
        }
      }
      if (n == 1) {
        u_.erase(u_.begin() + static_cast<int>(i));
        circles_.erase(circles_.begin() + static_cast<int>(i));
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

void Universe::set_universe_0() {
  K_0 = this->K_t();
  U_0 = this->U_t();
  E_0 = K_0 + U_0;
  P_0 = this->P_t();
  L_0 = this->L_t();

  this->set_energies();

  this->set_differences();
}

void Universe::set_energies() {
  K_ = K_0;
  U_ = U_0;
  E_ = E_0;
  P_ = P_0;
  L_ = L_0;
}

void Universe::set_differences() {
  dE_ = (this->E_ - this->E_0) / this->E_0;

  double denom_dP{0.};

  for (std::size_t j{0}; j < u_.size(); ++j) {
    denom_dP += u_[j].get_m() * u_[j].get_v().norm();
  }

  dP_ = (this->P_ - this->P_0).norm() / denom_dP;

  double denom_dL{0.};

  for (std::size_t j{0}; j < u_.size(); ++j) {
    denom_dL += u_[j].get_m() * u_[j].get_v().norm() * u_[j].get_r().norm();
  }

  dL_ = (this->L_ - this->L_0) / denom_dL;
}

void Universe::update_variables() {
  K_ = this->K_t();
  U_ = this->U_t();
  E_ = K_ + U_;
  P_ = this->P_t();
  L_ = this->L_t();
}

void Universe::set_a_0() {
  this->u_a_t_complete();
  for (std::size_t j{0}; j < u_.size(); ++j) {
    u_[j].set_a_(u_[j].get_a_fut());
  }
}

void Universe::simulation_steps(int steps) {
  for (std::size_t j{0}; j < u_.size(); ++j) {
    u_[j].set_dt(dt_);
  }
  for (int i{0}; i < steps; ++i) {
    this->r_t_complete();  // non mi ricordo assolutamente se si fa così...
    this->u_a_t_complete();
    this->u_a_v_complete();
    /*
    this->K_t();
    U_ = this->U_t();
    E_ = K_ + U_;
    */
    this->update_variables();

    for (std::size_t j{0}; j < u_.size(); ++j) {
      u_[j].set_a_(u_[j].get_a_fut());
    }
  }
}

void Universe::single_simulation_step() {
  for (std::size_t j{0}; j < u_.size(); ++j) {
    u_[j].set_dt(dt_);
  }
  this->check_collisions();
  this->r_t_complete();  // non mi ricordo assolutamente se si fa così...
  this->u_a_t_complete();
  this->u_a_v_complete();
  /*
  this->K_t();
  U_ = this->U_t();
  this->P_t();
  this->L_t();
  E_ = K_ + U_;
  */
  this->update_variables();
  this->set_differences();

  for (std::size_t j{0}; j < u_.size(); ++j) {
    u_[j].set_a_(u_[j].get_a_fut());
  }
}

void Universe::single_simulation_step_back() {
  dt_ = -dt_;
  this->single_simulation_step();
  dt_ = -dt_;
}

void Universe::new_config(double new_G, double new_dt, double new_scale) {
  this->set_G(new_G);
  this->set_dt(new_dt);
  this->set_scale(new_scale);
  K_0 = 0.;
  U_0 = 0.;
  E_0 = 0.;
  P_0 = {0., 0.};
  L_0 = 0.;

  u_.clear();
  circles_.clear();
}

void Universe::update_graphics() {
  for (std::size_t j{0}; j < u_.size(); ++j) {
    circles_[j].setPosition(
        static_cast<float>(u_[j].get_r().get_x() / scale_),
        static_cast<float>(-u_[j].get_r().get_y() / scale_));
  }
}

std::string scient(double value) {
  std::ostringstream stream;

  if (value != 0.0 && (std::fabs(value) > 1.e5 || std::fabs(value) < 1.e-10)) {
    stream << std::scientific << std::setprecision(5) << value;
  } else {
    stream << value;
  }

  return stream.str();
}

sf::Text space_instructions(sf::Font const &times) {
  sf::Text instructions{"Press SPACE to start / stop the simulation", times};
  sf::FloatRect bounds = instructions.getLocalBounds();
  instructions.setOrigin((bounds.left + bounds.width) / 2.f,
                         (bounds.top + bounds.height) / 2.f);
  instructions.setPosition(0, -340.);

  return instructions;
}

sf::Text ctrlE_instructions(sf::Font const &times) {
  sf::Text instructions{
      "Press ctrl + E to reset the simulation to its starting configuration",
      times, 20};
  sf::FloatRect bounds = instructions.getLocalBounds();
  instructions.setOrigin((bounds.left + bounds.width) / 2.f,
                         (bounds.top + bounds.height) / 2.f);
  instructions.setPosition(0, 340.);

  return instructions;
}

sf::Text arrows_instructions(sf::Font const &times) {
  sf::Text instructions{
      "Press --> to move forward in the simulation, press <-- to move back",
      times, 20};
  sf::FloatRect bounds = instructions.getLocalBounds();
  instructions.setOrigin((bounds.left + bounds.width) / 2.f,
                         (bounds.top + bounds.height) / 2.f);
  instructions.setPosition(0, -310.);

  return instructions;
}

std::vector<sf::Text> radii_disclaimer(sf::Font const &times) {
  std::vector<sf::Text> text;

  sf::Text title{"DISCLAIMER!", times, 20};
  title.setOrigin(title.getLocalBounds().width / 2.f, 0.f);
  title.setPosition(650, 280);
  text.push_back(title);

  sf::Text s1{"Planetary and solar radii not to scale!", times, 16};
  s1.setOrigin(s1.getLocalBounds().width / 2.f, 0);
  s1.setPosition(title.getPosition().x, title.getPosition().y + 30.f);
  text.push_back(s1);

  sf::Text s2{"Sun's radius 14 times too big", times, 16};
  s2.setOrigin(s2.getLocalBounds().width / 2.f, 0);
  s2.setPosition(s1.getPosition().x, s1.getPosition().y + 25.f);
  text.push_back(s2);

  sf::Text s3{"Planets' radius 614 times too big", times, 16};
  s3.setOrigin(s3.getLocalBounds().width / 2.f, 0);
  s3.setPosition(s2.getPosition().x, s2.getPosition().y + 25.f);
  text.push_back(s3);

  return text;
}

std::vector<sf::Text> intial_legend_setting(unsigned int w, unsigned int h,
                                            sf::Font const &times,
                                            Universe const &u) {
  std::vector<sf::Text> text;

  float wf = static_cast<float>(w);
  float hf = static_cast<float>(h);

  sf::Text legend_0_title{"Intial universe conditions", times, 20};
  legend_0_title.setPosition(-wf / 2.f + 10.f, -hf / 2.f + 5.f);
  text.push_back(legend_0_title);

  sf::Text K_0{"K = " + scient(u.get_K_0()) + " J", times, 16};
  K_0.setPosition(-wf / 2.f + 10.f, -hf / 2.f + 35.f);
  text.push_back(K_0);

  sf::Text U_0{"U = " + scient(u.get_U_0()) + " J", times, 16};
  U_0.setPosition(-wf / 2.f + 10.f, -hf / 2.f + 55.f);
  text.push_back(U_0);

  sf::Text E_0{"E = " + scient(u.get_E_0()) + " J", times, 16};
  E_0.setPosition(-wf / 2.f + 10.f, -hf / 2.f + 75.f);
  text.push_back(E_0);

  sf::Text P_0{"P = (" + scient(u.get_P_0().get_x()) + ", " +
                   scient(u.get_P_0().get_y()) + " ) kg m s^-1",
               times, 16};
  P_0.setPosition(-wf / 2.f + 10.f, -hf / 2.f + 95.f);
  text.push_back(P_0);

  sf::Text L_0{"L = " + scient(u.get_L_0()) + " kg m^2 s^-1", times, 16};
  L_0.setPosition(-wf / 2.f + 10.f, -hf / 2.f + 115.f);
  text.push_back(L_0);

  return text;
}

std::vector<sf::Text> current_legend_setting(unsigned int w, unsigned int h,
                                             sf::Font const &times,
                                             Universe const &u) {
  std::vector<sf::Text> text;

  float wf = static_cast<float>(w);
  float hf = static_cast<float>(h);

  sf::Text legend_title{"Current universe conditions", times, 20};
  legend_title.setPosition(-wf / 2.f + 10.f, -hf / 2.f + 150.f);
  text.push_back(legend_title);

  sf::Text K{"K = " + scient(u.get_K_()) + " J", times, 16};
  K.setPosition(-wf / 2.f + 10.f, -hf / 2.f + 180.f);
  text.push_back(K);

  sf::Text U{"U = " + scient(u.get_U_()) + " J", times, 16};
  U.setPosition(-wf / 2.f + 10.f, -hf / 2.f + 200.f);
  text.push_back(U);

  sf::Text E{"E = " + scient(u.get_E_()) + " J", times, 16};
  E.setPosition(-wf / 2.f + 10.f, -hf / 2.f + 220.f);
  text.push_back(E);

  sf::Text P{"P = (" + scient(u.get_P_0().get_x()) + ", " +
                 scient(u.get_P_0().get_y()) + " ) kg m s^-1",
             times, 16};
  P.setPosition(-wf / 2.f + 10.f, -hf / 2.f + 240.f);
  text.push_back(P);

  sf::Text L{"L = " + scient(u.get_L_()) + " kg m^2 s^-1", times, 16};
  L.setPosition(-wf / 2.f + 10.f, -hf / 2.f + 260.f);
  text.push_back(L);

  sf::Text dE{"(E_0 - E) % = " + scient(u.get_dE_()) + " J", times, 16};
  dE.setPosition(-wf / 2.f + 10.f, -hf / 2.f + 300.f);
  text.push_back(dE);

  sf::Text dP{"|P_0 - P|  = " + scient(u.get_dP_()) + ") kg m s^-1", times, 16};
  dP.setPosition(-wf / 2.f + 10.f, -hf / 2.f + 320.f);
  text.push_back(dP);

  sf::Text dL{"(L_0 - L) % = " + scient(u.get_dL_()) + " kg m^2 s^-1", times,
              16};
  dL.setPosition(-wf / 2.f + 10.f, -hf / 2.f + 340.f);
  text.push_back(dL);

  return text;
}

std::vector<sf::Text> u_informations(unsigned int w, unsigned int h,
                                     sf::Font const &times, int n_iterations,
                                     Universe const &u) {
  std::vector<sf::Text> info;
  float wf = static_cast<float>(w);
  float hf = static_cast<float>(h);

  sf::Text iterations{"Iterations: " + std::to_string(n_iterations), times, 16};
  iterations.setPosition(wf / 2.f - 120.f, -hf / 2.f + 5.f);
  info.push_back(iterations);

  sf::Text G{"G = " + std::to_string(u.get_G()), times, 16};
  G.setPosition(wf / 2.f - 120.f, -hf / 2.f + 25.f);
  info.push_back(G);

  sf::Text dt{"dt = " + std::to_string(u.get_dt()), times, 16};
  dt.setPosition(wf / 2.f - 120.f, -hf / 2.f + 45.f);
  info.push_back(dt);

  sf::Text scale{"scale = " + std::to_string(u.get_scale()), times, 16};
  scale.setPosition(wf / 2.f - 120.f, -hf / 2.f + 65.f);
  info.push_back(scale);

  sf::Text bodies{"# bodies = " + std::to_string(u.size()), times, 16};
  bodies.setPosition(wf / 2.f - 120.f, -hf / 2.f + 85.f);
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

  for (std::size_t i{0}; i < 7; ++i) {
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
  b1.setPosition(-790.f, 50.f);
  conf_buttons.push_back(b1);

  sf::RectangleShape b2;
  b2.setSize(sf::Vector2f(200.f, 40.f));
  b2.setFillColor(sf::Color::Blue);
  b2.setPosition(-790.f, 100.f);
  conf_buttons.push_back(b2);

  sf::RectangleShape b3;
  b3.setSize(sf::Vector2f(200.f, 40.f));
  b3.setFillColor(sf::Color::Blue);
  b3.setPosition(-790.f, 150.f);
  conf_buttons.push_back(b3);

  sf::RectangleShape b4;
  b4.setSize(sf::Vector2f(200.f, 40.f));
  b4.setFillColor(sf::Color::Blue);
  b4.setPosition(-790.f, 200.f);
  conf_buttons.push_back(b4);

  sf::RectangleShape b5;
  b5.setSize(sf::Vector2f(200.f, 40.f));
  b5.setFillColor(sf::Color::Blue);
  b5.setPosition(-790.f, 250.f);
  conf_buttons.push_back(b5);

  sf::RectangleShape b6;
  b6.setSize(sf::Vector2f(200.f, 40.f));
  b6.setFillColor(sf::Color::Blue);
  b6.setPosition(-790.f, 300.f);
  conf_buttons.push_back(b6);

  sf::RectangleShape b7;
  b7.setSize(sf::Vector2f(200.f, 40.f));
  b7.setFillColor(sf::Color::Blue);
  b7.setPosition(-790.f, 350.f);
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

}  // namespace nb
#endif