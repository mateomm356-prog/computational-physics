# Simulación del Péndulo Doble con Fricción Tipo Stokes

Este proyecto implementa la dinámica del péndulo doble utilizando las ecuaciones de movimiento en formulación Hamiltoniana, extendidas para incluir disipación tipo **Stokes** (fuerza proporcional a la velocidad angular). El sistema es un ejemplo clásico de **caos determinista** y permite estudiar fenómenos como sensibilidad a condiciones iniciales, atractores disipativos y transición entre movimiento periódico e irregular.

---

## 🧠 Modelo Físico

Las variables dinámicas del sistema son:

| Símbolo | Descripción |
|--------|-------------|
| `θ₁, θ₂` | Ángulos de cada péndulo respecto a la vertical |
| `P₁, P₂` | Momentos conjugados asociados |
| `m₁, m₂` | Masas |
| `l₁, l₂` | Longitudes |
| `b₁, b₂` | Coeficientes de fricción tipo Stokes |

La disipación modelada corresponde a un torque viscoso:

\[
\tau = -b\,\dot{\theta}
\]

que retira energía gradualmente del sistema.

La integración temporal se realiza usando **Euler-simpléctico modificado**, lo cual respeta aproximadamente la estructura Hamiltoniana en el caso conservativo y permite incorporar disipación explícita.

---

## 📁 Estructura del Proyecto

├── PenduloS/
│ ├── PenduloDobleS.cpp # Simulación principal (este archivo)
│ ├── EspacFase.py # Script Python para graficar el espacio de fases
│
├── lyapunov/
│ ├── Lyapunov.cpp # Cálculo del exponente de Lyapunov
│ ├── lyapunov_plot.gp # Script gnuplot para graficar λ(t)
│
├── PoincareS/
│ ├── Integrador.cpp # Integrador para mapa de Poincaré
│ ├── Poincare.cpp # Generación de sección de Poincaré
│ ├── poincare.plt # Script gnuplot
│
├── Makefile # Compilación automática
└── README.md # Este documento


---

## ⚙️ Compilación

Solo se requiere **g++ con soporte C++17** y opcionalmente:

- `python3 + matplotlib` (gráficas)
- `gnuplot` (trayectorias / Poincaré / Lyapunov)

Compilar y ejecutar el pendulo con:

```bash
make

//Hacer el calculo del exponente de Lyapunov con:

make lyapunov

//Hacer los diagramas de Poincare

make poincare


