#include "TextureManager.hpp"
#include <iostream>

TextureManager* TextureManager::instance = nullptr;

TextureManager* TextureManager::getInstance() {
    if (!instance) {
        instance = new TextureManager();
    }
    return instance;
}

bool TextureManager::loadTexture(const std::string& name, const std::string& filename) {
    sf::Texture texture;
    if (!texture.loadFromFile(filename)) {
        std::cerr << "Failed to load texture: " << filename << std::endl;
        return false;
    }
    
    textures[name] = texture;
    return true;
}

sf::Texture& TextureManager::getTexture(const std::string& name) {
    return textures.at(name);
}

void TextureManager::loadAllTextures() {
    // Базовые текстуры
    loadTexture("cell_hidden", "textures/cell_hidden.png");
    loadTexture("cell_revealed", "textures/cell_revealed.png");
    loadTexture("flag", "textures/flag.png");
    loadTexture("mine", "textures/mine.png");
    loadTexture("mine_exploded", "textures/mine_exploded.png");
    
    // Цифры
    for (int i = 1; i <= 8; ++i) {
        std::string name = "number_" + std::to_string(i);
        std::string filename = "textures/number_" + std::to_string(i) + ".png";
        loadTexture(name, filename);
    }
}
