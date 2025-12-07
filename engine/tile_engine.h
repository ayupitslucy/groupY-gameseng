#pragma once
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>



class LevelSystem {
public:
    enum Tile { EMPTY, START, END, WALL, ENEMY, WAYPOINT };

    static std::vector<sf::Vector2f> get_path();

    static void load_level(const std::string& filepath, float tile_size = 100.0f);  // Loads level from a given file path with optional tile size
    static void render();  // Renders the level to the screen

    static int get_width();  // Returns the tile width of the current level
    static int get_height();  // Returns the tile height of the current level
    static float get_tile_size();  // Returns the tile size of the current level

    static sf::Color get_color(Tile tile);  // Returns the colour of a tile type
    static void set_color(Tile tile, sf::Color color);  // Sets the color of a tile type

    static Tile get_tile_at_grid_coord(sf::Vector2i coord);  // Returns the tile type at a given grid coordinate
    static sf::Vector2f get_screen_coord_at_grid_coord(sf::Vector2i coord);  // Returns the screenspace coordinate of a given grid coordinate
    static sf::Vector2f get_screen_coord_center_at_grid_coord(sf::Vector2i coord);
    static Tile get_tile_at_screen_coord(sf::Vector2f coord);  // Returns the tile type at a given screenspace pos

    static sf::Vector2f get_start_position();  //  Returns the screenspace coordinate of starting tile
    static std::vector<sf::Vector2i> find_tiles(Tile tile);  // Returns all the grid coordinates of a given tile type

private:
    static std::unique_ptr<Tile[]> tiles;  // Internal array of tiles
    static float tile_size;
    static int width;
    static int height;
    static sf::Vector2f offset;  // Screenspace offset of level, when rendered.

    static std::map<Tile, sf::Color> colors;  // Colour to render each tile type
    static sf::Vector2f start_position;  // Starting position

    static std::vector<sf::RectangleShape> sprites;// Array of sfml sprites of each tile
    static void _build_sprites();  // Generate the sprites array

    LevelSystem() = delete;
    ~LevelSystem() = delete;
};