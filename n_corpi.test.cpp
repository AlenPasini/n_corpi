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

TEST_CASE("Testing the addition of the id to a body") {
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

TEST_CASE("Testin a simulation with many iterazrions") {
  {
    Universe u{};

    Body b0({0., 0.}, {0., 0.}, 1. * pow(10, 24));
    u.add(b0);

    Body b1({1 * pow(10, 7), 0.}, {0., 0.}, 1. * pow(10, 22));
    u.add(b1);

    Body b2({2 * pow(10, 7), 0.}, {0., 0.}, 1. * pow(10, 20));
    u.add(b2);

    u.simulation(1000);

    CHECK(u.get_body(0).get_r().get_x() ==
          doctest::Approx(8.3499877425 * pow(10, -2)));

    CHECK(u.get_circles()[0].getPosition().x ==
          doctest::Approx(8.3499877425 * pow(10, -7)));

    CHECK(u.get_body(1).get_r().get_x() ==
          doctest::Approx(9.9999916717 * pow(10, 6)));

    CHECK(u.get_body(2).get_r().get_x() ==
          doctest::Approx(1.9999997834 * pow(10, 7)));
    CHECK(u.get_body(2).get_a().get_x() ==
          doctest::Approx(-1.7342 * pow(10, -1)));
  }
  /*
    {
      Universe u{};

      Body b0({0., 0.}, {0., 0.}, 1. * pow(10, 24));
      u.add(b0);

      Body b1({50000., 0.}, {0., 25694.29}, 1. * pow(10, 22));
      u.add(b1);

      u.simulation(10);

      CHECK(u.get_body(0).get_r().get_x() == doctest::Approx(0.3335));
      CHECK(u.get_body(0).get_r().get_y() == doctest::Approx(2.828 *
    pow(10,-3)));

      CHECK(u.get_body(1).get_r().get_x() == doctest::Approx(49966.65));
      CHECK(u.get_body(1).get_r().get_y() == doctest::Approx(1284.43));
      CHECK(u.get_body(1).get_a().get_x() == doctest::Approx(-26689.5));
    }
      */
  /*
    {
      Universe u{};

      Body b0({0., 0.}, {0., 2.}, 5. * pow(10, 12));
      u.add(b0);

      Body b1({50., 0.}, {0., -1.5}, 3. * pow(10, 22));
      u.add(b1);

      Body b2({0., 80.}, {-1., 0.}, 4. * pow(10, 20));
      u.add(b2);

      u.simulation(10);

      CHECK(u.get_body(0).get_r().get_x() == doctest::Approx(0.0001));
      CHECK(u.get_body(0).get_r().get_y() == doctest::Approx(0.100052));

      CHECK(u.get_body(1).get_r().get_x() == doctest::Approx(49.999813));
      CHECK(u.get_body(1).get_r().get_y() == doctest::Approx(-0.074968));

      CHECK(u.get_body(2).get_r().get_x() == doctest::Approx(-0.049985));
      CHECK(u.get_body(2).get_r().get_y() == doctest::Approx(79.999911));
    }
      */
}

TEST_CASE("Testing the y component") {
  {
    Universe u{};

    Body b0({0., 0.}, {0., 0.}, 1. * pow(10, 24));
    u.add(b0);

    Body b1({0., 50000.}, {0., 0.}, 1. * pow(10, 22));
    u.add(b1);

    u.simulation(100);

    CHECK(u.get_body(0).get_v().get_y() == doctest::Approx(139.88));
    CHECK(u.get_body(0).get_r().get_y() ==
          doctest::Approx(34.14));

    CHECK(u.get_body(1).get_v().get_y() == doctest::Approx(-13987.71));
    CHECK(u.get_body(1).get_r().get_y() == doctest::Approx(46586.16));
  }
}

/*
TEST_CASE("Testing the conservation of mechanical energy") {
  {
    Universe u{};

    Body b0({0., 0.}, {1., 0.}, 3. * pow(10, 24));
    u.add(b0);

    Body b1({2 * pow(10, 7), 0.}, {-4., 0.}, 8. * pow(10, 22));
    u.add(b1);

    Body b2({4.5 * pow(10, 7), 0.}, {6., 0.}, 4. * pow(10, 21));
    u.add(b2);

    u.simulation(100);

    CHECK(u.get_K_0() == doctest::Approx(2.212 * pow(10, 24)));
    CHECK(u.get_U_0() == doctest::Approx(-8.1904042667 * pow(10, 29)));
    CHECK(u.get_E_0() == doctest::Approx(-8.1903821467 * pow(10, 29)));

    CHECK(u.get_K_() == doctest::Approx(2.3129430534 * pow(10, 24)));
    CHECK(u.get_U_() == doctest::Approx(-8.1904052810 * pow(10, 29)));
    CHECK(u.get_E_() == doctest::Approx(-8.1903821516 * pow(10, 29)));

    CHECK(u.get_E_0() == doctest::Approx(u.get_E_()));
  }
}
  */