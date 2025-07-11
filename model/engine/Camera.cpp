//
// Created by anton on 6/3/25.
//

#include "Camera.h"

#include <iostream>
#include <eigen3/Eigen/Dense>

using Eigen::Vector3d;

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
    updateProjectionMatrix();
}

double Camera::getZb() const
{
    return zb;
}

void Camera::setZb(const double zb)
{
    this->zb = zb;
    updateProjectionMatrix();
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

void Camera::updateShSw(const double &q)
{
    sw = q * sh;
    updateProjectionMatrix();
}


void Camera::move(const Point3D& step)
{
    Point3D pStep{};
    pStep = zVector * step.y;
    Point3D sStep{xVector * step.x};
    pStep += sStep;
    position += pStep;
    viewPoint += pStep;
    updateSystemMatrix();
}


Camera::Camera(Point3D pos, Point3D view, Point3D up, double zf, double zb, double sw, double sh): position{pos},
    viewPoint{view}, upVector{up}, zf{zf}, zb{zb}, sw{sw}, sh{sh}
{
    updateAxes();
    updateSystemMatrix();
    updateProjectionMatrix();
}

void Camera::updateAxes()
{
    zVector = viewPoint - position;
    Eigen::Vector3d Zv{zVector.x, zVector.y, zVector.z};
    Eigen::Vector3d rawYv{upVector.x, upVector.y, upVector.z};
    Eigen::Vector3d Xv = rawYv.cross(Zv);
    Xv.normalize();
    Zv.normalize();
    Eigen::Vector3d Yv = Zv.cross(Xv);
    Yv.normalize();
    upVector = {Yv.x(), Yv.y(), Yv.z()};
    xVector = {Xv.x(), Xv.y(), Xv.z()};
    zVector = {Zv.x(), Zv.y(), Zv.z()};
    updateRotationMatrix();
}

Eigen::Vector3d Camera::rotateAround(Eigen::Vector3d axis, Eigen::Vector3d vector, double angle)
{
    double cosA = std::cos(angle / 180.0 * M_PI);
    double sinA = std::sin(angle / 180.0 * M_PI);

    double cosr = 1 - cosA;
    double x = axis.x();
    double y = axis.y();
    double z = axis.z();

    const Eigen::Matrix3d rotation{
        {
            x * x * cosr + cosA, x * y * cosr - z * sinA, x * z * cosr + y * sinA
        },
        {x * y * cosr + z * sinA, y * y * cosr + cosA, y * z * cosr - x * sinA},
        {x * z * cosr - y * sinA, y * z * cosr + x * sinA, z * z * cosr + cosA}
    };

    Eigen::Vector3d newV;
    newV = rotation * vector;
    return newV;
}

void Camera::updateSystemMatrix()
{
    systemMatrix = Eigen::Matrix4d{
        {1, 0, 0, -position.x},
        {0, 1, 0, -position.y},
        {0, 0, 1, -position.z},
        {0, 0, 0, 1}
    };

    systemMatrixInverse = Eigen::Matrix4d{
        {1, 0, 0, position.x},
        {0, 1, 0, position.y},
        {0, 0, 1, position.z},
        {0, 0, 0, 1}
    };
    updateCameraMatrix();
}

void Camera::addRotation(Eigen::Matrix4d& totalRotation, const Eigen::Vector3d& begin, const Eigen::Vector3d& end, const Eigen::Vector3d& helpAxis)
{
    //check that we should rotate
    if (begin.dot(end) == begin.norm() * end.norm())
    {
        return;
    }
    double bx = begin.x();
    double by = begin.y();
    double bz = begin.z();
    double ex = end.x();
    double ey = end.y();
    double ez = end.z();
    //find axis vector
    Vector3d a(bx, by, bz);
    Vector3d b(ex, ey, ez);
    a.normalize();
    b.normalize();
    Eigen::Vector3d norm = a.cross(b);
    bool isAnti = false;
    if (norm.norm() == 0)
    {
        norm = helpAxis;
        isAnti = true;
    }
    //find angle
    double cosA = a.dot(b) / (a.norm() * b.norm());
    double sinA = norm.norm() / (a.norm() * b.norm());
    if (isAnti)
        sinA = 0;
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

    totalRotation = addRot * totalRotation;
}

void Camera::updateRotationMatrix()
{
    rotationMatrix = Eigen::Matrix4d::Identity();
    rotationMatrixInverse = Eigen::Matrix4d::Identity();
    Eigen::Matrix4d rotationMatrixInverseHelp = Eigen::Matrix4d::Identity();
    // connect z axes
    Eigen::Vector3d spaceZ{0, 0, 1};
    Eigen::Vector3d cameraZ{zVector.x, zVector.y, zVector.z};
    Eigen::Vector3d helpXAxis{xVector.x, xVector.y, xVector.z};


    addRotation(rotationMatrix, cameraZ, spaceZ, helpXAxis);

    // connect x axes
    Eigen::Vector3d spaceX{1, 0, 0};
    Eigen::Vector4d cameraX{xVector.x, xVector.y, xVector.z, 1};
    Vector3d preparedCameraX{cameraX.x(), cameraX.y(), cameraX.z()};
    addRotation(rotationMatrixInverse, spaceX, preparedCameraX, cameraZ);
    cameraX = rotationMatrix * cameraX;
    preparedCameraX.x() = cameraX.x();
    preparedCameraX.y() = cameraX.y();
    preparedCameraX.z() = cameraX.z();
    addRotation(rotationMatrix, preparedCameraX, spaceX, spaceZ);

    Eigen::Vector4d auxSpaceZ = {0,0,1,1};
    auxSpaceZ = rotationMatrixInverse * auxSpaceZ;
    Eigen::Vector3d preparedAuxZ{auxSpaceZ.x(), auxSpaceZ.y(), auxSpaceZ.z()};
    addRotation(rotationMatrixInverse, preparedAuxZ, cameraZ, helpXAxis);
    updateCameraMatrix();
}

void Camera::updateProjectionMatrix()
{
    projectionMatrix = Eigen::Matrix4d{
        {2 / sw * zf, 0, 0, 0},
        {0, 2 / sh * zf, 0, 0},
        {0, 0, zb / (zb - zf), -zf * zb / (zb - zf)},
        {0, 0, 1, 0}
    };
    updateCameraMatrix();
}

void Camera::updateCameraMatrix()
{
    cameraMatrix = projectionMatrix * rotationMatrix * systemMatrix;
    cameraMatrixInverse = systemMatrixInverse * rotationMatrixInverse;
}

Eigen::Matrix4d Camera::getCameraMatrix() const
{
    return cameraMatrix;
}

Eigen::Matrix4d Camera::getCameraMatrixInverse() const
{
    return cameraMatrixInverse;
}


void Camera::rotateAroundUp(const double& angle)
{
    Point3D preparedZ = viewPoint - position;
    Eigen::Vector3d prevZ{preparedZ.x, preparedZ.y, preparedZ.z};
    Eigen::Vector3d upAxis{upVector.x, upVector.y, upVector.z};
    auto newZ = rotateAround(upAxis, prevZ, angle);
    viewPoint = Point3D{newZ.x() + position.x, newZ.y() + position.y, newZ.z() + position.z};
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
    } else
    {
        preparedAngle = angle;
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
    Eigen::Vector3d zAxis{preparedZ.x, preparedZ.y, preparedZ.z};
    Eigen::Vector3d prevUp{upVector.x, upVector.y, upVector.z};
    Eigen::Vector3d newUp = rotateAround(zAxis, prevUp, angle);
    upVector = Point3D{newUp.x(), newUp.y(), newUp.z()};
    updateAxes();
}

bool Camera::isVisible(const Point3D& point) const
{
    Vector3d viewVector{viewPoint.x - position.x, viewPoint.y - position.y, viewPoint.z - position.z};
    viewVector.normalize();
    Vector3d objVector{
        point.x - (position.x + viewVector.x() * zf),
        point.y - (position.y + viewVector.y() * zf),
        point.z - (position.z + viewVector.z() * zf)
    };

    return viewVector.dot(objVector) > 0;
}

Ray Camera::emitRay(int x, int y, int iw, int ih) const
{
    double px = - (x + 0.5) / iw * sw + static_cast<double>(sw) / 2.0;
    double py = - (y + 0.5) / ih * sh + static_cast<double>(sh) / 2.0;
    Eigen::Vector4d ray{px,py , zf, 1};
    ray = cameraMatrixInverse * ray;
    ray /= ray.w();
    const auto result = Ray{position, {ray.x()-position.x, ray.y()-position.y, ray.z()-position.z}};
    return result;
}
