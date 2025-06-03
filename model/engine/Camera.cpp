//
// Created by anton on 6/3/25.
//

#include "Camera.h"

#include <Eigen/Dense>

const Point3D& Camera::getPosition() const
{
    return position;
}

void Camera::setPosition(const Point3D& position)
{
    this->position = position;
    updateAxes();
}

const Point3D& Camera::getViewPoint() const
{
    return viewPoint;
}

void Camera::setViewPoint(const Point3D& viewPoint)
{
    this->viewPoint = viewPoint;
    updateAxes();
}

const Point3D& Camera::getUpVector() const
{
    return upVector;
}

void Camera::setUpVector(const Point3D& upVector)
{
    this->upVector = upVector;
    updateAxes();
}

double Camera::getZf() const
{
    return zf;
}

void Camera::setZf(const double zf)
{
    this->zf = zf;
}

double Camera::getZb() const
{
    return zb;
}

void Camera::setZb(const double zb)
{
    this->zb = zb;
}

double Camera::getSw() const
{
    return sw;
}

void Camera::setSw(const double sw)
{
    this->sw = sw;
}

double Camera::getSh() const
{
    return sh;
}

void Camera::setSh(const double sh)
{
    this->sh = sh;
}

Camera::Camera(Point3D pos, Point3D view, Point3D up, double zf, double zb, double sw, double sh): position{pos},
    viewPoint{view}, upVector{up}, zf{zf}, zb{zb}, sw{sw}, sh{sh}
{
    updateAxes();
}

void Camera::updateAxes()
{
    Point3D zVector = viewPoint - position;
    Eigen::Vector3d Zv{zVector.x(), zVector.y(), zVector.z()};
    Eigen::Vector3d rawYv{upVector.x(), upVector.y(), upVector.z()};
    Eigen::Vector3d Xv = rawYv.cross(Zv);
    Xv.normalize();
    Eigen::Vector3d Yv = Zv.cross(Xv);
    Yv.normalize();
    upVector = {Yv.x(), Yv.y(), Zv.z()};
    xVector = {Xv.x(), Xv.y(), Zv.z()};
}

Eigen::Vector3d Camera::rotateAround(Eigen::Vector3d axis, Eigen::Vector3d vector, double angle)
{
    double cosA = std::cos(angle / 180.0 * M_PI);
    double sinA = std::sin(angle / 180.0 * M_PI);

    double cosr = 1 - cosA;
    double x = upVector.x();
    double y = upVector.y();
    double z = upVector.z();

    Eigen::Matrix3d rotation{
        {
            x * x * cosr + cosA, x * y * cosr - z * sinA, x * z * cosr + y * sinA
        },
        {x * y * cosr + z * sinA, y * y * cosr + cosA, y * z * cosr - x * sinA},
        {x * z * cosr - y * sinA, y * z * cosr + x * sinA, z * z * cosr + cosA}
    };

    Eigen::Vector3d newV = rotation * vector;
    return newV;
}

void Camera::updateSystemMatrix()
{
    systemMatrix = {
        {1, 0, 0, -position.x},
        {0, 1, 0, -position.y},
        {0, 0, 1, -position.z},
        {0, 0, 0, 1}
    };
    updateCameraMatrix();
}

void Camera::addRotation(Eigen::Matrix4d& totalRotation, const Eigen::Vector3d& begin, const Eigen::Vector3d& end)
{
    //check that we should rotate
    if (begin.dot(end) == 0.0)
    {
        return;
    }

    //find axis vector
    Eigen::Vector3d a{begin.x, begin.y, begin.z};
    Eigen::Vector3d b{end.x, end.y, end.z};
    a.normalize();
    b.normalize();
    Eigen::Vector3d norm = a.cross(b);
    //find angle
    double cosA = a.dot(b) / (a.norm() * b.norm());
    double sinA = norm.norm() / (a.norm() * b.norm());
    //find diff matrix
    double cosr = 1 - cosA;
    norm.normalize();
    double x = norm.x();
    double y = norm.y();
    double z = norm.z();

    Eigen::Matrix4d addRot{
        {x * x * cosr + cosA, x * y * cosr - z * sinA, x * z * cosr + y * sinA, 0},
        {x * y * cosr + z * sinA, y * y * cosr + cosA, y * z * cosr - x * sinA, 0},
        {x * z * cosr - y * sinA, y * z * cosr + x * sinA, z * z * cosr + cosA, 0},
        {0, 0, 0, 1}
    };
    //product total matrix on diff

    totalRotation = totalRotation * addRot;
}

void Camera::updateRotationMatrix()
{
    rotationMatrix = Eigen::Matrix4d::Identity();

    // connect z axes
    Eigen::Vector3d spaceZ {0,0,1};
    Eigen::Vector3d cameraZ {upVector.x(), upVector.y(), upVector.z()};
    addRotation(rotationMatrix, cameraZ, spaceZ);

    // connect x axes
    Eigen::Vector3d spaceX {0,0,1};
    Eigen::Vector3d cameraX {upVector.x(), upVector.y(), upVector.z()};
    cameraX = rotationMatrix * cameraX;
    addRotation(rotationMatrix, cameraX, spaceX);

    updateCameraMatrix();
}

void Camera::updateProjectionMatrix()
{
}

void Camera::updateCameraMatrix()
{
    cameraMatrix = projectionMatrix * rotationMatrix;
}

Eigen::Matrix4d Camera::getCameraMatrix() const
{
    return cameraMatrix;
}

void Camera::move(const Point3D& step)
{
    position += step;
    viewPoint += step;
}


void Camera::rotateAroundUp(const double& angle)
{
    Point3D preparedZ = viewPoint - position;
    Eigen::Vector3d prevZ{preparedZ.x(), preparedZ.y(), preparedZ.z()};
    Eigen::Vector3d upAxis{upVector.x, upVector.y, upVector.z};
    auto newZ = rotateAround(upAxis, prevZ, angle);
    viewPoint = {newZ.x() + position.x, newZ.y() + position.y, newZ.z() + position.z};
    updateAxes();
}

void Camera::rotateAroundX(const double& angle)
{
    double preparedAngle;
    if (angle > 80.0)
    {
        preparedAngle = 80.0;
    }
    else if (angle < -80.0)
    {
        preparedAngle = -80.0;
    }

    Point3D preparedZ = viewPoint - position;
    Eigen::Vector3d prevZ{preparedZ.x, preparedZ.y, preparedZ.z};
    Eigen::Vector3d xAxis{xVector.x, xVector.y, xVector.z};
    auto newZ = rotateAround(xAxis, prevZ, preparedAngle);
    viewPoint = {newZ.x() + position.x, newZ.y() + position.y, newZ.z() + position.z};

    updateAxes();
}

void Camera::rotateAroundZ(const double& angle)
{
    Point3D preparedZ = viewPoint - position;
    Eigen::Vector3d zAxis{preparedZ.x(), preparedZ.y(), preparedZ.z()};
    Eigen::Vector3d prevUp{upVector.x, upVector.y, upVector.z};
    Eigen::Vector3d newUp = rotateAround(zAxis, prevUp, angle);
    upVector = {newUp.x(), newUp.y(), newUp.z()};
    updateAxes();
}
