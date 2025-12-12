#include <iostream>
#include <iomanip>
#include "OsciladorArmonicoC.h"

int main() {
    // Parámetros físicos
    double m = 1.0;
    double k = 1.0;
    double T = 100.0;   // k_B = 1 units

    // Parámetros Monte Carlo
    double Dx = 0.6;   // paso inicial en x (ajustar)
    double Dp = 0.4;   // paso inicial en p (ajustar)
    long long Nterm = 500000;   // pasos de termalización
    long long Nprod = 200000;  // pasos de producción

    // Crear objeto (seed opcional)
    OsciladorArmonicoC osc(m, k, T, Dx, Dp, Nterm, Nprod, 0);

    std::cout << "Iniciando simulación Metropolis (x,p) para oscilador armónico\n";
    std::cout << "m=" << m << "  k=" << k << "  T=" << T << "\n";
    std::cout << "Dx=" << Dx << "  Dp=" << Dp << "\n";
    std::cout << "Termalización: " << Nterm << " pasos\n";
    std::cout << "Producción:    " << Nprod << " pasos\n";

    const std::string filename = "results/oscilador_canonico.dat";
    osc.run(filename);

    // Resultados
    double mx2 = osc.mean_x2();
    double mp2 = osc.mean_p2();
    double mE = osc.mean_E();
    double acc = osc.acceptance_rate();

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "\n--- Resultados (estimados MC) ---\n";
    std::cout << "<x^2> = " << mx2 << "\n";
    std::cout << "<p^2> = " << mp2 << "\n";
    std::cout << "<E>   = " << mE << "\n";
    std::cout << "k/2 <x^2> = " << 0.5 * k * mx2 << "\n";
    std::cout << "1/(2m) <p^2> = " << 0.5 * mp2 / m << "\n";
    std::cout << "Teórico: (1/beta) = " << 1.0 / (1.0 / T) << "  (k_B T)\n"; // trivial con k_B=1
    std::cout << "Acceptance rate = " << acc * 100.0 << " %\n";

    std::cout << "\nDatos guardados en: " << filename << "\n";
    std::cout << "Nota: ajusta Dx y Dp para obtener acceptance ~ 30-60% si lo deseas.\n";

    return 0;
}
