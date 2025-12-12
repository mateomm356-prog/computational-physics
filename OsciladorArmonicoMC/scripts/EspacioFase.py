import numpy as np
import matplotlib.pyplot as plt

# === Cargar datos ===
data = np.loadtxt("results/oscilador_canonico.dat")
x = data[:, 0]
p = data[:, 1]

# === Scatter plot (espacio de fase) ===
plt.figure(figsize=(6,6))
plt.scatter(x, p, s=1, alpha=0.25, color='black')

plt.xlabel("x (m)")
plt.ylabel("p (kg·m/s)")
plt.title("Espacio de Fase (x, p) — MCMC Canónico (SI)")
plt.grid(True)

# ❗ IMPORTANTE: quitar axis('equal') para que la figura no colapse
# plt.axis('equal')   # <- LO QUITAMOS

plt.tight_layout()
plt.savefig("results/espacio_fase.png", dpi=150)

print("Espacio de fase guardado en results/espacio_fase.png")
