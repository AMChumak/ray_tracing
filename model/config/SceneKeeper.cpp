//
// Created by anton on 6/3/25.
//

#include "SceneKeeper.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

#include "Box.h"
#include "Quadrangle.h"
#include "Sphere.h"
#include "Triangle.h"

SceneDescription SceneKeeper::getSceneDescription(std::string fileName)
{
    SceneDescription scene{};

    try
    {
        std::ifstream file(fileName);

        if (!file.is_open())
        {
            throw std::runtime_error("file not found");
        }

        int step = 0;

        int figureType = -1;
        int figureStep = 0;
        int currentLight = 0;

        //sphere parameters
        int maxSphereStep = 2;
        Point3D center{};
        double radius{};

        //box parameters
        int maxBoxStep = 2;
        Point3D minP{};
        Point3D maxP{};

        //triangle parameters
        int maxTriangleStep = 3;
        Point3D ta{};
        Point3D tb{};
        Point3D tc{};

        //quadrangle parameters
        int maxQuadStep = 5;
        Point3D qa{};
        Point3D qb{};
        Point3D qc{};
        Point3D qd{};

        // common optic props
        FigureOpticProps optics{};

        std::string figureTypeStr;
        while (!file.eof())
        {
            std::string raw;
            std::getline(file, raw);

            if (raw.empty())
                continue;

            auto prepared = raw.substr(0, raw.find("//"));

            if (prepared.empty())
                continue;

            bool isSpace = true;
            for (char& p : prepared)
            {
                if (!std::isspace(p))
                {
                    isSpace = false;
                    break;
                }
            }
            if (isSpace)
                continue;

            std::stringstream valuesStream(prepared);

            switch (step)
            {
            case 0: //read Ar Ag Ab
                {
                    valuesStream >> scene.props.ar >> scene.props.ag >> scene.props.ab;
                    step++;
                    break;
                }
            case 1: // read NL
                {
                    int nl;
                    valuesStream >> nl;
                    scene.props.lights.resize(nl);
                    step++;
                    break;
                }
            case 2: // read lights array
                {
                    valuesStream >> scene.props.lights[currentLight].position.x
                        >> scene.props.lights[currentLight].position.y
                        >> scene.props.lights[currentLight].position.z
                        >> scene.props.lights[currentLight].r
                        >> scene.props.lights[currentLight].g
                        >> scene.props.lights[currentLight].b;

                    if (currentLight == scene.props.lights.size() - 1)
                        step++;

                    currentLight++;
                    break;
                }
            case 3: //read figures
                {
                    if (figureType == -1)
                    {
                        figureStep = 0;
                        valuesStream >> figureTypeStr;
                        if (figureTypeStr == "SPHERE")
                            figureType = 1;
                        else if (figureTypeStr == "BOX")
                            figureType = 2;
                        else if (figureTypeStr == "TRIANGLE")
                            figureType = 3;
                        else if (figureTypeStr == "QUADRANGLE")
                            figureType = 4;
                        else
                            throw std::runtime_error("Invalid figure type");
                    }


                    switch (figureType)
                    {
                    case 1: //sphere
                        {
                            if (figureStep == 0)
                            {
                                valuesStream >> center.x >> center.y >> center.z;
                                ++figureStep;
                            }
                            else if (figureStep == 1)
                            {
                                valuesStream >> radius;
                                ++figureStep;
                            }
                            else
                            {
                                valuesStream >> optics.kdr >> optics.kdg >> optics.kdb >> optics.ksr >> optics.ksg >>
                                    optics.ksb >> optics.power;

                                // add figure
                                scene.figures.push_back(new Sphere(center, radius));
                                scene.optics.push_back(optics);
                                step++;
                                figureType = -1;
                            }
                            break;
                        }
                    case 2: //box
                        {
                            if (figureStep == 0)
                            {
                                valuesStream >> minP.x >> minP.y >> minP.z;
                                ++figureStep;
                            }
                            else if (figureStep == 1)
                            {
                                valuesStream >> maxP.x >> maxP.y >> maxP.z;
                                ++figureStep;
                            }
                            else
                            {
                                valuesStream >> optics.kdr >> optics.kdg >> optics.kdb >> optics.ksr >> optics.ksg >>
                                    optics.ksb >> optics.power;

                                // add figure
                                scene.figures.push_back(new Box(minP, maxP));
                                scene.optics.push_back(optics);
                                step++;
                                figureType = -1;
                            }
                            break;
                        }
                    case 3: //triangle
                        {
                            if (figureStep == 0)
                            {
                                valuesStream >> ta.x >>ta.y >>ta.z;
                                ++figureStep;
                            }
                            else if (figureStep == 1)
                            {
                                valuesStream >> tb.x >> tb.y >> tb.z;
                                ++figureStep;
                            }
                            else if (figureStep == 2)
                            {
                                valuesStream >> tc.x >> tc.y >> tc.z;
                                ++figureStep;
                            }
                            else
                            {
                                valuesStream >> optics.kdr >> optics.kdg >> optics.kdb >> optics.ksr >> optics.ksg >>
                                    optics.ksb >> optics.power;

                                // add figure
                                scene.figures.push_back(new Triangle(Polygon{ta,tb,tc}));
                                scene.optics.push_back(optics);
                                step++;
                                figureType = -1;
                            }
                            break;
                        }
                    case 4: //triangle
                        {
                            if (figureStep == 0)
                            {
                                valuesStream >> qa.x >>qa.y >>qa.z;
                                ++figureStep;
                            }
                            else if (figureStep == 1)
                            {
                                valuesStream >> qb.x >> qb.y >> qb.z;
                                ++figureStep;
                            }
                            else if (figureStep == 2)
                            {
                                valuesStream >> qc.x >> qc.y >> qc.z;
                                ++figureStep;
                            }
                            else if (figureStep == 3)
                            {
                                valuesStream >> qd.x >> qd.y >> qd.z;
                                ++figureStep;
                            }
                            else
                            {
                                valuesStream >> optics.kdr >> optics.kdg >> optics.kdb >> optics.ksr >> optics.ksg >>
                                    optics.ksb >> optics.power;

                                // add figure
                                scene.figures.push_back(new Quadrangle(qa,qb,qc,qd));
                                scene.optics.push_back(optics);
                                step++;
                                figureType = -1;
                            }
                            break;
                        }
                    }
                    break;
                }
            }
        }
        return scene;
    }
    catch (std::exception& e)
    {
        throw;
    }
}
