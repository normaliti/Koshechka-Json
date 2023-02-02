#ifndef TABLE
# define TABLE

#include <SFML/Config.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <optional>
#include <iostream>
#include "../json.hpp"


class Grid {
private:
    unsigned int _width;
    unsigned int _height;
    unsigned int _size;
    sf::Color fill_color = {255,255,255};
    sf::Color border_color = {255,255,255};
    sf::Vector2f origin = {0,0};
    unsigned int padding = 1;
    std::vector<sf::RectangleShape> rectangles;
   
  
    public:
    Grid() {}
    Grid(unsigned int width, unsigned int height, unsigned int size): _width(width), _height(height), _size(size) {
        generateGrid();
    }


    void flushRectangles() {
        rectangles.clear();
    }
    unsigned int getWidth() const {
        return _width;
    }

    unsigned int getHeight() const {
        return _height;
    }

    unsigned int getSize() const {
        return _size;
    }

    void draw(sf::RenderWindow & window) { 
        for(auto & r : rectangles) {
            window.draw(r);
        }
    }

    sf::Vector2f getOrigin() const {
        return(this->origin);
    }

    std::vector<sf::RectangleShape> getRectangles() const {
        return rectangles;
    }

    void setOrigin(const sf::Vector2f & pos) {
        sf::Vector2f offset = pos - origin;        
        for (int i = 0; i < _width *_height; i++)
        {
            sf:sf::Vector2f current_pos = rectangles[i].getPosition();
            rectangles[i].setPosition(current_pos+offset);
        }
        origin = pos;        
    }

    sf::Color getColor() const {
        return(this->fill_color);
    }

    void setWidth(unsigned int width) {
        _width = width;
    }

    void setHeight(unsigned int height) {
        _height = height;
    }

    void setSize(unsigned int size) {
        _size = size;
    }

    void setFillColor(const sf::Color & color) {
        fill_color = color;
        for (int i = 0; i < _width * _height; i++)
        {
            if (rectangles[i].getFillColor().a == 0)
                continue;
            rectangles[i].setFillColor(color);
            rectangles[i].setOutlineColor(color);
        }
    }

    void setAlphaState(unsigned int x, unsigned int y, bool state) {
        int num = x * _width + y;
        sf::Color current_color = rectangles[num].getFillColor();
        current_color.a = (state) ? 255 : 0;
        rectangles[num].setFillColor(current_color);
    }

    bool isPointInGrid(int x, int y) {
        int x_min = origin.x;
        int y_min = origin.y;
        int x_max = x_min + _width * _size + (_width - 1) * padding;
        int y_max = y_min + _height * _size + (_height - 1) * padding;

        /* std::cout << "x = " << x << "y = " << y << std::endl;
        std::cout << "x_min = " << x_min << "y_min = " << y_min << std::endl;
        std::cout << "x_max = " << x_max << "y_max = " << y_max << std::endl; */

        bool x_in_grid = (x > x_min) && (x < x_max);
        bool y_in_grid = (y > y_min) && (y < y_max);
        std::cout << (x_in_grid && y_in_grid) << std::endl;
        if (x_in_grid && y_in_grid) 
            return (true);
        return (false);
    }

    std::optional<sf::Vector2i> getClickedRectangleIdxXY(int x, int y) {
        if (isPointInGrid(x,y) == false)
            return(std::nullopt);
        int new_x = x - origin.x;
        int new_y = y - origin.y;
        int num_x = new_x / (_size + padding);
        int num_y = new_y / (_size + padding);

        /*
        bool is_pad_x = (new_x > (_size * num_x)) && (new_x < ((_size + padding) * num_x));
        bool is_pad_y = (new_y > (_size * num_y)) && (new_y < ((_size + padding) * num_y));
        */

        int s = _size;
        int se = _size + padding;

        x = new_x - (se * (num_x));
        y = new_y - (se * (num_y));

        bool is_pad_x = (se > x) && (x > s);
        bool is_pad_y = (se > y) && (y > s);

        if (is_pad_x || is_pad_y)
           return(std::nullopt);
        
        sf::Vector2i res = {num_x, num_y};
        return(res); 
    }

    void switchAlphaState(int x, int y) {
        int num = x * _width + y;
        sf::Color current_color = rectangles[num].getFillColor();
        current_color.a = (current_color.a == 0) ? 255 : 0;
        rectangles[num].setFillColor(current_color);
    }

    void generateHitMap(sf::Image & renderTarget) {
        int w_pixel = (_size + padding) * _width - padding;
        int h_pixel = (_size + padding) * _height - padding;


        for (int x = 0; x < w_pixel; x++) {
            for (int y = 0; y < h_pixel; y++) {
                auto hit_result = getClickedRectangleIdxXY(x + origin.x, y + origin.y);
                sf::Color result_color = (hit_result.has_value()) ? sf::Color(255, 0, 0, 100) : sf::Color(0, 0, 255, 100);
                renderTarget.setPixel(x, y, result_color);
            }
        }
    }

    void generateGrid() {    
        rectangles.reserve(_width * _height * 2) ;
        for (int i = 0; i < _width; i++)
        {            
            for (int j = 0; j < _height; j++)
            {
                sf::RectangleShape shape;
                shape.setSize(sf::Vector2f(this->_size, this->_size));
                shape.setFillColor(this->fill_color);
                shape.setOutlineColor(this->border_color);

                auto x = origin.x + i*((this->_size)+(this->padding));
                auto y = origin.y + j*((this->_size)+(this->padding)); 

                shape.setPosition(x, y);
                rectangles.push_back(shape);
            }
        }
    }

};



using json = nlohmann::json;
inline  void to_json(nlohmann::json & j, const Grid & g) {
    auto origin = g.getOrigin();
    auto color = g.getColor();
    auto width = g.getWidth();
    auto height = g.getHeight();
    std::vector<int> alphaChannelsOffIdx;
    for (int i = 0; i < width * height; i++)
    {
        bool res = (g.getRectangles()[i].getFillColor().a == 255 ? 1 : 0);
        if(res == false)
        alphaChannelsOffIdx.push_back(i);
    }
        j = {{"width", width}, {"height", height}, {"size", g.getSize()}, {"origin", {origin.x, origin.y}}, 
        {"color", {color.r, color.g, color.b, color.a}}, {"alpha", alphaChannelsOffIdx}};
    }

    inline void from_json(const nlohmann::json& j, Grid &g) {
        sf::Vector2f origin;
        origin.x = j.at("origin")[0];
        origin.y = j.at("origin")[1];
        unsigned int width = j.at("width");
        unsigned int height = j.at("height");
        unsigned int size = j.at("size");
        sf::Color fill_color;
        fill_color.r = j.at("color")[0];
        fill_color.g = j.at("color")[1];
        fill_color.b = j.at("color")[2];
        fill_color.a = j.at("color")[3];
        std::vector<int> alphaChannelsOffIdx = j.at("alpha");

        g.setWidth(width);
        g.setHeight(height);
        g.setSize(size);
        g.setOrigin(origin);
        g.setFillColor(fill_color);
        std::cout << "width: " << width << ", height: " << height << ", size: " << size << ", origin: " << origin.x << "," << origin.y << ", color: " << fill_color.r << " " << fill_color.g << " " << fill_color.b << " " << fill_color.a << std::endl;
        g.generateGrid();
        for (auto idx : alphaChannelsOffIdx) {
            int y = idx / width;
            int x = idx - y * width;
            g.switchAlphaState(x,y);
        }
    }


#endif