#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "Grid.hpp"

struct Node{
    std::array<int, 3> grid_index;
    double total_optical_cost;

    bool operator>(const Node& other) const{  // comparitor for algo 
        return total_optical_cost> other.total_optical_cost;
    }
};

class Solver{
    public:
        Solver(const Grid& phy_grid);
        // dijkastra's algorithm
        std::vector<Vector3D> findShortestOpticalPath(std::array<int, 3> start, std::array<int, 3> target);
    private:
        const Grid& grid;

};


#endif                                                                                                             