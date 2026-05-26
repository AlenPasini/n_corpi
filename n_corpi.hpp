struct Vector {
  double x;
  double y;
};


bool operator==(Vector const& v1, Vector const& v2);

class Body {
 private:
  Vector r_;
  Vector v_;
  Vector a_;
 public:
  Body(Vector r, Vector v) : r_{r}, v_{v} {}
  Vector get_r();
  Vector get_v();

  Vector &operator+=(Vector const &v);

};

Vector operator+(Vector const &v1, Vector const &v2);