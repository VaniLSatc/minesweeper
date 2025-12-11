#ifndef CELL_HPP
#define CELL_HPP

#include <SFML/Graphics.hpp>

enum class CellState {
    HIDDEN,
    REVEALED,
    FLAGGED,
    QUESTION_MARK
};

class Cell {
private:
    bool hasMine;
    bool hasExploded;
    int adjacentMines;
    CellState state;
    sf::Sprite sprite;
    sf::Vector2f position;
    
public:
    Cell();
    
    void setPosition(float x, float y);
    void setHasMine(bool mine);
    void setAdjacentMines(int count);
    void setState(CellState newState);
    void reveal();
    void toggleFlag();
    void explode();
    
    bool getHasMine() const { return hasMine; }
    int getAdjacentMines() const { return adjacentMines; }
    CellState getState() const { return state; }
    bool isRevealed() const { return state == CellState::REVEALED; }
    bool isFlagged() const { return state == CellState::FLAGGED; }
    bool isExploded() const { return hasExploded; }
    
    sf::FloatRect getBounds() const;
    void draw(sf::RenderWindow& window, const sf::Texture& hiddenTexture, 
              const sf::Texture& revealedTexture, const sf::Texture& flagTexture,
              const sf::Texture& mineTexture, const sf::Texture& explodedTexture,
              const sf::Texture* numberTextures);
};

#endif
