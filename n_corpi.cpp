// #include "n_corpi.hpp"

// #ifndef PF_N_CORPI_HPP. // provvisorio
// #define PF_N_CORPI_HPP

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
  Vector a_fut{0., 0.};
  double dt{0.005};

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

  void a_t() {}
};

class Universe {
  private:
  

}
// #endif