//
// Created by gerod on 25.06.2022.
//

#include "systems.hpp"
#include "raymath.h"
#include "utils.hpp"

namespace System {
    void control(
            Component::Transform &transform,
            Component::Velocity &velocity,
            const Component::PlayerController &_ // filter
    ) {
        const Vector2 mouse_position = GetMousePosition();
        // DrawCircleV(mouse_position, 40, RED);
        const Vector2 position = transform.position;
        const Vector2 vector = Vector2Normalize(Vector2Subtract(mouse_position, position));
        const float angle = 180.0f - Utils::Vector2Angle(vector, Vector2{0.0f, 1.0f}) * RAD2DEG;

        transform.rotation = angle;

        Vector2 velocity_value = Vector2Zero();
        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) velocity_value = Vector2Add(velocity_value, Vector2{0.0f, -1.0f});
        if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) velocity_value = Vector2Add(velocity_value, Vector2{0.0f, 1.0f});
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) velocity_value = Vector2Add(velocity_value, Vector2{-1.0f, 0.0f});
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) velocity_value = Vector2Add(velocity_value, Vector2{1.0f, 0.0f});

        // velocity.vector = Vector2Scale(velocity_value, game_settings.player_max_speed);
        velocity.vector = Vector2Scale(velocity_value, 3);
    }

    void move_transform(
            Component::Transform &transform,
            const Component::Velocity &velocity
            // delta_time
    ) {
        transform.position = Vector2Add(transform.position, velocity.vector);
        // transform.position += velocity.vector * delta_time.seconds;
    }

    void rotate_transform(
            Component::Transform &transform,
            const Component::AngularVelocity &angular_velocity
            // delta_time
    ) {
        transform.rotation += angular_velocity.value;
        // transform.rotation += angular_velocity.value * delta_time.seconds;
    }

    spawn_bullets::spawn_bullets()
            : spawn_bullets(0.0f)
    {
    }

    spawn_bullets::spawn_bullets(float delay)
            : delay(delay)
    {
    }

    void spawn_bullets::operator()(
            const Component::Transform &transform,
            const Component::PlayerController &_ // filter
            // delta_time
    ) {
        float delta_time = 1.0f / 60.0f;
        if (0.0f < delay) {
            delay -= delta_time;
        } else if (IsKeyDown(KEY_SPACE) || IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            Component::Transform bullet_transform { transform };
            Component::Velocity bullet_velocity { };
            Texture2D sprite_texture { };
            const float bullet_size = 10.0f;
            const float bullet_destroy_delay = 1.0f;

            /*
            CreateEntity()
                .Add(Component::Bullet { })
                .Add(bullet_transform)
                .Add(bullet_velocity)
                .Add(Component::Sprite { WHITE, sprite_texture, Vector2 { bullet_size, bullet_size } })
                .Add(Component::DestroyWhenOffScreen { bullet_size, bullet_destroy_delay })
                .Add(Component::Collider { bullet_size / 2.0f })
                ;
            */
        }
        /*
        if 0.0 < *delay {
                    *delay -= delta_time.seconds;
        } else if is_key_down(KeyCode::Space) {
            let rotation = Mat2::from_angle(transform.rotation.to_radians());
            let velocity = rotation * vec2(0.0, 1.0);

            let sound = macroquad::rand::gen_range(
                    0,
                    global_state.bullet_spawn_sounds.len(),
            );
            let sound = global_state.bullet_spawn_sounds[sound];
            play_sound_once(sound);

            cmd.push((
                             components::Bullet {},
                                     components::Transform {
                                             position: transform.position,
                                             rotation: transform.rotation,
                                     },
                                     components::Velocity {
                                             vector: velocity * 100.0,
                                     },
                                     components::Visual {
                                             color: game_settings.bullet_color,
                                             shape: components::Shape::Sprite {
                                                     texture: utils::TextureWrapper::new(0, game_settings.bullet_texture_rect),
                                                     size: Vec2::ONE * game_settings.bullet_size,
                                             },
                                     },
                                     components::DespawnWhenOffScreen {
                                             outer_bounds_radius: game_settings.bullet_size,
                                             despawn_delay: 0.0,
                                     },
                                     components::Collider {
                                             radius: game_settings.bullet_size / 2.0,
                                     },
                     ));
            *delay = game_settings.bullet_delay;
        }
        */
    }

    void draw_visual(
            const Component::Transform &transform,
            const Component::Sprite &sprite
    ) {
        const Vector2 position = transform.position;
        const float rotation = transform.rotation;

        const Vector2 texture_center = Vector2Scale(
                Vector2{static_cast<float>(sprite.texture.width), static_cast<float>(sprite.texture.height)},
                0.5f
        );

        DrawTexturePro(
                sprite.texture,
                Rectangle{0.0f, 0.0f,
                          static_cast<float>(sprite.texture.width), static_cast<float>(sprite.texture.height)},
                Rectangle{position.x, position.y,
                          static_cast<float>(sprite.texture.width), static_cast<float>(sprite.texture.height)},
                texture_center,
                rotation,
                sprite.color);
    }
}