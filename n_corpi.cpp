#include "n_corpi.hpp"

#ifndef PF_N_CORPI_HPP. //provvisorio
#define PF_N_CORPI_HPP

struct Posizione {
  double x;
  double y;
};

struct Velocità {
  double x;
  double y;
};

struct Accelerazione {
  double x;
  double y;
};

double operator+=(Posizione r1 , Posizione r2){
  return r1.x + r2.x && r1.y + r2.y;
}

double operator+=(Velocità v1 , Velocità v2){
  return v1.x + v2.x && v1.y + v2.y;
}

double operator+=(Accelerazione a1 , Accelerazione a2){
  return a1.x + a2.x && a1.y + a2.y;
}

class Corpo {
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