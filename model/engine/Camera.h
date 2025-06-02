//
// Created by anton on 6/3/25.
//

#ifndef CAMERA_H
#define CAMERA_H

#include <Eigen/src/Core/Matrix.h>
#include <Eigen/src/Core/functors/TernaryFunctors.h>

#include "Figure.h"

class Camera
{
public:
    [[nodiscard]] const Point3D& getPosition() const;
    void setPosition(const Point3D& position);
    [[nodiscard]] const Point3D& getViewPoint() const;
    void setViewPoint(const Point3D& viewPoint);
    [[nodiscard]] const Point3D& getUpVector() const;
    void setUpVector(const Point3D& upVector);
    [[nodiscard]] double getZf() const;
    void setZf(double zf);
    [[nodiscard]] double getZb() const;
    void setZb(double zb);
    [[nodiscard]] double getSw() const;
    void setSw(double sw);
    [[nodiscard]] double getSh() const;
    void setSh(double sh);

    void move(const Point3D& step);
    void rotateAroundUp(const double& angle);
    void rotateAroundX(const double& angle);
    void rotateAroundZ(const double& angle);

    Camera(Point3D pos, Point3D view, Point3D up, double zf, double zb, double sw, double sh);

private:
    void updateAxes();
    Eigen::Vector3d rotateAround(Eigen::Vector3d axis, Eigen::Vector3d vector, double angle);

private:
    Point3D position;
    Point3D viewPoint;
    Point3D upVector;
    Point3D xVector{};

    double zf;
    double zb;

    double sw;
    double sh;
};


#endif //CAMERA_H
