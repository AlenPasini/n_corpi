#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "n_corpi.cpp"

#include "doctest.h"

TEST_CASE("Testing position increment") {
  {
    Vector r{0.0, 0.0};
    Vector v{3.0, 4.0};

    Body b(r, v, 1., 1.);

    b.r_t();

    CHECK(b.get_r() == Vector{0.015, 0.020});
  }
}

TEST_CASE("Testing the addition of the id to a body") {
  {
    Body b1({0.0, 0.0}, {3.0, 4.0}, 1., 1.);
    CHECK(b1.get_id() == -1);
  }

  {
    Body b1({0.0, 0.0}, {3.0, 4.0}, 1., 1.);
    b1.add_id(2);
    CHECK(b1.get_id() == 2);
  }
}

TEST_CASE("Testing the Universe class") {
  {
    Universe u{};

    Body b1({0.0, 0.0}, {3.0, 4.0}, 1., 1.);
    u.add(b1);

    Body b2({0.1, 0.2}, {2.0, 3.0}, 1., 1.);
    u.add(b2);

    Body b3({-1.5, 2.3}, {0.2, -4.2}, 1., 1.);
    u.add(b3);

    CHECK(u.size() == 3);
  }

  {
    Universe u{};

    Body b1({0.0, 0.0}, {3.0, 4.0}, 1., 1.);
    u.add(b1);

    Body b2({0.1, 0.2}, {2.0, 3.0}, 1., 1.);
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

    Body b0({0., 0.}, {0., 0.}, 2. * pow(10, 24), 1.);
    u.add(b0);

    Body b1({2. * pow(10, 7), 0.0}, {0., 0.}, 3. * pow(10, 24), 1.);
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

    Body b0({0., 0.}, {0., 0.}, 2. * pow(10, 24), 1.);
    u.add(b0);

    Body b1({2. * pow(10, 7), 0.0}, {0., 0.}, 3. * pow(10, 24), 1.);
    u.add(b1);

    u.r_t_complete();

    u.u_a_t_complete();

    CHECK(u.get_body(0).get_a_fut().get_x() == 0.50025);

    CHECK(u.get_body(1).get_a_fut().get_x() == -0.3335);
  }
}

TEST_CASE("Testing the position increment universe function on two bodies") {
  {
    Body b0({0.0, 0.0}, {3.0, 4.0}, 1., 1.);
    Body b1({1.0, -2.0}, {2.0, -1.0}, 1., 1.);

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

    Body b0({0., 0.}, {0., 0.}, 2. * pow(10, 24), 1.);
    u.add(b0);

    Body b1({2. * pow(10, 7), 0.}, {0., 0.}, 3. * pow(10, 24), 1.);
    u.add(b1);

    u.simulation(1);

    CHECK(u.get_body(0).get_r().get_x() ==
          doctest::Approx(6.253125 * pow(10, -6)));
    CHECK(u.get_body(0).get_v().get_x() ==
          doctest::Approx(2.50125 * pow(10, -3)));
    CHECK(u.get_body(0).get_a().get_x() == doctest::Approx(0.50025));

    CHECK(u.get_body(1).get_r().get_x() == doctest::Approx(2. * pow(10, 7)));
    CHECK(u.get_body(1).get_v().get_x() ==
          doctest::Approx(-1.6675 * pow(10, -3)));
    CHECK(u.get_body(1).get_a().get_x() == doctest::Approx(-0.3335));
  }

  {
    Universe u{};

    Body b0({0., 0.}, {0., 0.}, 1. * pow(10, 24), 1.);
    u.add(b0);

    Body b1({1 * pow(10, 7), 0.}, {0., 0.}, 1. * pow(10, 22), 1.);
    u.add(b1);

    Body b2({2 * pow(10, 7), 0.}, {0., 0.}, 1. * pow(10, 20), 1.);
    u.add(b2);

    u.simulation(10);

    CHECK(u.get_body(0).get_r().get_x() ==
          doctest::Approx(8.35834 * pow(10, -6)));
    CHECK(u.get_body(0).get_v().get_x() ==
          doctest::Approx(3.3433375 * pow(10, -4)));
    CHECK(u.get_body(0).get_a().get_x() ==
          doctest::Approx(6.686675 * pow(10, -3)));

    CHECK(u.get_body(1).get_r().get_x() == doctest::Approx(1 * pow(10, 7)));
    CHECK(u.get_body(1).get_v().get_x() ==
          doctest::Approx(-3.3346665 * pow(10, -2)));
    CHECK(u.get_body(1).get_a().get_x() == doctest::Approx(-0.6669333));

    CHECK(u.get_body(2).get_r().get_x() == doctest::Approx(2 * pow(10, 7)));
    CHECK(u.get_body(2).get_v().get_x() ==
          doctest::Approx(-8.671 * pow(10, -3)));
    CHECK(u.get_body(2).get_a().get_x() == doctest::Approx(-0.17342));
  }
}

TEST_CASE("Testing a simulation with many iterations") {
  {
    Universe u{};

    Body b0({0., 0.}, {0., 0.}, 1. * pow(10, 24), 1.);
    u.add(b0);

    Body b1({1 * pow(10, 7), 0.}, {0., 0.}, 1. * pow(10, 22), 1.);
    u.add(b1);

    Body b2({2 * pow(10, 7), 0.}, {0., 0.}, 1. * pow(10, 20), 1.);
    u.add(b2);

    u.simulation(1000);

    CHECK(u.get_body(0).get_r().get_x() ==
          doctest::Approx(8.3583461 * pow(10, -2)));

    CHECK(u.get_body(1).get_r().get_x() ==
          doctest::Approx(9.9999916633 * pow(10, 6)));

    CHECK(u.get_body(2).get_r().get_x() ==
          doctest::Approx(1.9999997832 * pow(10, 7)));
  }
}

TEST_CASE("Testing the y component") {
  {
    Universe u{};

    Body b0({0., 0.}, {0., 0.}, 1. * pow(10, 24), 1.);
    u.add(b0);

    Body b1({0., 50000.}, {0., 0.}, 1. * pow(10, 22), 1.);
    u.add(b1);

    u.simulation(100);

    CHECK(u.get_body(0).get_r().get_y() == doctest::Approx(34.138356));
    CHECK(u.get_body(0).get_v().get_y() == doctest::Approx(139.8771175));

    CHECK(u.get_body(1).get_r().get_y() == doctest::Approx(46586.164389));
    CHECK(u.get_body(1).get_v().get_y() == doctest::Approx(-13987.711753));
  }
}

TEST_CASE("Testing a system with velocities on x and y") {
  {
    Universe u{};

    Body b0({0., 0.}, {10., -5.}, 1. * pow(10, 24), 1.);
    u.add(b0);

    Body b1({1. * pow(10, 7), 5. * pow(10, 6)}, {-20., 300.}, 1. * pow(10, 22),
            1.);
    u.add(b1);

    u.simulation(10);

    CHECK(u.get_body(0).get_r().get_x() ==
          doctest::Approx(5.00005966 * pow(10, -1)));
    CHECK(u.get_body(0).get_r().get_y() ==
          doctest::Approx(-2.4999701708 * pow(10, -1)));

    CHECK(u.get_body(0).get_v().get_x() == doctest::Approx(1.0000238633 * pow(10, 1)));

    CHECK(u.get_body(0).get_v().get_y() == doctest::Approx(-4.9998806833));

    CHECK(u.get_body(0).get_a().get_x() == doctest::Approx(4.7726557600 * pow(10, -3)));

    CHECK(u.get_body(0).get_a().get_y() == doctest::Approx(2.3863355163 * pow(10,-3)));



    CHECK(u.get_body(1).get_r().get_x() ==
          doctest::Approx(9.99999 * pow(10, 6)));
    CHECK(u.get_body(1).get_r().get_y() ==
          doctest::Approx(5.000015 * pow(10, 6)));

    CHECK(u.get_body(1).get_v().get_x() == doctest::Approx(-2.0023863298 * pow(10, 1)));

    CHECK(u.get_body(1).get_v().get_y() == doctest::Approx(2.9998806833 * pow(10,2)));

    CHECK(u.get_body(1).get_a().get_x() == doctest::Approx(-4.7726557600 * pow(10, -1)));

    CHECK(u.get_body(1).get_a().get_y() == doctest::Approx(-2.3863355163 * pow(10,-1)));

  }
}