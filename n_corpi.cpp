// #include "n_corpi.hpp"

// #ifndef PF_N_CORPI_HPP. // provvisorio
// #define PF_N_CORPI_HPP

#include <cmath>
#include <iostream>
#include <vector>

struct Vector {
  double x;
  double y;

  Vector &operator+=(Vector const &v) {
    x += v.x;
    y += v.y;
    return *this;
  }

  double get_x() { return x; }
  double get_y() { return y; }
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
  double m_;
  int id_{-1};
  double dt{0.005};
  double G{6.67 * pow(10., -11.)};
  double eps{pow(10., -12.)};

 public:
  Body(Vector r, Vector v, double m) : r_{r}, v_{v}, m_{m} {}
  Vector get_r() { return r_; }
  Vector get_v() { return v_; }
  Vector get_a() { return a_; }
  Vector get_a_fut() { return a_fut_; }
  double get_m() { return m_; }
  int get_id() const { return id_; }
  // deve essere const perché altrimenti non me la fa paragonare
  // nell'operatore ==
  double get_dt() { return dt; }

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

  void set_a_(Vector a_t_fut) {a_ = a_t_fut;}
};

bool operator==(Body const &b1, Body const &b2) {
  return b1.get_id() == b2.get_id();
}

class Universe {
 private:
  std::vector<Body> u_{};

  double dt{0.005};
  double G{6.67 * pow(10., -11.)};
  double eps{pow(10., -12.)};

 public:
  int size() { return u_.size(); }

  void add(Body &b) {
    u_.push_back(b);
    u_.back().add_id(u_.size() - 1);
  }

  Body get_body(int id) { return u_[id]; }

  double get_eps() { return eps; }
  double get_G() { return G; }

  // al momento ogni body prende un id identificativa quando viene
  // inserito dentro a Universe. Non ha quindi senso avere un body al di fuori
  // di Universe. Ogni body al di fuori di Universe ha id = -1, che non ha
  // senso.
  // Bisogna inserire un "require qualcosa??" Non lo so fare

  void u_a_t(Body &b) {
    double a_t_x = 0.0;
    double a_t_y = 0.0;

    for (size_t j{0}; j < u_.size(); ++j) {
      if (b == u_[j]) {
      } else {
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

  /*
    double u_a_t_2(Body &b) {
      double r_diff_square = 0.;

      for (size_t j{0}; j < u_.size()-1; ++j) {
        if (b.get_id() != u_[j].get_id()) {
          r_diff_square = u_[j].get_r().get_x();
        }
      }
      return r_diff_square;
    }
  */
  // usa il vettore posizione aggiornato. Non penso che influisca in qualche
  // modo sulla scrittura della funzione, ma è FONDAMENTALE che venga chiamato
  // prima l'aggiornaemnto della posizione e poi quello dell'accelerazione

  // deve prendere l'accelerazione del Body (Body in input?) e poi fare la
  // turbo sommatoria. Deve quindi poter accedere anche a tutti gli altri Body
  // e in particolare alla loro posizione all'interno del mega vector. Deve
  // infatti saltare lo step della sommatoria del Body in questione.

  void r_t_complete() {
    for (size_t i{0}; i < u_.size(); ++i) {
      u_[i].r_t();
    }
  }

  void u_a_t_complete() {
    for (size_t i{0}; i < u_.size(); ++i) {
      u_a_t(u_[i]);
    }
  }

  void u_a_v_complete() {
    for (size_t i{0}; i < u_.size(); ++i) {
      u_[i].v_t();
    }
  }

  void simulation(int steps) {
    for (int i{0}; i < steps; ++i) {
      this->r_t_complete();
      this->u_a_t_complete();
      this->u_a_v_complete();
      for (size_t j{0}; j < u_.size(); ++j) {
        u_[j].set_a_(u_[j].get_a_fut());
      }
    }
  }
  // non mi ricordo assolutamente se si fa così...
};
// #endif