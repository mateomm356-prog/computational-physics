"""
animar.py
Genera una animación de la trayectoria del CR3BP (mu=0.5)
en el frame rotante. Exporta results/animacion.gif
"""

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
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

t  = data[:, 0]
x  = data[:, 1]
y  = data[:, 2]

mu = 0.5

# Reducir frames para animación fluida
stride = max(1, len(t) // 800)
t_anim = t[::stride]
x_anim = x[::stride]
y_anim = y[::stride]
N = len(t_anim)

# -------------------------------------------------------
# Configurar figura
# -------------------------------------------------------
fig, ax = plt.subplots(figsize=(8, 8))
fig.patch.set_facecolor("#0d0d1a")
ax.set_facecolor("#0d0d1a")

xpad = (x.max() - x.min()) * 0.12 + 0.15
ypad = (y.max() - y.min()) * 0.12 + 0.15
ax.set_xlim(x.min() - xpad, x.max() + xpad)
ax.set_ylim(y.min() - ypad, y.max() + ypad)
ax.set_aspect("equal")

ax.set_xlabel("x [adim.]", color="white", fontsize=12)
ax.set_ylabel("y [adim.]", color="white", fontsize=12)
ax.set_title(r"CR3BP — $\mu = 0.5$ — Frame rotante", color="white", fontsize=13)
ax.tick_params(colors="white")
for spine in ax.spines.values():
    spine.set_edgecolor("#444466")
ax.grid(True, color="#222244", linewidth=0.4, linestyle="--")

# Primarios
ax.plot(-mu, 0, 'o', color="#00cfff", markersize=12, zorder=5, label=r"$m_1$")
ax.plot(1 - mu, 0, 'o', color="#ff6b6b", markersize=12, zorder=5, label=r"$m_2$")



# Elementos animados
trail_len = 120  # longitud de la estela
trail, = ax.plot([], [], '-', color="#a78bfa", linewidth=1.2, alpha=0.7)
head,  = ax.plot([], [], 'o', color="white", markersize=6, zorder=6)
time_text = ax.text(0.03, 0.96, '', transform=ax.transAxes,
                    color="white", fontsize=10, va='top')

# -------------------------------------------------------
# Funciones de animación
# -------------------------------------------------------
def init():
    trail.set_data([], [])
    head.set_data([], [])
    time_text.set_text('')
    return trail, head, time_text

def update(frame):
    i_start = max(0, frame - trail_len)
    trail.set_data(x_anim[i_start:frame+1], y_anim[i_start:frame+1])
    head.set_data([x_anim[frame]], [y_anim[frame]])
    time_text.set_text(f"t = {t_anim[frame]:.2f} [adim.]")
    return trail, head, time_text

# -------------------------------------------------------
# Crear y guardar animación
# -------------------------------------------------------
ani = animation.FuncAnimation(
    fig, update,
    frames=N,
    init_func=init,
    interval=20,
    blit=True
)

out_path = os.path.join(os.path.dirname(__file__), "../results/animacion.gif")
print("Generando animación, esto puede tomar unos segundos...")
ani.save(out_path, writer="pillow", fps=30, dpi=100)
print(f"Animación guardada en results/animacion.gif")
plt.show()
