# Simulación de N Partículas en una Caja

Este proyecto implementa la simulación del movimiento de **N partículas** dentro de una caja rectangular de dimensiones `W × H`.  
Cada partícula posee posición, velocidad y radio, y puede **colisionar** tanto con las paredes como con otras partículas.  
La simulación se desarrolla en **C++** y los datos se exportan para su graficación en **Python (Matplotlib)**.

---

## 🎯 Objetivo

Modelar el comportamiento dinámico y estadístico de un sistema de partículas en 2D, observando:
- Conservación de momento en colisiones.
- Distribución de velocidades emergente (tipo **Maxwell–Boltzmann**).
- Evolución espacial y temporal del sistema.

---

## 🧱 Contenido del Proyecto

| Archivo / Carpeta | Descripción |
|------------------|-------------|
| `main.cpp` | Código principal de la simulación. |
| `pepa.h / pepa.cpp` | Clase que define las partículas (posición, velocidad y métodos de actualización). |
| `caja.h / caja.cpp` | Clase que define el contenedor y el manejo de colisiones con paredes. |
| `results/` | Carpeta donde se guardan los archivos de salida. |
| `python/plot.py` | Script para graficar trayectorias, histogramas y comparación con Maxwell-Boltzmann. |
| `README.md` | Este documento. |

---

## ⚙️ Parámetros Principales

Los parámetros globales pueden ajustarse en `main.cpp`:

```cpp
int N = 200;     // Número de partículas
double W = 4.0; // Ancho de la caja
double H = 4.0; // Alto de la caja
double dt = 0.01; // Paso temporal

Para ejecutar el programa se hace mediante el Makefile con los comandos:

-make ## compilar el programa
-make run ## ejecutar
-make histogramas ## hacer el histogramas de velocidades
-make trayectorias ## grafica las trayectorias de 6 particulas dentro de la caja

