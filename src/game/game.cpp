//
// Created by gerod on 18.06.2022.
//

#include "game.hpp"
#include <iostream>

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
    ///
    World<Component::PlayerController, Component::Transform> generic_world;
    auto v = World<Component::PlayerController, Component::Transform>::get_index<Component::Transform>();
    std::cout << " v=" << v << std::endl;
    generic_world.add_component<Component::Transform>(0, Vector2 { 512.0f, 512.0f }, 0);
    ///


    Image image = LoadImage("../assets/playerShip1_red.png");     // Loaded in CPU memory (RAM)
    Texture2D texture = LoadTextureFromImage(image);          // Image converted to texture, GPU memory (VRAM)
    UnloadImage(image);   // Once image has been converted to texture and uploaded to VRAM, it can be unloaded from RAM
    _textures.push_back(texture);

    _world.get_components<Component::PlayerController>().emplace_back(Component::PlayerController{});
    _world.get_components<Component::Transform>().emplace_back(Component::Transform(Vector2 { 512.0f, 512.0f }, 0));
    _world.get_components<Component::Velocity>().emplace_back(Component::Velocity());
//    _world._angularVelocities.emplace_back(Component::AngularVelocity(10));
    _world.get_components<Component::Sprite>().emplace_back(Component::Sprite(WHITE, texture, Vector2 { 20.0f, 20.0f }));

    _spawn_bullets = System::spawn_bullets { };
}

void Game::update()
{
    System::control(
            _world.get_components<Component::Transform>()[0],
            _world.get_components<Component::Velocity>()[0],
            _world.get_components<Component::PlayerController>()[0]
    );
    System::move_transform(
            _world.get_components<Component::Transform>()[0],
            _world.get_components<Component::Velocity>()[0]
    );
//    System::rotate_transform(_transforms[0], _angularVelocities[0]);
    _spawn_bullets(
            _world.get_components<Component::Transform>()[0],
            _world.get_components<Component::PlayerController>()[0]
    );
    System::draw_visual(
            _world.get_components<Component::Transform>()[0],
            _world.get_components<Component::Sprite>()[0]
    );

    /*
    .add_system(systems::control_rotation_system())
            .add_system(systems::move_transform_system())
            .add_system(systems::rotate_transform_system())
            .add_system(systems::spawn_bullets_system(0.0))
            .add_system(systems::spawn_asteroids_system(1.0))
            .add_system(systems::bullets_vs_asteroids_system())
            .add_system(systems::despawn_objects_system())
            .add_system(systems::increase_lifetime_system())
                    // render
            .add_system(systems::update_screen_size_system(0.0, Vec2::ZERO))
            .add_system(systems::clear_screen_system())
            .add_system(systems::draw_visual_system())
            .add_system(systems::draw_fps_system(
                    0,
    [0.0; systems::FPS_HISTORY_SIZE],
    ))
    */
}
