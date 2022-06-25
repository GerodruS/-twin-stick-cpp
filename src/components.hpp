//
// Created by gerod on 18.06.2022.
//

#pragma once

#include "raylib.h"
#include "raymath.h"

namespace Component {
    struct Transform {
        Vector2 position;
        float rotation;

        Transform()
                : Transform(Vector2Zero(), 0.0f)
        {
        }

        Transform(Vector2 position_, float rotation_)
                : position(position_)
                , rotation(rotation_)
        {
        }
    };

    struct Velocity {
        Vector2 vector;

        Velocity()
                : Velocity(Vector2Zero())
        {
        }

        explicit Velocity(Vector2 vector_)
                : vector(vector_)
        {
        }
    };

    struct AngularVelocity {
        float value;

        AngularVelocity()
                : AngularVelocity(0.0f)
        {
        }

        explicit AngularVelocity(float value_)
                : value(value_)
        {
        }
    };

    struct PlayerController {
    };

    struct Sprite {
        Color color;
        Texture2D texture;
        Vector2 size;

        Sprite()
                = delete;

        Sprite(Color color_, Texture2D texture_, Vector2 size_)
                : color(color_)
                , texture(texture_)
                , size(size_)
        {
        }
    };

    struct Collider {
        float radius;

        Collider()
                : Collider(0.0f)
        {
        }

        explicit Collider(float radius_)
                : radius(radius_)
        {
        }
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
