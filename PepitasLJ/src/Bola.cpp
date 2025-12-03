#include "Bola.h"
#include "Caja.h"
#include <cmath>

using Eigen::Vector2d;

// Constructores
Bola::Bola()
    : r_(Vector2d::Zero()), v_(Vector2d::Zero()), a_(Vector2d::Zero()),
      masa_(1.0), radio_(0.05) {}

Bola::Bola(const Vector2d &r0, const Vector2d &v0, double masa, double radio)
    : r_(r0), v_(v0), a_(Vector2d::Zero()), masa_(masa), radio_(radio) {}

void Bola::inicie(const Vector2d &r0, const Vector2d &v0, double masa, double radio) {
    r_ = r0;
    v_ = v0;
    masa_ = masa;
    radio_ = radio;
    a_.setZero();
}

// --------------------------------------------------
// Velocity Verlet
// --------------------------------------------------
// half-kick + drift
void Bola::moverVerlet(double dt) {
    v_ += 0.5 * a_ * dt;
    r_ += v_ * dt;
    // La aceleración nueva se calcula después en main()
}

// final kick
void Bola::actualizarVelocidadFinal(double dt, const Vector2d &a_new) {
    v_ += 0.5 * a_new * dt;
    a_ = a_new;
}

// choque con la caja
void Bola::rebotePared(const Caja &caja) {
    caja.resolverChoquePared(*this);
}

// fuerza Lennard–Jones
Vector2d Bola::fuerzaLJ(const Bola &otra, double epsilon0, double sigma) const {
    Vector2d dr = otra.r_ - r_;
    double r = dr.norm();
    if (r < 1e-12) r = 1e-12;

    double inv_r = sigma / r;
    double sr6 = inv_r * inv_r * inv_r; // (σ/r)^3
    sr6 = sr6 * sr6;                     // (σ/r)^6
    double sr12 = sr6 * sr6;             // (σ/r)^12

    double scalar = 24.0 * epsilon0 * (2.0 * sr12 - sr6) / (r * r);

    return scalar * dr;
}
