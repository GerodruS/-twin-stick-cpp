//
// Created by gerod on 18.06.2022.
//

#pragma once

#include "raylib.h"
#include "components.hpp"

namespace System {
    void control(
            Component::Transform &transform,
            Component::Velocity &velocity,
            const Component::PlayerController &_ // filter
    );

    void move_transform(
            Component::Transform &transform,
            const Component::Velocity &velocity
            // delta_time
    );

    void rotate_transform(
            Component::Transform &transform,
            const Component::AngularVelocity &angular_velocity
            // delta_time
    );

    struct spawn_bullets {
        float delay;

        spawn_bullets();

        explicit spawn_bullets(float delay);

        void operator()(
                const Component::Transform &transform,
                const Component::PlayerController &_ // filter
                // delta_time
        );
    };

    void draw_visual(
            const Component::Transform &transform,
            const Component::Sprite &sprite
    );
}

