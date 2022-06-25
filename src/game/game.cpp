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

    _playerController.emplace_back(Component::PlayerController{});
    _transforms.emplace_back(Component::Transform(Vector2 { 512.0f, 512.0f }, 0));
    _velocities.emplace_back(Component::Velocity());
//    _angularVelocities.emplace_back(Component::AngularVelocity(10));
    _sprites.emplace_back(Component::Sprite(WHITE, texture, Vector2 { 20.0f, 20.0f }));

    _spawn_bullets = System::spawn_bullets { };
}

void Game::update()
{
    System::control(_transforms[0], _velocities[0], _playerController[0]);
    System::move_transform(_transforms[0], _velocities[0]);
//    System::rotate_transform(_transforms[0], _angularVelocities[0]);
    _spawn_bullets(_transforms[0], _playerController[0]);
    System::draw_visual(_transforms[0], _sprites[0]);

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
