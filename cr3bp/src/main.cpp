#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include "Body.h"
#include "CR3BP.h"
#include "Integrator.h"

int main() {
    // -------------------------------------------------------
    // Parámetros del sistema
    // -------------------------------------------------------
    const double mu      = 0.0015;
    const double t_final = 10.0 * 2.0 * M_PI;
    const double h       = 2.0 * M_PI / 3000;
    const long   N       = static_cast<long>(t_final / h);

   
    Body particle(
        1.5,    // x0
        0.0,    // y0
        0.02,    // vx0
       -1.868   // vy0 = sqrt(1/2.5) - 2.5
    );

    CR3BP     sys(mu);
    Integrator integrator(h);

    // -------------------------------------------------------
    // Abrir archivo de resultados
    // -------------------------------------------------------
    std::ofstream out("results/trajectory.csv");
    if (!out.is_open()) {
        std::cerr << "Error: no se pudo abrir results/trajectory.csv\n";
        return 1;
    }

    out << std::fixed << std::setprecision(8);
    out << "t,x,y,vx,vy,jacobi\n";

    double t = 0.0;
    double C0 = sys.jacobi(particle);  // constante de Jacobi inicial

    std::cout << "CR3BP - Sistema con mu = " << mu << "\n";
    std::cout << "Pasos totales: " << N << "\n";
    std::cout << "Constante de Jacobi inicial: " << C0 << "\n";
    std::cout << "Integrando...\n";

    // Guardar cada 10 pasos para no saturar el archivo
    const int save_every = 1;

    for (long i = 0; i <= N; ++i) {
        if (i % save_every == 0) {
            out << t << ","
                << particle.x  << ","
                << particle.y  << ","
                << particle.vx << ","
                << particle.vy << ","
                << sys.jacobi(particle) << "\n";
        }
        integrator.step(particle, sys);
        t += h;
    }

    out.close();

    double Cf = sys.jacobi(particle);
    std::cout << "Integración completa.\n";
    std::cout << "Constante de Jacobi final:   " << Cf << "\n";
    std::cout << "Error relativo en Jacobi:    "
              << std::abs((Cf - C0) / C0) << "\n";
    std::cout << "Resultados en results/trajectory.csv\n";

    return 0;
}
