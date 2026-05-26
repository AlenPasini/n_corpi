// #include "n_corpi.hpp"

// #ifndef PF_N_CORPI_HPP. // provvisorio
// #define PF_N_CORPI_HPP

#include <cmath>
#include <vector>

struct Vector {
  double x;
  double y;

  Vector &operator+=(Vector const &v) {
    x += v.x;
    y += v.y;
    return *this;
  }
};

Vector operator+(Vector const &v1, Vector const &v2) {
  auto result{v1};
  return result += v2;
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
  double dt{0.005};
  double G{6.67 * pow(10., -11.)};
  double eps { pow(10., -12.) };

 public:
  Body(Vector r, Vector v) : r_{r}, v_{v} {}
  Vector get_r() { return r_; }
  Vector get_v() { return v_; }
  Vector get_a() { return a_; }
  double get_dt() { return dt; }

  void r_t() {
    r_.x += v_.x * dt + 0.5 * a_.x * dt * dt;
    r_.y += v_.y * dt + 0.5 * a_.y * dt * dt;
  }

  void v_t() {
    v_.x += 0.5 * (a_.x + a_fut_.x) * dt;
    v_.y += 0.5 * (a_.y + a_fut_.y) * dt;
  }
};

class Universe {
 private:
  std::vector<Body> u_{};

 public:
  void add(Body const &b) { u_.push_back(b); }

  void a_t(...) {
    // deve prendere l'accelerazione del Body (Body in input?) e poi fare la
    // turbo sommatoria. Deve quindi poter accedere anche a tutti gli altri Body
    // e in particolare alla loro posizione all'interno del mega vector. Deve
    // infatti saltare lo step della sommatoria del Body in questione.
  }

  void a_t_complete(...) {
    // deve fare a_t su tutti i componenti del mega vector.
    // probabilmente non necessita nessun input...
    // dovrebbe semplicemente essere un mega ciclo di lunghezza len(u_) che fa n
    // volte a_t
  }

}
// #endif