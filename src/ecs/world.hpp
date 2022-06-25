//
// Created by gerod on 25.06.2022.
//

#pragma once

#include <tuple>
#include <vector>
#include "entity.hpp"

// 1. add entities and components
// 1.1. immediate
// 1.2. delayed

// 2. iterate
// 2.1. through entities with 1,2,..,n non-empty components
// 2.2. through a pair of entities with...
// 2.2.1. ...identical components set
// 2.2.2. ...different components set
// 2.3. through entities with 1,2,..,n components, any component can be empty or not

// 3. remove entities and components
// 3.1. immediate
// 3.2. delayed

template<typename... Ts>
class World {
private:
    using tuple_of_vectors = std::tuple<std::vector<Ts>...>;
    tuple_of_vectors components;

    ///
    template <class T, class Tuple>
    struct Index;

    template <class T, class... Types>
    struct Index<T, std::tuple<T, Types...>> {
        static const std::size_t value = 0;
    };

    template <class T, class U, class... Types>
    struct Index<T, std::tuple<U, Types...>> {
        static const std::size_t value = 1 + Index<T, std::tuple<Types...>>::value;
    };
    ///

public:
    static constexpr size_t ComponentsCount = std::tuple_size_v<tuple_of_vectors>;
    std::vector<EntityComponents<ComponentsCount>> entities;

    template<typename T>
    constexpr static std::size_t get_index() {
        return Index<std::vector<T>, World::tuple_of_vectors>::value;
    }

    EntityId add_entity() {
        const EntityId id = entities.size();
        entities.emplace_back();
        return id;
    }

    template <typename C, typename ... Args>
    void add_component(EntityId id, Args && ... args)
    {
        // std::get<std::vector<C>>(components).emplace_back(std::forward<Args>(args) ...);
    }

    template<typename T>
    std::vector<T>& get_components() {
        return std::get<std::vector<T>>(components);
    }
};