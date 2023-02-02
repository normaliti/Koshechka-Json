#include "imgui.h"
#include "imgui-SFML.h" 

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iostream>
#include "interface/interface.hpp"
#include "interface/table.hpp"
#include "interface/utils.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "ImGui + SFML = <3");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color(100,100,100, 255));

    Grid g(10, 10, 25);

    sf::Clock deltaClock;

    sf::Sprite sprite;
    sf::Image img;
    
    img.create(400-15, 400-15);

    //g.generateHitMap(img);

    sf::Texture texture;
    
    texture.loadFromImage(img);
    sprite.setTexture(texture);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.mouseButton.button == sf::Mouse::Left && event.type == sf::Event::MouseButtonPressed)
            {
                sf::Vector2i position = {event.mouseButton.x, event.mouseButton.y};
                std::cout << position.x << " " << position.y << std::endl;
                if (g.isPointInGrid(position.x, position.y)) {
                    std::cout << "In Grid" << std::endl;
                    auto rect_idx_opt = g.getClickedRectangleIdxXY(position.x, position.y);
                    if (rect_idx_opt) {
                        g.switchAlphaState(rect_idx_opt->x, rect_idx_opt->y);
                    }
                }
            }

        }

        ImGui::SFML::Update(window, deltaClock.restart());

        drawInterface(shape, g);

        window.clear();
        //window.draw(shape);
        g.draw(window);
        //window.draw(sprite);

        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}