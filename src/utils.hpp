//
// Created by gerod on 18.06.2022.
//

#pragma once

#include "raylib.h"
#include "raymath.h"

namespace Utils {
    float Vector2Angle(const Vector2 v1, const Vector2 v2) {
        return atan2f(v2.y, v2.x) - atan2f(v1.y, v1.x);
    }
}
