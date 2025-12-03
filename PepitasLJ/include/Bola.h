#pragma once
#include <Eigen/Dense>

using Eigen::Vector2d;

class Caja; // forward

class Bola {
private:
    Vector2d r_;   // posición
    Vector2d v_;   // velocidad
    Vector2d a_;   // aceleración
    double masa_;
    double radio_;

public:
    // constructores
    Bola();
    Bola(const Vector2d &r0, const Vector2d &v0, double masa, double radio);

    // inicializar
    void inicie(const Vector2d &r0, const Vector2d &v0, double masa, double radio);

    // getters / setters
    Vector2d getPosicion() const { return r_; }
    Vector2d getVelocidad() const { return v_; }
    Vector2d getAceleracion() const { return a_; }
    double getMasa() const { return masa_; }
    double getRadio() const { return radio_; }

    void setPosicion(const Vector2d &r) { r_ = r; }
    void setVelocidad(const Vector2d &v) { v_ = v; }
    void setAceleracion(const Vector2d &a) { a_ = a; }
    void sumA(const Vector2d &da) { a_ += da; }

    // integración Verlet
    void moverVerlet(double dt);                      // half-kick + drift
    void actualizarVelocidadFinal(double dt, const Vector2d &a_new); // final kick

    // choque con la caja
    void rebotePared(const Caja &caja);

    // fuerza Lennard–Jones
    Vector2d fuerzaLJ(const Bola &otra, double epsilon0, double sigma) const;
};
