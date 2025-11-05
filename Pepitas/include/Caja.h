#pragma once
#include <vector>
#include <Eigen/Dense>

class Bola; // forward

class Caja {
private:
    double x_min_, x_max_, y_min_, y_max_;

public:
    // Crea caja desde (0,0) hasta (W,H) si se pasan W,H (sobrecarga disponible)
    Caja(double x_min, double x_max, double y_min, double y_max)
        : x_min_(x_min), x_max_(x_max), y_min_(y_min), y_max_(y_max) {}

    // Getters
    double getXmin() const { return x_min_; }
    double getXmax() const { return x_max_; }
    double getYmin() const { return y_min_; }
    double getYmax() const { return y_max_; }

    // Maneja choques con la pared para una sola bola
    void resolverChoquePared(Bola &b) const;

    // Maneja choques con la pared para múltiples bolas (itera y llama al método anterior)
    void resolverChoquesParedMultiple(std::vector<Bola> &bolas) const;
};

