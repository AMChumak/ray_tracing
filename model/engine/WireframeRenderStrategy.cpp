//
// Created by anton on 6/3/25.
//

#include "WireframeRenderStrategy.h"
#include <QPainter>
#include <eigen3/Eigen/Dense>

int comparePoints(const Point3D& p1, const Point3D& p2)
{
    if (p1.z - p2.z > 0) return 1;
    else if (p1.z - p2.z < 0) return -1;
    else return 0;
}


void WireframeRenderStrategy::render(QImage& image, SceneDescription& scene, ConfigState& config, Camera& camera)
{
    std::vector<Line> lines{};

    Eigen::Matrix4d transformation = camera.getCameraMatrix();

    for (int i = 0; i < scene.figures.size(); i++)
    {
        auto& figure = scene.figures[i];
        std::vector<Polygon> polygons = figure->polygons();
        QColor linesColor{
            static_cast<int>(scene.optics[i].kdr * 255),
            static_cast<int>(scene.optics[i].kdg * 255),
            static_cast<int>(scene.optics[i].kdb * 255)
        };
        for (auto polygon : polygons)
        {
            if (!camera.isVisible(polygon.points[0]) || !camera.isVisible(polygon.points[1]) || !camera.isVisible(polygon.points[2]))
                continue;
            Eigen::Vector4d vA{polygon.points[0].x, polygon.points[0].y, polygon.points[0].z,1};
            Eigen::Vector4d vB{polygon.points[1].x, polygon.points[1].y, polygon.points[1].z,1};
            Eigen::Vector4d vC{polygon.points[2].x, polygon.points[2].y, polygon.points[2].z,1};


            vA = transformation * vA;
            vB = transformation * vB;
            vC = transformation * vC;
            vA /= vA.w();
            vB /= vB.w();
            vC /= vC.w();

            // check normal
            bool isVisible = true;

            Eigen::Vector3d v1{(vA - vB).x(), (vA - vB).y(), (vA - vB).z()};
            Eigen::Vector3d v2{(vC - vA).x(),(vC - vA).y(),(vC - vA).z()};
            if (v1.cross(v2).z() <= 0) // co-directional normals, polygon is hidden so inverted cause of camera's X Axis
                isVisible = false;

            QColor lineColor = linesColor;
            if (!isVisible)
            {
                //continue;
                lineColor = {linesColor.red() / 2, linesColor.green() / 2, linesColor.blue() / 2};
            }

            Point3D p1{vA.x(), vA.y(), vA.z()};
            Point3D p2{vB.x(), vB.y(), vB.z()};
            Point3D p3{vC.x(), vC.y(), vC.z()};

            lines.push_back(Line{p1, p2, lineColor});
            lines.push_back(Line{p2, p3, lineColor});
            lines.push_back(Line{p3, p1, lineColor});
        }
    }
    std::sort(lines.begin(), lines.end(), [](const Line& a, const Line& b)
    {
        if (comparePoints(a.begin, b.begin) < 0) return true;
        return false;
    });


    //draw
    int w = image.size().width();
    int h = image.size().height();
    image.fill(QColor::fromRgb(config.br, config.bg, config.bb));

    QPainter painter(&image);

    for (auto& line : lines)
    {
        int x1 = w * line.begin.x + w / 2;
        int y1 = h * line.begin.y + h / 2;
        int x2 = w * line.end.x + w / 2;
        int y2 = h * line.end.y + h / 2;
        painter.setPen(QPen(line.color));
        painter.drawLine(x1, y1, x2, y2);
    }
    painter.end();
}
