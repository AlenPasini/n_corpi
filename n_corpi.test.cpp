#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "n_corpi.cpp"

#include "doctest.h"

TEST_CASE("Testing position increment") {
  {
    Vector r{0.0, 0.0};
    Vector v{3.0, 4.0};

    Body b(r, v);

    b.r_t();

    CHECK(b.get_r() == Vector{0.015, 0.020});
  }
}

TEST_CASE("Testing the addition of the position to a body") {
  {
    Body b1({0.0, 0.0}, {3.0, 4.0});
    CHECK(b1.get_pos() == -1);
  }

  {
    Body b1({0.0, 0.0}, {3.0, 4.0});
    b1.add_pos(2);
    CHECK(b1.get_pos() == 2);
  }
}

TEST_CASE("Testing the Universe class") {
  {
    Universe u{};

    Body b1({0.0, 0.0}, {3.0, 4.0});
    u.add(b1);

    Body b2({0.1, 0.2}, {2.0, 3.0});
    u.add(b2);

    Body b3({-1.5, 2.3}, {0.2, -4.2});
    u.add(b3);

    CHECK(u.size() == 3);
  }
  
  {
    Universe u{};

    Body b1({0.0, 0.0}, {3.0, 4.0});
    u.add(b1);

    Body b2({0.1, 0.2}, {2.0, 3.0});
    u.add(b2);

    CHECK(b2.get_pos() == 2);
  }
}
