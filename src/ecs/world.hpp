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

    /*
    template <typename ... Components>
    struct Iterator {
    public:
        Iterator(World& world_)
            : _world(world_)
            , _indices()
        {
            std::fill(std::begin(_indices), std::end(_indices), 0);
        }

        bool CanStep() {
            bool can_step = true;
            for (auto i : _indices) {
                can_step = can_step && i < _world.template components<>();
            }
            return can_step;
        }

        void Process() {

        }

        void PostProcess() {

        }

    private:
        World& _world;
        std::array<std::size_t, sizeof...(Components)> _indices;

    };
    */

//    template <typename ... Components>
//    void for_each() {
//        std::array<std::size_t, sizeof...(Components)> indices;
//        std::fill(std::begin(indices), std::end(indices), 0);
//        for_each<sizeof...(Components), Components ...>();
//    }
//
//    template <size_t Depth, typename ... Components>
//    void for_each() {
////        std::array<std::size_t, sizeof...(Components)> indices;
////        std::fill(std::begin(indices), std::end(indices), 0);
////        const std::size_t depth = sizeof...(Components);
////        for_each<Components>();
//    }
//
//    template <typename ... Components>
//    void for_each<0>() {
////        ComponentMask mask;
////        mask.set(get_index<C>());
////        return mask;
//    }

//    template<typename C>
//    struct EntityWithComponent {
//        struct Iterator
//        {
//            using iterator_category = std::bidirectional_iterator_tag;
//            using difference_type   = std::ptrdiff_t;
//            using value_type        = C;
//            using pointer           = C*;  // or also value_type*
//            using reference         = C&;  // or also value_type&
//        };
//
//    private:
//        World& _world;
//    };

    template <typename C>
    class iterator {
    public:
        using self_type = iterator;
        using iterator_category = std::forward_iterator_tag;
        using difference_type = int;
        using value_type = C;
        using reference = value_type &;
        using pointer = value_type *;

        static self_type create_begin(World& world) {
            auto entities_i = std::find_if(
                    world.entities.begin(),
                    world.entities.end(),
                    [](ComponentMask& elem) { return elem[get_index<C>()]; });
            return self_type(
                    world,
                    entities_i,
                    world.components<C>().begin()
            );
        }

        static self_type create_end(World& world) {
            return self_type(
                world,
                world.entities.end(),
                world.components<C>().end()
            );
        }

    public:
        iterator(
            World& world,
            typename std::vector<ComponentMask>::iterator entity_i,
            typename std::vector<C>::iterator component_i
            )
            : _world(world)
            , _entity_i(entity_i)
            , _component_i(component_i)
        {
        }

        self_type operator++() {
            step_forward();
            return *this;
        }

        self_type operator++(int n) {
            step_forward(n);
            return *this;
        }

        reference operator*() { return *_component_i; }
        pointer operator->() { return _component_i.operator->(); }
        bool operator==(const self_type &rhs) const noexcept { return  equal(rhs); }
        bool operator!=(const self_type &rhs) const noexcept { return !equal(rhs); }

    private:
        World &_world;
        typename std::vector<ComponentMask>::iterator _entity_i;
        typename std::vector<C>::iterator _component_i;

        void step_forward() {
            if (_entity_i == _world.entities.end()) return;

            _entity_i = std::find_if(
                    _entity_i + 1,
                    _world.entities.end(),
                    [](ComponentMask& elem) { return elem[get_index<C>()]; });
            ++_component_i;
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

    template <typename C>
    iterator<C> begin() {
        return iterator<C>::create_begin(*this);
    }

    template <typename C>
    iterator<C> end() {
        return iterator<C>::create_end(*this);
    }

    template <typename C>
    class iterator_factory {
    public:
        iterator_factory(World& world)
            : _world(world)
        {
        }

        iterator<C> begin() {
            return iterator<C>::create_begin(_world);
        }

        iterator<C> end() {
            return iterator<C>::create_end(_world);
        }

    private:
        World &_world;
    };

    template <typename C>
    iterator_factory<C> each() {
        return iterator_factory<C>(*this);
    }

    template <typename C0, typename C1, typename C2>
    class iterator3 {
    public:
        using self_type = iterator3;
        using iterator_category = std::forward_iterator_tag;
        using difference_type = int;
        using value_type = std::tuple<C0, C1, C2>;
        using reference = std::tuple<C0&, C1&, C2&>;
        using pointer = std::tuple<C0*, C1*, C2*>;
        using components_iterators = std::tuple<
                typename std::vector<C0>::iterator,
                typename std::vector<C1>::iterator,
                typename std::vector<C2>::iterator
            >;

        static self_type create_begin(World& world) {
            constexpr auto count = 3;

            auto components_i = std::make_tuple(
                world.components<C0>().begin(),
                world.components<C1>().begin(),
                world.components<C2>().begin()
            );

            for (auto entity_i = world.entities.begin(); entity_i != world.entities.end(); ++entity_i) {
                std::array<bool, count> contains {
                        (*entity_i)[get_index<C0>()],
                        (*entity_i)[get_index<C1>()],
                        (*entity_i)[get_index<C2>()],
                };

                if (std::all_of(contains.cbegin(),
                                contains.cend(),
                                [](auto elem){ return elem; })) {
                    return iterator3(
                            world,
                            entity_i,
                            components_i);
                }

                components_i = std::make_tuple(
                        std::get<0>(components_i) + static_cast<int>(contains[0]),
                        std::get<1>(components_i) + static_cast<int>(contains[1]),
                        std::get<2>(components_i) + static_cast<int>(contains[2])
                );
            }
            return create_end(world);
        }

        static self_type create_end(World& world) {
            return iterator3(
                    world,
                    world.entities.end(),
                    components_iterators(
                            world.components<C0>().end(),
                            world.components<C1>().end(),
                            world.components<C2>().end()
                            )
            );
        }

    public:
        iterator3(
                World& world,
                typename std::vector<ComponentMask>::iterator entity_i,
                components_iterators component_i
        )
                : _world(world)
                , _entity_i(entity_i)
                , _component_i(component_i)
        {
        }

        self_type operator++() {
            step_forward();
            return *this;
        }

        self_type operator++(int n) {
            step_forward(n);
            return *this;
        }

        reference operator*() { return reference(
                *(std::get<0>(_component_i)),
                *(std::get<1>(_component_i)),
                *(std::get<2>(_component_i))); }
        pointer operator->() { return pointer(
                    &std::get<0>(_component_i).operator->(),
                    &std::get<1>(_component_i).operator->(),
                    &std::get<2>(_component_i).operator->()); }
        bool operator==(const self_type &rhs) const noexcept { return  equal(rhs); }
        bool operator!=(const self_type &rhs) const noexcept { return !equal(rhs); }

    private:
        World &_world;
        typename std::vector<ComponentMask>::iterator _entity_i;
        components_iterators _component_i;

        void step_forward() {
            if (_entity_i == _world.entities.end()) return;

            constexpr auto count = 3;
            for (++_entity_i; _entity_i != _world.entities.end(); ++_entity_i) {
                std::array<bool, count> contains {
                        (*_entity_i)[get_index<C0>()],
                        (*_entity_i)[get_index<C1>()],
                        (*_entity_i)[get_index<C2>()],
                };

                _component_i = std::make_tuple(
                        std::get<0>(_component_i) + static_cast<int>(contains[0]),
                        std::get<1>(_component_i) + static_cast<int>(contains[1]),
                        std::get<2>(_component_i) + static_cast<int>(contains[2])
                );

                if (std::all_of(contains.cbegin(),
                                contains.cend(),
                                [](auto elem){ return elem; })) {
                    return;
                }

            }

            // _entity_i = _world.entities.end();
            _component_i = std::make_tuple(
                    _world.components<C0>().end(),
                    _world.components<C1>().end(),
                    _world.components<C2>().end()
            );
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

    template <typename C0, typename C1, typename C2>
    class iterator3_factory {
    public:
        iterator3_factory(World& world)
                : _world(world)
        {
        }

        iterator3<C0, C1, C2> begin() {
            return iterator3<C0, C1, C2>::create_begin(_world);
        }

        iterator3<C0, C1, C2> end() {
            return iterator3<C0, C1, C2>::create_end(_world);
        }

    private:
        World &_world;
    };

    template <typename C0, typename C1, typename C2>
    auto each3() {
        return iterator3_factory<C0, C1, C2>(*this);
    }
};
