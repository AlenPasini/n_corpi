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

    Body b0({0., 0.}, {0., 0.}, 2. * pow(10, 24));
    u.add(b0);

    Body b1({2. * pow(10, 7), 0.0}, {0., 0.}, 3. * pow(10, 24));
    u.add(b1);

    b0.r_t();
    b1.r_t();

    u.u_a_t(b0);
    u.u_a_t(b1);

    CHECK(b0.get_a_fut().get_x() == 0.50025);

    CHECK(b1.get_a_fut().get_x() == -0.3335);
  }

  {
    Universe u{};

    Body b0({0., 0.}, {0., 0.}, 2. * pow(10, 24));
    u.add(b0);

    Body b1({2. * pow(10, 7), 0.0}, {0., 0.}, 3. * pow(10, 24));
    u.add(b1);

    u.r_t_complete();

    u.u_a_t_complete();

    CHECK(u.get_body(0).get_a_fut().get_x() == 0.50025);

    CHECK(u.get_body(1).get_a_fut().get_x() == -0.3335);
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

TEST_CASE("Testing the position increment universe function on two bodies") {
  {
    Body b0({0.0, 0.0}, {3.0, 4.0}, 1.);
    Body b1({1.0, -2.0}, {2.0, -1.0}, 1.);

    Universe u{};

    u.add(b0);
    u.add(b1);

    u.r_t_complete();

    CHECK(u.get_body(0).get_r() == Vector{0.015, 0.02});
    CHECK(u.get_body(1).get_r() == Vector{1.01, -2.005});
  }
}

TEST_CASE("Testing an entire simulation") {
  {
    Universe u{};

    Body b0({0., 0.}, {0., 0.}, 2. * pow(10, 24));
    u.add(b0);

    Body b1({2. * pow(10, 7), 0.}, {0., 0.}, 3. * pow(10, 24));
    u.add(b1);

    u.simulation(1);

    CHECK(u.get_body(0).get_r().get_x() == doctest::Approx(0.));
    CHECK(u.get_body(0).get_v().get_x() ==
          doctest::Approx(1.250625 * pow(10, -3)));
    CHECK(u.get_body(0).get_a().get_x() == doctest::Approx(0.50025));

    CHECK(u.get_body(1).get_r().get_x() == doctest::Approx(2. * pow(10, 7)));
    CHECK(u.get_body(1).get_v().get_x() ==
          doctest::Approx(-8.3375 * pow(10, -4)));
    CHECK(u.get_body(1).get_a().get_x() == doctest::Approx(-0.3335));
  }

  {
    Universe u{};

    Body b0({0., 0.}, {0., 0.}, 1. * pow(10, 24));
    u.add(b0);

    Body b1({1 * pow(10, 7), 0.}, {0., 0.}, 1. * pow(10, 22));
    u.add(b1);

    u.simulation(2);

    CHECK(u.get_body(0).get_r().get_x() ==
          doctest::Approx(1.6675 * pow(10, -7)));
    CHECK(u.get_body(0).get_v().get_x() ==
          doctest::Approx(5.0025 * pow(10, -5)));
    CHECK(u.get_body(0).get_a().get_x() == doctest::Approx(6.67 * pow(10, -3)));

    CHECK(u.get_body(1).get_r().get_x() == doctest::Approx(1 * pow(10, 7)));
    CHECK(u.get_body(1).get_v().get_x() ==
          doctest::Approx(-5.0025 * pow(10, -3)));
    CHECK(u.get_body(1).get_a().get_x() == doctest::Approx(-0.667));
  }

  {
    Universe u{};

    Body b0({0., 0.}, {0., 0.}, 1. * pow(10, 24));
    u.add(b0);

    Body b1({1 * pow(10, 7), 0.}, {0., 0.}, 1. * pow(10, 22));
    u.add(b1);

    u.simulation(10);

    CHECK(u.get_body(0).get_r().get_x() ==
          doctest::Approx(7.5038 * pow(10, -6)));
    CHECK(u.get_body(0).get_v().get_x() ==
          doctest::Approx(3.1683 * pow(10, -4)));
    CHECK(u.get_body(0).get_a().get_x() == doctest::Approx(6.67 * pow(10, -3)));

    CHECK(u.get_body(1).get_r().get_x() == doctest::Approx(1 * pow(10, 7)));
    CHECK(u.get_body(1).get_v().get_x() ==
          doctest::Approx(-3.1683 * pow(10, -2)));
    CHECK(u.get_body(1).get_a().get_x() == doctest::Approx(-0.667));
  }

  {
    Universe u{};

    Body b0({0., 0.}, {0., 0.}, 1. * pow(10, 24));
    u.add(b0);

    Body b1({1 * pow(10, 7), 0.}, {0., 0.}, 1. * pow(10, 22));
    u.add(b1);

    Body b2({2 * pow(10, 7), 0.}, {0., 0.}, 1. * pow(10, 20));
    u.add(b2);

    u.simulation(10);

    CHECK(u.get_body(0).get_r().get_x() ==
          doctest::Approx(7.5225 * pow(10, -6)));
    CHECK(u.get_body(0).get_v().get_x() ==
          doctest::Approx(3.1761706 * pow(10, -4)));
    CHECK(u.get_body(0).get_a().get_x() ==
          doctest::Approx(6.686675 * pow(10, -3)));

    CHECK(u.get_body(1).get_r().get_x() == doctest::Approx(1 * pow(10, 7)));
    CHECK(u.get_body(1).get_v().get_x() ==
          doctest::Approx(-3.16793318 * pow(10, -2)));
    CHECK(u.get_body(1).get_a().get_x() == doctest::Approx(-0.6669333));

    CHECK(u.get_body(2).get_r().get_x() == doctest::Approx(2 * pow(10, 7)));
    CHECK(u.get_body(2).get_v().get_x() ==
          doctest::Approx(-8.23745 * pow(10, -3)));
    CHECK(u.get_body(2).get_a().get_x() == doctest::Approx(-0.17342));
  }
}
