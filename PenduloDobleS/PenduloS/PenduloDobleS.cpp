#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
using namespace std;

// Constantes globales
const double g = 9.8;


// Clase para el Péndulo Doble con disipación
class PenduloDoble {
private:
    //masas, longitudes y constantes de fricción- de momento sin el calculo de b1 y b2, que tienen la forma bi=6*pi*eta*ri
    double m1, m2, l1, l2;
    double b1, b2;   // coeficientes de fricción

    // Variables dinámicas
    double theta1, theta2; // posiciones 
    double P1, P2;         // momentos

public:
    void Inicie(double th1, double th2, double P1_0, double P2_0,
                double m1_, double m2_, double l1_, double l2_,
                double b1_, double b2_) {
        theta1 = th1; theta2 = th2;
        P1 = P1_0;    P2 = P2_0;
        m1 = m1_;     m2 = m2_;
        l1 = l1_;     l2 = l2_;
        b1 = b1_;     b2 = b2_;
    }

    // Un paso de integración tipo Euler-simpléctico + disipación
    void Muevase(double dt);

    // Getters
    double GetTheta1() const { return theta1; }
    double GetTheta2() const { return theta2; }
    double GetP1() const { return P1; }
    double GetP2() const { return P2; }
};

void PenduloDoble::Muevase(double dt) {
    // Variables auxiliares
    double D = theta1 - theta2;

    double a = (m1 + m2) * l1 * l1;
    double b = m2 * l1 * l2 * cos(D);
    double c = m2 * l2 * l2;
    double detM = a * c - b * b;

    // Inversa de M 
    double Minv11 = c / detM;
    double Minv12 = -b / detM;
    double Minv21 = -b / detM;
    double Minv22 = a / detM;

    // Velocidades angulares (qdot = Minv * P)
    double dth1 = Minv11 * P1 + Minv12 * P2;
    double dth2 = Minv21 * P1 + Minv22 * P2;

    // Gradiente del potencial
    double dVdth1 = (m1 + m2) * g * l1 * sin(theta1);
    double dVdth2 = m2 * g * l2 * sin(theta2);

    // Matriz C(θ) para disipación
    double C11 = (b1 + b2) * l1 * l1;
    double C12 = b2 * l1 * l2 * cos(D);
    double C21 = C12;
    double C22 = b2 * l2 * l2;

    // Términos disipativos: C * qdot
    double R1 = C11 * dth1 + C12 * dth2;
    double R2 = C21 * dth1 + C22 * dth2;

    // Ecuaciones de Hamilton modificadas
    double dP1 = -dVdth1 - R1;
    double dP2 = -dVdth2 - R2;

    // ---- Paso de integración (Euler-simpléctico con disipación explícita) ----
    // Primero actualizar momentos
    P1 += dP1 * dt;
    P2 += dP2 * dt;

    // Recalcular velocidades con nuevos momentos
    dth1 = Minv11 * P1 + Minv12 * P2;
    dth2 = Minv21 * P1 + Minv22 * P2;

    // Luego actualizar ángulos
    theta1 += dth1 * dt;
    theta2 += dth2 * dt;
}

// -------------------- Función Principal --------------------
int main() {
    PenduloDoble pendulo;
    pendulo.Inicie(M_PI/3, -M_PI/6, 0.0, 0.0,   // θ1, θ2, P1, P2 iniciales
                   1.0, 1.0, 1.0, 1.0,         // m1, m2, l1, l2
                   0.2, 0.2);                  // coeficientes de disipación

    double tmax = 50.0, dt = 0.01;
    double t = 0.0;

    ofstream file("pendulo_doble.dat");
    file << fixed << setprecision(6);
    file << setw(10) << "t(s)"
         << setw(15) << "theta1(rad)"
         << setw(15) << "theta2(rad)"
         << setw(15) << "P1"
         << setw(15) << "P2" << endl;

    for (t = 0; t <= tmax; t += dt) {
        file << setw(10) << t
             << setw(15) << pendulo.GetTheta1()
             << setw(15) << pendulo.GetTheta2()
             << setw(15) << pendulo.GetP1()
             << setw(15) << pendulo.GetP2()
             << endl;

        pendulo.Muevase(dt);
    }

    file.close();
    cout << "Datos guardados en 'pendulo_doble.dat'" << endl;
    return 0;
}
