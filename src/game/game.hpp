//
// Created by gerod on 18.06.2022.
//

#pragma once

#include <vector>
#include "../components.hpp"
#include "../systems.hpp"


class Game {
public:
    Game();
    ~Game();

    void init();

    void update();

private:
    std::vector<Component::PlayerController> _playerController;
    std::vector<Component::Transform> _transforms;
    std::vector<Component::Velocity> _velocities;
    std::vector<Component::AngularVelocity> _angularVelocities;
    std::vector<Component::Sprite> _sprites;

    System::spawn_bullets _spawn_bullets;

    std::vector<Texture2D> _textures;
};
