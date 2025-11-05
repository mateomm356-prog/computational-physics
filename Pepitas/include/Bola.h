#pragma once
#include <Eigen/Dense>

using Eigen::Vector2d;

class Caja; // forward

class Bola {
private:
    Vector2d r_;   // posición
    Vector2d v_;   // velocidad
    Vector2d a_;   // aceleración (para Verlet; por defecto cero)
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

    // integración (velocity verlet)
    // step1: v_half = v + 0.5*a*dt  -> aplicado dentro moverVerlet
    // moverVerlet realizará pasos apropiados (ver .cpp)
    void moverVerlet(double dt);

    // choque con la caja (usamos método de Caja)
    void rebotePared(const Caja &caja);

    // choque elástico par-a-par con 'otra' (resuelve impulso y corrige solapamiento)
    void choqueConOtraBola(Bola &otra);
};




