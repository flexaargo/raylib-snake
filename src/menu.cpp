#include <raylib.h>
#include <optional>
#include <vector>

#include "game_consts.hpp"

constexpr Color TEXT_COLOR = WHITE;
constexpr float MENU_WINDOW_PADDING = 32.f;
constexpr int TITLE_FONT_SIZE = 64;

constexpr const char *ARROW_KEY_UP_PATH = "assets/textures/arrow_key_up.png";
constexpr const char *ARROW_KEY_DOWN_PATH = "assets/textures/arrow_key_down.png";
constexpr const char *ARROW_KEY_LEFT_PATH = "assets/textures/arrow_key_left.png";
constexpr const char *ARROW_KEY_RIGHT_PATH = "assets/textures/arrow_key_right.png";

struct Textures{
    Texture arrowKeyUp;
    Texture arrowKeyDown;
    Texture arrowKeyLeft;
    Texture arrowKeyRight;
};

std::optional<Textures> loadedTextures = std::nullopt;

void LoadMenuTextures() {
    if (loadedTextures.has_value()) return;
    loadedTextures = {
        .arrowKeyUp=LoadTexture(ARROW_KEY_UP_PATH),
        .arrowKeyDown=LoadTexture(ARROW_KEY_DOWN_PATH),
        .arrowKeyLeft=LoadTexture(ARROW_KEY_LEFT_PATH),
        .arrowKeyRight=LoadTexture(ARROW_KEY_RIGHT_PATH),
    };
}

void UnloadMenuTextures() {
    if (!loadedTextures.has_value()) return;
    UnloadTexture(loadedTextures->arrowKeyUp);
    UnloadTexture(loadedTextures->arrowKeyDown);
    UnloadTexture(loadedTextures->arrowKeyLeft);
    UnloadTexture(loadedTextures->arrowKeyRight);
    loadedTextures = std::nullopt;
}

void DrawArrowKeys() {
    if (!loadedTextures.has_value()) return;
    float imagePadding = 4.0f;
    float imageScale = 2.0f;
    Vector2 imageSize = {
        loadedTextures->arrowKeyUp.width * imageScale,
        loadedTextures->arrowKeyUp.height * imageScale,
    };

    // halfway + 64px + offset for image center
    float textureOriginY = (WINDOW_HEIGHT / 2.0f) + (32.0f * 2.0f) + (imageSize.y / 2.0f);
    // halfway - offset for image center
    float textureOriginX = (WINDOW_WIDTH / 2.0f) - (imageSize.x / 2.0f);

    DrawTextureEx(loadedTextures->arrowKeyUp, Vector2{textureOriginX, textureOriginY}, 0, imageScale, WHITE);

    std::vector<Texture *> bottomRowTextures = {
        &loadedTextures->arrowKeyLeft,
        &loadedTextures->arrowKeyDown,
        &loadedTextures->arrowKeyRight,
    };

    textureOriginY += imagePadding + imageSize.y;
    textureOriginX -= imageSize.x + imagePadding;

    for (const auto &texture_p : bottomRowTextures) {
        if (!texture_p) continue;
        const auto &texture = *texture_p;

        DrawTextureEx(texture, Vector2{textureOriginX, textureOriginY}, 0, imageScale, WHITE);
        textureOriginX += imagePadding + imageSize.x;
    }
}

void DrawMenu() {
    const char *menuTitle = "SNAKE!";
    const int menuTitleWidth = MeasureText(menuTitle, TITLE_FONT_SIZE);
    const Vector2 menuTitleOrigin{
        (WINDOW_WIDTH/2.0f) - (menuTitleWidth/2.0f),
        MENU_WINDOW_PADDING,
    };
    DrawText("SNAKE!", menuTitleOrigin.x, menuTitleOrigin.y, TITLE_FONT_SIZE, TEXT_COLOR);
    DrawArrowKeys();
}
