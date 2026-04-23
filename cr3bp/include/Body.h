#pragma once

// Representa el estado de la partícula m3 en el frame rotante
struct Body {
    double x, y;    // posición
    double vx, vy;  // velocidad

    Body(double x0, double y0, double vx0, double vy0)
        : x(x0), y(y0), vx(vx0), vy(vy0) {}
};
