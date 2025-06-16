#include <SDL3/SDL.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>
#include <iostream>

const int window_w = 720, window_h = 720;
bool kill = false;
const float cellSize = 24.0;
const int arraySize = window_w / cellSize;
int gridArray[arraySize][arraySize];

void drawCells(SDL_Renderer *renderer) {
  for (float i = 0; i < window_w; i += cellSize) {
    for (float j = 0; j < window_h; j += cellSize) {
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
      SDL_FRect cell = {i, j, cellSize, cellSize};
      SDL_RenderRect(renderer, &cell);
    }
  }
}

void fillGrid(int gridArray[arraySize][arraySize]) {
  for (int i = 0; i < arraySize; i++) {
    for (int j = 0; j < arraySize; j++) {
      gridArray[i][j] = 0;
    }
  }
}

void printGridArray(int gridArray[arraySize][arraySize]) {
  for (int i = 0; i < arraySize; i++) {
    for (int j = 0; j < arraySize; j++) {
      std::cout << gridArray[i][j] << " ";
    }
    std::cout << std::endl;
  }
}

void updateGridArray(int gridArray[arraySize][arraySize]) {
  float mouseX, mouseY;
  SDL_GetMouseState(&mouseX, &mouseY);
  int i_mouseX = mouseX / cellSize, i_mouseY = mouseY / cellSize;
  // std::cout << "Filling cell: " << i_mouseX << " " << i_mouseY << std::endl;

  gridArray[i_mouseY][i_mouseX] = 1;
}

void sandRules(int gridArray[arraySize][arraySize]) {
  for (int i = arraySize - 2; i >= 0; i--) {
    for (int j = 0; j < arraySize; j++) {
      if (gridArray[i][j] == 1 && gridArray[i + 1][j] == 0) {
        gridArray[i + 1][j] = 1;
        gridArray[i][j] = 0;
      } else if (gridArray[i][j] == 1 && gridArray[i + 1][j - 1] == 0 &&
                 j != 0) {
        gridArray[i + 1][j - 1] = 1;
        gridArray[i][j] = 0;
      } else if (gridArray[i][j] == 1 && gridArray[i + 1][j + 1] == 0 &&
                 j != (window_h / cellSize) - 1) {
        gridArray[i + 1][j + 1] = 1;
        gridArray[i][j] = 0;
      }
    }
  }
}

void fillCell(SDL_Renderer *renderer, int gridArray[arraySize][arraySize]) {
  for (int i = 0; i < arraySize; i++) {
    for (int j = 0; j < arraySize; j++) {
      if (gridArray[i][j] == 1) {
        SDL_SetRenderDrawColor(renderer, 194, 178, 128, 0);
        SDL_FRect cell = {j * cellSize, i * cellSize, cellSize, cellSize};
        SDL_RenderFillRect(renderer, &cell);
      }
    }
  }
}

void highlightCell(SDL_Renderer *renderer) {
  float mouseX, mouseY;
  SDL_GetMouseState(&mouseX, &mouseY);
  int i_mouseX = mouseX / cellSize, i_mouseY = mouseY / cellSize;

  // std::cout << i_mouseX << " " << i_mouseY << std::endl;
  SDL_SetRenderDrawColor(renderer, 250, 50, 100, 0);
  SDL_FRect cell = {i_mouseX * cellSize, i_mouseY * cellSize, cellSize,
                    cellSize};
  SDL_RenderFillRect(renderer, &cell);
}

int main() {

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    std::cerr << "SDL Failed to initialize the video submodule"
              << SDL_GetError() << std::endl;
    return 1;
  }

  std::cout << "Video driver: " << SDL_GetCurrentVideoDriver() << std::endl;

  SDL_Window *window =
      SDL_CreateWindow("FallingSand", window_w, window_h, SDL_WINDOW_RESIZABLE);
  if (window == NULL) {
    std::cerr << "Failed to create SDL window" << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
  if (renderer == nullptr) {
    std::cerr << "Failed to create Renderer: " << SDL_GetError() << std::endl;
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  Uint32 lastUpdate = SDL_GetTicks();
  Uint32 startTime = SDL_GetTicks();
  int frameCount = 0;
  float fps = 0.0f;
  const Uint32 tickInterval = 50;

  fillGrid(gridArray);

  while (!kill) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {

      case SDL_EVENT_QUIT:
        kill = true;

      case SDL_EVENT_MOUSE_BUTTON_DOWN:

        printGridArray(gridArray);
      }
    }
    Uint32 currentUpdate = SDL_GetTicks();
    if (currentUpdate - lastUpdate > tickInterval) {
      sandRules(gridArray);
      lastUpdate = currentUpdate;
    }

    updateGridArray(gridArray);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    drawCells(renderer);
    highlightCell(renderer);
    fillCell(renderer, gridArray);
    SDL_RenderPresent(renderer);

    frameCount++;
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - startTime >= 1000) {
      fps = frameCount * 1000.0 / (currentTime - startTime);
      std::cout << "FPS: " << fps << std::endl;

      startTime = currentTime;
      frameCount = 0;
    }
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}
