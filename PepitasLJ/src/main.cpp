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

// --------------------------------------------------
// Inicializar partículas en malla
// --------------------------------------------------
void inicializarEnMalla(vector<Bola> &bolas, const Caja &caja,
                        int N, double radio, double masa, double v_init_max)
{
    int cols = static_cast<int>(ceil(sqrt((double)N)));
    int rows = static_cast<int>(ceil((double)N / cols));

    double xmin = caja.getXmin();
    double xmax = caja.getXmax();
    double ymin = caja.getYmin();
    double ymax = caja.getYmax();
    double margin = radio * 1.2;
    double usableW = xmax - xmin - 2.0 * margin;
    double usableH = ymax - ymin - 2.0 * margin;
    double sepX = (cols > 1) ? usableW / (cols - 1) : 0.0;
    double sepY = (rows > 1) ? usableH / (rows - 1) : 0.0;

    std::mt19937_64 rng((unsigned)chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<double> dv(-v_init_max, v_init_max);

    int placed = 0;
    for (int r = 0; r < rows && placed < N; ++r) {
        for (int c = 0; c < cols && placed < N; ++c) {
            double x = xmin + margin + c * sepX;
            double y = ymin + margin + r * sepY;
            Vector2d pos(x, y);
            Vector2d vel(dv(rng)*0.1, dv(rng)*0.1);
            bolas.emplace_back(pos, vel, masa, radio);
            placed++;
        }
    }
}

// --------------------------------------------------
// Contar líneas / bolas para animación
// --------------------------------------------------
int contarLineas(const string &nombreArchivo) {
    ifstream archivo(nombreArchivo);
    int lineas = 0;
    string linea;
    while (getline(archivo, linea)) lineas++;
    return lineas;
}

int contarBolas(const string &nombreArchivo) {
    ifstream archivo(nombreArchivo);
    string linea;
    getline(archivo, linea);
    archivo.close();
    stringstream ss(linea);
    vector<double> valores; double v;
    while(ss >> v) valores.push_back(v);
    return (valores.size() - 1) / 4;
}

// --------------------------------------------------
// Generar animación con Gnuplot
// --------------------------------------------------
void generarAnimacion(const string &nombreArchivo, double dt, const string &gifSalida) {
    int frames = contarLineas(nombreArchivo);
    int N = contarBolas(nombreArchivo);
    ofstream g("scripts/animate2D.gnu");
    g << "set terminal gif animate delay 6 size 800,600\n";
    g << "set output '" << gifSalida << "'\n";
    g << "set xrange [0:10]\nset yrange [0:10]\n";
    g << "unset key\nset size ratio -1\n";
    g << "do for [i=1:" << frames << "] {\n";
    g << "set label 1 sprintf('t = %.3f s', " << dt << "*(i-1)) at graph 0.02,0.95 front\n";
    g << "set label 2 'N = " << N << " bolas' at graph 0.02,0.90 front\n";
    g << "plot ";
    for(int k=0;k<N;k++){
        int colx=2+4*k; int coly=3+4*k;
        g << "'" << nombreArchivo << "' every ::i::i using " << colx << ":" << coly
          << " with points pt 7 ps 1.5";
        if(k<N-1) g << ", ";
    }
    g << "\n}\n"; g.close();
    system("gnuplot scripts/animate2D.gnu");
    cout << "✅ Animación generada en: " << gifSalida << endl;
}

// --------------------------------------------------
// MAIN
// --------------------------------------------------
int main(int argc, char *argv[]) {
    int N = 200; double W=7.0,H=7.0;
    double radio=0.09, masa=1.0, dt=0.1, t_total=10.0, v_init_max=1.2;

    if(argc>=2) N = atoi(argv[1]);
    if(argc>=3) t_total = atof(argv[2]);

    Caja caja(0.0, W, 0.0, H);
    vector<Bola> bolas; bolas.reserve(N);
    inicializarEnMalla(bolas, caja, N, radio, masa, v_init_max);

    const double epsilon0 = 1.0;   // Lennard-Jones
    const double sigma    = 0.1;

    int M = bolas.size();
    // aceleraciones iniciales
    for(auto &b : bolas) b.setAceleracion(Vector2d::Zero());
    for(int i=0;i<M;i++)
        for(int j=i+1;j<M;j++){
            Vector2d F = bolas[i].fuerzaLJ(bolas[j], epsilon0, sigma);
            bolas[i].sumA(F/masa);
            bolas[j].sumA(-F/masa);
        }

    ofstream out("results/datos.txt");
    if(!out.is_open()){ cerr<<"No se pudo abrir datos.txt\n"; return 1; }

    int pasos = static_cast<int>(ceil(t_total/dt));
    double tiempo=0.0;

    for(int step=0; step<pasos; ++step){
        // 1) half-kick + drift
        for(auto &b : bolas) b.moverVerlet(dt);

        // 2) paredes
        caja.resolverChoquesParedMultiple(bolas);

        // 3) limpiar aceleraciones
        for(auto &b : bolas) b.setAceleracion(Vector2d::Zero());

        // 4) calcular fuerzas Lennard-Jones
        for(int i=0;i<M;i++)
            for(int j=i+1;j<M;j++){
                Vector2d F = bolas[i].fuerzaLJ(bolas[j], epsilon0, sigma);
                bolas[i].sumA(F/masa);
                bolas[j].sumA(-F/masa);
            }

        // 5) final kick
        for(auto &b : bolas)
            b.actualizarVelocidadFinal(dt, b.getAceleracion());

        // guardar
        out.setf(ios::fixed); out.precision(6);
        out << tiempo;
        for(const auto &b : bolas){
            Vector2d r = b.getPosicion();
            Vector2d v = b.getVelocidad();
            out << " " << r.x() << " " << r.y() << " " << v.x() << " " << v.y();
        }
        out << "\n";

        tiempo += dt;
    }

    out.close();
    cout << "Simulación completada. Datos en datos.txt\n";

    generarAnimacion("results/datos.txt", dt, "results/simulacion.gif");

    return 0;
}
