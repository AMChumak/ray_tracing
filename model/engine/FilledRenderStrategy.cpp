//
// Created by anton on 6/3/25.
//

#include "FilledRenderStrategy.h"

#include <iostream>
#include <qtextstream.h>
#include <omp.h>

struct Intensity
{
    double r;
    double g;
    double b;
};

struct ReflRes
{
    Ray normal;
    int objIdx;
};


ReflRes getNormalInNearestReflection(SceneDescription& scene, Ray& ray)
{
    int res = -1;
    Ray normal{};
    double norm = -1;
    for (int i = 0; scene.figures.size() > i; i++)
    {
        auto* figure = scene.figures[i];
        Ray candidate = figure->getNormalInReflection(ray);
        if (candidate.direction == Point3D{})
            continue;

        Eigen::Vector3d vec{
            candidate.origin.x - ray.origin.x, candidate.origin.y - ray.origin.y, candidate.origin.z - ray.origin.z
        };
        double cnorm = vec.norm();

        if (norm < 0 || cnorm < norm)
        {
            res = i;
            normal = candidate;
            norm = cnorm;
        }
    }
    return ReflRes{normal, res};
}

Intensity computePointIntensity(SceneDescription& scene, ConfigState& config, int objIdx, Ray antiV, Ray normal,
                                int tracingRest)
{
    Intensity intensity{};

    //add ambient light
    intensity.r += static_cast<double>(scene.props.ar) / 255.0 * scene.optics[objIdx].kdr;
    intensity.g += static_cast<double>(scene.props.ag) / 255.0 * scene.optics[objIdx].kdg;
    intensity.b += static_cast<double>(scene.props.ab) / 255.0 * scene.optics[objIdx].kdb;


    //normal and view vectors
    Eigen::Vector3d viewV{-antiV.direction.x, -antiV.direction.y, -antiV.direction.z};
    viewV.normalize();
    Eigen::Vector3d normalV{normal.direction.x, normal.direction.y, normal.direction.z};
    normalV.normalize();

    //add light from light sources
    for (auto& ls : scene.props.lights)
    {
        Ray lightSourseRay{ls.position, normal.origin - ls.position};
        //check that light source is visible
        ReflRes firstReflection = getNormalInNearestReflection(scene, lightSourseRay);
        if (firstReflection.normal.direction == Point3D{})
        {
            continue;
        }
        //check distance
        double dx = firstReflection.normal.origin.x - normal.origin.x;
        double dy = firstReflection.normal.origin.y - normal.origin.y;
        double dz = firstReflection.normal.origin.z - normal.origin.z;
        if (dx * dx + dy * dy + dz * dz > 0.0000000001) //there is could be error in computations
        {
            continue;
        }

        //light source is visible. Add intensity

        Eigen::Vector3d lightV{lightSourseRay.direction.x, lightSourseRay.direction.y, lightSourseRay.direction.z};
        double lsDist = lightV.dot(lightV);
        lightV.normalize();

        Eigen::Vector3d bisectV{viewV - lightV}; //H vector
        bisectV.normalize();

        double diffuseI = normalV.dot(-lightV);
        double reflectI = normalV.dot(bisectV);


        intensity.r += static_cast<double>(ls.r) / 255.0 / lsDist *
        (scene.optics[objIdx].kdr * diffuseI + scene.optics[objIdx].ksr * std::pow(
            reflectI, scene.optics[objIdx].power));

        intensity.g += static_cast<double>(ls.g) / 255.0 / lsDist *
        (scene.optics[objIdx].kdg * diffuseI + scene.optics[objIdx].ksg * std::pow(
            reflectI, scene.optics[objIdx].power));

        intensity.b  += static_cast<double>(ls.b) / 255.0 / lsDist *
        (scene.optics[objIdx].kdb * diffuseI + scene.optics[objIdx].ksb * std::pow(
            reflectI, scene.optics[objIdx].power));

    }

    if (!tracingRest)
        return intensity;

    //compute reflection from other objects

    Eigen::Vector3d projVN = normalV * normalV.dot(viewV) / normalV.dot(normalV);
    Eigen::Vector3d reflV = -viewV + 2 * projVN;
    Ray reflection{normal.origin, Point3D{reflV.x(), reflV.y(), reflV.z()}};

    ReflRes nextNormal = getNormalInNearestReflection(scene, reflection);
    if (nextNormal.normal.direction == Point3D{})
    {
        //add ambient reflection
        intensity.r += static_cast<double>(scene.props.ar) / 255.0 * scene.optics[objIdx].ksr;
        intensity.g += static_cast<double>(scene.props.ag) / 255.0 * scene.optics[objIdx].ksg;
        intensity.b += static_cast<double>(scene.props.ab) / 255.0 * scene.optics[objIdx].ksb;
        return intensity;
    }

    Intensity nextIntensity = computePointIntensity(scene, config,
                                                    nextNormal.objIdx, reflection,
                                                    nextNormal.normal, tracingRest - 1);

    Eigen::Vector3d nextAntiViewV{
        nextNormal.normal.origin.x - normal.origin.x,
        nextNormal.normal.origin.y - normal.origin.y,
        nextNormal.normal.origin.z - normal.origin.z
    };

    double reflDist = nextAntiViewV.norm();

    //add reflection from object
    intensity.r += nextIntensity.r / (reflDist * reflDist) * scene.optics[objIdx].ksr;
    intensity.g += nextIntensity.g / (reflDist * reflDist) * scene.optics[objIdx].ksg;
    intensity.b += nextIntensity.b / (reflDist * reflDist) * scene.optics[objIdx].ksb;
    return intensity;
}


/*draw 1 pixel*/
static void drawPixel(std::vector<std::vector<Intensity>>& result, SceneDescription& scene, ConfigState& config,
                      Camera& camera, int qual, int x,
                      int y, int iw, int ih)
{
    //find object or set background
    Ray viewRay = camera.emitRay(x, y, iw, ih);
    ReflRes firstReflection = getNormalInNearestReflection(scene, viewRay);
    if (firstReflection.normal.direction == Point3D{})
    {
        result[x][y].r = -1;
        result[x][y].g = -1;
        result[x][y].b = -1;
        return;
    }

    result[x][y] = computePointIntensity(scene, config, firstReflection.objIdx, viewRay, firstReflection.normal,
                                         config.depth - 1);
}

void FilledRenderStrategy::render(QImage& image, SceneDescription& scene, ConfigState& config, Camera& camera)
{
    int iw = image.width();
    int ih = image.height();
    std::vector<std::vector<Intensity>> result;
    result.resize(image.width());
    {
        std::lock_guard guard{rcMutex};
        readyCnt = 0;
    }
    double maxV = 0;
#pragma omp parallel for
    for (int x = 0; x < image.width(); x++)
    {
        result[x].resize(image.height());
    }

#pragma omp parallel for collapse(2)
    for (int x = 0; x < image.width(); x++)
    {
        for (int y = 0; y < image.height(); y++)
        {
            drawPixel(result, scene, config, camera, 0, x, y, iw, ih);

#pragma omp critical
            {
                {
                    std::lock_guard guard(rcMutex);
                    readyCnt++;
                }
                if (maxV < result[x][y].r)
                    maxV = result[x][y].r;

                if (maxV < result[x][y].g)
                    maxV = result[x][y].g;

                if (maxV < result[x][y].b)
                    maxV = result[x][y].b;
            }
        }
    }
#pragma omp parallel for
    for (int y = 0; y < ih; y++)
    {
        auto* line = image.scanLine(y);

        for (int x = 0; x < iw; x++)
        {
            if (result[x][y].r >= 0)
            {
                double blue = (result[x][y].b / maxV);
                blue = std::pow(blue,1.0 / config.gamma);
                line[4 * x + 0] = static_cast<uchar>(static_cast<int>(blue * 255) + 0.5);
                double green = (result[x][y].g / maxV);
                green = std::pow(green,1.0 /config.gamma);
                line[4 * x + 1] = static_cast<uchar>(static_cast<int>(green * 255) + 0.5);
                double red = (result[x][y].r / maxV);
                red = std::pow(red,1.0 /config.gamma);
                line[4 * x + 2] = static_cast<uchar>(static_cast<int>(red * 255) + 0.5);
            }
            else
            {
                line[4 * x + 0] = static_cast<uchar>(config.bb);
                line[4 * x + 1] = static_cast<uchar>(config.bg);
                line[4 * x + 2] = static_cast<uchar>(config.br);
            }
            line[4 * x + 3] = 0xFF;
        }
    }
    {
        std::lock_guard guard{rcMutex};
        readyCnt = 0;
    }
}

int FilledRenderStrategy::getReadyCnt()
{
    std::lock_guard guard{rcMutex};
    return readyCnt;
}
