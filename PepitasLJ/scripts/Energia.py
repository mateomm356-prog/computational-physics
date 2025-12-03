import numpy as np
import matplotlib.pyplot as plt

# Parámetros Lennard-Jones (de tu main.cpp)
epsilon0 = 1.0
sigma = 0.1
masa = 1.0  # todas las bolas igual

# Leer archivo
datos = np.loadtxt("results/datos.txt")
tiempos = datos[:,0]
N = (datos.shape[1] - 1) // 4

K_list = []
U_list = []

for fila in datos:
    t = fila[0]
    r = []
    v = []
    for i in range(N):
        x = fila[1 + 4*i]
        y = fila[1 + 4*i + 1]
        vx = fila[1 + 4*i + 2]
        vy = fila[1 + 4*i + 3]
        r.append([x,y])
        v.append([vx,vy])
    r = np.array(r)
    v = np.array(v)

    # Energia cinetica
    K = 0.5 * masa * np.sum(np.sum(v**2, axis=1))
    K_list.append(K)

    # Energia potencial Lennard-Jones
    U = 0.0
    for i in range(N):
        for j in range(i+1, N):
            dr = r[j] - r[i]
            dist = np.linalg.norm(dr)
            if dist < 1e-12: dist = 1e-12
            sr6 = (sigma / dist)**6
            sr12 = sr6**2
            U += 4*epsilon0*(sr12 - sr6)
    U_list.append(U)

K_list = np.array(K_list)
U_list = np.array(U_list)
E_list = K_list + U_list

# Graficas
plt.figure(figsize=(8,5))
plt.plot(tiempos, K_list, label="Kinetica")
plt.plot(tiempos, U_list, label="Potencial LJ")
plt.plot(tiempos, E_list, label="Total")
plt.xlabel("Tiempo")
plt.ylabel("Energia")
plt.legend()
plt.title("Energia del sistema vs tiempo")
plt.grid(True)
plt.show()

# Promedios
print("K promedio:", np.mean(K_list))
print("U promedio:", np.mean(U_list))
print("E total promedio:", np.mean(E_list))
