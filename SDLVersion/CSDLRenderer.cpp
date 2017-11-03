#include <string>
#include <utility>
#include <functional>
#include <memory>
#include <map>
#include <sg14/fixed_point>
#include <EASTL/vector.h>
#include <EASTL/array.h>

using eastl::vector;
using eastl::array;

using sg14::fixed_point;

#include "RaycastCommon.h"
#include "NativeBitmap.h"
#include "CRenderer.h"

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <cmath>
#include "NativeBitmap.h"
#include "LoadPNG.h"

#ifdef __EMSCRIPTEN__
#include <emscripten/html5.h>
#endif

namespace odb {

    SDL_Surface *video;

#ifdef __EMSCRIPTEN__
    void enterFullScreenMode() {
    EmscriptenFullscreenStrategy s;
    memset(&s, 0, sizeof(s));
    s.scaleMode = EMSCRIPTEN_FULLSCREEN_SCALE_ASPECT;
    s.canvasResolutionScaleMode = EMSCRIPTEN_FULLSCREEN_CANVAS_SCALE_NONE;
    s.filteringMode = EMSCRIPTEN_FULLSCREEN_FILTERING_DEFAULT;
    emscripten_enter_soft_fullscreen(0, &s);
}
#endif


    CRenderer::CRenderer() {
        SDL_Init(SDL_INIT_EVERYTHING);
        video = SDL_SetVideoMode(320, 200, 32, 0);

#ifdef __EMSCRIPTEN__
        enterFullScreenMode();
#endif
    }

    void CRenderer::sleep(long ms) {
#ifndef __EMSCRIPTEN__
        SDL_Delay(33);
#endif
    }

    void CRenderer::handleSystemEvents() {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {

            if (event.type == SDL_QUIT) {
#ifndef __EMSCRIPTEN__
                exit(0);
#endif
            }

            if (event.type == SDL_KEYUP) {
                switch (event.key.keysym.sym) {
                    case SDLK_q:
#ifndef __EMSCRIPTEN__
                        exit(0);
#endif
                    default:
                        break;
                }
            }

            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_SPACE:
                        exit(0);
                        mNeedsToRedraw = true;
                        mCached = false;
                        break;

                    case SDLK_LEFT:
                        speedX -= 10;
                        mNeedsToRedraw = true;
                        mCached = false;
                        break;
                    case SDLK_RIGHT:
                        speedX += 10;
                        mNeedsToRedraw = true;
                        mCached = false;
                        break;
                    case SDLK_UP:
                        speedY -= 10;
                        mNeedsToRedraw = true;
                        mCached = false;
                        break;
                    case SDLK_DOWN:
                        speedY += 10;
                        mNeedsToRedraw = true;
                        mCached = false;
                        break;

                    case SDLK_z:
                        speedZ += 10;
                        mNeedsToRedraw = true;
                        mCached = false;
                        break;

                    case SDLK_a:
                        speedZ -= 10;
                        mNeedsToRedraw = true;
                        mCached = false;
                        break;

                    default:
                        break;
                }
            }

        }
    }

    void CRenderer::fill(int x1, int y1, int w, int h, const array<uint8_t, 4> &colour) {
        SDL_Rect rect;
        rect.x = x1;
        rect.y = y1;
        rect.w = w;
        rect.h = h;

        SDL_FillRect(video, &rect, SDL_MapRGB(video->format, colour[1], colour[2], colour[3]));
    }

    void CRenderer::put(int x, int y, uint32_t pixel) {
        SDL_Rect rect;
        rect.x = x;
        rect.y = y;
        rect.w = 1;
        rect.h = 1;

        SDL_FillRect(video, &rect, SDL_MapRGB(video->format, ((pixel & 0x000000FF)), ((pixel & 0x0000FF00) >> 8),
                                              ((pixel & 0x00FF0000) >> 16)));
    }

    void CRenderer::putRaw(int x, int y, uint32_t pixel) {
        put(x, y, pixel);
    }

    void CRenderer::flip() {
        SDL_Flip(video);
    }

    void CRenderer::clear() {
        SDL_FillRect(video, nullptr, 0);
    }
}
