#include <functional>
#include <memory>
#include <iostream>
#include <fstream>
#include <map>
#include <sg14/fixed_point>
#include <EASTL/vector.h>
#include <EASTL/array.h>

using eastl::vector;
using eastl::array;

using sg14::fixed_point;

#include "NativeBitmap.h"
#include "RasterizerCommon.h"
#include "CRenderer.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>

#endif

void *operator new[](size_t size, const char *pName, int flags, unsigned debugFlags,
                     const char *file, int line) {
    return malloc(size);
}

void *operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char *pName,
                     int flags, unsigned debugFlags, const char *file, int line) {
    return malloc(size);
}

std::shared_ptr<odb::CRenderer> renderer;

void gameLoopTick() {
    renderer->render(33);
    renderer->handleSystemEvents();
}


void sysTick() {
    gameLoopTick();
}

int main() {

    renderer = std::make_shared<odb::CRenderer>();

#ifdef __EMSCRIPTEN__
    emscripten_request_fullscreen(0, 1);
    emscripten_set_main_loop( sysTick, 30, 1 );
#else


    while (true) {
        renderer->sleep(33);
        gameLoopTick();

    }
#endif
    return 0;
}
