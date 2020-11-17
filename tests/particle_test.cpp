#include <core/particle.h>

#include <catch2/catch.hpp>

TEST_CASE("Construction from velocity and position", "[constructor]") {
  size_t type = 0;
  glm::vec2 position(1, 2);
  glm::vec2 velocity(3, 4);
  ci::Color color = ci::Color("red");
  float radius = 10;
  double mass = 1;

  SECTION("Expected construction", "[position][velocity]") {
    idealgas::Particle particle(type, position, velocity, color, radius, mass);
    REQUIRE(particle.GetPosition().x == 1);
    REQUIRE(particle.GetPosition().y == 2);
    REQUIRE(particle.GetVelocity().x == 3);
    REQUIRE(particle.GetVelocity().y == 4);
  }

  SECTION("Construction from vectors with special value", "[edge-case]") {
    SECTION("Construction from 0 value vector") {
      SECTION("Position zero vector","[position]") {
        glm::vec2 zero_pos(0,0);
        idealgas::Particle particle(type, zero_pos, velocity, color, radius, mass);
        REQUIRE(particle.GetPosition().x == 0);
        REQUIRE(particle.GetPosition().y == 0);
        REQUIRE(particle.GetVelocity().x == 3);
        REQUIRE(particle.GetVelocity().y == 4);
      }

      SECTION("Velocity zero vector","[velocity]") {
        glm::vec2 zero_vel(0,0);
        idealgas::Particle particle(type, position, zero_vel, color, radius, mass);
        REQUIRE(particle.GetPosition().x == 1);
        REQUIRE(particle.GetPosition().y == 2);
        REQUIRE(particle.GetVelocity().x == 0);
        REQUIRE(particle.GetVelocity().y == 0);
      }
    }

    SECTION("Construction from negative value vector") {
      SECTION("Position zero vector","[position]") {
        glm::vec2 negative_pos(-5,-10);
        idealgas::Particle particle(type, negative_pos, velocity, color, radius, mass);
        REQUIRE(particle.GetPosition().x == -5);
        REQUIRE(particle.GetPosition().y == -10);
        REQUIRE(particle.GetVelocity().x == 3);
        REQUIRE(particle.GetVelocity().y == 4);
      }

      SECTION("Velocity zero vector","[velocity]") {
        glm::vec2 negative_vel(-20,-30);
        idealgas::Particle particle(type, position, negative_vel, color, radius, mass);
        REQUIRE(particle.GetPosition().x == 1);
        REQUIRE(particle.GetPosition().y == 2);
        REQUIRE(particle.GetVelocity().x == -20);
        REQUIRE(particle.GetVelocity().y == -30);
      }
    }
  }

  SECTION("Construction from default vector") {
    SECTION("Position default vector","[position]") {
      glm::vec2 empty_pos;
      idealgas::Particle particle(type, empty_pos, velocity, color, radius, mass);
      REQUIRE(particle.GetPosition().x == 0);
      REQUIRE(particle.GetPosition().y == 0);
      REQUIRE(particle.GetVelocity().x == 3);
      REQUIRE(particle.GetVelocity().y == 4);
    }

    SECTION("Velocity default vector","[velocity]") {
      glm::vec2 empty_vel;
      idealgas::Particle particle(type, position, empty_vel, color, radius, mass);
      REQUIRE(particle.GetPosition().x == 1);
      REQUIRE(particle.GetPosition().y == 2);
      REQUIRE(particle.GetVelocity().x == 0);
      REQUIRE(particle.GetVelocity().y == 0);
    }
  }
}

TEST_CASE("Process particle movement","[position]") {
  size_t type = 0;
  glm::vec2 position(1, 2);
  glm::vec2 velocity(3, 4);
  ci::Color color = ci::Color("red");
  float radius = 10;
  double mass = 1;

  SECTION("Expected process movement") {
    idealgas::Particle particle(type, position, velocity, color, radius, mass);

    SECTION("After 1 iteration of process movement") {
      particle.ProcessMovement();
      REQUIRE(particle.GetPosition().x == 4);
      REQUIRE(particle.GetPosition().y == 6);
    }

    SECTION("After multiple iterations of process movement") {
      particle.ProcessMovement();
      particle.ProcessMovement();
      particle.ProcessMovement();
      REQUIRE(particle.GetPosition().x == 10);
      REQUIRE(particle.GetPosition().y == 14);
    }
  }

  SECTION("Process movement with velocity with special value", "[edge-case]") {
    SECTION("Zero velocity") {
      glm::vec2 zero_vel(0, 0);
      idealgas::Particle particle(type, position, zero_vel, color, radius, mass);
      particle.ProcessMovement();
      particle.ProcessMovement();
      particle.ProcessMovement();
      REQUIRE(particle.GetPosition().x == 1);
      REQUIRE(particle.GetPosition().y == 2);
    }

    SECTION("Negative velocity") {
      glm::vec2 negative_vel(-1, -2);
      idealgas::Particle particle(type, position, negative_vel, color, radius, mass);
      particle.ProcessMovement();
      particle.ProcessMovement();
      particle.ProcessMovement();
      REQUIRE(particle.GetPosition().x == -2);
      REQUIRE(particle.GetPosition().y == -4);
    }
  }
}

TEST_CASE("Process X wall collision", "[velocity][wall][collision]") {
  SECTION("Collision occurs negates X velocity") {
    size_t type = 0;
    ci::Color color = ci::Color("red");
    float radius = 10;
    double mass = 1;

    SECTION("Particle right of wall") {
      glm::vec2 position(1, 2);
      glm::vec2 velocity(-3, -4);

      idealgas::Particle particle(type, position, velocity, color, radius, mass);

      // abs(position - wall_pos) = 9
      // radius = 10
      particle.ProcessXWallCollision(-8);
      REQUIRE(particle.GetVelocity().x == 3);
      REQUIRE(particle.GetVelocity().y == -4);
    }

    SECTION("Particle left of wall") {
      glm::vec2 position(1, 2);
      glm::vec2 velocity(3, 4);
      idealgas::Particle particle(type, position, velocity, color, radius, mass);

      // abs(position - wall_pos) = 9
      // radius = 10
      particle.ProcessXWallCollision(10);
      REQUIRE(particle.GetVelocity().x == -3);
      REQUIRE(particle.GetVelocity().y == 4);
    }
  }

  SECTION("Collision does not occur") {
    size_t type = 0;
    ci::Color color = ci::Color("red");
    float radius = 10;
    double mass = 1;

    SECTION("Distance from wall larger than radius") {
      glm::vec2 position(1, 2);
      glm::vec2 velocity(3, 4);
      idealgas::Particle particle(type, position, velocity, color, radius, mass);

      // abs(position - wall_pos) = 11
      // radius = 10
      particle.ProcessXWallCollision(12);
      REQUIRE(particle.GetVelocity().x == 3);
      REQUIRE(particle.GetVelocity().y == 4);
    }

    SECTION("Distance within radius but moving away from wall") {
      glm::vec2 position(1, 2);
      glm::vec2 velocity(-3, -4);
      idealgas::Particle particle(type, position, velocity, color, radius, mass);

      // abs(position - wall_pos) = 6
      // radius = 10
      particle.ProcessXWallCollision(7);
      REQUIRE(particle.GetVelocity().x == -3);
      REQUIRE(particle.GetVelocity().y == -4);
    }
  }
}

TEST_CASE("Process Y wall collision", "[velocity][wall][collision]") {
  SECTION("Collision occurs negates Y velocity") {
    size_t type = 0;
    ci::Color color = ci::Color("red");
    float radius = 10;
    double mass = 1;

    SECTION("Particle above wall") {
      glm::vec2 position(1, 2);
      glm::vec2 velocity(-3, -4);
      idealgas::Particle particle(type, position, velocity, color, radius, mass);

      // abs(position - wall_pos) = 9
      // radius = 10
      particle.ProcessYWallCollision(-7);
      REQUIRE(particle.GetVelocity().x == -3);
      REQUIRE(particle.GetVelocity().y == 4);
    }

    SECTION("Particle below wall") {
      glm::vec2 position(1, 2);
      glm::vec2 velocity(3, 4);
      idealgas::Particle particle(type, position, velocity, color, radius, mass);

      // abs(position - wall_pos) = 9
      // radius = 10
      particle.ProcessYWallCollision(11);
      REQUIRE(particle.GetVelocity().x == 3);
      REQUIRE(particle.GetVelocity().y == -4);
    }
  }

  SECTION("Collision does not occur") {
    size_t type = 0;
    ci::Color color = ci::Color("red");
    float radius = 10;
    double mass = 1;

    SECTION("Distance from wall larger than radius") {
      glm::vec2 position(1, 2);
      glm::vec2 velocity(3, 4);
      idealgas::Particle particle(type, position, velocity, color, radius, mass);

      // abs(position - wall_pos) = 11
      // radius = 10
      particle.ProcessYWallCollision(13);
      REQUIRE(particle.GetVelocity().x == 3);
      REQUIRE(particle.GetVelocity().y == 4);
    }

    SECTION("Distance within radius but moving away from wall") {
      glm::vec2 position(1, 2);
      glm::vec2 velocity(-3, -4);
      idealgas::Particle particle(type, position, velocity, color, radius, mass);

      // abs(position - wall_pos) = 6
      // radius = 10
      particle.ProcessYWallCollision(8);
      REQUIRE(particle.GetVelocity().x == -3);
      REQUIRE(particle.GetVelocity().y == -4);
    }
  }
}

TEST_CASE("Process two particle collision", "[velocity][particle][collision]") {
  size_t type = 0;
  ci::Color color = ci::Color("red");
  float radius = 10;
  double mass = 1;

  SECTION("Collision between particles changes velocity") {
    glm::vec2 position_a(1, 2);
    glm::vec2 velocity_a(3, 4);
    glm::vec2 position_b(5, 6);
    glm::vec2 velocity_b(-1, -1);
    idealgas::Particle particle_a(type, position_a, velocity_a, color, radius, mass);
    idealgas::Particle particle_b(type, position_b, velocity_b, color, radius, mass);

    SECTION("Expected collision between two particles") {
      // Distance between particles = sqrt((5 - 1) ^ 2 + (6 - 1) ^ 2)) = 5.66
      // Sum of radii = 20
      // (v1 - v2) . (x1 - x2) = [4, 5] . [-4, -4] = (4 * -4) + (5 * -4) = -36 < 0
      idealgas::CollideParticles(particle_a, particle_b);

      // v1' = [3, 4] - ([4, 5] . [-4, -4]) / (|[-4, -4]| ^ 2) * [-4, -4]
      // = [3, 4] - (-36/32) * [-4, -4]
      // = [3, 4] - [4.5, 4.5]
      // = [-1.5, -0.5]
      // v2' = [-1, -1] - ([-4, -5] . [4, 4]) / (|[4, 4]| ^ 2) * [4, 4]
      // = [-1, -1] - (-36/32) * [4, 4]
      // = [-1, -1] + [4.5, 4.5]
      // = [3.5, 3.5]
      REQUIRE(particle_a.GetVelocity().x == Approx(-1.5));
      REQUIRE(particle_a.GetVelocity().y == Approx(-0.5));
      REQUIRE(particle_b.GetVelocity().x == Approx(3.5));
      REQUIRE(particle_b.GetVelocity().y == Approx(3.5));
    }

    SECTION("Process collision with reversed Particle arguments produces same result") {
      idealgas::CollideParticles(particle_b, particle_a);
      REQUIRE(particle_a.GetVelocity().x == Approx(-1.5));
      REQUIRE(particle_a.GetVelocity().y == Approx(-0.5));
      REQUIRE(particle_b.GetVelocity().x == Approx(3.5));
      REQUIRE(particle_b.GetVelocity().y == Approx(3.5));
    }

    SECTION("Collision change is affected by particle mass", "[mass]") {
      double mass_a = 2;
      double mass_b = 8;
      particle_a = idealgas::Particle(type, position_a, velocity_a, color, radius, mass_a);
      particle_b = idealgas::Particle(type, position_b, velocity_b, color, radius, mass_b);
      idealgas::CollideParticles(particle_a, particle_b);
      // v1' = [3, 4] - ([4, 5] . [-4, -4]) / (|[-4, -4]| ^ 2) * [-4, -4] * (2 * 8 / 10)
      // = [3, 4] - (-36/32) * [-4, -4] * (16 / 10)
      // = [3, 4] - [7.2, 7.2]
      // = [-4.2, -3.2]
      // v2' = [-1, -1] - ([-4, -5] . [4, 4]) / (|[4, 4]| ^ 2) * [4, 4] * (4 / 10)
      // = [-1, -1] - (-36/32) * [4, 4] * (4 / 10)
      // = [-1, -1] + [1.8, 1.8]
      // = [0.8, 0.8]
      REQUIRE(particle_a.GetVelocity().x == Approx(-4.2));
      REQUIRE(particle_a.GetVelocity().y == Approx(-3.2));
      REQUIRE(particle_b.GetVelocity().x == Approx(0.8));
      REQUIRE(particle_b.GetVelocity().y == Approx(0.8));
    }

    SECTION("One particle of 0 mass", "[mass]") {
      double mass_a = 0;
      double mass_b = 8;
      particle_a = idealgas::Particle(type, position_a, velocity_a, color, radius, mass_a);
      particle_b = idealgas::Particle(type, position_b, velocity_b, color, radius, mass_b);
      idealgas::CollideParticles(particle_a, particle_b);
      // v1' = [3, 4] - ([4, 5] . [-4, -4]) / (|[-4, -4]| ^ 2) * [-4, -4] * (2 * 8 / 8)
      // = [3, 4] - (-36/32) * [-4, -4] * (16 / 10)
      // = [3, 4] - [9, 9]
      // = [-6, -5]
      // v2' = [-1, -1] - ([-4, -5] . [4, 4]) / (|[4, 4]| ^ 2) * [4, 4] * (0 / 10)
      // = [-1, -1] - (-36/32) * [4, 4] * (0 / 10)
      // = [-1, -1] + [0, 0]
      // = [-1, -1]
      REQUIRE(particle_a.GetVelocity().x == Approx(-6));
      REQUIRE(particle_a.GetVelocity().y == Approx(-5));
      REQUIRE(particle_b.GetVelocity().x == Approx(-1));
      REQUIRE(particle_b.GetVelocity().y == Approx(-1));
    }
  }

  SECTION("Collision does not occur") {
    SECTION("Distance larger than radius sum") {
      SECTION("X distance larger than radius sum") {
        glm::vec2 position_a(1, 1);
        glm::vec2 velocity_a(1, 1);
        glm::vec2 position_b(22, 1);
        glm::vec2 velocity_b(1, 1);

        idealgas::Particle particle_a(type, position_a, velocity_a, color, radius, mass);
        idealgas::Particle particle_b(type, position_b, velocity_b, color, radius, mass);
        // Distance between particles = 21
        // Sum of radii = 20
        REQUIRE(idealgas::CheckCollision(particle_a, particle_b) == false);
      }

      SECTION("Y distance larger than radius sum") {
        glm::vec2 position_a(1, 1);
        glm::vec2 velocity_a(1, 1);
        glm::vec2 position_b(1, 22);
        glm::vec2 velocity_b(1, 1);

        idealgas::Particle particle_a(type, position_a, velocity_a, color, radius, mass);
        idealgas::Particle particle_b(type, position_b, velocity_b, color, radius, mass);
        // Distance between particles = 21
        // Sum of radii = 20
        REQUIRE(idealgas::CheckCollision(particle_a, particle_b) == false);
      }

      SECTION("Euclidean distance larger than radius sum") {
        glm::vec2 position_a(1, 1);
        glm::vec2 velocity_a(1, 1);
        glm::vec2 position_b(16, 16);
        glm::vec2 velocity_b(1, 1);

        idealgas::Particle particle_a(type, position_a, velocity_a, color, radius, mass);
        idealgas::Particle particle_b(type, position_b, velocity_b, color, radius, mass);
        // Distance between particles = sqrt((16 - 1) ^ 2 + (16 - 1) ^ 2)) = 21.213
        // Sum of radii = 20

        REQUIRE(idealgas::CheckCollision(particle_a, particle_b) == false);
      }
    }

    SECTION("Distance within radius sum but moving away from each other") {
      glm::vec2 position_a(1, 1);
      glm::vec2 velocity_a(-2, -3);
      glm::vec2 position_b(10, 10);
      glm::vec2 velocity_b(1, 1);

      idealgas::Particle particle_a(type, position_a, velocity_a, color, radius, mass);
      idealgas::Particle particle_b(type, position_b, velocity_b, color, radius, mass);
      // Distance between particles = sqrt((10 - 1) ^ 2 + (10 - 1) ^ 2)) = 12.727
      // Sum of radii = 20
      // (v1 - v2) . (x1 - x2) = [-3, -4] . [-9, -9] = (-3 * -9) + (-4 * -9) = 63 > 0
      REQUIRE(idealgas::CheckCollision(particle_a, particle_b) == false);
    }

    SECTION("Relative movement edge case", "[edge-case]") {
      SECTION("Particles have identical positions") {
        glm::vec2 position_a(1, 2);
        glm::vec2 velocity_a(3, 4);
        glm::vec2 position_b(1, 2);
        glm::vec2 velocity_b(-3, -4);

        idealgas::Particle particle_a(type, position_a, velocity_a, color, radius, mass);
        idealgas::Particle particle_b(type, position_b, velocity_b, color, radius, mass);
        // Distance between particles = 0
        // Sum of radii = 20
        // (v1 - v2) . (x1 - x2) = [6, 8] . [0, 0] = 0

        REQUIRE(idealgas::CheckCollision(particle_a, particle_b) == false);
      }

      SECTION("Particles have identical velocities") {
        glm::vec2 position_a(1, 2);
        glm::vec2 velocity_a(3, 4);
        glm::vec2 position_b(3, 3);
        glm::vec2 velocity_b(3, 4);

        idealgas::Particle particle_a(type, position_a, velocity_a, color, radius, mass);
        idealgas::Particle particle_b(type, position_b, velocity_b, color, radius, mass);
        // Distance between particles = 2.24
        // Sum of radii = 20
        // (v1 - v2) . (x1 - x2) = [0, 0] . [2, 1] = 0
        REQUIRE(idealgas::CheckCollision(particle_a, particle_b) == false);
      }
    }
  }
}

TEST_CASE("Variable radii", "[radius]") {
  size_t type = 0;
  ci::Color color = ci::Color("red");
  float radius_a = 3;
  float radius_b = 3;
  double mass = 2;
  glm::vec2 position_a(1, 2);
  glm::vec2 velocity_a(3, 4);
  glm::vec2 position_b(5, 6);
  glm::vec2 velocity_b(-1, -1);
  idealgas::Particle particle_a(type, position_a, velocity_a, color, radius_a, mass);
  idealgas::Particle particle_b(type, position_b, velocity_b, color, radius_b, mass);

  SECTION("Expected collision between two particles") {
    // Distance between particles = sqrt((5 - 1) ^ 2 + (6 - 1) ^ 2)) = 5.66
    // Sum of radii = 6
    // (v1 - v2) . (x1 - x2) = [4, 5] . [-4, -4] = (4 * -4) + (5 * -4) = -36 < 0
    REQUIRE(idealgas::CheckCollision(particle_a, particle_b) == true);
  }

  SECTION("Particle does not collide with small particle at same position") {
    float radius_c = 2;
    idealgas::Particle particle_c(type, position_b, velocity_b, color, radius_c, mass);
    // Distance between particles = sqrt((5 - 1) ^ 2 + (6 - 1) ^ 2)) = 5.66
    // Sum of radii = 5 (DOES NOT COLLIDE)
    // (v1 - v2) . (x1 - x2) = [4, 5] . [-4, -4] = (4 * -4) + (5 * -4) = -36 < 0
    REQUIRE(idealgas::CheckCollision(particle_a, particle_c) == false);
  }

  SECTION("Particles of radii 0 do not collide") {
    glm::vec2 position(20, 20);
    glm::vec2 velocity_a(3, 4);
    glm::vec2 velocity_b(-1, -1);
    idealgas::Particle particle_d(type, position, velocity_a, color, 0, mass);
    idealgas::Particle particle_e(type, position, velocity_b, color, 0, mass);
    REQUIRE(idealgas::CheckCollision(particle_d, particle_e) == false);
  }
}