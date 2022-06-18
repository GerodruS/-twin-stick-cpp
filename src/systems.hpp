//
// Created by gerod on 18.06.2022.
//

#pragma once

#include "raylib.h"
#include "raymath.h"

#include "components.hpp"
#include "utils.hpp"

namespace System {
    void control(
            Component::Transform &transform,
            Component::Velocity &velocity
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
        if (IsKeyDown(KEY_F) || IsKeyDown(KEY_RIGHT)) velocity_value = Vector2Add(velocity_value, Vector2{1.0f, 0.0f});

        // velocity.vector = Vector2Scale(velocity_value, game_settings.player_max_speed);
        velocity.vector = velocity_value;
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

