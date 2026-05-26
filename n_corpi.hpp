struct Vettore {
  double x;
  double y;
};


bool operator==(Vettore const& v1, Vettore const& v2);

class Corpo {
 private:
  Vettore r_;
  Vettore v_;
  Vettore a_;
 public:
  Corpo(Vettore r, Vettore v) : r_{r}, v_{v} {}
  Vettore get_r();
  Vettore get_v();

  Vettore &operator+=(Vettore const &v);

};

Vettore operator+(Vettore const &v1, Vettore const &v2);