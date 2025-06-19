//
// Created by anton on 6/2/25.
//

#include "ConfigKeeper.h"

#include <fstream>
#include <sstream>

#include "Figure.h"

ConfigKeeper::ConfigKeeper()
{
    state = {168, 217, 255, 1, 3,0, Point3D{0, 0, 0}, Point3D{0, 10, 0}, Point3D{0, 0, 1}, 8, 100, 1, 1080.0 / 1920.0};
}

std::string ConfigKeeper::readConfig(const std::string& configFile)
{
    try
    {
        std::ifstream file(configFile);

        if (!file.is_open())
        {
            return "file not found";
        }

        int step = 0;

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
            for (char & p : prepared)
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
            case 0: // find br bg bb
                {
                    valuesStream >> (int &)state.br >> (int &)state.bg >> (int &)state.bb;
                    break;
                }
            case 1: // find gamma
                {
                    valuesStream >> state.gamma;
                    break;
                }
            case 2: // find depth
                {
                    valuesStream >> state.depth;
                    break;
                }
            case 3: // find quality
                {
                    valuesStream >> state.quality;
                    break;
                }
            case 4: // find eyeX eyeY eyeZ
                {
                    valuesStream >> state.eye.x >> state.eye.y >> state.eye.z;
                    break;
                }
            case 5: // find viewX viewY viewZ
                {
                    valuesStream >> state.view.x >> state.view.y >> state.view.z;
                    break;
                }
            case 6: // find upX upY upZ
                {
                    valuesStream >> state.up.x >> state.up.y>> state.up.z;
                    break;
                }
            case 7: // find zf zb
                {
                    valuesStream >> state.zf >> state.zb;
                    break;
                }
            case 8: // find sw sh
                {
                    valuesStream >> state.sw >> state.sh;
                    break;
                }
            default:
                {
                    return "internal error related to parser step";
                }
            }

            if (step == 8)
                break;

            ++step;
        }
        file.close();
    }
    catch (std::exception& e)
    {
        return e.what();
    }
    return "";
}

std::string ConfigKeeper::writeConfig(const std::string& configFile) const
{
    try
    {
        std::ofstream file(configFile);

        if (!file.is_open())
        {
            return "file couldn't be created";
        }

        file << state.br << " " << state.bg << " " << state.bb << " // Br Bg Bb - цвет фона в формате 0..255" << std::endl;
        file << state.gamma << " // гамма в формате 0.0 ... 1.0" << std::endl;
        file << state.depth << " // глубина прорисовки в формате 1..10" << std::endl;
        file << state.quality  << " // качество в формате 0..4" << std::endl;
        file << state.eye.x << " " << state.eye.y << " " << state.eye.z  << " // eyeX eyeY eyeZ - точка камеры в координатах пространства (double)" << std::endl;
        file << state.view.x << " " << state.view.y << " " << state.view.z   << " // viewX viewY viewZ - точка на которую смотрит камера в координатах пространства (double)" <<std::endl;
        file << state.up.x << " " << state.up.y << " " << state.up.z << " // upX upY upZ - вектор вверх в координатах пространства" << std::endl;
        file << state.br << " " << state.bg << " " << state.bb << " // Br Bg Bb - цвет фона в формате 0..255" << std::endl;
        file << state.zf << " " << state.zf << " // zf zb - ближняя и дальняя границы прорисовки в координатах камеры" << std::endl;
        file << state.sw << " " << state.sh << " //ширина и высота матрицы камеры (матрица находится на дальности zf от eye по направлению к view и повёрнута по вектору up)" << std::endl;
        file.close();
    }
    catch (std::exception& e)
    {
        return e.what();
    }
    return "";
}
