#ifndef OSCILADOR_ARMONICO_C_H
#define OSCILADOR_ARMONICO_C_H

#include <random>
#include <string>

class OsciladorArmonicoC {
public:
    // Constructor: m, k, T, Dx, Dp, Nterm (termalización), Nprod (producción)
    OsciladorArmonicoC(double m_, double k_, double T_,
                       double Dx_, double Dp_,
                       long long Nterm_, long long Nprod_,
                       unsigned long long seed = 0);

    // Ejecuta toda la simulación y guarda los datos en filename
    void run(const std::string &filename);

    // Getters (resultados)
    double mean_x2() const;
    double mean_p2() const;
    double mean_E() const;
    double acceptance_rate() const;

    // Opcional: ajustar Dx/Dp manualmente
    void set_steps(double Dx_, double Dp_);

private:
    // Parámetros físicos
    double m;
    double k;
    double T;
    double beta; // 1/(k_B T) with k_B = 1

    // Parámetros Monte Carlo
    double Dx;
    double Dp;
    long long Nterm;
    long long Nprod;

    // Estado
    double x;
    double p;

    // RNG
    std::mt19937_64 rng;
    std::uniform_real_distribution<double> uni01; // [0,1)
    std::uniform_real_distribution<double> dist_dx; // [-Dx, Dx]
    std::uniform_real_distribution<double> dist_dp; // [-Dp, Dp]

    // Acumuladores
    long long accepted;
    long long proposed;
    long long accepted_term;
    long long proposed_term;

    long long samples; // number of recorded samples
    long long written_samples; // for output file

    double sum_x2;
    double sum_p2;
    double sum_E;

    // Helpers
    double energy(double x_, double p_) const;
    bool try_step(); // realiza una propuesta (x',p') y decide, actualiza x,p y contadores
};
#endif // OSCILADOR_ARMONICO_C_H
