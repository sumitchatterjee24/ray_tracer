#include "Solver.hpp"
#include "Grid.hpp"
#include <fstream>

#include <bits/stdc++.h>
using namespace std;

int main(){
    cout << "----starting anisotropic grid simulation----" << endl;

   
    Tensor3x3 transformation_matrix = {{{
        {{1.0, 0.0, 0.0}},
        {{0.0, 1.0, 0.0}},
        {{0.0, 0.0, 1.0}}
    }}}; 
    
    double box_length = 1.0;
    array<double, 3> grid_size = {200.0, 200.0, 200.0};

    
    Grid physics_grid(transformation_matrix, box_length, grid_size);

    
 

std::array<int, 3> start  = {10, 10, 10};
    std::array<int, 3> target = {90, 70, 50};
  
    double no = 1.4; // Ordinary index 
    double ne = 1.7; // Extraordinary index 
    
    Tensor3x3 quartz_tensor = {{{
        {{no * no, 0.0,     0.0    }},
        {{0.0,     no * no, 0.0    }},
        {{0.0,     0.0,     ne * ne}}
    }}};

    // Split the grid perfectly down the middle along the X-axis
   int NX = 200, NY = 200, NZ = 200;
  int interface_x = NX / 2; // 50

    for (int x = 0; x < NX; ++x) {
        for (int y = 0; y < NY; ++y) {
            for (int z = 0; z < NZ; ++z) {
                if (x >= interface_x) {
                    physics_grid.material_grid[x][y][z] = quartz_tensor;
                } else {
                    physics_grid.material_grid[x][y][z] = transformation_matrix;
                }
            }
        }
    }
    cout << "Grid successfully split: Left half [X < 25] is Vacuum | Right half [X >= 25] is Quartz!" << endl;
    // -------------------------------------------
       Solver ray_solver(physics_grid);
    cout << "Calculating shortest path using fermi's principle (^^)." << endl;
    vector<Vector3D> path = ray_solver.findShortestOpticalPath(start, target);

    std::ofstream out("ray_path.csv");
    if (out.is_open()) {
        out << "x,y,z\n";
        for (const auto& pt : path) {
            out << pt.x << "," << pt.y << "," << pt.z << "\n";
        }
        out.close();
        cout << "Exported trajectory to ray_path.csv" << endl;
    }

    cout << "Shortest path calculated (mwehehehe). length: " << path.size() << " voxels." << endl;
    cout << "Path coordinates (in grid indices):" << endl;
    
    
    size_t display_limit = min(path.size(), static_cast<size_t>(100));
    for(size_t i = 0; i < display_limit; ++i){
        cout << "(" << path[i].x << ", " << path[i].y << ", " << path[i].z << ")" << endl;
    }
    
   

    return 0;
}
