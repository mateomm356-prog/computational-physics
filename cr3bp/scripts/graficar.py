"""
graficar.py
Genera la gráfica de la trayectoria del CR3BP (mu=0.5)
en el frame rotante con los primarios fijos.
"""

import numpy as np
import matplotlib.pyplot as plt
from matplotlib.collections import LineCollection
import sys
import os

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

t   = data[:, 0]
x   = data[:, 1]
y   = data[:, 2]
CJ  = data[:, 5]

mu = 0.0015

# -------------------------------------------------------
# Figura principal: trayectoria
# -------------------------------------------------------
fig, ax1 = plt.subplots(figsize=(8, 8))
fig.patch.set_facecolor("#0d0d1a")
ax1.set_facecolor("#0d0d1a")

points = np.array([x, y]).T.reshape(-1, 1, 2)
segments = np.concatenate([points[:-1], points[1:]], axis=1)

norm = plt.Normalize(t.min(), t.max())
lc = LineCollection(segments, cmap="plasma", norm=norm, linewidth=0.8, alpha=0.85)
lc.set_array(t)
ax1.add_collection(lc)

# Primarios
ax1.plot(-mu, 0, 'o', color="#00cfff", markersize=10, label=r"$m_1$", zorder=5)
ax1.plot(1 - mu, 0, 'o', color="#ff6b6b", markersize=10, label=r"$m_2$", zorder=5)

# Punto inicial y final
ax1.plot(x[0], y[0], '*', color="white", markersize=10, label="Inicio", zorder=6)
ax1.plot(x[-1], y[-1], 'x', color="yellow", markersize=8, label="Final", zorder=6)

cbar = fig.colorbar(lc, ax=ax1, pad=0.02)
cbar.set_label("Tiempo [adim.]", color="white", fontsize=10)
cbar.ax.yaxis.set_tick_params(color="white")
plt.setp(cbar.ax.yaxis.get_ticklabels(), color="white")

ax1.set_xlim(x.min() - 0.2, x.max() + 0.2)
ax1.set_ylim(y.min() - 0.2, y.max() + 0.2)
ax1.set_aspect("equal")
ax1.set_xlabel("x [adim.]", color="white", fontsize=11)
ax1.set_ylabel("y [adim.]", color="white", fontsize=11)
ax1.set_title("Trayectoria en el frame rotante\n"
              r"CR3BP — $\mu = 0.5$", color="white", fontsize=12)
ax1.tick_params(colors="white")
for spine in ax1.spines.values():
    spine.set_edgecolor("#444466")
ax1.legend(facecolor="#1a1a2e", edgecolor="#444466",
           labelcolor="white", fontsize=9, loc="upper right")
ax1.grid(True, color="#222244", linewidth=0.4, linestyle="--")

plt.tight_layout()
out_path = os.path.join(os.path.dirname(__file__), "../results/trayectoria.png")
plt.savefig(out_path, dpi=150, bbox_inches="tight",
            facecolor=fig.get_facecolor())
print(f"Gráfica guardada en results/trayectoria.png")
plt.show()

