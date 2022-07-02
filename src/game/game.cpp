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

    // test
    for (auto i = _world.begin<Component::PlayerController>(); i != _world.end<Component::PlayerController>(); ++i){

    }
    for (auto i : _world.each<Component::PlayerController>()) {
        const auto& pc = i;
    }
    for (auto i : _world.each3<Component::PlayerController, Component::Transform, Component::Velocity>()) {
        const auto& pc = std::get<0>(i);
        const auto& tr = std::get<1>(i);
        const auto& ve = std::get<2>(i);
    }
    //
}

void Game::update()
{
//    {
//        std::size_t index_a = 0;
//        std::size_t index_b = 0;
//        std::size_t index_c = 0;
//        for (auto& entity : _world.entities) {
//            const bool has_a = entity.test(_world.get_index<Component::Transform>());
//            const bool has_b = entity.test(_world.get_index<Component::Velocity>());
//            const bool has_c = entity.test(_world.get_index<Component::PlayerController>());
//
//            if (has_a && has_b && has_c) {
//                System::control(
//                        _world.components<Component::Transform>()[index_a],
//                        _world.components<Component::Velocity>()[index_b],
//                        _world.components<Component::PlayerController>()[index_c]
//                );
//            }
//
//            index_c += static_cast<std::size_t>(has_c);
//            index_b += static_cast<std::size_t>(has_b);
//            index_a += static_cast<std::size_t>(has_a);
//        }
//    }

    System::control(
            _world.components<Component::Transform>()[0],
            _world.components<Component::Velocity>()[0],
            _world.components<Component::PlayerController>()[0]
    );
    System::move_transform(
            _world.components<Component::Transform>()[0],
            _world.components<Component::Velocity>()[0]
    );
//    System::rotate_transform(_transforms[0], _angularVelocities[0]);
    _spawn_bullets(
            _world.components<Component::Transform>()[0],
            _world.components<Component::PlayerController>()[0]
    );
    System::draw_visual(
            _world.components<Component::Transform>()[0],
            _world.components<Component::Sprite>()[0]
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
