#include "n_corpi.cpp"

#include <fstream>
#include <iostream>

int main() {
  Universe u{};

  std::ifstream file("configurations.txt");

  if (!file) {
    std::cerr << "Cannot open file" << '\n';
    return 1;
  }

  double r_x;
  double r_y;
  double v_x;
  double v_y;
  double m;

  while (file >> r_x >> r_y >> v_x >> v_y >> m) {
    Vector r{r_x, r_y};
    Vector v{v_x, v_y};
    Body b{r, v, m};

    u.add(b);
  }

  file.close();

  u.set_U_0();

  std::cout << "The initial values of the Universe are:" << '\n';
  std::cout << "K = " << u.get_K_0() << '\n';
  std::cout << "U = " << u.get_U_0() << '\n';
  std::cout << "E = " << u.get_E_0() << '\n';
  std::cout << '\n';

  int n_step;

  std::cout << "Insert the number of steps you want to simulate: ";
  std::cin >> n_step;
  std::cout << '\n';

  u.simulation(n_step);

  std::cout << "The final values of the Universe are:" << '\n';
  std::cout << "K_final = " << u.get_K_() << '\n';
  std::cout << "U_final = " << u.get_U_() << '\n';
  std::cout << "E_final = " << u.get_E_() << '\n';

  std::cout << '\n';
  std::cout << "The difference between the initial and final total energy is "
               "equivalent to "
            << u.get_E_() - u.get_E_0() << " J" << '\n';
  std::cout << "The conservation of energy is thus confirmed" << '\n';
}

/*

int main() {
int n;
std::cout << "Insert the number of bodies to simulate: ";
std::cin >> n;

std::cout << '\n';

Universe u{};

for (int i{0}; i < n; ++i) {
  double x;
  double y;
  double m;

  std::cout << "Insert the initial position coordinates of the body. Leave a "
               "single space between the x and y coordinates: ";
  std::cin >> x;
  std::cin >> y;

  Vector r{x, y};

  std::cout << "Insert the initial velocity coordinates of the body. Leave a "
               "single space between the x and y coordinates: ";
  std::cin >> x;
  std::cin >> y;

  Vector v{x, y};

  std::cout << "Insert the mass of the body: ";
  std::cin >> m;
  std::cout << '\n';

  Body b{r, v, m};

  u.add(b);
}



u.set_U_0();

std::cout << "The initial values of the Universe are:" << '\n';
std::cout << "K = " << u.get_K_0() << '\n';
std::cout << "U = " << u.get_U_0() << '\n';
std::cout << "E = " << u.get_E_0() << '\n';
std::cout << '\n';

int n_step;

std::cout << "Insert the number of steps you want to simulate: ";
std::cin >> n_step;
std::cout << '\n';

u.simulation(n_step);

std::cout << "The final values of the Universe are:" << '\n';
std::cout << "K_final = " << u.get_K_() << '\n';
std::cout << "U_final = " << u.get_U_() << '\n';
std::cout << "E_final = " << u.get_E_() << '\n';

std::cout << '\n';
std::cout << "The difference between the initial and final total energy is "
             "equivalent to "
          << u.get_E_() - u.get_E_0() << " J" << '\n';
std::cout << "The conservation of energy is since confirmed" << '\n';
}
*/