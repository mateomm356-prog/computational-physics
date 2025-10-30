#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
using namespace std;

struct Punto {
    double t, theta1, theta2, p1, p2;
};

const int NMAX = 10000; // máximo de líneas

int main() {
    ifstream file0("pendulo_doble.dat");
    ifstream file1("pendulo_doble1.dat");

    if (!file0.is_open() || !file1.is_open()) {
        cerr << "Error abriendo archivos" << endl;
        return 1;
    }

    // Saltar encabezado
    string header;
    getline(file0, header);
    getline(file1, header);


    Punto traj0[NMAX], traj1[NMAX];
    int N = 0;

    // Leer datos de ambos archivos
    while (file0 >> traj0[N].t >> traj0[N].theta1 >> traj0[N].theta2 >> traj0[N].p1 >> traj0[N].p2 &&
           file1 >> traj1[N].t >> traj1[N].theta1 >> traj1[N].theta2 >> traj1[N].p1 >> traj1[N].p2) {
        N++;
        if (N >= NMAX) break;
    }

    file0.close();
    file1.close();

    if (N < 2) {
        cerr << "Muy pocos datos para calcular Lyapunov" << endl;
        return 1;
    }

    // Distancia inicial
    double dx1 = traj0[0].theta1 - traj1[0].theta1;
    double dx2 = traj0[0].theta2 - traj1[0].theta2;
    double dp1 = traj0[0].p1 - traj1[0].p1;
    double dp2 = traj0[0].p2 - traj1[0].p2;
    double d0 = sqrt(dx1*dx1 + dp1*dp1);

    ofstream fout("lyapunov.dat");
    fout << fixed << setprecision(6);
    fout << setw(10) << "t(s)" 
         << setw(20) << "distancia" 
         << setw(20) << "ln(d/d0)" 
         << setw(20) << "lambda(t)" << endl;

    for (int i = 1; i < N; i++) {
        double dx1 = traj0[i].theta1 - traj1[i].theta1;
        double dx2 = traj0[i].theta2 - traj1[i].theta2;
        double dp1 = traj0[i].p1 - traj1[i].p1;
        double dp2 = traj0[i].p2 - traj1[i].p2;

        double dist = sqrt(dx1*dx1 + dp1*dp1);

        double ln_ratio = log(dist / d0);
        double lambda_t = ln_ratio / traj0[i].t;  // estimación del exponente

        fout << setw(10) << traj0[i].t
             << setw(20) << dist
             << setw(20) << ln_ratio
             << setw(20) << lambda_t << endl;
    }

    fout.close();
    cout << "Datos guardados en 'lyapunov.dat'" << endl;

    return 0;
}
