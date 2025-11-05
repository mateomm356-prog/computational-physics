#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <random>
#include <chrono>
#include <cmath>
#include <fstream>

#include <Eigen/Dense>
#include "Caja.h"
#include "Bola.h"

using Eigen::Vector2d;
using namespace std;

// Genera N partículas en una malla (rejilla) dentro de la caja.
// Coloca hasta una malla rows x cols >= N, centrada y con separación "sep" calculada.
void inicializarEnMalla(vector<Bola> &bolas, const Caja &caja,
                        int N, double radio, double masa, double v_init_max)
{
    // calcular número de columnas/filas para una malla aproximada
    int cols = static_cast<int>(ceil(sqrt((double)N)));
    int rows = static_cast<int>(ceil((double)N / cols));

    double xmin = caja.getXmin();
    double xmax = caja.getXmax();
    double ymin = caja.getYmin();
    double ymax = caja.getYmax();

    // dejar un margen igual a radio + pequeña fracción
    double margin = radio * 1.2;

    double usableW = xmax - xmin - 2.0 * margin;
    double usableH = ymax - ymin - 2.0 * margin;

    double sepX = (cols > 1) ? usableW / (cols - 1) : 0.0;
    double sepY = (rows > 1) ? usableH / (rows - 1) : 0.0;

    // distribuye en malla
    std::mt19937_64 rng((unsigned)chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<double> dv(-v_init_max, v_init_max);

    int placed = 0;
    for (int r = 0; r < rows && placed < N; ++r) {
        for (int c = 0; c < cols && placed < N; ++c) {
            double x = xmin + margin + c * sepX;
            double y = ymin + margin + r * sepY;
            Vector2d pos(x, y);
            Vector2d vel(dv(rng)*0.1, dv(rng)*0.1); // velocidades iniciales pequeñas
            bolas.emplace_back(pos, vel, masa, radio);
            placed++;
        }
    }
}

int contarLineas(const string &nombreArchivo) {
    ifstream archivo(nombreArchivo);
    int lineas = 0;
    string linea;
    while (getline(archivo, linea)) {
        lineas++;
    }
    return lineas;
}

int contarBolas(const string &nombreArchivo) {
    ifstream archivo(nombreArchivo);
    string linea;
    getline(archivo, linea); // solo la primera línea
    archivo.close();

    stringstream ss(linea);
    vector<double> valores;
    double v;
    while (ss >> v) valores.push_back(v);

    // t ocupa 1 columna; cada bola usa 4 columnas (x,y,vx,vy)
    return (valores.size() - 1) / 4;
}

void generarAnimacion(const string &nombreArchivo, double dt, const string &gifSalida) {
    int frames = contarLineas(nombreArchivo);
    int N = contarBolas(nombreArchivo);

    ofstream g("scripts/animate2D.gnu");
    g << "set terminal gif animate delay 6 size 800,600\n";
    g << "set output '" << gifSalida << "'\n";
    g << "set xrange [0:10]\n";
    g << "set yrange [0:10]\n";
    g << "unset key\n";
    g << "set size ratio -1\n";
    g << "do for [i=1:" << frames << "] {\n";
    g << "   set label 1 sprintf('t = %.3f s', " << dt << "*(i-1)) at graph 0.02,0.95 front\n";
    g << "   set label 2 'N = " << N << " bolas' at graph 0.02,0.90 front\n";
    g << "   plot ";

    for(int k = 0; k < N; k++) {
        int colx = 2 + 4*k;
        int coly = 3 + 4*k;
        g << "'" << nombreArchivo << "' every ::i::i using "
          << colx << ":" << coly << " with points pt 7 ps 1.5";
        if (k < N-1) g << ", ";
    }

    g << "\n}\n";
    g.close();

    system("gnuplot scripts/animate2D.gnu");
    cout << "✅ Animación generada en: " << gifSalida << endl;
}

int main(int argc, char *argv[]) {
    // Parámetros (puedes ajustar o leer por argumentos)
    int N = 100;             // número de partículas por defecto
    double W = 4.0;         // ancho de la caja (x in [0, W])
    double H = 4.0;         // alto de la caja (y in [0, H])
    double radio = 0.09;    // radio de cada partícula
    double masa = 1.0;      // masa (todas iguales)
    double dt = 0.1;      // paso temporal
    double t_total = 20.0;   // tiempo total
    double v_init_max = 0.9; // escala de velocidades iniciales

    if (argc >= 2) N = atoi(argv[1]);
    if (argc >= 3) t_total = atof(argv[2]);

    // crear caja (desde 0..W y 0..H)
    Caja caja(0.0, W, 0.0, H);

    // vector de bolas
    vector<Bola> bolas;
    bolas.reserve(N);

    // inicializar en rejilla/malla
    inicializarEnMalla(bolas, caja, N, radio, masa, v_init_max);

    // abrir archivo de salida
    ofstream out("results/datos.txt");
    if (!out.is_open()) {
        cerr << "No se pudo abrir datos.txt para escribir\n";
        return 1;
    }

    int pasos = static_cast<int>(ceil(t_total / dt));
    double tiempo = 0.0;

    // loop principal
    for (int step = 0; step < pasos; ++step) {
        // 1) Velocity-Verlet: mover (cada Bola tiene moverVerlet)
        for (auto &b : bolas) b.moverVerlet(dt);

        // 2) resolver choques pared (múltiples)
        caja.resolverChoquesParedMultiple(bolas);

        // 3) choques entre pares (doble bucle)
        int M = (int)bolas.size();
        for (int i = 0; i < M; ++i) {
            for (int j = i + 1; j < M; ++j) {
                bolas[i].choqueConOtraBola(bolas[j]);
            }
        }

        // 4) guardar estado en archivo: t, (x,y,vx,vy) para cada bola
        out.setf(std::ios::fixed);
        out.precision(6);
        out << tiempo;
        for (const auto &b : bolas) {
            Vector2d r = b.getPosicion();
            Vector2d v = b.getVelocidad();
            out << " " << r.x() << " " << r.y() << " " << v.x() << " " << v.y();
        }
        out << "\n";

        tiempo += dt;
    }

    out.close();
cout << "Simulación completada. Datos en datos.txt (t, x1 y1 vx1 vy1 ...)\n";

// Crear animación (opcional)
generarAnimacion("results/datos.txt", dt, "results/simulacion.gif");
cout << "GIF listo: results/simulacion.gif\n";

return 0;

}



