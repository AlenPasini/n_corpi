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
  Posizione r_;
  Velocità v_;
  Accelerazione a_;

public:
  Corpo(Posizione r, Velocità v) : r_{r}, v_{v} {}
  Posizione get_r() { return r_; }
  Velocità get_v() { return v_; }
};

#endif