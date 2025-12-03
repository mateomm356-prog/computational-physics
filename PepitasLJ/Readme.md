# ⚛️ Simulación de N Partículas en una Caja con Potencial Lennard–Jones

Este proyecto implementa una simulación de Dinámica Molecular (MD) en 2D para modelar el movimiento de **N partículas** interactuantes dentro de una caja rectangular cerrada (`W × H`).

La principal diferencia con respecto a las simulaciones de colisión elástica es la inclusión de un **potencial de interacción de Lennard–Jones (LJ)** que rige las fuerzas entre las partículas.

---

## 🎯 Objetivo

Modelar el comportamiento termodinámico y estadístico de un sistema de partículas interactuantes, observando:

* **Interacción Lennard–Jones:** Simulación de fuerzas atractivas y repulsivas entre partículas (fases de la materia).
* **Conservación de Energía:** Integración precisa mediante **Velocity Verlet** para asegurar la estabilidad del sistema.
* **Análisis Estadístico:** Verificación de la **Distribución de Velocidades de Maxwell–Boltzmann** y cálculo de propiedades termodinámicas.
* **Visualización Global:** Graficación de las **trayectorias de todas las partículas** en el sistema.

---

## 💻 Estructura del Código y Componentes Principales

| Archivo / Carpeta | Descripción |
|-------------------|-------------|
| `main.cpp` | Configuración de la simulación (parámetros globales) e inicialización de todas las partículas. |
| `pepa.h / pepa.cpp` | Clase que modela cada **partícula** e implementa el **integrador Velocity Verlet** y el cálculo de la fuerza total (LJ + Paredes). |
| `caja.h / caja.cpp` | Clase que define los límites del sistema (caja) y maneja las **colisiones elásticas con las paredes**. |
| `results/` | Directorio para guardar `datos.txt` (trayectorias, energías) y las gráficas generadas. |
| `python/plot.py` | Script para graficar las trayectorias de **todas las partículas**, histogramas de velocidad y energías. |
| `README.md` | Este documento. |

---

## 🧪 Interacción por Potencial de Lennard–Jones

La interacción entre dos partículas $i$ y $j$ a una distancia $r$ viene dada por el potencial LJ:

$$
V_{\text{LJ}}(r)=4\varepsilon\left[\left(\frac{\sigma}{r}\right)^{12}-
\left(\frac{\sigma}{r}\right)^6\right]
$$

Esta interacción se traduce en una fuerza implementada en la clase `pepa`, utilizada para actualizar la aceleración de cada partícula en cada paso temporal.

---

## ⚙️ Uso y Comandos del Makefile

Los parámetros principales (`N`, `W`, `H`, `dt`, $\varepsilon$, $\sigma$) se configuran en `main.cpp`.

El proyecto utiliza un `Makefile` para automatizar los procesos de compilación y visualización:

| Comando | Acción |
|--------------------|------------------------------------------------------|
| `make` | **Compila** el programa (`g++ -std=c++17`). |
| `make run` | **Ejecuta** la simulación y genera el archivo `results/datos.txt`. |
| `make trayectorias` | **Grafica las trayectorias** de todas las partículas en la caja. |
| `make histogramas` | **Grafica el histograma de velocidades** y lo compara con el ajuste de Maxwell–Boltzmann. |