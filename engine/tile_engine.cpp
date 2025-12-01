#include "tile_engine.h"
#include <iostream>
#include <fstream>
#include "renderer.h"

std::unique_ptr<LevelSystem::Tile[]> LevelSystem::tiles;
int LevelSystem::width;
int LevelSystem::height;
sf::Vector2f LevelSystem::offset(0.0f, 0.0f);  // FIXME?

float LevelSystem::tile_size(100.f);
std::vector<std::unique_ptr<sf::RectangleShape>> LevelSystem::sprites;

std::map<LevelSystem::Tile, sf::Color> LevelSystem::colors{ {WALL, sf::Color::White}, {END, sf::Color::Red} };
sf::Vector2f LevelSystem::start_position;

void LevelSystem::load_level(const std::string& filepath, float tile_size) {
    LevelSystem::tile_size = tile_size;
    int width = 0, height = 0;
    std::string buffer;

    // Load in file to buffer
    std::ifstream fh(filepath);
    if (fh.good()) {
        buffer.assign(
            (std::istreambuf_iterator<char>(fh)),
            (std::istreambuf_iterator<char>())
        );
    } else {
        throw std::string("Couldn't open level file: ") + filepath;
    }
    int x = 0;

    std::vector<Tile> temp_tiles;
    for (int i = 0; i < buffer.size(); i++) {
        const char tile = buffer[i];
        switch (tile) {
        case 'w':
            temp_tiles.push_back(WALL);
            break;
        case 's':
            temp_tiles.push_back(START);
            LevelSystem::start_position = LevelSystem::get_screen_coord_at_grid_coord({ x, height });
            break;
        case 'e':
            temp_tiles.push_back(END);
            break;
        case ' ':
            temp_tiles.push_back(EMPTY);
            break;
        case '+':
            temp_tiles.push_back(WAYPOINT);
            break;
        case 'p':
            temp_tiles.push_back(ENEMY);
            break;
        case '\n':
            if (width == 0) {  // If we haven't written width yet
                width = i;  // Set width
            }
            x = -1;  // Gets increment to start at index 0 after switch
            height++;
            break;
        default:
            std::cout << "Don't know what tile type \"" << tile << "\" is" << std::endl;
        }
        x++;
    }

    std::cout << "temp tiles: " << temp_tiles.size()
        << " expected: " << (width * height)        // check the character count, if its unexpected throw error below
        << " width=" << width << " height=" << height << "\n";


    if (temp_tiles.size() != (width * height)) {
        throw std::string("Can't parse level file: ") + filepath;  // The file should end with a newline
    }
    LevelSystem::tiles = std::make_unique<Tile[]>(width * height);
    LevelSystem::width = width;  // Set static class vars
    LevelSystem::height = height;
    std::copy(temp_tiles.begin(), temp_tiles.end(), &LevelSystem::tiles[0]);
    std::cout << "Level " << filepath << " Loaded. " << width << "x" << height << std::endl;
    LevelSystem::_build_sprites();
}

void LevelSystem::_build_sprites() {
    LevelSystem::sprites.clear();
    for (int y = 0; y < LevelSystem::get_height(); y++) {
        for (int x = 0; x < LevelSystem::get_width(); x++) {
            std::unique_ptr<sf::RectangleShape> shape = std::make_unique<sf::RectangleShape>();
            shape->setPosition(get_screen_coord_at_grid_coord({ x, y }) + offset);
            shape->setSize(sf::Vector2f(LevelSystem::tile_size, LevelSystem::tile_size));
            shape->setFillColor(get_color(get_tile_at_grid_coord({ x, y })));
            LevelSystem::sprites.push_back(move(shape));
        }
    }
}

std::vector<sf::Vector2f> LevelSystem::get_path() {
    std::vector<sf::Vector2f> path;

    // START
    auto starts = find_tiles(START);
    if (starts.size() != 1)
        throw std::runtime_error("Level must contain exactly ONE START tile.");

    path.push_back(get_screen_coord_at_grid_coord(starts[0]));

    // WAYPOINTS
    auto wps = find_tiles(WAYPOINT);

    std::sort(wps.begin(), wps.end(), [](auto& a, auto& b) {
        if (a.y == b.y) return a.x < b.x;
    return a.y < b.y;
        });

    for (auto& wp : wps)
        path.push_back(get_screen_coord_at_grid_coord(wp));

    // END
    auto ends = find_tiles(END);
    if (ends.size() != 1)
        throw std::runtime_error("Level must contain exactly ONE END tile.");

    path.push_back(get_screen_coord_at_grid_coord(ends[0]));

    return path;
}

void LevelSystem::render() {
    for (int i = 0; i < LevelSystem::width * LevelSystem::height; i++) {
        Renderer::queue(LevelSystem::sprites[i].get());
    }
}

int LevelSystem::get_width() {
    return LevelSystem::width;
}

int LevelSystem::get_height() {
    return LevelSystem::height;
}

float LevelSystem::get_tile_size() {
    return LevelSystem::tile_size;
}

sf::Color LevelSystem::get_color(Tile tile) {
    auto entry = LevelSystem::colors.find(tile);
    if (entry == LevelSystem::colors.end()) {
        LevelSystem::colors[tile] = sf::Color::Transparent;
    }

    return LevelSystem::colors[tile];
}

void LevelSystem::set_color(Tile tile, sf::Color color) {
    LevelSystem::colors[tile] = color;
}

LevelSystem::Tile LevelSystem::get_tile_at_grid_coord(sf::Vector2i coord) {
    if (coord.x > LevelSystem::width || coord.y > LevelSystem::height) {
        throw std::string("Tile out of range: ") + std::to_string(coord.x) + "," + std::to_string(coord.y) + ")";
    }
    return LevelSystem::tiles[(coord.y * static_cast<size_t>(LevelSystem::width)) + coord.x];
}

sf::Vector2f LevelSystem::get_screen_coord_at_grid_coord(sf::Vector2i coord) {
    return sf::Vector2f(coord.x, coord.y) * tile_size + offset;
}

LevelSystem::Tile LevelSystem::get_tile_at_screen_coord(sf::Vector2f coord) {
    sf::Vector2f offset_coord = coord - LevelSystem::offset;
    if (offset_coord.x < 0 || offset_coord.y < 0) {
        throw std::string("Tile out of range ");
    }
    return get_tile_at_grid_coord(sf::Vector2i((offset_coord) / (LevelSystem::tile_size)));
}

sf::Vector2f LevelSystem::get_start_position() {
    return LevelSystem::start_position;
}

std::vector<sf::Vector2i> LevelSystem::find_tiles(Tile tile) {
    std::vector<sf::Vector2i> tile_positions;
    for (int x = 0; x < LevelSystem::width; x++) {
        for (int y = 0; y < LevelSystem::height; y++) {
            if (LevelSystem::get_tile_at_grid_coord(sf::Vector2i(x, y)) == tile) {
                tile_positions.push_back(sf::Vector2i(x, y));
            }
        }
    }
    return tile_positions;
}