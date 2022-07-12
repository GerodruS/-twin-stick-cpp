//
// Created by gerod on 25.06.2022.
//

#pragma once

#include <limits>
#include <tuple>
#include <vector>
#include <array>
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
    tuple_of_vectors _components;

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
    using ComponentMask = EntityComponents<ComponentsCount>;
    std::vector<ComponentMask> entities;

    template<typename T>
    constexpr static std::size_t get_index() {
        return Index<std::vector<T>, World::tuple_of_vectors>::value;
    }

    EntityId add_entity() {
        const EntityId id = entities.size();
        entities.emplace_back();
        entities[id].reset();
        return id;
    }

    template <typename C, typename ... Args>
    void add_component(EntityId id, Args && ... args)
    {
        const auto i = new_component_index<C>(id);
        components<C>().emplace(components<C>().begin() + i, std::forward<Args>(args) ...);
        entities[id].set(get_index<C>());
    }

    template<typename T>
    std::vector<T>& components() {
        return std::get<std::vector<T>>(_components);
    }

    template <typename C1, typename C2, typename ... Components>
    ComponentMask get_mask() {
        return get_mask<C1>() | get_mask<C2, Components ...>();
    }

    template <typename C>
    ComponentMask get_mask() {
        ComponentMask mask;
        mask.set(get_index<C>());
        return mask;
    }

    template <typename C>
    bool has_component(EntityId entityId) {
        return entities[entityId][get_index<C>()];
    }

    template <typename C>
    C* try_get_component(EntityId entityId) {
        const auto index = component_index<C>(entityId);
        return index < components<C>().size()
            ? &(components<C>()[index])
            : nullptr;
    }

    template <typename C>
    auto component_index(EntityId entityId) {
        if (!has_component<C>(entityId)) {
            return std::numeric_limits<size_t>::max();
        }
        std::size_t index = 0;
        for (auto i = std::begin(entities), end = std::begin(entities) + entityId; i != end; ++i) {
            index += static_cast<std::size_t>(*i[get_index<C>()]);
        }
        return index;
    }

    template <typename C>
    auto new_component_index(EntityId entityId) {
        if (has_component<C>(entityId)) {
            return std::numeric_limits<size_t>::max();
        }
        std::size_t index = 0;
        for (auto i = std::begin(entities), end = std::begin(entities) + entityId; i != end; ++i) {
            index += static_cast<std::size_t>((*i)[get_index<C>()]);
        }
        return index;
    }

    template <typename... Cs>
    class Iterator {
    public:
        using self_type = Iterator;
        using iterator_category = std::forward_iterator_tag;
        using difference_type = int;
        using value_type = std::tuple<Cs ...>;
        using reference = std::tuple<Cs& ...>;
        using pointer = std::tuple<Cs* ...>;
        using components_iterators = std::tuple<typename std::vector<Cs>::iterator ...>;

        static self_type create_begin(World& world) {
            constexpr auto count = sizeof...(Cs);

            auto components_i = std::make_tuple(world.components<Cs>().begin() ...);

            for (auto entity_i = world.entities.begin(); entity_i != world.entities.end(); ++entity_i) {
                std::array<bool, count> contains {(*entity_i)[get_index<Cs>()] ...};

                if (std::all_of(contains.cbegin(),
                                contains.cend(),
                                [](auto elem){ return elem; })) {
                    return Iterator(
                            world,
                            entity_i,
                            components_i);
                }

                components_i = std::make_tuple(std::get<typename std::vector<Cs>::iterator>(components_i) + static_cast<int>(contains[Index<Cs, value_type>::value]) ...);
            }
            return create_end(world);
        }

        static self_type create_end(World& world) {
            return Iterator(
                    world,
                    world.entities.end(),
                    components_iterators(world.components<Cs>().end() ...)
            );
        }

    public:
        Iterator(
                World& world,
                typename std::vector<ComponentMask>::iterator entity_i,
                components_iterators component_i
        )
                : _world(world)
                , _entity_i(entity_i)
                , _component_i(component_i)
        {
        }

        self_type& operator++() {
            step_forward();
            return *this;
        }

        self_type operator++(int n) {
            step_forward(n);
            return *this;
        }

        reference operator*() { return reference(*(std::get<typename std::vector<Cs>::iterator>(_component_i)) ...); }
        pointer operator->() { return pointer(&std::get<typename std::vector<Cs>::iterator>(_component_i).operator->() ...); }
        bool operator==(const self_type &rhs) const noexcept { return  equal(rhs); }
        bool operator!=(const self_type &rhs) const noexcept { return !equal(rhs); }

    private:
        World &_world;
        typename std::vector<ComponentMask>::iterator _entity_i;
        components_iterators _component_i;

        void step_forward() {
            if (_entity_i == _world.entities.end()) return;

            constexpr auto count = sizeof...(Cs);
            for (++_entity_i; _entity_i != _world.entities.end(); ++_entity_i) {
                std::array<bool, count> contains { (*_entity_i)[get_index<Cs>()] ... };

                _component_i = std::make_tuple(std::get<typename std::vector<Cs>::iterator>(_component_i) + static_cast<int>(contains[Index<Cs, value_type>::value]) ...);

                if (std::all_of(contains.cbegin(),
                                contains.cend(),
                                [](auto elem){ return elem; })) {
                    return;
                }

            }

            // _entity_i = _world.entities.end();
            _component_i = std::make_tuple(_world.components<Cs>().end() ...);
        }

        void step_forward(int n) {
            for (; 0 < n; --n) {
                step_forward();
            }
        }

        bool equal(const self_type &rhs) const noexcept {
            return &_world == &(rhs._world)
                   && _entity_i == rhs._entity_i
                   && _component_i == rhs._component_i
                   ;
        }
    };

    template <typename... Cs>
    class IteratorFactory {
    public:
        explicit IteratorFactory(World& world)
                : _world(world)
        {
        }

        Iterator<Cs ...> begin() {
            return Iterator<Cs ...>::create_begin(_world);
        }

        Iterator<Cs ...> end() {
            return Iterator<Cs ...>::create_end(_world);
        }

    private:
        World &_world;
    };

    template<typename... Cs>
    auto each() {
        return IteratorFactory<Cs ...>(*this);
    }
};
