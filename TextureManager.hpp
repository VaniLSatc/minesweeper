#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class TextureManager {
private:
    static TextureManager* instance;
    std::map<std::string, sf::Texture> textures;
    
    TextureManager() {} // Приватный конструктор
    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;
    
public:
    static TextureManager* getInstance();
    
    bool loadTexture(const std::string& name, const std::string& filename);
    sf::Texture& getTexture(const std::string& name);
    
    void loadAllTextures();
};

#endif
