//
// Created by gerod on 18.06.2022.
//

#pragma once

#include <vector>
#include <tuple>
#include "../components.hpp"
#include "../systems.hpp"
#include "../ecs/entity.hpp"
#include "../ecs/world.hpp"


class Game {
private:
    using CustomWorld = World<
            Component::PlayerController,
            Component::Transform,
            Component::Velocity,
            Component::AngularVelocity,
            Component::Sprite,
            Component::Bullet,
            Component::DestroyWhenOffScreen,
            Component::Collider
            >;

public:
    Game();
    ~Game();

    void init();

    void update();

    // 1. add entities and components
    // 1.1. immediate
    EntityId AddEntityImmediately()
    {
        return _world.add_entity();
    }
//    void AddComponentImmediately(EntityId entityId, )
//    {
//        return _world.AddEntity();
//    }
    // 1.2. delayed
    EntityId AddEntityDelayed()
    {
        return _world_another.add_entity();
    }
    template <typename C, typename ... Args>
    void AddComponentDelayed(EntityId id, Args && ... args)
    {
        _world_another.add_component<C, Args ...>(id, std::forward<Args>(args) ...);
    }
    void ProcessDelayedOperations()
    {
        CopyAndClear(_world_another, _world);
    }
    static void CopyAndClear(CustomWorld& from_world, CustomWorld& to_world)
    {
        CopyAndClear(from_world.entities, to_world.entities);

        CopyAndClear<Component::PlayerController>(from_world, to_world);
        CopyAndClear<Component::Transform>(from_world, to_world);
        CopyAndClear<Component::Velocity>(from_world, to_world);
        CopyAndClear<Component::AngularVelocity>(from_world, to_world);
        CopyAndClear<Component::Sprite>(from_world, to_world);
    }

    template <typename T>
    static void CopyAndClear(std::vector<T>& from, std::vector<T>& to) {
        to.insert(std::end(to), std::begin(from), std::end(from));
        from.clear();
    }

    template <typename C>
    static void CopyAndClear(CustomWorld& from_world, CustomWorld& to_world) {
        auto& from_container = from_world.template components<C>();
        auto& to_container = to_world.template components<C>();
        to_container.insert(std::end(to_container), std::begin(from_container), std::end(from_container));
        from_container.clear();
    }

    Texture get_texture(size_t index) const;

private:
    CustomWorld _world;
    CustomWorld _world_another;

    System::spawn_bullets _spawn_bullets;
    std::vector<Texture2D> _textures;

    void add_texture(const char* const file_name);
};
