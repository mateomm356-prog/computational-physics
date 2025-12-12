# histogramas.py
import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import norm

# === Cargar datos ===
data = np.loadtxt("results/oscilador_canonico.dat")
x = data[:, 0]
p = data[:, 1]

# Parámetros del caso simulado
mu_x = 0.0
sigma_x = 10   # sqrt(<x^2>) = 1 para T=1, k=1
mu_p = 0.0
sigma_p = 10   # sqrt(<p^2>) = 1 para T=1, m=1

# === Histograma de x ===
plt.figure(figsize=(7,5))
plt.hist(x, bins=60, density=True, alpha=0.6, color='steelblue', label='MC')

xvals = np.linspace(min(x), max(x), 300)
plt.plot(xvals, norm.pdf(xvals, mu_x, sigma_x), 'r-', lw=2, label='Gaussiana teórica')

plt.xlabel("x")
plt.ylabel("P(x)")
plt.title("Histograma de x con distribución teórica")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("results/histograma_x.png", dpi=150)

# === Histograma de p ===
plt.figure(figsize=(7,5))
plt.hist(p, bins=60, density=True, alpha=0.6, color='seagreen', label='MC')

pvals = np.linspace(min(p), max(p), 300)
plt.plot(pvals, norm.pdf(pvals, mu_p, sigma_p), 'r-', lw=2, label='Gaussiana teórica')

plt.xlabel("p")
plt.ylabel("P(p)")
plt.title("Histograma de p con distribución teórica")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("results/histograma_p.png", dpi=150)
