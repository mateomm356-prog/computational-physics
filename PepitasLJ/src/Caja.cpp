#include "Caja.h"
#include "Bola.h"    // necesitamos definición de Bola para acceder a getters/setters
#include <Eigen/Dense>

using Eigen::Vector2d;

void Caja::resolverChoquePared(Bola &b) const {
    Vector2d r = b.getPosicion();
    Vector2d v = b.getVelocidad();
    double R = b.getRadio();

    // X
    if (r.x() - R < x_min_) {
        r.x() = x_min_ + R;
        v.x() = -v.x();
    } else if (r.x() + R > x_max_) {
        r.x() = x_max_ - R;
        v.x() = -v.x();
    }

    // Y
    if (r.y() - R < y_min_) {
        r.y() = y_min_ + R;
        v.y() = -v.y();
    } else if (r.y() + R > y_max_) {
        r.y() = y_max_ - R;
        v.y() = -v.y();
    }

    b.setPosicion(r);
    b.setVelocidad(v);
}

void Caja::resolverChoquesParedMultiple(std::vector<Bola> &bolas) const {
    for (auto &b : bolas) resolverChoquePared(b);
}



    
