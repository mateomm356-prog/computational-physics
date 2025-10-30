#ifndef INTEGRADOR_H
#define INTEGRADOR_H

// Estado = {theta1, theta2, P1, P2}
void muevase(double y[4], double dt,
             double m1, double m2,
             double l1, double l2,
             double g, double b1, double b2);

#endif

