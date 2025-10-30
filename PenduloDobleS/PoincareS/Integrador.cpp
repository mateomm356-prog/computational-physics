#include "Integrador.h"
#include <cmath>

void muevase(double y[4], double dt,
             double m1, double m2,
             double l1, double l2,
             double g, double b1, double b2) {
    double theta1 = y[0], theta2 = y[1];
    double P1 = y[2], P2 = y[3];

 // Variables auxiliares
    double D = theta1 - theta2;

    double a = (m1 + m2) * l1 * l1;
    double b = m2 * l1 * l2 * cos(D);
    double c = m2 * l2 * l2;
    double detM = a * c - b * b;

    // Inversa de M 
    double Minv11 = c / detM;
    double Minv12 = -b / detM;
    double Minv21 = -b / detM;
    double Minv22 = a / detM;

    // Velocidades angulares (qdot = Minv * P)
    double dth1 = Minv11 * P1 + Minv12 * P2;
    double dth2 = Minv21 * P1 + Minv22 * P2;

    // Gradiente del potencial
    double dVdth1 = (m1 + m2) * g * l1 * sin(theta1);
    double dVdth2 = m2 * g * l2 * sin(theta2);

    // Matriz C(θ) para disipación
    double C11 = (b1 + b2) * l1 * l1;
    double C12 = b2 * l1 * l2 * cos(D);
    double C21 = C12;
    double C22 = b2 * l2 * l2;

    // Términos disipativos: C * qdot
    double R1 = C11 * dth1 + C12 * dth2;
    double R2 = C21 * dth1 + C22 * dth2;

    // Ecuaciones de Hamilton modificadas
    double dP1 = -dVdth1 - R1;
    double dP2 = -dVdth2 - R2;

    // ---- Paso de integración (Euler-simpléctico con disipación explícita) ----
    // Primero actualizar momentos
    P1 += dP1 * dt;
    P2 += dP2 * dt;

    // Recalcular velocidades con nuevos momentos
    dth1 = Minv11 * P1 + Minv12 * P2;
    dth2 = Minv21 * P1 + Minv22 * P2;

    // Luego actualizar ángulos
    theta1 += dth1 * dt;
    theta2 += dth2 * dt;

    // Guardar resultados en y
    y[0] = theta1;
    y[1] = theta2;
    y[2] = P1;
    y[3] = P2;
}

