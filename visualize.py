import os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.lines import Line2D

# Load CSV output
csv_path = "build/ray_path.csv" if os.path.exists("build/ray_path.csv") else "ray_path.csv"
data = pd.read_csv(csv_path)
x, y, z = data['x'].values, data['y'].values, data['z'].values

MAX_DIM = 200

fig = plt.figure(figsize=(10, 8))
ax = fig.add_subplot(111, projection='3d')



ax.plot(x, y, z, color='crimson', linewidth=2.0)
ax.scatter([x[0]], [y[0]], [z[0]], color='green', s=60)
ax.scatter([x[-1]], [y[-1]], [z[-1]], color='blue', s=60)

ax.set_xlim(0, MAX_DIM)
ax.set_ylim(0, MAX_DIM)
ax.set_zlim(0, MAX_DIM)
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
ax.set_title("Anisotropic Fermat Ray Path (Vacuum vs. Quartz)", fontsize=13, fontweight='bold')

# Explicit legend 
legend_elements = [
    Line2D([0], [0], color='crimson', lw=2, label='Ray Trajectory'),
    Line2D([0], [0], marker='o', color='w', markerfacecolor='green', markersize=8, label=f'Start ({int(x[0])},{int(y[0])},{int(z[0])})'),
    Line2D([0], [0], marker='o', color='w', markerfacecolor='blue', markersize=8, label=f'Target ({int(x[-1])},{int(y[-1])},{int(z[-1])})'),
 
]
ax.legend(handles=legend_elements, loc='upper left')

plt.tight_layout()
plt.savefig("ray_3d.png", dpi=200)
plt.show()

fig2, (ax_xy, ax_xz, ax_yz) = plt.subplots(1, 3, figsize=(16, 5))

# XY
ax_xy.plot(x, y, color='crimson', linewidth=1.8)

ax_xy.set_ylim(0, MAX_DIM)
ax_xy.set_title("XY Projection (Top View)")
ax_xy.set_xlabel("X")
ax_xy.set_ylabel("Y")
ax_xy.grid(True)
ax_xy.legend()

# XZ
ax_xz.plot(x, z, color='navy', linewidth=1.8)

ax_xz.set_xlim(0, MAX_DIM)
ax_xz.set_ylim(0, MAX_DIM)
ax_xz.set_title("XZ Projection (Front View)")
ax_xz.set_xlabel("X")
ax_xz.set_ylabel("Z")
ax_xz.grid(True)
ax_xz.legend()

# YZ
ax_yz.plot(y, z, color='darkgreen', linewidth=1.8)
ax_yz.set_xlim(0, MAX_DIM)
ax_yz.set_ylim(0, MAX_DIM)
ax_yz.set_title("YZ Projection (Side View)")
ax_yz.set_xlabel("Y")
ax_yz.set_ylabel("Z")
ax_yz.grid(True)

plt.suptitle("Orthogonal Ray Projections Showing Interface Refraction", fontsize=14, fontweight='bold')
plt.tight_layout()
plt.savefig("ray_2d_projections.png", dpi=200)
plt.show()