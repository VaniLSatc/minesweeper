#ifndef DIFFICULTY_HPP
#define DIFFICULTY_HPP

#include <string>

enum class DifficultyLevel {
    EASY,
    MEDIUM,
    HARD,
    CUSTOM
};

struct GameSettings {
    int width;
    int height;
    int mines;
    std::string name;
};

class Difficulty {
private:
    DifficultyLevel currentLevel;
    GameSettings settings;
    
public:
    Difficulty();
    
    void setLevel(DifficultyLevel level);
    void setCustomSettings(int width, int height, int mines);
    
    const GameSettings& getSettings() const { return settings; }
    DifficultyLevel getLevel() const { return currentLevel; }
    
    static GameSettings getPreset(DifficultyLevel level);
};

#endif
