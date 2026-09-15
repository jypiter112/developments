import matplotlib.pyplot as plt
import numpy as np

# --- scalar field f(x,y) = x^2 + y^2 (a bowl) ---
def f(x, y):
    return x**2 + y**2

def grad_f(x, y):
    return 2*x, 2*y

# --- surface for context ---
X, Y = np.meshgrid(np.linspace(-2, 2, 50), np.linspace(-2, 2, 50))
Z = f(X, Y)

# --- vector field (gradient) on a coarser grid ---
gx, gy = np.meshgrid(np.linspace(-2, 2, 8), np.linspace(-2, 2, 8))
gz = f(gx, gy)
gu, gv = grad_f(gx, gy)
gw = np.zeros_like(gu)  # gradient lies in the x-y plane, projected onto surface

# --- gradient descent from a starting point ---
start = np.array([-1.8, -1.8])
lr = 0.1
steps = 30

path = [start.copy()]
point = start.copy()
for _ in range(steps):
    dx, dy = grad_f(*point)
    point = point - lr * np.array([dx, dy])
    path.append(point.copy())

path = np.array(path)
path_z = f(path[:, 0], path[:, 1])

# --- plot ---
fig = plt.figure(figsize=(9, 7))
ax = fig.add_subplot(projection="3d")

# surface
ax.plot_surface(X, Y, Z, cmap="viridis", alpha=0.5, edgecolor="none")

# vector field (gradient arrows on the surface)
magnitude = np.sqrt(gu**2 + gv**2)
colors = plt.cm.plasma(magnitude / magnitude.max())
ax.quiver(gx, gy, gz, gu, gv, gw, length=0.15, normalize=True,
          colors=colors.reshape(-1, 4), linewidth=1)

# steepest descent path
ax.plot(path[:, 0], path[:, 1], path_z, color="red", linewidth=2, marker="o", markersize=3)
ax.scatter(*path[0], path_z[0], color="black", s=60, label="start")
ax.scatter(*path[-1], path_z[-1], color="green", s=60, label="end (min)")

ax.set_xlabel("x")
ax.set_ylabel("y")
ax.set_zlabel("f(x, y)")
ax.legend()
plt.show()
