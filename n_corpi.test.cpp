#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "n_corpi.hpp"

#include "doctest.h"

TEST_CASE("Testing the norm function for vector") {
  {
    Vector v{2.0, -3.0};

    CHECK(v.norm() == std::sqrt(13.));
    CHECK(v.norm() == std::sqrt(v.get_x() * v.get_x() + v.get_y() * v.get_y()));
  }
}

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
    Universe u{1., 0.005, 1.e-2};

    Body b1({0.0, 0.0}, {3.0, 4.0}, 1., 1.);
    u.add(b1);

    Body b2({0.1, 0.2}, {2.0, 3.0}, 1., 1.);
    u.add(b2);

    Body b3({-1.5, 2.3}, {0.2, -4.2}, 1., 1.);
    u.add(b3);

    CHECK(u.size() == 3);
  }

  {
    Universe u{1., 0.005, 1.e-2};

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
    Universe u{6.67e-11, 0.005, 1.e5};

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

    Universe u{6.67e-11, 0.005, 1.e5};

    u.add(b0);
    u.add(b1);

    u.r_t_complete();

    CHECK(u.get_body(0).get_r() == Vector{0.015, 0.02});
    CHECK(u.get_body(1).get_r() == Vector{1.01, -2.005});
  }
}

TEST_CASE("Testing an entire simulation") {
  {
    Universe u{6.67e-11, 0.005, 1.e5};

    Body b0({0., 0.}, {0., 0.}, 2. * pow(10, 24), 1.);
    u.add(b0);

    Body b1({2. * pow(10, 7), 0.}, {0., 0.}, 3. * pow(10, 24), 1.);
    u.add(b1);
    u.set_a_0();
    u.simulation_steps(1);

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
    Universe u{6.67e-11, 0.005, 1.e5};

    Body b0({0., 0.}, {0., 0.}, 1. * pow(10, 24), 1.);
    u.add(b0);

    Body b1({1 * pow(10, 7), 0.}, {0., 0.}, 1. * pow(10, 22), 1.);
    u.add(b1);

    Body b2({2 * pow(10, 7), 0.}, {0., 0.}, 1. * pow(10, 20), 1.);
    u.add(b2);
    u.set_a_0();
    u.simulation_steps(10);

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
    Universe u{6.67e-11, 0.005, 1.e5};

    Body b0({0., 0.}, {0., 0.}, 1. * pow(10, 24), 1.);
    u.add(b0);

    Body b1({1 * pow(10, 7), 0.}, {0., 0.}, 1. * pow(10, 22), 1.);
    u.add(b1);

    Body b2({2 * pow(10, 7), 0.}, {0., 0.}, 1. * pow(10, 20), 1.);
    u.add(b2);
    u.set_a_0();
    u.simulation_steps(1000);

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
    Universe u{6.67e-11, 0.005, 1.e5};

    Body b0({0., 0.}, {0., 0.}, 1. * pow(10, 24), 1.);
    u.add(b0);

    Body b1({0., 50000.}, {0., 0.}, 1. * pow(10, 22), 1.);
    u.add(b1);

    u.set_a_0();
    u.simulation_steps(100);

    CHECK(u.get_body(0).get_r().get_y() == doctest::Approx(34.138356));
    CHECK(u.get_body(0).get_v().get_y() == doctest::Approx(139.8771175));

    CHECK(u.get_body(1).get_r().get_y() == doctest::Approx(46586.164389));
    CHECK(u.get_body(1).get_v().get_y() == doctest::Approx(-13987.711753));
  }
}

TEST_CASE("Testing a system with velocities on x and y") {
  {
    Universe u{6.67e-11, 0.005, 1.e5};

    Body b0({0., 0.}, {10., -5.}, 1. * pow(10, 24), 1.);
    u.add(b0);

    Body b1({1. * pow(10, 7), 5. * pow(10, 6)}, {-20., 300.}, 1. * pow(10, 22),
            1.);
    u.add(b1);
    u.set_a_0();
    u.simulation_steps(10);

    CHECK(u.get_body(0).get_r().get_x() ==
          doctest::Approx(5.00005966 * pow(10, -1)));
    CHECK(u.get_body(0).get_r().get_y() ==
          doctest::Approx(-2.4999701708 * pow(10, -1)));

    CHECK(u.get_body(0).get_v().get_x() ==
          doctest::Approx(1.0000238633 * pow(10, 1)));

    CHECK(u.get_body(0).get_v().get_y() == doctest::Approx(-4.9998806833));

    CHECK(u.get_body(0).get_a().get_x() ==
          doctest::Approx(4.7726557600 * pow(10, -3)));

    CHECK(u.get_body(0).get_a().get_y() ==
          doctest::Approx(2.3863355163 * pow(10, -3)));

    CHECK(u.get_body(1).get_r().get_x() ==
          doctest::Approx(9.99999 * pow(10, 6)));
    CHECK(u.get_body(1).get_r().get_y() ==
          doctest::Approx(5.000015 * pow(10, 6)));

    CHECK(u.get_body(1).get_v().get_x() ==
          doctest::Approx(-2.0023863298 * pow(10, 1)));

    CHECK(u.get_body(1).get_v().get_y() ==
          doctest::Approx(2.9998806833 * pow(10, 2)));

    CHECK(u.get_body(1).get_a().get_x() ==
          doctest::Approx(-4.7726557600 * pow(10, -1)));

    CHECK(u.get_body(1).get_a().get_y() ==
          doctest::Approx(-2.3863355163 * pow(10, -1)));
  }
}

TEST_CASE("Testing collisions") {
  {
    Universe u{1., 0.005, 1e-2};

    Body b0({-1., -1.}, {0., 0.}, 1., 0.1);
    u.add(b0);

    Body b1({1., 1.}, {0., 0.}, 1., 0.1);
    u.add(b1);

    u.set_universe_0();
    u.set_a_0();
    u.set_energies();

    for (int i{0}; i < 742; ++i) {
      u.single_simulation_step();
    }

    CHECK(u.size() == 2);
    CHECK(u.get_body(0).get_v().get_x() == -u.get_body(1).get_v().get_x());
    CHECK(u.get_body(0).get_v().get_y() == -u.get_body(1).get_v().get_y());

    for (int i{0}; i < 3; ++i) {
      u.single_simulation_step();
    }
    CHECK(u.size() == 1);
    CHECK(u.get_body(0).get_v().get_x() == 0.0);
    CHECK(u.get_body(0).get_a().get_x() == 0.0);

    CHECK(u.get_body(0).get_v().get_y() == 0.0);
    CHECK(u.get_body(0).get_a().get_y() == 0.0);

    CHECK(u.get_body(0).get_k() == 0.2);
  }
}

TEST_CASE("Testing conservations (no collisions)") {
  {
    Universe u{1., 0.005, 5.e-3};
    Body b0({-0.5, 0.}, {0., -0.7071067812}, 1., 0.05);
    u.add(b0);

    Body b1({0.5, 0.}, {0., 0.7071067812}, 1., 0.05);
    u.add(b1);

    u.set_universe_0();
    u.set_a_0();
    u.set_energies();

    for (int i{0}; i < 1000; ++i) {
      u.single_simulation_step();
    }

    CHECK(u.get_K_0() == doctest::Approx(0.5));
    CHECK(u.get_U_0() == doctest::Approx(-0.999987502));
    CHECK(u.get_E_0() == doctest::Approx(-0.4999875002));
    CHECK(u.get_P_0().get_x() == doctest::Approx(0.));
    CHECK(u.get_P_0().get_y() == doctest::Approx(0.));
    CHECK(u.get_L_0() == doctest::Approx(0.7071067812));

    CHECK(u.get_K_() == doctest::Approx(0.4999852783));
    CHECK(u.get_U_() == doctest::Approx(-0.9999727784));
    CHECK(u.get_E_() == doctest::Approx(-0.4999875000));
    CHECK(u.get_P_().get_x() == doctest::Approx(0.));
    CHECK(u.get_P_().get_y() == doctest::Approx(0.));
    CHECK(u.get_L_() == doctest::Approx(0.7071067812));
  }
}

TEST_CASE("Testing conservations (collisions)") {
  {
    Universe u{1., 0.005, 5.e-3};

    Body b0({-1., 0.}, {1., 0.}, 1., 0.1);
    u.add(b0);

    Body b1({1., 0.}, {-1., 0.}, 1., 0.1);
    u.add(b1);

    u.set_universe_0();
    u.set_a_0();
    u.set_energies();

    for (int i{0}; i < 145; ++i) {
      u.single_simulation_step();
    }

    CHECK(u.get_K_0() == doctest::Approx(1.));
    CHECK(u.get_U_0() == doctest::Approx(-0.5));
    CHECK(u.get_E_0() == doctest::Approx(0.5));
    CHECK(u.get_P_0().get_x() == doctest::Approx(0.));
    CHECK(u.get_P_0().get_y() == doctest::Approx(0.));
    CHECK(u.get_L_0() == doctest::Approx(0.));

    CHECK(u.get_K_() == doctest::Approx(5.5594088522));
    CHECK(u.get_U_() == doctest::Approx(-5.0462410119));
    CHECK(u.get_E_() == doctest::Approx(0.5131678402));
    CHECK(u.get_P_().get_x() == doctest::Approx(0.));
    CHECK(u.get_P_().get_y() == doctest::Approx(0.));
    CHECK(u.get_L_() == doctest::Approx(0.));
  }
}

/*
TEST_CASE("Testing Lagrange L1 point stability on three bodies") {
  {
    Body b0({-1.0, 0.0}, {0.0, -0.2}, 100.0, 0.1);
    Body b1({1.0, 0.0}, {0.0, 0.2}, 1.0, 0.1);
    Body b2({0.704, 0.0}, {0.0, 0.141}, 0.0001, 0.1);

    Universe u{};

    u.add(b0);
    u.add(b1);
    u.add(b2);

    u.r_t_complete();


    CHECK(u.get_body(0).get_r() == Vector{-1.0, -0.001});
    CHECK(u.get_body(1).get_r() == Vector{1.0, 0.001});
    CHECK(u.get_body(2).get_r() == Vector{0.704, 0.000705});

    double distanza_iniziale_l1 = 99.0 - 77.0;  // 22.0

    Vector pos_b1_dopo = u.get_body(1).get_r();
    Vector pos_b2_dopo = u.get_body(2).get_r();

    // Calcoliamo la distanza tra i vettori (supponendo che la tua classe Vector
    // abbia un metodo di distanza o modulo)
    double distanza_dopo_passo = (pos_b1_dopo - pos_b2_dopo).norm();

    // Verifichiamo che la distanza sia rimasta invariata a meno di tolleranze
    // decimali minima (es. 0.001)
    CHECK(std::abs(distanza_dopo_passo - distanza_iniziale_l1) < 0.001);
  }

}
*/