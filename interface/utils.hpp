#ifndef UTILS
#define UTILS

#include <SFML/Config.hpp>
#include <SFML/Graphics/Color.hpp>
struct Vec3f {
    Vec3f() {
        data[0] = 0.0f;
        data[1] = 0.0f;
        data[2] = 0.0f;
    }
    Vec3f(const sf::Color & color) {
        data[0] = static_cast<float>(color.r) / 255.f;
        data[1] = static_cast<float>(color.g) / 255.f;
        data[2] = static_cast<float>(color.b) / 255.f;
    }

    float * getPtr() {return data;}

    operator sf::Color () {
        sf::Color result(
            static_cast<sf::Uint8>(data[0] * 255.0), 
            static_cast<sf::Uint8>(data[1] * 255.0),
            static_cast<sf::Uint8>(data[2]* 255.0),
            255);
            return result;
    }

    float data[3];
};

#endif
