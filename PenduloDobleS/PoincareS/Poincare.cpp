#include "Integrador.h"
#include <fstream>
#include <cmath>

int main() {
    std::ofstream fout("poincare.dat");
    
    int N = 30;  // densidad de la grilla
    double y1min = M_PI/3 - 0.1, y1max = M_PI/3 + 0.1;        // rango para θ1
    double y2min = -M_PI/6 - 0.1, y2max = -M_PI/6 + 0.1;    // rango para θ2

    int nsteps = 10000;   // más pasos para que se vean varias intersecciones
    double h = 0.01;
    double omega = 5.0;
    double T = 2.0 * M_PI / omega;

    int nskip = 0;  // número de periodos a ignorar antes de empezar a guardar

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            double y[4] = {
                y1min + i * (y1max - y1min) / (N - 1),  // θ1 inicial
                y2min + j * (y2max - y2min) / (N - 1),  // θ2 inicial
                0.0, 0.0                               // P1, P2 iniciales
            };

            double t = 0.0;

            for (int k = 0; k < nsteps; k++) {
                muevase(y, h, 1.0, 1.0, 1.0, 1.0, 9.8, 0.8, 0.8);
                t += h;

                // saltamos los primeros nskip*T
                if (t < nskip * T) continue;

                // guardamos solo cerca de múltiplos de T
                if (std::fabs(fmod(t, T)) < h/2.0) {
                    fout << y[0] << "\t" << y[2] << "\n";
                }
            }

            fout << "\n"; // separador entre trayectorias de la grilla
        }
    }

    fout.close();
    return 0;
}



