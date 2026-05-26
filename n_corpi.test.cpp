#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "n_corpi.cpp"

#include "doctest.h"

TEST_CASE("Testing dell'incremento della posizione") {
  {
    Vector r{0.0, 0.0};
    Vector v{3.0, 4.0};

    Body c(r, v);

    c.r_t();

    CHECK(c.get_r() == Vector{0.015, 0.020});
  }
}

TEST_CASE("Testing dell'incremento della velocità")
