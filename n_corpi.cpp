#include "n_corpi.hpp"

#ifndef PF_N_CORPI_HPP. // provvisorio
#define PF_N_CORPI_HPP

struct Vettore
{
  double x;
  double y;

  Vettore &operator+=(Vettore const &v)
  {
    x += v.x;
    y += v.y;
    return *this;
  }
};


Vettore operator+(Vettore const &v1, Vettore const &v2)
{
  auto result{v1};
  return result += v2;
}

bool operator==(Vettore const& v1, Vettore const& v2)
{
  return v1.x == v2.x && v1.y == v2.y;
}


class Corpo
{
private:
  Vettore r_;
  Vettore v_;
  Vettore a_{0.,0.};
  double dt{0.005};
public:
  Corpo(Vettore r, Vettore v) : r_{r}, v_{v} {}
  Vettore get_r() { return r_; }
  Vettore get_v() { return v_; }
  Vettore get_a() { return a_; }
  double  get_dt() { return dt; }
  void r_t(){
        r_.x += v_.x * dt + 0.5 * a_.x * dt * dt;
        r_.y += v_.y * dt + 0.5 * a_.y * dt * dt;
  }




};

#endif