#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "n_corpi.cpp"

#include "doctest.h"

TEST_CASE("Testing della classe che gestisce la creazione di un corpo") {
  {
    Posizione r{1.0, 2.0};
    Velocità v{3.0, 4.0};

    Corpo c(r, v);

    CHECK(c.get_r() == (1.0, 2.0));
    CHECK(c.get_v() == (3.0, 4.0));
  }