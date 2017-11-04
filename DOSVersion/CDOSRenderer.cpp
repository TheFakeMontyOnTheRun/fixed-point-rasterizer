#include <memory>
#include <map>
#include <conio.h>
#include <iterator>
#include <dpmi.h>
#include <go32.h>
#include <sys/movedata.h>
#include <pc.h>
#include <sys/farptr.h>
#include <bios.h>
#include <sys/nearptr.h>

#include <cstdlib>
#include <cstdio>
#include <functional>
#include <cmath>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <memory>
#include <map>
#include <sg14/fixed_point>
#include <EASTL/vector.h>
#include <EASTL/array.h>

using eastl::vector;
using eastl::array;

using sg14::fixed_point;

#include "RasterizerCommon.h"
#include "CRenderer.h"

namespace odb {

    array<uint32_t, 320 * 200> mBuffer;
    array<uint8_t, 320 * 200> mMemorySnapshot;

    uint32_t origin = 0;
    uint32_t lastOrigin = -1;
    uint8_t shade;
    long frame = 0;

    void CRenderer::putRaw(uint16_t x, uint16_t y, uint32_t pixel) {

        if (x < 0 || x >= 320 || y < 0 || y > 199) {
            return;
        }

        mBuffer[(320 * y) + x] = pixel;
    }

    unsigned char getPaletteEntry(int origin) {
        unsigned char shade = 0;
        shade += (((((origin & 0x0000FF)) << 2) >> 8)) << 6;
        shade += (((((origin & 0x00FF00) >> 8) << 3) >> 8)) << 3;
        shade += (((((origin & 0xFF0000) >> 16) << 3) >> 8)) << 0;
        return shade;
    }

    CRenderer::CRenderer() {

        __dpmi_regs reg;

        reg.x.ax = 0x13;
        __dpmi_int(0x10, &reg);

        outp(0x03c8, 0);

        for (int r = 0; r < 4; ++r) {
            for (int g = 0; g < 8; ++g) {
                for (int b = 0; b < 8; ++b) {
                    outp(0x03c9, (r * (21)));
                    outp(0x03c9, (g * (8)));
                    outp(0x03c9, (b * (8)));
                }
            }
        }
    }

    void CRenderer::sleep(long ms) {
    }

    void CRenderer::handleSystemEvents() {

        const static FixP delta{2};

        auto lastKey = bioskey(0x11);

        bdos(0xC, 0, 0);

        switch (lastKey) {
            case 18656:
                //up
                mSpeed.mY -= delta;
                mNeedsToRedraw = true;
                break;

            case 8051:
            case 20704:
                //down
                mSpeed.mY += delta;
                mNeedsToRedraw = true;
                break;

            case 19424: //right arrow
            case 4209: //q
                //left
                mSpeed.mX += delta;
                mNeedsToRedraw = true;
                break;

            case 4709: //e
            case 19936: //right arrow
                //right
                mSpeed.mX -= delta;
                mNeedsToRedraw = true;
                break;

            case 3849:
            case 14624:
            case 11785: //c
            case 5236: //t
                //space
                exit(0);
                break;
            case 561:
                clearScr = true;
                mNeedsToRedraw = true;
                break;
            case 818:
                clearScr = false;
                mNeedsToRedraw = true;
                break;

            case 7777:
                mSpeed.mZ += delta;
                mNeedsToRedraw = true;
                break;

            case 11386:
                mSpeed.mZ -= delta;
                mNeedsToRedraw = true;
                break;

            case 0:
                break;
            default:
                printf("WTF is %d", lastKey);
                exit(0);

        }

    }

    void CRenderer::flip() {
        auto source = std::begin(mBuffer);
        auto destination = std::begin(mMemorySnapshot);
        for (int offset = 0; offset < 320 * 200; ++offset) {

            auto origin = *source;

            if (origin != lastOrigin) {
                shade = getPaletteEntry(origin);
            }

            lastOrigin = origin;

            *destination = shade;

            source = std::next(source);
            destination = std::next(destination);
        }

        dosmemput(&mMemorySnapshot[0], 320 * 200, 0xa0000);

        gotoxy(1, 1);
        printf("%d", ++frame);


    }

    void CRenderer::clear() {
        std::fill(std::begin(mBuffer), std::end(mBuffer), 0);
    }
}