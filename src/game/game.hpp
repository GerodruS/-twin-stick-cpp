//
// Created by gerod on 18.06.2022.
//

#pragma once

#include <vector>
#include "../components.hpp"

class Game {
public:
    Game();
    ~Game();

    void init();

    void update();

private:
    std::vector<Component::Transform> _transforms;
    std::vector<Component::Velocity> _velocities;
    std::vector<Component::Sprite> _sprites;

    std::vector<Texture2D> _textures;
};
