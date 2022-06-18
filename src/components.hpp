//
// Created by gerod on 18.06.2022.
//

#pragma once

#include "raylib.h"

namespace Component
{
    struct Transform {
        Vector2 position;
        float rotation;
    };

    struct Velocity {
        Vector2 vector;
    };

    struct AngularVelocity {
        float value;
    };

    struct PlayerController {
    };

    struct Sprite {
        Color color;
        Texture2D texture;
        Vector2 size;
    };

    struct Collider {
        float radius;
    };

    struct DestroyWhenOffScreen {
        float outer_bounds_radius;
        float delay;
    };

    struct Bullet {
    };

    struct Asteroid {
    };
}
