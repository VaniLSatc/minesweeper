#include "Game.hpp"
#include <iostream>

Game::Game() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Minesweeper"), 
                showMenu(true), selectedDifficulty(DifficultyLevel::EASY) {
    
    window.setFramerateLimit(60);
    
    // Загружаем шрифт
    if (!font.loadFromFile("arial.ttf")) {
        // Пробуем другие возможные пути
        if (!font.loadFromFile("fonts/arial.ttf")) {
            if (!font.loadFromFile("/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf")) {
                std::cerr << "Failed to load font. Using default." << std::endl;
                // Попробуем создать базовый шрифт
                if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
                    std::cerr << "Cannot find any font file!" << std::endl;
                }
            }
        }
    }
    
    // Инициализируем менеджер текстур
    TextureManager::getInstance()->loadAllTextures();
    loadTextures();
    
    field = std::make_unique<Field>();
}

void Game::loadTextures() {
    TextureManager* tm = TextureManager::getInstance();
    
    hiddenTexture = tm->getTexture("cell_hidden");
    revealedTexture = tm->getTexture("cell_revealed");
    flagTexture = tm->getTexture("flag");
    mineTexture = tm->getTexture("mine");
    explodedTexture = tm->getTexture("mine_exploded");
    
    for (int i = 0; i < 8; ++i) {
        std::string name = "number_" + std::to_string(i + 1);
        numberTextures[i] = tm->getTexture(name);
    }
}

void Game::run() {
    while (window.isOpen()) {
        handleEvents();
        update();
        render();
    }
}

void Game::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        
        if (showMenu) {
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                
                // Обработка выбора сложности
                if (mousePos.x >= 350 && mousePos.x <= 650) {
                    if (mousePos.y >= 150 && mousePos.y <= 200) {
                        selectedDifficulty = DifficultyLevel::EASY;
                        startGame();
                    } else if (mousePos.y >= 220 && mousePos.y <= 270) {
                        selectedDifficulty = DifficultyLevel::MEDIUM;
                        startGame();
                    } else if (mousePos.y >= 290 && mousePos.y <= 340) {
                        selectedDifficulty = DifficultyLevel::HARD;
                        startGame();
                    }
                }
            }
        } else {
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                
                // Обрабатываем кнопки на панели
                // Кнопка возврата в меню
                if (mousePos.x >= 10 && mousePos.x <= 160 && 
                    mousePos.y >= 10 && mousePos.y <= 40) {
                    showMenu = true;
                    continue;
                }
                
                // Кнопка перезапуска
                if (mousePos.x >= 180 && mousePos.x <= 330 && 
                    mousePos.y >= 10 && mousePos.y <= 40) {
                    resetGame();
                    continue;
                }
                
                // Получаем настройки текущей сложности для преобразования координат
                GameSettings settings = difficulty.getSettings();
                
                // Рассчитываем положение и размер игрового поля
                int fieldWidth = settings.width * CELL_SIZE;
                int fieldHeight = settings.height * CELL_SIZE;
                int fieldX = (WINDOW_WIDTH - fieldWidth) / 2;
                int fieldY = INFO_PANEL_HEIGHT + (WINDOW_HEIGHT - INFO_PANEL_HEIGHT - fieldHeight) / 2;
                
                // Проверяем, находится ли клик внутри игрового поля
                if (mousePos.x >= fieldX && mousePos.x < fieldX + fieldWidth &&
                    mousePos.y >= fieldY && mousePos.y < fieldY + fieldHeight) {
                    
                    // Преобразуем координаты мыши в координаты игрового поля
                    int fieldMouseX = mousePos.x - fieldX;
                    int fieldMouseY = mousePos.y - fieldY;
                    
                    // Масштабируем координаты, если поле было масштабировано
                    float scale = 1.0f;
                    if (fieldWidth > WINDOW_WIDTH || fieldHeight > (WINDOW_HEIGHT - INFO_PANEL_HEIGHT)) {
                        float scaleX = (WINDOW_WIDTH * 0.9f) / (settings.width * CELL_SIZE);
                        float scaleY = ((WINDOW_HEIGHT - INFO_PANEL_HEIGHT) * 0.9f) / (settings.height * CELL_SIZE);
                        scale = std::min(scaleX, scaleY);
                    }
                    
                    // Применяем обратное масштабирование
                    int scaledMouseX = static_cast<int>(fieldMouseX / scale);
                    int scaledMouseY = static_cast<int>(fieldMouseY / scale);
                    
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        field->handleLeftClick(scaledMouseX, scaledMouseY);
                    } else if (event.mouseButton.button == sf::Mouse::Right) {
                        field->handleRightClick(scaledMouseX, scaledMouseY);
                    }
                }
            }
            
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    showMenu = true;
                } else if (event.key.code == sf::Keyboard::R) {
                    resetGame();
                }
            }
        }
    }
}

void Game::update() {
    // Обновление состояния игры
}

void Game::render() {
    window.clear(sf::Color(200, 200, 200));
    
    if (showMenu) {
        renderMenu();
    } else {
        renderGame();
    }
    
    window.display();
}

void Game::renderMenu() {
    // Фон меню
    sf::RectangleShape background(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    background.setFillColor(sf::Color(230, 230, 230));
    window.draw(background);
    
    sf::Text title("Minesweeper", font, 60);
    title.setFillColor(sf::Color::Blue);
    title.setStyle(sf::Text::Bold);
    title.setPosition(WINDOW_WIDTH/2 - title.getLocalBounds().width/2, 50);
    window.draw(title);
    
    // Кнопки выбора сложности
    std::vector<std::string> difficulties = {"Easy (9x9, 10 mines)", 
                                             "Medium (16x16, 40 mines)", 
                                             "Hard (30x16, 99 mines)"};
    
    for (size_t i = 0; i < difficulties.size(); ++i) {
        sf::RectangleShape button(sf::Vector2f(300, 50));
        button.setPosition(WINDOW_WIDTH/2 - 150, 180 + i * 80);
        button.setFillColor(sf::Color(100, 100, 255));
        button.setOutlineColor(sf::Color::Black);
        button.setOutlineThickness(2);
        window.draw(button);
        
        sf::Text text(difficulties[i], font, 24);
        text.setFillColor(sf::Color::White);
        text.setStyle(sf::Text::Bold);
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setPosition(WINDOW_WIDTH/2 - textBounds.width/2, 190 + i * 80);
        window.draw(text);
    }
    
    sf::Text instruction("Select difficulty and click to start", font, 28);
    instruction.setFillColor(sf::Color::Black);
    instruction.setStyle(sf::Text::Bold);
    sf::FloatRect instrBounds = instruction.getLocalBounds();
    instruction.setPosition(WINDOW_WIDTH/2 - instrBounds.width/2, 450);
    window.draw(instruction);
    
    // Инструкция по управлению
    sf::Text controls("In game: Left click - reveal cell, Right click - flag, R - restart, ESC - menu", 
                     font, 18);
    controls.setFillColor(sf::Color(100, 100, 100));
    controls.setPosition(WINDOW_WIDTH/2 - controls.getLocalBounds().width/2, 520);
    window.draw(controls);
}

void Game::renderGame() {
    // Получаем настройки текущей сложности
    GameSettings settings = difficulty.getSettings();
    int fieldWidthPixels = settings.width * CELL_SIZE;
    int fieldHeightPixels = settings.height * CELL_SIZE;
    
    // Центрируем игровое поле в окне
    int fieldX = (WINDOW_WIDTH - fieldWidthPixels) / 2;
    int fieldY = INFO_PANEL_HEIGHT + (WINDOW_HEIGHT - INFO_PANEL_HEIGHT - fieldHeightPixels) / 2;
    
    // Если поле слишком большое для окна, масштабируем его
    float scale = 1.0f;
    int scaledFieldWidth = fieldWidthPixels;
    int scaledFieldHeight = fieldHeightPixels;
    
    if (fieldWidthPixels > WINDOW_WIDTH || fieldHeightPixels > (WINDOW_HEIGHT - INFO_PANEL_HEIGHT)) {
        float scaleX = (WINDOW_WIDTH * 0.9f) / fieldWidthPixels;
        float scaleY = ((WINDOW_HEIGHT - INFO_PANEL_HEIGHT) * 0.9f) / fieldHeightPixels;
        scale = std::min(scaleX, scaleY);
        scaledFieldWidth = static_cast<int>(fieldWidthPixels * scale);
        scaledFieldHeight = static_cast<int>(fieldHeightPixels * scale);
        fieldX = (WINDOW_WIDTH - scaledFieldWidth) / 2;
        fieldY = INFO_PANEL_HEIGHT + (WINDOW_HEIGHT - INFO_PANEL_HEIGHT - scaledFieldHeight) / 2;
    }
    
    // Создаем вид для отрисовки поля
    sf::View fieldView(sf::FloatRect(0, 0, fieldWidthPixels, fieldHeightPixels));
    
    // Устанавливаем вьюпорт с учетом масштабирования и позиционирования
    fieldView.setViewport(sf::FloatRect(
        fieldX / (float)WINDOW_WIDTH,
        fieldY / (float)WINDOW_HEIGHT,
        scaledFieldWidth / (float)WINDOW_WIDTH,
        scaledFieldHeight / (float)WINDOW_HEIGHT
    ));
    
    // Применяем масштабирование
    fieldView.zoom(1.0f / scale);
    
    // Сохраняем текущий вид
    sf::View defaultView = window.getView();
    
    // Устанавливаем вид для поля и рисуем его
    window.setView(fieldView);
    field->draw(window, hiddenTexture, revealedTexture, flagTexture,
                mineTexture, explodedTexture, numberTextures);
    
    // Возвращаем обычный вид для отрисовки UI
    window.setView(defaultView);
    
    // Панель информации
    sf::RectangleShape infoPanel(sf::Vector2f(WINDOW_WIDTH, INFO_PANEL_HEIGHT));
    infoPanel.setPosition(0, 0);
    infoPanel.setFillColor(sf::Color(180, 180, 180));
    infoPanel.setOutlineColor(sf::Color::Black);
    infoPanel.setOutlineThickness(2);
    window.draw(infoPanel);
    
    // Кнопка меню
    sf::RectangleShape menuButton(sf::Vector2f(150, 30));
    menuButton.setPosition(10, 10);
    menuButton.setFillColor(sf::Color(100, 100, 255));
    menuButton.setOutlineColor(sf::Color::Black);
    menuButton.setOutlineThickness(1);
    window.draw(menuButton);
    
    sf::Text menuText("Menu (Esc)", font, 16);
    menuText.setFillColor(sf::Color::White);
    menuText.setStyle(sf::Text::Bold);
    menuText.setPosition(20, 12);
    window.draw(menuText);
    
    // Кнопка перезапуска
    sf::RectangleShape restartButton(sf::Vector2f(150, 30));
    restartButton.setPosition(180, 10);
    restartButton.setFillColor(sf::Color(100, 255, 100));
    restartButton.setOutlineColor(sf::Color::Black);
    restartButton.setOutlineThickness(1);
    window.draw(restartButton);
    
    sf::Text restartText("Restart (R)", font, 16);
    restartText.setFillColor(sf::Color::White);
    restartText.setStyle(sf::Text::Bold);
    restartText.setPosition(190, 12);
    window.draw(restartText);
    
    // Информация о минах
    std::string mineInfo = "Mines: " + std::to_string(field->getFlagsPlaced()) + 
                          "/" + std::to_string(field->getTotalMines());
    sf::Text minesText(mineInfo, font, 20);
    minesText.setFillColor(sf::Color::Black);
    minesText.setStyle(sf::Text::Bold);
    minesText.setPosition(WINDOW_WIDTH/2 - minesText.getLocalBounds().width/2, 12);
    window.draw(minesText);
    
    // Отображение текущего уровня сложности
    std::string levelText = "Level: " + settings.name;
    sf::Text levelDisplay(levelText, font, 18);
    levelDisplay.setFillColor(sf::Color(0, 0, 139));
    levelDisplay.setStyle(sf::Text::Bold);
    levelDisplay.setPosition(WINDOW_WIDTH - levelDisplay.getLocalBounds().width - 20, 12);
    window.draw(levelDisplay);
    
    // Статус игры
    if (field->isGameOver()) {
        sf::RectangleShape gameOverOverlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
        gameOverOverlay.setFillColor(sf::Color(0, 0, 0, 150));
        window.draw(gameOverOverlay);
        
        sf::Text gameOverText("Game Over!", font, 60);
        gameOverText.setFillColor(sf::Color::Red);
        gameOverText.setStyle(sf::Text::Bold);
        sf::FloatRect gameOverBounds = gameOverText.getLocalBounds();
        gameOverText.setPosition(WINDOW_WIDTH/2 - gameOverBounds.width/2, 
                                WINDOW_HEIGHT/2 - gameOverBounds.height/2 - 30);
        window.draw(gameOverText);
        
        sf::Text restartHint("Press R to restart or ESC for menu", font, 24);
        restartHint.setFillColor(sf::Color::White);
        restartHint.setPosition(WINDOW_WIDTH/2 - restartHint.getLocalBounds().width/2, 
                               WINDOW_HEIGHT/2 + 40);
        window.draw(restartHint);
    } else if (field->isGameWon()) {
        sf::RectangleShape winOverlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
        winOverlay.setFillColor(sf::Color(0, 100, 0, 150));
        window.draw(winOverlay);
        
        sf::Text winText("You Win!", font, 60);
        winText.setFillColor(sf::Color::Green);
        winText.setStyle(sf::Text::Bold);
        sf::FloatRect winBounds = winText.getLocalBounds();
        winText.setPosition(WINDOW_WIDTH/2 - winBounds.width/2, 
                           WINDOW_HEIGHT/2 - winBounds.height/2 - 30);
        window.draw(winText);
        
        sf::Text restartHint("Press R to play again or ESC for menu", font, 24);
        restartHint.setFillColor(sf::Color::White);
        restartHint.setPosition(WINDOW_WIDTH/2 - restartHint.getLocalBounds().width/2, 
                               WINDOW_HEIGHT/2 + 40);
        window.draw(restartHint);
    }
}

void Game::startGame() {
    difficulty.setLevel(selectedDifficulty);
    GameSettings settings = difficulty.getSettings();
    
    field->initialize(settings);
    showMenu = false;
}

void Game::resetGame() {
    GameSettings settings = difficulty.getSettings();
    field->initialize(settings);
}