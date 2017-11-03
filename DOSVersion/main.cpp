#include <functional>
#include <memory>
#include <iostream>
#include <utility>
#include <string>
#include <fstream>
#include <map>
#include <sg14/fixed_point>
#include <EASTL/vector.h>
#include <EASTL/array.h>

using eastl::vector;
using eastl::array;

using sg14::fixed_point;

#include "NativeBitmap.h"
#include "RaycastCommon.h"
#include "CRenderer.h"

void* operator new[](size_t size, const char* pName, int flags, unsigned debugFlags,
                     const char* file, int line) {
    return malloc( size );
}

void* operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* pName,
                     int flags, unsigned debugFlags, const char* file, int line) {
    return malloc( size );
}

int main() {

    odb::CRenderer renderer;

    while ( true ) {
        renderer.render( 33 );
        renderer.handleSystemEvents();
    }

    return 0;
}
