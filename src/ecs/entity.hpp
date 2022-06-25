//
// Created by gerod on 25.06.2022.
//

#pragma once

#include <cstddef>
#include <bitset>

using EntityId = std::size_t;

template <size_t N>
using EntityComponents = std::bitset<N>;
