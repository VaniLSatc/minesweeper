#include "Cell.hpp"
#include <iostream>

Cell::Cell() : hasMine(false), hasExploded(false), adjacentMines(0), 
                state(CellState::HIDDEN) {
    sprite.setScale(0.5f, 0.5f); // Масштабируем текстуры
}

void Cell::setPosition(float x, float y) {
    position = sf::Vector2f(x, y);
    sprite.setPosition(position);
}

void Cell::setHasMine(bool mine) {
    hasMine = mine;
}

void Cell::setAdjacentMines(int count) {
    adjacentMines = count;
}

void Cell::setState(CellState newState) {
    state = newState;
}

void Cell::reveal() {
    if (state != CellState::FLAGGED) {
        state = CellState::REVEALED;
    }
}

void Cell::toggleFlag() {
    if (state == CellState::HIDDEN) {
        state = CellState::FLAGGED;
    } else if (state == CellState::FLAGGED) {
        state = CellState::HIDDEN;
    }
}

void Cell::explode() {
    hasExploded = true;
    state = CellState::REVEALED;
}

sf::FloatRect Cell::getBounds() const {
    return sprite.getGlobalBounds();
}

void Cell::draw(sf::RenderWindow& window, const sf::Texture& hiddenTexture, 
                const sf::Texture& revealedTexture, const sf::Texture& flagTexture,
                const sf::Texture& mineTexture, const sf::Texture& explodedTexture,
                const sf::Texture* numberTextures) {
    
    switch (state) {
        case CellState::HIDDEN:
        case CellState::QUESTION_MARK:
            sprite.setTexture(hiddenTexture);
            break;
            
        case CellState::FLAGGED:
            sprite.setTexture(hiddenTexture);
            window.draw(sprite);
            sprite.setTexture(flagTexture);
            break;
            
        case CellState::REVEALED:
            if (hasExploded) {
                sprite.setTexture(explodedTexture);
            } else if (hasMine) {
                sprite.setTexture(mineTexture);
            } else {
                sprite.setTexture(revealedTexture);
                if (adjacentMines > 0 && numberTextures != nullptr) {
                    window.draw(sprite);
                    sprite.setTexture(numberTextures[adjacentMines - 1]);
                }
            }
            break;
    }
    
    window.draw(sprite);
}
