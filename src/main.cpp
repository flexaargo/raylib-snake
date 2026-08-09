#include <cstdlib>
#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>

struct Vector2i {
    int x;
    int y;
};

constexpr Vector2 Vector2iToVector2(const Vector2i &v) {
    return {
        static_cast<float>(v.x),
        static_cast<float>(v.y),
    };
}

constexpr bool Vector2iEquals(const Vector2i &v1, const Vector2i &v2) {
    return v1.x == v2.x && v1.y == v2.y;
}

constexpr int TICKS_PER_SECOND = 5;
constexpr float TIME_PER_TICK = 1.0f / static_cast<float>(TICKS_PER_SECOND);

constexpr int CELL_WIDTH = 32;
constexpr int CELL_HEIGHT = 32;
constexpr Vector2i CELL_SIZE{CELL_WIDTH, CELL_HEIGHT};
constexpr int APPLE_WIDTH = 16;
constexpr int APPLE_HEIGHT = 16;
constexpr Vector2i APPLE_SIZE{APPLE_WIDTH, APPLE_HEIGHT};
constexpr float APPLE_SPAWN_TIMEOUT_SECS = 5.0f;

/// 16x9 grid + 1 extra cell so there is a center
constexpr int GRID_WIDTH = 16 * 2 + 1;
constexpr int GRID_HEIGHT = 9 * 2 + 1;
constexpr int WINDOW_WIDTH = CELL_WIDTH * GRID_WIDTH;
constexpr int WINDOW_HEIGHT = CELL_HEIGHT * GRID_HEIGHT;

constexpr Color SNAKE_COLOR = WHITE;
constexpr Color APPLE_COLOR = GREEN;

constexpr Vector2i GetCenterCellPos() {
    return{GRID_WIDTH / 2, GRID_HEIGHT / 2};
}

constexpr Vector2 CellToScreenCoords(const Vector2i &cell) {
    return {static_cast<float>(cell.x * CELL_WIDTH), static_cast<float>(cell.y * CELL_HEIGHT)};
}

bool IsVector2Zero(const Vector2 &v) {
    return Vector2Equals(v, Vector2Zero());
}

constexpr bool IsVector2iZero(const Vector2i &v) {
    return v.x == 0 && v.y == 0;
}

constexpr Vector2i CellOffset(const Vector2i &pos, const Vector2i &offset) {
    return {pos.x + offset.x, pos.y + offset.y};
}

float tickAccumulator = 0.0f;
float appleSpawnAccumulator = APPLE_SPAWN_TIMEOUT_SECS;
Vector2i inputDir{};
Vector2i moveDir{};
std::vector<Vector2i> snakeBody{GetCenterCellPos()};
std::vector<Vector2i> apples{};

std::mt19937 rng(std::random_device{}());

void IncreaseSnakeBody() {
    if (snakeBody.empty()) return;
    snakeBody.insert(snakeBody.end(), snakeBody.back());
}

void UpdateInputDir() {
    Vector2i newInputVector{};
    newInputVector.y = IsKeyPressed(KEY_DOWN) - IsKeyPressed(KEY_UP);
    newInputVector.x = IsKeyPressed(KEY_RIGHT) - IsKeyPressed(KEY_LEFT);
    if (IsVector2iZero(newInputVector)) return;
    bool isDiagonalInput = std::abs(newInputVector.x) && std::abs(newInputVector.y);
    if (isDiagonalInput) return;
    bool isOppositeDir = (newInputVector.x + moveDir.x) == 0 && (newInputVector.y + moveDir.y) == 0;
    if (isOppositeDir) return;
    inputDir = newInputVector;
}

void DrawSnake() {
    for (const auto &point : snakeBody) {
        Vector2 screenCoords = CellToScreenCoords(point);
        DrawRectangle(screenCoords.x, screenCoords.y, CELL_WIDTH, CELL_HEIGHT, SNAKE_COLOR);
    }
}

void DrawApples() {
    for (const auto &apple : apples) {
        Vector2 cellOrigin = CellToScreenCoords(apple);
        Vector2 appleOriginOffset = Vector2Divide(
            Vector2Subtract(Vector2iToVector2(CELL_SIZE), Vector2iToVector2(APPLE_SIZE)),
            {2, 2}
        );
        Vector2 appleOrigin = Vector2Add(cellOrigin, appleOriginOffset);
        DrawRectangle(appleOrigin.x, appleOrigin.y, APPLE_WIDTH, APPLE_HEIGHT, APPLE_COLOR);
    }
}

void UpdateSnakePosition() {
    if (IsVector2iZero(inputDir)) return;
    if (snakeBody.empty()) return;
    // basically looping thru snake in reverse and setting pos = next body pos
    for (auto it = snakeBody.rbegin(); it != snakeBody.rend(); ++it) {
        auto &curr = *it;
        auto nextIt = it + 1;
        if (nextIt != snakeBody.rend()) {
            curr = *nextIt;
        } else {
            // this is the head of the snake
            curr = CellOffset(curr, inputDir);
        }
    }
    moveDir = inputDir;
}

void Update(float delta) {
    UpdateInputDir();
}

bool IsInvalidAppleSpawnPosition(Vector2i spawnPos) {
    const auto snakeOverlapPos = std::find_if(snakeBody.begin(), snakeBody.end(), [spawnPos](const auto &snakeBodyPos) {
        return Vector2iEquals(spawnPos, snakeBodyPos);
    });
    if (snakeOverlapPos != snakeBody.end()) return true;
    const auto appleOverlapPos = std::find_if(apples.begin(), apples.end(), [spawnPos](const auto &applePos) {
        return Vector2iEquals(spawnPos, applePos);
    });
    if (appleOverlapPos != apples.end()) return true;
    return false;
}

void TrySpawnApple(float delta) {
    appleSpawnAccumulator += delta;
    if (appleSpawnAccumulator < APPLE_SPAWN_TIMEOUT_SECS) return;
    appleSpawnAccumulator = 0.0f;

    std::uniform_int_distribution<int> appleSpawnLocationX(1, GRID_WIDTH-1);
    std::uniform_int_distribution<int> appleSpawnLocationY(1, GRID_HEIGHT-1);
    Vector2i spawnPosition{};
    do {
        spawnPosition = {appleSpawnLocationX(rng), appleSpawnLocationY(rng)};
    } while (IsInvalidAppleSpawnPosition(spawnPosition));
    apples.push_back(spawnPosition);
}

void CheckAppleCollision() {
    if (snakeBody.empty()) return;
    Vector2i &head = snakeBody.front();
    int i = 0;
    for (const auto &apple : apples) {
        if (Vector2iEquals(apple, head)) {
            apples.erase(apples.begin() + i);
            IncreaseSnakeBody();
            return;
        }
        ++i;
    }
}

void TickUpdate(float delta) {
    UpdateSnakePosition();
    CheckAppleCollision();
    TrySpawnApple(delta);
}

int main(void) {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Snake!");
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        float frameTime = GetFrameTime();
        tickAccumulator += frameTime;
        Update(frameTime);
        if (tickAccumulator >= TIME_PER_TICK) {
            TickUpdate(tickAccumulator);
            tickAccumulator = 0;
        }
        BeginDrawing();
        ClearBackground(DARKGRAY);
        DrawApples();
        DrawSnake();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
