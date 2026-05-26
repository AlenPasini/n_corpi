#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "n_corpi.cpp"

#include "doctest.h"

TEST_CASE("Testing dell'incremento della posizione") {
  {
    Vettore r{0.0, 0.0};
    Vettore v{3.0, 4.0};

    Corpo c(r, v);

    c.r_t();

    CHECK(c.get_r() == Vettore{0.015, 0.020});
  }
}
