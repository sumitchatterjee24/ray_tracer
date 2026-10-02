#include "Grid.hpp"

#include<cmath>
#include<algorithm>
#include <stdexcept>

Tensor3x3 Tensor3x3::Inverse() const{
    Tensor3x3 inv;
    double det = matrix[0][0]*(matrix[1][1]*matrix[2][2]-matrix[1][2]*matrix[2][1])-
                 matrix[0][1]*(matrix[1][0]*matrix[2][2]-matrix[1][2]*matrix[2][0])+
                 matrix[0][2]*(matrix[1][0]*matrix[2][1]-matrix[1][1]*matrix[2][0]);
   if (std::abs(det) < 1e-9) { // Using a tiny threshold is safer than det == 0 for doubles!
        throw std::runtime_error("Matrix is singular, cannot compute inverse.");
    }
    double inv_det = 1.0/det;
    inv.matrix[0][0] = (matrix[1][1]*matrix[2][2]-matrix[1][2]*matrix[2][1])*inv_det;
    inv.matrix[0][1] = (matrix[0][2]*matrix[2][1]-matrix[0][1]*matrix[2][2])*inv_det;
    inv.matrix[0][2] = (matrix[0][1]*matrix[1][2]-matrix[0][2]*matrix[1][1])*inv_det;
    inv.matrix[1][0] = (matrix[1][2]*matrix[2][0]-matrix[1][0]*matrix[2][2])*inv_det;
    inv.matrix[1][1] = (matrix[0][0]*matrix[2][2]-matrix[0][2]*matrix[2][0])*inv_det;
    inv.matrix[1][2] = (matrix[0][2]*matrix[1][0]-matrix[0][0]*matrix[1][2])*inv_det;
    inv.matrix[2][0] = (matrix[1][0]*matrix[2][1]-matrix[1][1]*matrix[2][0])*inv_det;
    inv.matrix[2][1] = (matrix[0][1]*matrix[2][0]-matrix[0][0]*matrix[2][1])*inv_det;
    inv.matrix[2][2] = (matrix[0][0]*matrix[1][1]-matrix[0][1]*matrix[1][0])*inv_det;

    return inv;
}

Vector3D Tensor3x3::Transform(const Vector3D& vec) const{
    Vector3D result;
    result.x = matrix[0][0]*vec.x + matrix[0][1]*vec.y + matrix[0][2]*vec.z;
    result.y = matrix[1][0]*vec.x + matrix[1][1]*vec.y + matrix[1][2]*vec.z;
    result.z = matrix[2][0]*vec.x + matrix[2][1]*vec.y + matrix[2][2]*vec.z;
    return result;
}

Grid ::Grid(Tensor3x3 Transformation_matrix, double box_length, std::array<double,3> grid_size){
    inverse_basis = Transformation_matrix.Inverse();
    cell_size = box_length;
    dimension = grid_size;

    Tensor3x3 I3 ={{{{1,0,0},{0,1,0},{0,0,1}}}};
    material_grid.resize(static_cast<size_t>(grid_size[0]), std::vector<std::vector<Tensor3x3>>(static_cast<size_t>(grid_size[1]), std::vector<Tensor3x3>(static_cast<size_t>(grid_size[2]),I3))); //later for n
}

std::array<int, 3> Grid::GridIndex(const Vector3D& physicl_pos) const{
    Vector3D local_pos = inverse_basis.Transform(physicl_pos);
    std::array<int, 3> index;
    index[0] = static_cast<int>(std::floor(local_pos.x / cell_size));
    index[1] = static_cast<int>(std::floor(local_pos.y / cell_size));
    index[2] = static_cast<int>(std::floor(local_pos.z / cell_size));

    index[0] = std::max(0, std::min(index[0], static_cast<int>(dimension[0]) - 1));
    index[1] = std::max(0, std::min(index[1], static_cast<int>(dimension[1]) - 1));
    index[2] = std::max(0, std::min(index[2], static_cast<int>(dimension[2]) - 1));
    return index;
}
double Grid::getRI(const std::array<int,3>& index, const Vector3D&direction) const{
    if(index[0]<0 || index[0]>=dimension[0] || index[1]<0 || index[1]>=dimension[1] || index[2]<0 || index[2]>=dimension[2]){
       return 1.0; //outside vacuum
    }
    double dis= std::sqrt(direction.x*direction.x + direction.y*direction.y + direction.z*direction.z);
    if(dis<1e-9){
        return 1.0; //small at edge
    }
    Vector3D dircap = {direction.x/dis ,direction.y/dis , direction.z/dis};
    const Tensor3x3& local_tensor = material_grid[index[0]][index[1]][index[2]];
    Vector3D eta_r = local_tensor.Transform(dircap);
    double r_eta_r= eta_r.x*dircap.x + eta_r.y*dircap.y + eta_r.z*dircap.z;
    return std::sqrt(std::max(1.0, r_eta_r));
}
    