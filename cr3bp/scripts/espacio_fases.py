"""
espacio_fases.py
Visualiza el espacio de fases del CR3BP:
  - Posición x vs momento px = vx  (plano x-px)
  - Posición y vs momento py = vy  (plano y-py)

En un sistema hamiltoniano con integrador simpléctico
(Leapfrog), las trayectorias en el espacio de fases
deben formar curvas cerradas o cuasi-periódicas.
Si el integrador disipa energía artificialmente,
las curvas espiralan hacia adentro o afuera.
La estructura cerrada confirma la conservación
de la estructura hamiltoniana.
"""

import numpy as np
import matplotlib.pyplot as plt
from matplotlib.collections import LineCollection
import os
import sys

# -------------------------------------------------------
# Cargar datos
# -------------------------------------------------------
csv_path = os.path.join(os.path.dirname(__file__), "../results/trajectory.csv")
try:
    data = np.loadtxt(csv_path, delimiter=",", skiprows=1)
except FileNotFoundError:
    print("Error: no se encontró results/trajectory.csv")
    print("Ejecuta 'make' primero para correr la simulación.")
    sys.exit(1)

t  = data[:, 0]
x  = data[:, 1]
y  = data[:, 2]
vx = data[:, 3]
vy = data[:, 4]

# Momento generalizado (por unidad de masa de m3)
# En el frame rotante el momento canonico es:
#   px = vx - y   (de dL/d(dot x))
#   py = vy + x   (de dL/d(dot y))
px = vx - y
py = vy + x

# -------------------------------------------------------
# Figura: espacio de fases
# -------------------------------------------------------
fig, axes = plt.subplots(1, 2, figsize=(14, 6))
fig.patch.set_facecolor("#0d0d1a")

norm = plt.Normalize(t.min(), t.max())

for ax, q, p, ql, pl, titulo in [
    (axes[0], x,  px, "x [adim.]",  r"$p_x = \dot{x} - y$  [adim.]",
     "Espacio de fases — plano $x$-$p_x$"),
    (axes[1], y,  py, "y [adim.]",  r"$p_y = \dot{y} + x$  [adim.]",
     "Espacio de fases — plano $y$-$p_y$"),
]:
    ax.set_facecolor("#0d0d1a")

    # Colorear por tiempo para ver la evolución
    points   = np.array([q, p]).T.reshape(-1, 1, 2)
    segments = np.concatenate([points[:-1], points[1:]], axis=1)
    lc = LineCollection(segments, cmap="plasma", norm=norm,
                        linewidth=0.7, alpha=0.85)
    lc.set_array(t)
    ax.add_collection(lc)

    # Punto inicial
    ax.plot(q[0], p[0], '*', color="white", markersize=10,
            label="Inicio", zorder=5)

    cbar = fig.colorbar(lc, ax=ax, pad=0.02)
    cbar.set_label("Tiempo [adim.]", color="white", fontsize=9)
    cbar.ax.yaxis.set_tick_params(color="white")
    plt.setp(cbar.ax.yaxis.get_ticklabels(), color="white")

    ax.set_xlim(q.min() - 0.1, q.max() + 0.1)
    ax.set_ylim(p.min() - 0.1, p.max() + 0.1)
    ax.set_xlabel(ql, color="white", fontsize=11)
    ax.set_ylabel(pl, color="white", fontsize=11)
    ax.set_title(titulo, color="white", fontsize=12)
    ax.tick_params(colors="white")
    for spine in ax.spines.values():
        spine.set_edgecolor("#444466")
    ax.legend(facecolor="#1a1a2e", edgecolor="#444466",
              labelcolor="white", fontsize=9)
    ax.grid(True, color="#222244", linewidth=0.4, linestyle="--")

plt.suptitle(r"CR3BP — $\mu = 0.5$ — Espacio de fases en el frame rotante"
             "\nCurvas cerradas indican conservación de la estructura hamiltoniana",
             color="white", fontsize=11, y=1.02)

plt.tight_layout()
out_path = os.path.join(os.path.dirname(__file__), "../results/espacio_fases.png")
plt.savefig(out_path, dpi=150, bbox_inches="tight",
            facecolor=fig.get_facecolor())
print("Gráfica guardada en results/espacio_fases.png")
plt.show()