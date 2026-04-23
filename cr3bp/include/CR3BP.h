#pragma once
#include "Body.h"
#include <cmath>

// Encapsula los parámetros del sistema y las ecuaciones de movimiento del CR3BP
class CR3BP {
public:
    double mu;  // parámetro de masa: mu = m2/(m1+m2)

    explicit CR3BP(double mu) : mu(mu) {}

    // Distancias de m3 a cada primario
    double r13(double x, double y) const {
        return std::sqrt((x + mu) * (x + mu) + y * y);
    }

    double r23(double x, double y) const {
        return std::sqrt((x - 1.0 + mu) * (x - 1.0 + mu) + y * y);
    }

    // Aceleraciones (lado derecho de las ecuaciones de movimiento)
    // ddot_x = 2*vy + x - (1-mu)*(x+mu)/r13^3 - mu*(x-1+mu)/r23^3
    // ddot_y = -2*vx + y - (1-mu)*y/r13^3     - mu*y/r23^3
    double ax(double x, double y, double vy) const {
        double r1 = r13(x, y);
        double r2 = r23(x, y);
        return 2.0 * vy + x
               - (1.0 - mu) * (x + mu)       / (r1 * r1 * r1)
               - mu         * (x - 1.0 + mu) / (r2 * r2 * r2);
    }

    double ay(double x, double y, double vx) const {
        double r1 = r13(x, y);
        double r2 = r23(x, y);
        return -2.0 * vx + y
               - (1.0 - mu) * y / (r1 * r1 * r1)
               - mu         * y / (r2 * r2 * r2);
    }

    // Integral de Jacobi (constante de movimiento, útil para verificar precisión)
    double jacobi(const Body& b) const {
        double r1 = r13(b.x, b.y);
        double r2 = r23(b.x, b.y);
        double v2 = b.vx * b.vx + b.vy * b.vy;
        return b.x * b.x + b.y * b.y
               + 2.0 * (1.0 - mu) / r1
               + 2.0 * mu / r2
               - v2;
    }
};
