#include "Difficulty.hpp"

Difficulty::Difficulty() : currentLevel(DifficultyLevel::EASY) {
    settings = getPreset(DifficultyLevel::EASY);
}

void Difficulty::setLevel(DifficultyLevel level) {
    currentLevel = level;
    settings = getPreset(level);
}

void Difficulty::setCustomSettings(int width, int height, int mines) {
    currentLevel = DifficultyLevel::CUSTOM;
    settings.width = width;
    settings.height = height;
    settings.mines = mines;
    settings.name = "Custom";
}

GameSettings Difficulty::getPreset(DifficultyLevel level) {
    switch (level) {
        case DifficultyLevel::EASY:
            return {9, 9, 10, "Easy"};
        case DifficultyLevel::MEDIUM:
            return {16, 16, 40, "Medium"};
        case DifficultyLevel::HARD:
            return {30, 16, 99, "Hard"};
        default:
            return {9, 9, 10, "Easy"};
    }
}
