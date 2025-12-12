import numpy as np
import matplotlib.pyplot as plt

# === Cargar datos ===
data = np.loadtxt("results/oscilador_canonico.dat")

# tercera columna = energía E
E = data[:, 2]

# === Histograma ===
plt.figure(figsize=(7,5))

plt.hist(E, bins=80, density=True, alpha=0.7)

plt.xlabel("Energía E (J)")
plt.ylabel("Distribución")
plt.title("Histograma de Energía — MCMC Canónico")
plt.grid(True)

plt.tight_layout()
plt.savefig("results/histograma_E.png", dpi=150)
plt.show()
