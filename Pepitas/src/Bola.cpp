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

// Velocity Verlet:
// v_half = v + 0.5 * a * dt
// r_new = r + v_half * dt
// compute a_new (here no external forces => a_new = 0 unless set externally)
// v_new = v_half + 0.5 * a_new * dt
void Bola::moverVerlet(double dt) {
    // half kick
    Vector2d v_half = v_ + 0.5 * a_ * dt;
    // drift
    r_ += v_half * dt;
    // here normally compute new acceleration a_new; in our case external a is zero,
    // but we keep the structure. If user sets a_ externally between steps, it will be used.
    Vector2d a_new = a_; // for now a_ (zero) — placeholder for forces
    // final kick
    v_ = v_half + 0.5 * a_new * dt;
    // set current acceleration to a_new (keeps consistency)
    a_ = a_new;
}

void Bola::rebotePared(const Caja &caja) {
    caja.resolverChoquePared(*this);
}

// Choque entre bolas: impulso elástico (e = 1)


void Bola::choqueConOtraBola(Bola &otra) {
    Vector2d dr = otra.r_ - r_;
    double dist = dr.norm();
    double minDist = radio_ + otra.radio_;

    // Evitar división por cero
    if (dist <= 1e-12) {
        // pequeña perturbación en x
        dr.x() = 1e-6;
        dist = dr.norm();
    }

    if (dist < minDist) {
        // normal unitario desde this hacia otra
        Vector2d n = dr / dist;

        // velocidad relativa medida como v_rel = (v1 - v2)·n
        Vector2d dv = v_ - otra.v_;
        double vrel_n = dv.dot(n);

        // si se están acercando (vrel_n > 0), entonces aplicar impulso
        if (vrel_n > 0) {
            double inv_m1 = 1.0 / masa_;
            double inv_m2 = 1.0 / otra.masa_;
            double e = 1.0; // restitución (1 = elástico)

            // impulso escalar (fórmula estándar)
            double j = - (1.0 + e) * vrel_n / (inv_m1 + inv_m2);

            // actualizar velocidades
            v_ += (j * inv_m1) * n;
            otra.v_ -= (j * inv_m2) * n;
        }

        // corrección de solapamiento (50/50)
        double overlap = minDist - dist;
        Vector2d corr = 0.5 * overlap * n;
        r_ -= corr;
        otra.r_ += corr;
    }
}


