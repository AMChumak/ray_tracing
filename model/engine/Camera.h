//
// Created by anton on 6/3/25.
//

#ifndef CAMERA_H
#define CAMERA_H

#include <eigen3/Eigen/Dense>

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

    [[nodiscard]] Ray emitRay(double x, double y) const;

    void zoom(double q);

    [[nodiscard]] Eigen::Matrix4d getCameraMatrix() const;
    [[nodiscard]] Eigen::Matrix4d getCameraMatrixInverse() const;

    Camera(Point3D pos, Point3D view, Point3D up, double zf, double zb, double sw, double sh);

private:
    void updateAxes();
    Eigen::Vector3d rotateAround(Eigen::Vector3d axis, Eigen::Vector3d vector, double angle);
    void updateSystemMatrix();
    static void addRotation (Eigen::Matrix4d &totalRotation, const Eigen::Vector3d &begin, const Eigen::Vector3d &end);
    void updateRotationMatrix();
    void updateProjectionMatrix();
    void updateCameraMatrix();

private:
    Point3D position{};
    Point3D viewPoint{};
    Point3D upVector{};
    Point3D xVector{};
    Point3D zVector{};

    double zf;
    double zb;

    double sw;
    double sh;

    Eigen::Matrix4d systemMatrix; //matrix to transition between World system and camera system
    Eigen::Matrix4d systemMatrixInverse;
    Eigen::Matrix4d rotationMatrix; //matrix for rotation
    Eigen::Matrix4d rotationMatrixInverse;
    Eigen::Matrix4d projectionMatrix; // matrix of projection to screen
    Eigen::Matrix4d cameraMatrix; // production of these matrices
    Eigen::Matrix4d cameraMatrixInverse;
};

inline void Camera::zoom(double q)
{
    if (q <= 0) return;

    zf *= q;
    updateProjectionMatrix();
}


#endif //CAMERA_H
