#pragma once
#include "Body.h"
#include "CR3BP.h"

// Integrador simpléctico Leapfrog (Störmer-Verlet)
// Preserva la estructura hamiltoniana del sistema
class Integrator {
public:
    double h;  // paso de integración

    explicit Integrator(double step) : h(step) {}

    // Un paso Leapfrog:
    // 1. medio paso en velocidad
    // 2. paso completo en posición
    // 3. medio paso en velocidad
    void step(Body& b, const CR3BP& sys) const {
        // Aceleraciones al inicio del paso
        double ax0 = sys.ax(b.x, b.y, b.vy);
        double ay0 = sys.ay(b.x, b.y, b.vx);

        // Medio paso en velocidad
        double vx_half = b.vx + 0.5 * h * ax0;
        double vy_half = b.vy + 0.5 * h * ay0;

        // Paso completo en posición
        b.x += h * vx_half;
        b.y += h * vy_half;

        // Aceleraciones en la nueva posición
        double ax1 = sys.ax(b.x, b.y, vy_half);
        double ay1 = sys.ay(b.x, b.y, vx_half);

        // Medio paso final en velocidad
        b.vx = vx_half + 0.5 * h * ax1;
        b.vy = vy_half + 0.5 * h * ay1;
    }
};
