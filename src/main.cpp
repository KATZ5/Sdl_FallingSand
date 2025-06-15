#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <iostream>

const int window_W = 640, window_h = 360;

int main() {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    std::cerr << "SDL Failed to initialize the video submodule" << std::endl;
    return 1;
  }

  return 0;
}
