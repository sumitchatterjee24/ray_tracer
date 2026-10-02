#include "Solver.hpp"
#include <iostream>
#include <queue>
#include <map>
#include <cmath>
#include <algorithm>

Solver::Solver(const Grid& phy_grid): grid(phy_grid) {}

inline double getHeuristic(const std::array<int, 3>& a, const std::array<int, 3>& target, const std::array<int, 3>& start) {
    double dx = a[0] - target[0];
    double dy = a[1] - target[1];
    double dz = a[2] - target[2];
    double h = std::sqrt(dx * dx + dy * dy + dz * dz);

    // Cross-product straight-line tie-breaker
    double line_dx = target[0] - start[0];
    double line_dy = target[1] - start[1];
    double line_dz = target[2] - start[2];

    double cross_x = (a[1] - start[1]) * line_dz - (a[2] - start[2]) * line_dy;
    double cross_y = (a[2] - start[2]) * line_dx - (a[0] - start[0]) * line_dz;
    double cross_z = (a[0] - start[0]) * line_dy - (a[1] - start[1]) * line_dx;
    double cross_dist = std::sqrt(cross_x * cross_x + cross_y * cross_y + cross_z * cross_z);

    return h * 1.5 + (cross_dist * 0.001);
}

struct ComparePQ {
    bool operator()(const std::pair<double, Node>& a, const std::pair<double, Node>& b) const {
        return a.first > b.first;
    }
};

std::vector<Vector3D> Solver::findShortestOpticalPath(std::array<int, 3> start, std::array<int, 3> target){
    typedef std::pair<double, Node> PQElement;
    std::priority_queue<PQElement, std::vector<PQElement>, ComparePQ> pq;

    std::map<std::array<int, 3>, double> cost;
    std::map<std::array<int, 3>, std::array<int, 3>> path;

    // --- DYNAMIC BOUNDS FETCHED FROM GRID ---
    std::array<int, 3> bounds = grid.getDimensions();
    const int max_x = bounds[0];
    const int max_y = bounds[1];
    const int max_z = bounds[2];

    cost[start] = 0.0;
    pq.push({getHeuristic(start, target, start) * 1.0001, {start, 0.0}});
    bool target_reached = false;

    const std::array<int, 3> directions[26] = {
        {1, 0, 0}, {-1, 0, 0},
        {0, 1, 0}, {0, -1, 0},
        {0, 0, 1}, {0, 0, -1},
        {1, 1, 0}, {-1, -1, 0},
        {1, -1, 0}, {-1, 1, 0},
        {0, 1, 1}, {0, -1, -1},
        {0, 1, -1}, {0, -1, 1},
        {1, 0, 1}, {-1, 0, -1},
        {1, 0, -1}, {-1, 0, 1},
        {1, 1, 1}, {-1, -1, -1},
        {1, 1, -1}, {-1, -1, 1},
        {1, -1, 1}, {-1, 1, -1},
        {1, -1, -1}, {-1, 1, 1}
    };

    while(!pq.empty() && !target_reached){
        Node current = pq.top().second;
        pq.pop();

        if (current.grid_index == target) {
            target_reached = true;
            break;
        }

        if (current.total_optical_cost > cost[current.grid_index]) {
            continue;
        }

        for (const auto& dir : directions) {
            std::array<int, 3> neighbor = {
                current.grid_index[0] + dir[0],
                current.grid_index[1] + dir[1],
                current.grid_index[2] + dir[2]
            };

            // DYNAMIC BOUNDARY CONSTRAINTS
            if (neighbor[0] < 0 || neighbor[0] >= max_x ||
                neighbor[1] < 0 || neighbor[1] >= max_y ||
                neighbor[2] < 0 || neighbor[2] >= max_z) {
                continue;
            }

            Vector3D direc_vector = {
                static_cast<double>(dir[0]), 
                static_cast<double>(dir[1]), 
                static_cast<double>(dir[2])
            };

            double ri = grid.getRI(current.grid_index, direc_vector);
            double length = std::sqrt(static_cast<double>(dir[0]*dir[0] + dir[1]*dir[1] + dir[2]*dir[2]));
            double next_cost = current.total_optical_cost + (ri * length);

            if (cost.find(neighbor) == cost.end() || next_cost < cost[neighbor]) {
                cost[neighbor] = next_cost;
                path[neighbor] = current.grid_index;

                double f_score = next_cost + getHeuristic(neighbor, target, start);
                pq.push({f_score, {neighbor, next_cost}});
            }
        }
    }

    std::vector<Vector3D> result_path;
    std::array<int, 3> curr = target;
    while (path.find(curr) != path.end()) {
        result_path.push_back({static_cast<double>(curr[0]), static_cast<double>(curr[1]), static_cast<double>(curr[2])});
        curr = path[curr];
    }
    result_path.push_back({static_cast<double>(start[0]), static_cast<double>(start[1]), static_cast<double>(start[2])});
    std::reverse(result_path.begin(), result_path.end());
    return result_path;
}
