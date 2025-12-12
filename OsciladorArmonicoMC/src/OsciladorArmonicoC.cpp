#include "OsciladorArmonicoC.h"

#include <cmath>
#include <chrono>
#include <fstream>
#include <iostream>
#include <filesystem> // C++17

// al inicio de OsciladorArmonicoC.cpp, después de includes
static constexpr double kB_SI = 1.380649e-23; // J/K


OsciladorArmonicoC::OsciladorArmonicoC(double m_, double k_, double T_,
                                       double Dx_, double Dp_,
                                       long long Nterm_, long long Nprod_,
                                       unsigned long long seed)
    : m(m_), k(k_), T(T_), Dx(Dx_), Dp(Dp_),
      Nterm(Nterm_), Nprod(Nprod_),
      x(0.02), p(0.01),
      uni01(0.0, 1.0),
      dist_dx(-Dx_, Dx_), dist_dp(-Dp_, Dp_),
      accepted(0), proposed(0),
      accepted_term(0), proposed_term(0),
      samples(0), written_samples(0),
      sum_x2(0.0), sum_p2(0.0), sum_E(0.0)
{
    // k_B = 1 units
    if (T <= 0.0) T = 1.0;
    beta = 1.0 / T;

    // seed RNG (if seed==0 use time-based seed)
    if (seed == 0) {
        seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    }
    rng.seed(seed);
}

double OsciladorArmonicoC::energy(double x_, double p_) const {
    return (p_*p_)/(2.0*m) + 0.5*k*x_*x_;
}

bool OsciladorArmonicoC::try_step() {
    // Propuesta simétrica
    std::uniform_real_distribution<double> dxdist(-Dx, Dx);
    std::uniform_real_distribution<double> dpdist(-Dp, Dp);

    double x_new = x + dxdist(rng);
    double p_new = p + dpdist(rng);

    double E_old = energy(x, p);
    double E_new = energy(x_new, p_new);

    double dE = E_new - E_old;

    proposed++;
    if (dE <= 0.0) {
        x = x_new;
        p = p_new;
        accepted++;
        return true;
    } else {
        double r = uni01(rng);
        double A = std::exp(-beta * dE);
        if (r < A) {
            x = x_new;
            p = p_new;
            accepted++;
            return true;
        } else {
            return false;
        }
    }
}

void OsciladorArmonicoC::run(const std::string &filename) {
    // Asegurar directorio results existe
    std::filesystem::path outpath(filename);
    if (outpath.has_parent_path()) {
        std::filesystem::create_directories(outpath.parent_path());
    }

    // Re-crear distribuciones dependientes de Dx, Dp
    dist_dx = std::uniform_real_distribution<double>(-Dx, Dx);
    dist_dp = std::uniform_real_distribution<double>(-Dp, Dp);

    // Termalización
    for (long long i = 0; i < Nterm; ++i) {
        // usamos try_step pero no acumulamos medidas
        bool acc = try_step();
        proposed_term++;
        if (acc) accepted_term++;
    }

    // Producción: abrimos archivo y guardamos configuraciones
    std::ofstream fout(filename);
    if (!fout) {
        std::cerr << "Error: no se pudo abrir " << filename << " para escribir.\n";
        return;
    }
    fout << "# x p E\n";

    for (long long i = 0; i < Nprod; ++i) {
        bool acc = try_step();
        // acumuladores para estadísticas (independientemente si fue aceptado)
        double E = energy(x, p);
        sum_x2 += x * x;
        sum_p2 += p * p;
        sum_E  += E;
        samples++;

        // guardar estado actual (puedes comentar si no quieres archivo grande)
        fout << x << " " << p << " " << E << "\n";
        written_samples++;
    }
    fout.close();
}

double OsciladorArmonicoC::mean_x2() const {
    return (samples > 0) ? (sum_x2 / static_cast<double>(samples)) : 0.0;
}
double OsciladorArmonicoC::mean_p2() const {
    return (samples > 0) ? (sum_p2 / static_cast<double>(samples)) : 0.0;
}
double OsciladorArmonicoC::mean_E() const {
    return (samples > 0) ? (sum_E / static_cast<double>(samples)) : 0.0;
}
double OsciladorArmonicoC::acceptance_rate() const {
    long long total_prop = proposed; // incluye termal + prod
    return (total_prop > 0) ? (static_cast<double>(accepted) / static_cast<double>(total_prop)) : 0.0;
}

void OsciladorArmonicoC::set_steps(double Dx_, double Dp_) {
    Dx = Dx_;
    Dp = Dp_;
    dist_dx = std::uniform_real_distribution<double>(-Dx, Dx);
    dist_dp = std::uniform_real_distribution<double>(-Dp, Dp);
}
