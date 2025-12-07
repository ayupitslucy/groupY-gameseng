#include "tile_engine.h"
#include <iostream>
#include <fstream>
#include "renderer.h"
#include <algorithm>
#include <queue>

std::unique_ptr<LevelSystem::Tile[]> LevelSystem::tiles;
int LevelSystem::width;
int LevelSystem::height;
sf::Vector2f LevelSystem::offset(0.0f, 0.0f);  // FIXME?

float LevelSystem::tile_size(100.f);
std::vector<sf::RectangleShape> LevelSystem::sprites;

std::map<LevelSystem::Tile, sf::Color> LevelSystem::colors{ {WALL, sf::Color::White}, {END, sf::Color::Red} };
sf::Vector2f LevelSystem::start_position;

void LevelSystem::load_level(const std::string& filepath, float tsize) {
    tile_size = tsize;

    std::ifstream fh(filepath);
    if (!fh.is_open()) {
        throw std::runtime_error("Couldn't open level file: " + filepath);
    }

    std::vector<std::string> lines;
    std::string line;

    while (std::getline(fh, line)) {
        // Strip CR on Windows
        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        if (!line.empty())
            lines.push_back(line);
    }

    if (lines.empty())
        throw std::runtime_error("Level file is empty.");

    height = lines.size();
    width = lines[0].size();

    // Validate all rows same width
    for (auto& row : lines)
        if (row.size() != width)
            throw std::runtime_error("Level rows have inconsistent width in file: " + filepath);

    // Fill tiles
    std::vector<Tile> temp_tiles;
    temp_tiles.reserve(width * height);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {

            char tile = lines[y][x];
            Tile t = EMPTY;

            switch (tile) {
            case 'w': t = WALL; break;
            case '+': t = WAYPOINT; break;
            case 's': t = START; break;
            case 'e': t = END; break;
            case 'p': t = ENEMY; break;
            case ' ': t = EMPTY; break;
            default:
                std::cout << "Unknown tile '" << tile << "'\n";
            }

            if (t == START)
                start_position = get_screen_coord_at_grid_coord({ x, y });

            temp_tiles.push_back(t);
        }
    }

    tiles = std::make_unique<Tile[]>(width * height);
    std::copy(temp_tiles.begin(), temp_tiles.end(), &tiles[0]);

    std::cout << "Loaded " << filepath << " (" << width << "x" << height << ")\n";

    _build_sprites();
}

void LevelSystem::_build_sprites() {
    sprites.clear();
    sprites.reserve(width * height);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            sf::RectangleShape tile;

            tile.setSize({ tile_size, tile_size });
            tile.setPosition(get_screen_coord_at_grid_coord({ x, y }));
            tile.setFillColor(LevelSystem::get_color(LevelSystem::get_tile_at_grid_coord({ x, y })));

            sprites.push_back(tile);
        }
    }
}

std::vector<sf::Vector2f> LevelSystem::get_path() {
    //  Locate START and END
    auto starts = find_tiles(START);
    auto ends   = find_tiles(END);

    if (starts.size() != 1)
        throw std::runtime_error("Level must contain exactly ONE START tile.");
    if (ends.size() != 1)
        throw std::runtime_error("Level must contain exactly ONE END tile.");

    sf::Vector2i start = starts[0];
    sf::Vector2i goal  = ends[0];

    const int w = width;
    const int h = height;

    auto index = [w](sf::Vector2i p) {
        return p.y * w + p.x;
    };

    //  Breadth First search setup
    std::vector<int> prev(w * h, -1);   // store predecessor index
    std::queue<sf::Vector2i> q;

    auto inBounds = [w, h](sf::Vector2i p) {
        return p.x >= 0 && p.x < w && p.y >= 0 && p.y < h;
    };

    auto isWalkable = [](Tile t) {
        return t == EMPTY || t == START || t == END || t == WAYPOINT;
    };

    q.push(start);
    prev[index(start)] = index(start);   // mark as visited with self

    const sf::Vector2i dirs[4] = {
        { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }
    };

    bool found = false;

    // BFS loop
    while (!q.empty() && !found) {
        sf::Vector2i cur = q.front();
        q.pop();

        for (auto d : dirs) {
            sf::Vector2i nxt = cur + d;

            if (!inBounds(nxt))
                continue;

            if (!isWalkable(get_tile_at_grid_coord(nxt)))
                continue;

            int ni = index(nxt);
            if (prev[ni] != -1)
                continue;   // already visited

            prev[ni] = index(cur);

            if (nxt == goal) {
                found = true;
                break;
            }

            q.push(nxt);
        }
    }

    if (!found) {
        throw std::runtime_error("No path from START to END in level!");
    }

    // Reconstruct grid path from END to START
    std::vector<sf::Vector2i> gridPath;
    sf::Vector2i cur = goal;
    int curIdx = index(cur);

    while (true) {
        gridPath.push_back(cur);
        if (cur == start) break;

        curIdx = prev[curIdx];
        cur = { curIdx % w, curIdx / w };
    }

    std::reverse(gridPath.begin(), gridPath.end());

    // Convert grid coords - screen coords (centre of each tile)
    std::vector<sf::Vector2f> path;
    path.reserve(gridPath.size());
    for (auto& p : gridPath) {
        path.push_back(get_screen_coord_center_at_grid_coord(p));
    }

    return path;
}

void LevelSystem::render() {
    for (auto& sprite : sprites) {
        Renderer::queue(&sprite);   // SAFE! Sprite exists for whole frame.
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

sf::Vector2f LevelSystem::get_screen_coord_center_at_grid_coord(sf::Vector2i coord) {
    return get_screen_coord_at_grid_coord(coord) + sf::Vector2f(tile_size / 2.f, tile_size / 2.f);
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