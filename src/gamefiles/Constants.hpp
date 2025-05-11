#pragma once // Ensures this header is included only once per compilation unit

// --- Global Constants ---
// Using constants makes the code easier to read and modify.

// Physics Constants
const float GRAVITY = 0.8f;           // Downward acceleration (pixels/frame^2)
const float PLAYER_MOVE_SPEED = 5.0f;   // Horizontal speed (pixels/frame)
const float PLAYER_JUMP_VELOCITY = -18.0f; // Initial jump velocity (pixels/frame, negative is up)

// Tile Constants
const int TILE_SIZE = 40;             // Width and height of each tile (pixels)
const float COLLISION_EPSILON = 0.01f;  // Small offset for collision checks

// Window Constants
const unsigned int WINDOW_WIDTH = 800;    // Width of the game window (pixels)
const unsigned int WINDOW_HEIGHT = 600;   // Height of the game window (pixels)
