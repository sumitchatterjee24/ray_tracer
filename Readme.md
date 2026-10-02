A 3D optical ray-tracing simulation in C++ that models refraction across anisotropic media (uniaxial crystals/quartz) using Fermat's principle of least optical path length, visualized via Python/Matplotlib.

## Features
- **Tensor-based Index Calculations:** 3D dielectric permittivity tensor transformations ($n = \sqrt{\hat{d}^T \mathbf{T} \hat{d}}$).
- **Fermat Path Optimization:** Numerical minimization of optical path length across dielectric boundaries.
- **Parametric Grid:** Configurable 3D voxel resolution and material interface specifications.
- **Orthographic & 3D Visualizer:** Python script rendering the 3D ray trajectory and 2D projections ($XY, XZ, YZ$).

## Project Structure
ray_tracer/
├── include/
│   ├── Grid.hpp
│   └── Solver.hpp
├── src/
│   ├── Grid.cpp
│   ├── Solver.cpp
│   └── main.cpp
├── CMakeLists.txt
├── visualize.py
└── README.md
## Build and Run

### Prerequisites
- C++17 compliant compiler (`g++` / `clang++`)
- CMake (>= 3.16)
- Python 3 with `numpy`, `pandas`, `matplotlib`

### Compile

mkdir -p build && cd build
cmake ..
make
./RayTracer


cd ..
python3 visualize.py