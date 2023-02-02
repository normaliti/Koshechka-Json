#include "imgui.h"
#include "imgui-SFML.h" 
#include <SFML/Graphics/CircleShape.hpp>
#include <iostream>
#include <ostream>
#include <strings.h>
#include "utils.hpp"
#include "table.hpp"
#include <fstream>


char buf[512];
void drawMyFirstWindow() {
    ImGui::Begin("Hello, Koshka-Gorbushka!");    
    ImGui::Button("Look at this pretty button");
    ImGui::End();
}

void drawCalculator() {
     ImGui::Begin("Magic calculator");
     static float numbers[2];
     static float result;
     ImGui::InputFloat2("Input two numbers", numbers);

     if (ImGui::Button("+")) {
        result = numbers[0]+numbers[1];
     }

     ImGui::SameLine();

     if (ImGui::Button("-")) {
        result = numbers[0]-numbers[1];
     }
     
     ImGui::SameLine();

     if (ImGui::Button("/")) {
        if (numbers[1] == 0)
            result = 9999999;
        else
            result = numbers[0]/numbers[1];
     }
     ImGui::SameLine();
     if (ImGui::Button("*")) {
        result = numbers[0]*numbers[1];
     }
    ImGui::LabelText("", "Result: %f", result);

     ImGui::End();
}

void drawCircleController(sf::CircleShape &circle)
{
    ImGui::Begin("Circle controller");
    float r = circle.getRadius();
    Vec3f color_normalized = Vec3f(circle.getFillColor());
    ImGui::ColorPicker3("color", color_normalized.getPtr());
    ImGui::SliderFloat("radius", &r, 50, 200);
    circle.setFillColor(color_normalized);
    circle.setRadius(r);
    ImGui::End();
}

void drawGridController(Grid &g) {
    int x = g.getOrigin().x;
    int y = g.getOrigin().y;
    Vec3f color;
    ImGui::Begin("Grid controller");
    ImGui::SliderInt("X", &x, 0, 500);
    ImGui::SliderInt("Y", &y, 0, 500);

    color = Vec3f(g.getColor());
    ImGui::ColorPicker3("color", color.getPtr());

    static int pos[2];
    ImGui::InputInt2("Input x and y of square:", pos);

    if (ImGui::Button("Change fill state")) {
        static bool state = true;
        state = !state;
        g.setAlphaState(pos[0], pos[1], state);
    }

    g.setOrigin(sf::Vector2f((float)x, (float)y));
    g.setFillColor(color);

    
    //bzero(buf, 512);
    std::string file_name(buf);
    ImGui::InputText("File name: ", buf, 512);
    if(ImGui::Button("Save")) {
        nlohmann::json j = g;
        
        std::ofstream file(file_name);
        file << j << std::endl;
    }
    ImGui::SameLine();
    if (ImGui::Button("Load")) {
        std::ifstream i(file_name);
        json j;
        i >> j;
        g.flushRectangles();
        from_json(j, g);
    }
    ImGui::End();
}



void drawInterface(sf::CircleShape &circle, Grid &g) {
    drawMyFirstWindow();
    drawCalculator();
    drawCircleController(circle);
    drawGridController(g);
}
