#ifndef SNAKE_GAME_CONSTS_H_
#define SNAKE_GAME_CONSTS_H_

constexpr int TICKS_PER_SECOND = 5;
constexpr float TIME_PER_TICK = 1.0f / static_cast<float>(TICKS_PER_SECOND);

constexpr int CELL_WIDTH = 32;
constexpr int CELL_HEIGHT = 32;
/// 16x9 grid + 1 extra cell so there is a center
constexpr int GRID_WIDTH = 16 * 2 + 1;
constexpr int GRID_HEIGHT = 9 * 2 + 1;
constexpr int WINDOW_WIDTH = CELL_WIDTH * GRID_WIDTH;
constexpr int WINDOW_HEIGHT = CELL_HEIGHT * GRID_HEIGHT;

#endif
