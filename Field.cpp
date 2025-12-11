#include "Field.hpp"
#include <iostream>
#include <random>
#include <queue>

Field::Field() : width(0), height(0), totalMines(0), flagsPlaced(0), 
                 cellsRevealed(0), firstClick(true), gameOver(false), 
                 gameWon(false), fieldGenerated(false) {}

void Field::initialize(const GameSettings& settings) {
    width = settings.width;
    height = settings.height;
    totalMines = settings.mines;
    
    cells.clear();
    cells.resize(height, std::vector<Cell>(width));
    
    // Устанавливаем позиции клеток
    float cellSize = 32.0f; // Размер клетки в пикселях
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            cells[y][x].setPosition(x * cellSize, y * cellSize);
        }
    }
    
    flagsPlaced = 0;
    cellsRevealed = 0;
    firstClick = true;
    gameOver = false;
    gameWon = false;
    fieldGenerated = false;
}

void Field::generateField(int safeX, int safeY) {
    placeMines(safeX, safeY);
    calculateAdjacentMines();
    fieldGenerated = true;
}

void Field::placeMines(int safeX, int safeY) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disX(0, width - 1);
    std::uniform_int_distribution<> disY(0, height - 1);
    
    int minesPlaced = 0;
    while (minesPlaced < totalMines) {
        int x = disX(gen);
        int y = disY(gen);
        
        // Не ставим мину на безопасную клетку и вокруг нее
        if (std::abs(x - safeX) <= 1 && std::abs(y - safeY) <= 1) {
            continue;
        }
        
        if (!cells[y][x].getHasMine()) {
            cells[y][x].setHasMine(true);
            minesPlaced++;
        }
    }
}

void Field::calculateAdjacentMines() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (!cells[y][x].getHasMine()) {
                int count = 0;
                
                // Проверяем все соседние клетки
                for (int dy = -1; dy <= 1; ++dy) {
                    for (int dx = -1; dx <= 1; ++dx) {
                        int nx = x + dx;
                        int ny = y + dy;
                        
                        if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                            if (cells[ny][nx].getHasMine()) {
                                count++;
                            }
                        }
                    }
                }
                
                cells[y][x].setAdjacentMines(count);
            }
        }
    }
}

void Field::handleLeftClick(int x, int y) {
    if (gameOver || gameWon) return;
        
    int cellX = x / 32;
    int cellY = y / 32;
        
    if (cellX < 0 || cellX >= width || cellY < 0 || cellY >= height) {
        return;
    }
    
    Cell& cell = cells[cellY][cellX];
    
    if (cell.isFlagged() || cell.isRevealed()) {
        return;
    }
    
    if (firstClick) {
        generateField(cellX, cellY);
        firstClick = false;
    }
    
    if (cell.getHasMine()) {
        cell.explode();
        gameOver = true;
        revealAllMines();
        return;
    }
    
    if (!fieldGenerated) {
        return;
    }
    
    revealEmptyArea(cellX, cellY);
    
    // Проверка победы
    if (cellsRevealed == (width * height - totalMines)) {
        gameWon = true;
    }
}

void Field::handleRightClick(int x, int y) {
    if (gameOver || gameWon) return;
        
    int cellX = x / 32;
    int cellY = y / 32;
    
    if (cellX < 0 || cellX >= width || cellY < 0 || cellY >= height) {
        return;
    }
    
    Cell& cell = cells[cellY][cellX];
    
    if (cell.isRevealed()) {
        return;
    }
    
    if (cell.isFlagged()) {
        flagsPlaced--;
    } else {
        flagsPlaced++;
    }
    
    cell.toggleFlag();
}

void Field::revealEmptyArea(int x, int y) {
    std::queue<std::pair<int, int>> toReveal;
    toReveal.push({x, y});
    
    while (!toReveal.empty()) {
        auto [cx, cy] = toReveal.front();
        toReveal.pop();
        
        if (cx < 0 || cx >= width || cy < 0 || cy >= height) {
            continue;
        }
        
        Cell& cell = cells[cy][cx];
        
        if (cell.isRevealed() || cell.isFlagged()) {
            continue;
        }
        
        cell.reveal();
        cellsRevealed++;
        
        // Если клетка пустая, добавляем соседей
        if (cell.getAdjacentMines() == 0) {
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    int nx = cx + dx;
                    int ny = cy + dy;
                    
                    if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                        if (!cells[ny][nx].isRevealed() && !cells[ny][nx].isFlagged()) {
                            toReveal.push({nx, ny});
                        }
                    }
                }
            }
        }
    }
}

void Field::revealAllMines() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (cells[y][x].getHasMine() && !cells[y][x].isFlagged()) {
                cells[y][x].reveal();
            }
        }
    }
}

void Field::draw(sf::RenderWindow& window, const sf::Texture& hiddenTexture, 
                 const sf::Texture& revealedTexture, const sf::Texture& flagTexture,
                 const sf::Texture& mineTexture, const sf::Texture& explodedTexture,
                 const sf::Texture* numberTextures) {
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            cells[y][x].draw(window, hiddenTexture, revealedTexture, flagTexture,
                            mineTexture, explodedTexture, numberTextures);
        }
    }
}