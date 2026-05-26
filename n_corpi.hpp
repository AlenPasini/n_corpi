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

double operator+=(Posizione r1 , Posizione r2);

double operator+=(Velocità v1 , Velocità v2);

double operator+=(Accelerazione a1 , Accelerazione a2);

class Corpo {
 private:
  Posizione r_;
  Velocità v_;
  Accelerazione a_;
 public:
  Corpo(Posizione r, Velocità v) : r_{r}, v_{v} {}
  Posizione get_r();
  Velocità get_v();
};