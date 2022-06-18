//
// Created by gerod on 18.06.2022.
//

#include "game.hpp"
#include "../systems.hpp"

Game::Game() {

}

Game::~Game() {
    for (auto texture: _textures) {
        UnloadTexture(texture);
    }
}

void Game::init() {
    Image image = LoadImage("../assets/playerShip1_red.png");     // Loaded in CPU memory (RAM)
    Texture2D texture = LoadTextureFromImage(image);          // Image converted to texture, GPU memory (VRAM)
    UnloadImage(image);   // Once image has been converted to texture and uploaded to VRAM, it can be unloaded from RAM
    _textures.push_back(texture);

    _transforms.emplace_back(Component::Transform(Vector2 { 512.0f, 512.0f }, 0));
    _velocities.emplace_back(Component::Velocity());
    _sprites.emplace_back(Component::Sprite(WHITE, texture, Vector2 { 20.0f, 20.0f }));
}

void Game::update() {
    System::control(_transforms[0], _velocities[0]);
    System::draw_visual(_transforms[0], _sprites[0]);
}
