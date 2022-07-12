//
// Created by gerod on 18.06.2022.
//

#include "game.hpp"

Game::Game()
    = default;

Game::~Game()
{
    for (auto texture: _textures) {
        UnloadTexture(texture);
    }
}

void Game::init()
{
    Image image = LoadImage("../assets/playerShip1_red.png");     // Loaded in CPU memory (RAM)
    Texture2D texture = LoadTextureFromImage(image);          // Image converted to texture, GPU memory (VRAM)
    UnloadImage(image);   // Once image has been converted to texture and uploaded to VRAM, it can be unloaded from RAM
    _textures.push_back(texture);

    const auto entityId = _world.add_entity();
    _world.add_component<Component::PlayerController>(entityId);
    _world.add_component<Component::Transform>(entityId, Vector2 {512.0f, 512.0f }, 0.0f);
    _world.add_component<Component::Velocity>(entityId);
    _world.add_component<Component::AngularVelocity>(entityId, 10);
    _world.add_component<Component::Sprite>(entityId, WHITE, texture, Vector2 {20.0f, 20.0f });

    const auto entityId2 = _world.add_entity();
    _world.add_component<Component::PlayerController>(entityId2);
    _world.add_component<Component::Transform>(entityId2, Vector2 {256.0f, 512.0f + 256.0f }, 0.0f);
    _world.add_component<Component::Velocity>(entityId2);
    _world.add_component<Component::AngularVelocity>(entityId2, 10);
    _world.add_component<Component::Sprite>(entityId2, RED, texture, Vector2 {20.0f, 20.0f });

    const auto entityId3 = _world.add_entity();
    _world.add_component<Component::PlayerController>(entityId3);
    _world.add_component<Component::Transform>(entityId3, Vector2 {512.0f + 256.0f, 512.0f + 256.0f }, 0.0f);
    _world.add_component<Component::Velocity>(entityId3);
    _world.add_component<Component::AngularVelocity>(entityId3, 10);
    _world.add_component<Component::Sprite>(entityId3, GREEN, texture, Vector2 {20.0f, 20.0f });

    const auto entityId4 = _world.add_entity();
    _world.add_component<Component::PlayerController>(entityId4);
    _world.add_component<Component::Transform>(entityId4, Vector2 {0.0f, 256.0f }, 0.0f);
    _world.add_component<Component::Velocity>(entityId4);
    _world.add_component<Component::AngularVelocity>(entityId4, 10);
    _world.add_component<Component::Sprite>(entityId4, BLUE, texture, Vector2 {20.0f, 20.0f });

    _spawn_bullets = System::spawn_bullets { };
}

void Game::update()
{
    for (auto i : _world.each<Component::Transform, Component::Velocity, Component::PlayerController>()) {
        System::control(
                std::get<Component::Transform&>(i),
                std::get<Component::Velocity&>(i),
                std::get<Component::PlayerController&>(i)
        );
    }
    for (auto i : _world.each<Component::Transform, Component::Velocity>()) {
        System::move_transform(
                std::get<Component::Transform&>(i),
                std::get<Component::Velocity&>(i)
        );
    }
    for (auto i : _world.each<Component::Transform, Component::AngularVelocity>()) {
        System::rotate_transform(
                std::get<Component::Transform&>(i),
                std::get<Component::AngularVelocity&>(i)
        );
    }
    for (auto i : _world.each<Component::Transform, Component::PlayerController>()) {
        _spawn_bullets(
                std::get<Component::Transform&>(i),
                std::get<Component::PlayerController&>(i)
        );
    }
    for (auto i : _world.each<Component::Transform, Component::Sprite>()) {
        System::draw_visual(
                std::get<Component::Transform&>(i),
                std::get<Component::Sprite&>(i)
        );
    }
}
