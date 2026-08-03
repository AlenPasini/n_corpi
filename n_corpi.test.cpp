#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "n_corpi.cpp"

#include "doctest.h"

TEST_CASE("Testing position increment") {
  {
    Vector r{0.0, 0.0};
    Vector v{3.0, 4.0};

    Body b(r, v, 1.);

    b.r_t();

    CHECK(b.get_r() == Vector{0.015, 0.020});
  }
}

TEST_CASE("Testing the addition of the position to a body") {
  {
    Body b1({0.0, 0.0}, {3.0, 4.0}, 1.);
    CHECK(b1.get_id() == -1);
  }

  {
    Body b1({0.0, 0.0}, {3.0, 4.0}, 1.);
    b1.add_id(2);
    CHECK(b1.get_id() == 2);
  }
}

TEST_CASE("Testing the Universe class") {
  {
    Universe u{};

    Body b1({0.0, 0.0}, {3.0, 4.0}, 1.);
    u.add(b1);

    Body b2({0.1, 0.2}, {2.0, 3.0}, 1.);
    u.add(b2);

    Body b3({-1.5, 2.3}, {0.2, -4.2}, 1.);
    u.add(b3);

    CHECK(u.size() == 3);
  }

  {
    Universe u{};

    Body b1({0.0, 0.0}, {3.0, 4.0}, 1.);
    u.add(b1);

    Body b2({0.1, 0.2}, {2.0, 3.0}, 1.);
    u.add(b2);

    CHECK(u.get_body(1).get_v() == Vector{2.0, 3.0});
    CHECK(u.get_body(1).get_r() == Vector{0.1, 0.2});
    CHECK(u.get_body(1).get_id() == 1);
    CHECK(u.get_body(1).get_r().get_x() == 0.1);
  }
}


TEST_CASE("Testing the acceleration function on two bodies") {
{
  Universe u{};

    Body b0({0., 0.}, {0., 0.}, 2.*pow(10, 24));
    u.add(b0);

    Body b1({2.*pow(10,7), 0.0}, {0., 0.}, 3.*pow(10, 24));
    u.add(b1);

    u.u_a_t(b0);
    u.u_a_t(b1);

    b0.r_t();
    b1.r_t();

    CHECK (b0.get_a_fut().get_x() == 0.50025);

    CHECK (b1.get_a_fut().get_x() == -0.3335);
}

}


/*
TEST_CASE("Testing the acceleration function on two bodies, step by step") {
{
  Universe u{};

    Body b1({0., 0.}, {0., 0.}, 2.*pow(10, 24));
    u.add(b1);

    Body b2({2.*pow(10,7), 0.0}, {0., 0.}, 3.*pow(10, 24));
    u.add(b2);

    CHECK (u.u_a_t_2(b2) == 8*pow(10,21));

}

}
*/

