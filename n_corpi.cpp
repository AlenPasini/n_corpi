// #include "n_corpi.hpp"

// #ifndef PF_N_CORPI_HPP. // provvisorio
// #define PF_N_CORPI_HPP

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <vector>

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

  double get_x() { return x; }
  double get_y() { return y; }
};

Vector operator+(Vector const &v1, Vector const &v2)
{
  auto result{v1};
  return result += v2;
}

Vector operator-(Vector const &v1, Vector const &v2)
{
  auto result{v1};
  return result -= v2;
}

bool operator==(Vector const &v1, Vector const &v2)
{
  return v1.x == v2.x && v1.y == v2.y;
}

class Body
{
private:
  Vector r_;
  double k_;
  Vector v_;
  Vector a_{0., 0.};
  Vector a_fut_{0., 0.};
  double m_;
  int id_{-1};
  double dt{0.005};

public:
  Body(Vector r, Vector v, double m, double k) : r_{r}, v_{v}, m_{m}, k_{k} {}
  Vector get_r() { return r_; }
  double get_k() { return k_; }
  Vector get_v() { return v_; }
  Vector get_a() { return a_; }
  Vector get_a_fut() { return a_fut_; }
  double get_m() { return m_; }
  int get_id() const { return id_; }
  // deve essere const perché altrimenti non me la fa paragonare
  // nell'operatore ==

  void r_t()
  {
    r_.x += v_.x * dt + 0.5 * a_.x * dt * dt;
    r_.y += v_.y * dt + 0.5 * a_.y * dt * dt;
  }

  void v_t()
  {
    v_.x += 0.5 * (a_.x + a_fut_.x) * dt;
    v_.y += 0.5 * (a_.y + a_fut_.y) * dt;
  }

  void add_id(int id) { id_ = id; }

  void a_t(double a_t_x, double a_t_y) { a_fut_ = {a_t_x, a_t_y}; }

  void set_a_(Vector a_t_fut) { a_ = a_t_fut; }
};

bool operator==(Body const &b1, Body const &b2)
{
  return b1.get_id() == b2.get_id();
}

class Universe
{
private:
  std::vector<Body> u_{};
  std::vector<sf::CircleShape> circles_{};

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

  double dt{0.005};
  double G{6.67 * pow(10., -11.)};
  double eps{pow(10., -12.)};
  double scale_{1e5};

public:
  int size() { return u_.size(); }

  void set_dt(double dt_input) { dt = dt_input; }

  void add_body(Body &b)
  {
    u_.push_back(b);
    u_.back().add_id(u_.size() - 1);

    K_0 += 0.5 * b.get_m() *
           (pow(b.get_v().get_x(), 2) + pow(b.get_v().get_y(), 2));

    P_0 += b.get_m() *
           std::sqrt((pow(b.get_v().get_x(), 2) + pow(b.get_v().get_y(), 2)));

    L_0 += b.get_m() * (b.get_v().get_y() * b.get_r().get_x() -
                        b.get_v().get_x() * b.get_r().get_y());
  }

  void add_circle(Body &b)
  {
    sf::CircleShape circle(10.);
    circle.setOrigin(10., 10.);

    circle.setPosition(b.get_r().get_x() / scale_, b.get_r().get_y() / scale_);

    circles_.push_back(circle);
  }

  void add(Body &b)
  {
    add_body(b);
    add_circle(b);
  }

  std::vector<sf::CircleShape> &get_circles() { return circles_; }

  Body get_body(int id) { return u_[id]; }

  double get_eps() { return eps; }
  double get_G() { return G; }
  double get_dt() { return dt; }

  double get_K_0() { return K_0; }
  double get_U_0() { return U_0; }
  double get_E_0() { return E_0; }

  double get_K_() { return K_; }
  double get_U_() { return U_; }
  double get_E_() { return E_; }

  // al momento ogni body prende un id identificativa quando viene
  // inserito dentro a Universe. Non ha quindi senso avere un body al di fuori
  // di Universe. Ogni body al di fuori di Universe ha id = -1, che non ha
  // senso.
  // Bisogna inserire un "require qualcosa??" Non lo so fare

  void u_a_t(Body &b)
  {
    double a_t_x = 0.0;
    double a_t_y = 0.0;

    for (size_t j{0}; j < u_.size(); ++j)
    {
      if (b == u_[j])
      {
      }
      else
      {
        double denom_x{pow(pow(u_[j].get_r().get_x() - b.get_r().get_x(), 2) +
                               get_eps() * get_eps(),
                           1.5)};

        double num_x{get_G() * u_[j].get_m() *
                     (b.get_r().get_x() - u_[j].get_r().get_x())};

        double denom_y{pow(pow(u_[j].get_r().get_y() - b.get_r().get_y(), 2) +
                               get_eps() * get_eps(),
                           1.5)};

        double num_y{get_G() * u_[j].get_m() *
                     (b.get_r().get_y() - u_[j].get_r().get_y())};

        a_t_x -= num_x / denom_x;
        a_t_y -= num_y / denom_y;
      }
    }

    b.a_t(a_t_x, a_t_y);
  }

  void r_t_complete()
  {
    for (size_t i{0}; i < u_.size(); ++i)
    {
      u_[i].r_t();
    }
  }

  void u_a_t_complete()
  {
    for (size_t i{0}; i < u_.size(); ++i)
    {
      u_a_t(u_[i]);
    }
  }

  void u_a_v_complete()
  {
    for (size_t i{0}; i < u_.size(); ++i)
    {
      u_[i].v_t();
    }
  }

  void K_t()
  {
    K_ = 0.;
    for (size_t j{0}; j < u_.size(); ++j)
    {
      K_ += 0.5 * u_[j].get_m() *
            (pow(u_[j].get_v().get_x(), 2) + pow(u_[j].get_v().get_y(), 2));
    }
  }

  void P_t()
  {
    P_ = 0.;
    for (size_t j{0}; j < u_.size(); ++j)
    {
      P_ += u_[j].get_m() * std::sqrt((pow(u_[j].get_v().get_x(), 2) +
                                       pow(u_[j].get_v().get_y(), 2)));
    }
  }

  void L_t()
  {
    L_ = 0.;
    for (size_t j{0}; j < u_.size(); ++j)
    {
      L_ += u_[j].get_m() * (u_[j].get_v().get_y() * u_[j].get_r().get_x() -
                             u_[j].get_v().get_x() * u_[j].get_r().get_y());
    }
  }

  double U_t()
  {
    double U{};
    for (size_t i{0}; i < u_.size() - 1; ++i)
    {
      for (size_t j{i + 1}; j < u_.size(); ++j)
      {
        double num = get_G() * u_[i].get_m() * u_[j].get_m();

        double denom =
            std::sqrt(pow(u_[i].get_r().get_x() - u_[j].get_r().get_x(), 2) +
                      pow(u_[i].get_r().get_y() - u_[j].get_r().get_y(), 2));

        U -= num / denom;
      }
    }

    return U;
  }

  void collision()
  {
    Vector r_n{};
    Vector v_n{};
    double m_n{0};
    int k=1;
    while(k != 0)
    {
      k=0;
    for (size_t i{0}; i < u_.size() - 1; ++i)
    {
      for (size_t j{i + 1}; j < u_.size(); ++j)
      {
        if (std::abs(std::sqrt(pow(u_[i].get_r().get_x(), 2) + pow(u_[i].get_r().get_y(), 2)) - std::sqrt(pow(u_[j].get_r().get_x(), 2) + pow(u_[j].get_r().get_y(), 2))) <= u_[i].get_k() + u_[j].get_k())
        {
           m_n = u_[i].get_m() + u_[j].get_m();
           r_n.x = (u_[i].get_m() * u_[i].get_r().get_x() + u_[j].get_m() * u_[j].get_r().get_x())/m_n;
           r_n.y = (u_[i].get_m() * u_[i].get_r().get_y() + u_[j].get_m() * u_[j].get_r().get_y())/m_n;
           v_n.x = (u_[i].get_m() * u_[i].get_v().get_x() + u_[j].get_m() * u_[j].get_r().get_v())/m_n;
           r_n.x = (u_[i].get_m() * u_[i].get_v().get_y() + u_[j].get_m() * u_[j].get_v().get_y())/m_n;
          k=1;
          break;
        }
      }
      if(k=1)
        {
          break;
        }
    }
  }
  }

    void set_U_0()
    {
      U_0 = this->U_t();
      E_0 = K_0 + U_0;
    }

    void simulation(int steps)
    {
      for (int i{0}; i < steps; ++i)
      {
        this->r_t_complete(); // non mi ricordo assolutamente se si fa così...
        this->u_a_t_complete();
        this->u_a_v_complete();
        this->K_t();
        U_ = this->U_t();
        E_ = K_ + U_;

        for (size_t j{0}; j < u_.size(); ++j)
        {
          u_[j].set_a_(u_[j].get_a_fut());
          circles_[j].setPosition(u_[j].get_r().get_x() / scale_,
                                  -u_[j].get_r().get_y() / scale_);
        }
      }
    }

    void simulation_graphics(double dt)
    {
      set_dt(dt);
      this->r_t_complete(); // non mi ricordo assolutamente se si fa così...
      this->u_a_t_complete();
      this->u_a_v_complete();
      this->K_t();
      U_ = this->U_t();
      E_ = K_ + U_;

      for (size_t j{0}; j < u_.size(); ++j)
      {
        u_[j].set_a_(u_[j].get_a_fut());
        circles_[j].setPosition(u_[j].get_r().get_x() / scale_,
                                -u_[j].get_r().get_y() / scale_);
      }
    }

    void single_simulation()
    {
      this->r_t_complete(); // non mi ricordo assolutamente se si fa così...
      this->u_a_t_complete();
      this->u_a_v_complete();
      this->K_t();
      U_ = this->U_t();
      E_ = K_ + U_;

      std::cout << "Position body: " << u_[0].get_r().get_x() << "    ";

      for (size_t j{0}; j < u_.size(); ++j)
      {
        u_[j].set_a_(u_[j].get_a_fut());
        circles_[j].setPosition(u_[j].get_r().get_x() / scale_,
                                -u_[j].get_r().get_y() / scale_);
      }
      std::cout << "Position circle: " << circles_[0].getPosition().x << '\n';
    }
  };
  // #endif