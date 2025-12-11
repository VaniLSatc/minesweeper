#ifndef FIELD_HPP
#define FIELD_HPP

#include <vector>
#include <SFML/Graphics.hpp>
#include "Cell.hpp"
#include "Difficulty.hpp"

class Field {
private:
    std::vector<std::vector<Cell>> cells;
    int width;
    int height;
    int totalMines;
    int flagsPlaced;
    int cellsRevealed;
    bool firstClick;
    bool gameOver;
    bool gameWon;
    bool fieldGenerated;
    
    void generateField(int safeX, int safeY);
    void placeMines(int safeX, int safeY);
    void calculateAdjacentMines();
    void revealEmptyArea(int x, int y);
    
public:
    Field();
    
    void initialize(const GameSettings& settings);
    void handleLeftClick(int x, int y);
    void handleRightClick(int x, int y);
    void revealAllMines();
    
    bool isGameOver() const { return gameOver; }
    bool isGameWon() const { return gameWon; }
    bool isFirstClick() const { return firstClick; }
    int getFlagsPlaced() const { return flagsPlaced; }
    int getTotalMines() const { return totalMines; }
    int getCellsRevealed() const { return cellsRevealed; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    
    void draw(sf::RenderWindow& window, const sf::Texture& hiddenTexture, 
              const sf::Texture& revealedTexture, const sf::Texture& flagTexture,
              const sf::Texture& mineTexture, const sf::Texture& explodedTexture,
              const sf::Texture* numberTextures);
};

#endif
