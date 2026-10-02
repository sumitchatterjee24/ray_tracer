#ifndef GRID_HPP
#define GRID_HPP

#include<vector>
#include<array>


struct Vector3D{
    double x, y, z;
    //liner oprators :)
    Vector3D operator+(const Vector3D& other) const{
        return{x+other.x, y+other.y, z+ other.z}
    }
    Vector3D operator-(const Vector3D& other) const{
        return{x - other.x, y - other.y, z- other.z}
    }
    Vector3D operator*(double scaler) const{
        return {x*scaler, y*scaler, z*scaler}
    }

};

struct Tensor3x3 {
    std::arry<std::arry<double,3>,3> matrix;
    Tensor3x3 Inverse() const;
    Vector3D Transform(const Vector3D& vec) const;//linear algebra hehehe

};

class Grid{
public:
   //constructor from input
   Grid(Tensor3x3 Transformation_matrix, double box_length, std::arry<double,3> grid_size);

   std::array<int, 3> GridIndex(const Vector3D& physicl_pos) const;//for position to index 

   double getRI(const std::array<int,3>& index, const Vector3D&direction) const;// to find direc-wise refrective ind mwehehehe
private:
    Tensor3x3 inverse_basis;
    double cell_size;
    std::array<double,3> dimension;
    std::vector<std::vector<std::vector<Tensor3x3>>> material_grid;//optical path 


};




#endif