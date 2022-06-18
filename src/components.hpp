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

        Transform(Vector2 position, float rotation)
                : position(position)
                , rotation(rotation)
        {
        }
    };

    struct Velocity {
        Vector2 vector;

        Velocity()
                : Velocity(Vector2Zero())
        {
        }

        Velocity(Vector2 vector)
                : vector(vector)
        {
        }
    };

    struct AngularVelocity {
        float value;

        AngularVelocity()
                : AngularVelocity(0.0f)
        {
        }

        AngularVelocity(float value)
                : value(value)
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

        Sprite(Color color, Texture2D texture, Vector2 size)
                : color(color)
                , texture(texture)
                , size(size)
        {
        }
    };

    struct Collider {
        float radius;

        Collider()
                : Collider(0.0f)
        {
        }

        Collider(float radius)
                : radius(radius)
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
