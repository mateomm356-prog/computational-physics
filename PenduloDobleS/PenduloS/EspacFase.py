import numpy as np
import matplotlib.pyplot as plt

# Cargar datos
data = np.loadtxt("pendulo_doble.dat", skiprows=1)

t = data[:,0]
theta1 = data[:,1]
theta2 = data[:,2]
P1 = data[:,3]
P2 = data[:,4]

# Crear figura
fig, axs = plt.subplots(1, 2, figsize=(12,6))

# Espacio de fase (theta1, P1) con colormap rainbow
sc1 = axs[0].scatter(theta1, P1, c=t, cmap='rainbow', s=1)
axs[0].set_xlabel(r"$\theta_1$ (rad)")
axs[0].set_ylabel(r"$P_1$")
axs[0].set_title("Espacio de fase con β=0 $(\\theta_1, P_1)$")
axs[0].grid(True)
cbar1 = plt.colorbar(sc1, ax=axs[0])
cbar1.set_label("Tiempo")

# Espacio de fase (theta2, P2) con colormap rainbow
sc2 = axs[1].scatter(theta2, P2, c=t, cmap='rainbow', s=1)
axs[1].set_xlabel(r"$\theta_2$ (rad)")
axs[1].set_ylabel(r"$P_2$")
axs[1].set_title("Espacio de fase β=0 $(\\theta_2, P_2)$")
axs[1].grid(True)
cbar2 = plt.colorbar(sc2, ax=axs[1])
cbar2.set_label("Tiempo")

plt.tight_layout()
plt.show()
