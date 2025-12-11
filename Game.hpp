#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "Field.hpp"
#include "Difficulty.hpp"
#include "TextureManager.hpp"
#include <memory>

// Константы размеров окна
const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 700;
const int INFO_PANEL_HEIGHT = 50;
const int CELL_SIZE = 32;

class Game {
private:
    sf::RenderWindow window;
    std::unique_ptr<Field> field;
    Difficulty difficulty;
    
    // Текстуры
    sf::Texture hiddenTexture;
    sf::Texture revealedTexture;
    sf::Texture flagTexture;
    sf::Texture mineTexture;
    sf::Texture explodedTexture;
    sf::Texture numberTextures[8];
    
    // Шрифт для текста
    sf::Font font;
    
    bool showMenu;
    DifficultyLevel selectedDifficulty;
    
    void loadTextures();
    void handleEvents();
    void update();
    void render();
    void renderMenu();
    void renderGame();
    void startGame();
    void resetGame();
    
public:
    Game();
    void run();
};

#endif
