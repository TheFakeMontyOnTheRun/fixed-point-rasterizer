#include <memory>
#include <utility>
#include <map>
#include <cstdlib>
#include <cstdio>
#include <functional>
#include <cmath>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <memory>
#include <iostream>
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
#include "LoadPNG.h"

namespace odb {

    const static bool kShouldDrawOutline = false;

    vector<std::shared_ptr<odb::NativeBitmap>> textures{
            loadPNG("res/tile0.png"),
    };


    Vec2 CRenderer::project(const Vec3&  p ) {
        FixP halfHeight{100};
        FixP oneOver = divide( halfHeight, p.mZ );

        return {
                halfHeight + multiply(p.mX, oneOver),
                halfHeight - multiply(p.mY, oneOver)
        };
    }

    void CRenderer::drawCubeAt(const Vec3& center) {
        FixP height = FixP(1);
        FixP one{ 1 };
        FixP distance{ 4 };
        auto ulz0 = project(center + Vec3{ -one, height - one, distance - one});
        auto urz0 = project(center + Vec3{ one, height - one, distance - one });
        auto llz0 = project(center + Vec3{ -one, height + one, distance - one });
        auto lrz0 = project(center + Vec3{ one, height + one, distance - one });
        auto ulz1 = project(center + Vec3{ -one, height - one, distance + one });
        auto urz1 = project(center + Vec3{ one, height - one, distance + one });
        auto llz1 = project(center + Vec3{ -one, height + one, distance + one });
        auto lrz1 = project(center + Vec3{ one, height + one, distance + one});

        drawWall( urz0.mX, urz1.mX,
                  urz0.mY, lrz0.mY,
                  urz1.mY, lrz1.mY );


        drawFloor( ulz1.mY, urz0.mY,
ulz1.mX, urz1.mX,
                   ulz0.mX, urz0.mX);

        drawFloor( llz1.mY, lrz0.mY,
                   llz1.mX, lrz1.mX,
                   llz0.mX, lrz0.mX);

        drawWall( ulz1.mX, ulz0.mX,
                  ulz1.mY, llz1.mY,
                  urz0.mY, lrz0.mY );

        drawWall( ulz0.mX, urz0.mX,
                  ulz0.mY, llz0.mY,
                  urz0.mY, lrz0.mY );

        if ( kShouldDrawOutline ) {
            drawLine(ulz0,urz0);
            drawLine(llz0,lrz0);
            drawLine(ulz0, llz0);
            drawLine(urz0, lrz0);

            drawLine(ulz1,urz1);
            drawLine(llz1,lrz1);
            drawLine(ulz1, llz1);
            drawLine(urz1, lrz1);

            drawLine(ulz0, ulz1);
            drawLine(llz0, llz1);
            drawLine(urz0, urz1);
            drawLine(lrz0, lrz1);
        }
    }


    /*
     *         /|x1y0
     * x0y0   / |
     *       |  |
     *       |  |
     * x0y1  |  |
     *       \  |
     *        \ |
     *         \| x1y1
     */
    void CRenderer::drawWall( FixP x0, FixP x1, FixP x0y0, FixP x0y1, FixP x1y0, FixP x1y1 ) {

        if ( x0 > x1) {
            //switch x0 with x1
            x0 = x0 + x1;
            x1 = x0 - x1;
            x0 = x0 - x1;

            //switch x0y0 with x1y0
            x0y0 = x0y0 + x1y0;
            x1y0 = x0y0 - x1y0;
            x0y0 = x0y0 - x1y0;

            //switch x0y1 with x1y1
            x0y1 = x0y1 + x1y1;
            x1y1 = x0y1 - x1y1;
            x0y1 = x0y1 - x1y1;
        }

        auto x = static_cast<uint16_t >(x0);
        auto limit = static_cast<uint16_t >(x1);

        if ( x == limit ) {
            return;
        }

        FixP upperY0 = x0y0;
        FixP lowerY0 = x0y1;
        FixP upperY1 = x1y0;
        FixP lowerY1 = x1y1;

        if ( x0y0 > x0y1 ) {
            upperY0 = x0y1;
            lowerY0 = x0y0;
            upperY1 = x1y1;
            lowerY1 = x1y0;
        };

        FixP upperDy = upperY1 - upperY0;
        FixP lowerDy = lowerY1 - lowerY0;

        FixP y0 = upperY0;
        FixP y1 = lowerY0;

        FixP dX{limit - x};
        FixP upperDyDx = upperDy / dX;
        FixP lowerDyDx = lowerDy / dX;

        uint32_t pixel = 0;

        FixP u{0};

        //0xFF here acts as a dirty value, indicating there is no last value. But even if we had
        //textures this big, it would be only at the end of the run.
        uint8_t lastU = 0xFF;
        uint8_t lastV = 0xFF;

        //we can use this statically, since the textures are already loaded.
        //we don't need to fetch that data on every run.
        const static int* data = textures[0]->getPixelData();
        const static int8_t textureWidth = textures[0]->getWidth();
        const static FixP textureSize{ textureWidth };

        FixP du = textureSize / (dX);
        auto ix = x;

        for (; ix < limit; ++ix ) {

            FixP dv = textureSize / ( y1 - y0 );
            FixP v{0};
            auto iu = static_cast<uint8_t >(u);

            auto iY0 = static_cast<uint16_t >(y0);
            auto iY1 = static_cast<uint16_t >(y1);

            for ( auto iy = iY0; iy < iY1; ++iy ) {

                auto iv = static_cast<uint8_t >(v);

                if ( iv != lastV || iu != lastU ) {
                    pixel = data[ (iv * textureWidth ) + iu];
                }

                lastU = iu;
                lastV = iv;

                if ( pixel & 0xFF000000 ) {
                    putRaw( ix, iy, pixel);
                }

                v += dv;
            }

            y0 += upperDyDx;
            y1 += lowerDyDx;
            u += du;
        }

    }


        /*
         *     x0y0 ____________ x1y0
         *         /            \
         *        /             \
         *  x0y1 /______________\ x1y1
         */
    void CRenderer::drawFloor(FixP y0, FixP y1, FixP x0y0, FixP x1y0, FixP x0y1, FixP x1y1 ) {

        //if we have a trapezoid in which the base is smaller
        if ( y0 > y1) {
            //switch y0 with y1
            y0 = y0 + y1;
            y1 = y0 - y1;
            y0 = y0 - y1;

            //switch x0y0 with x0y1
            x0y0 = x0y0 + x0y1;
            x0y1 = x0y0 - x0y1;
            x0y0 = x0y0 - x0y1;

            //switch x1y0 with x1y1
            x1y0 = x1y0 + x1y1;
            x1y1 = x1y0 - x1y1;
            x1y0 = x1y0 - x1y1;
        }

        auto y = static_cast<uint16_t >(y0);
        auto limit = static_cast<uint16_t >(y1);

        if ( y == limit ) {
            return;
        }

        FixP upperX0 = x0y0;
        FixP upperX1 = x1y0;
        FixP lowerX0 = x0y1;
        FixP lowerX1 = x1y1;

        //what if the trapezoid is flipped horizontally?
        if ( x0y0 > x1y0 ) {
            upperX0 = x1y0;
            upperX1 = x0y0;
            lowerX0 = x1y1;
            lowerX1 = x0y1;
        };

        FixP leftDX = lowerX0 - upperX0;
        FixP rightDX = lowerX1 - upperX1;
        FixP dY = y1 - y0;
        FixP leftDxDy = leftDX / dY;
        FixP rightDxDy = rightDX / dY;
        FixP x0 = upperX0;
        FixP x1 = upperX1;

        uint32_t pixel = 0 ;

        FixP v{0};

        //0xFF here acts as a dirty value, indicating there is no last value. But even if we had
        //textures this big, it would be only at the end of the run.
        uint8_t lastU = 0xFF;
        uint8_t lastV = 0xFF;

        auto iy = static_cast<uint16_t >(y);

        //we can use this statically, since the textures are already loaded.
        //we don't need to fetch that data on every run.
        const static int* data = textures[0]->getPixelData();
        const static int8_t textureWidth = textures[0]->getWidth();
        const static FixP textureSize{ textureWidth };

        FixP dv = textureSize / (dY);

        for (; iy < limit; ++iy ) {

            auto iX0 = static_cast<uint16_t >(x0);
            auto iX1 = static_cast<uint16_t >(x1);

            FixP du = textureSize / ( x1 - x0 );
            FixP u{0};
            auto iv = static_cast<uint8_t >(v);

            for ( auto ix = iX0; ix < iX1; ++ix ) {

                auto iu = static_cast<uint8_t >(u);

                //only fetch the next texel if we really changed the u, v coordinates
                //(otherwise, would fetch the same thing anyway)
                if ( iv != lastV || iu != lastU ) {
                    pixel = data[ (iv * textureWidth ) + iu];
                }

                lastU = iu;
                lastV = iv;

                if ( pixel & 0xFF000000 ) {
                    putRaw( ix, iy, pixel);
                }

                u += du;
            }

            x0 += leftDxDy;
            x1 += rightDxDy;

            v += dv;
        }
    }

    void CRenderer::drawLine(const Vec2& p0, const Vec2& p1) {
        drawLine(static_cast<uint16_t >(p0.mX),
                 static_cast<uint16_t >(p0.mY),
                 static_cast<uint16_t >(p1.mX),
                 static_cast<uint16_t >(p1.mY)
        );
    }

    void CRenderer::drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {

        if ( x0 == x1 ) {

            uint16_t _y0 = y0;
            uint16_t _y1 = y1;

            if ( y0 > y1 ) {
                _y0 = y1;
                _y1 = y0;
            }

            for ( int y = _y0; y <= _y1; ++y ) {
                putRaw( x0, y, 0xFFFFFFFF);
            }
            return;
        }

        if ( y0 == y1 ) {
            uint16_t _x0 = x0;
            uint16_t _x1 = x1;

            if ( x0 > x1 ) {
                _x0 = x1;
                _x1 = x0;
            }

            for ( int x = _x0; x <= _x1; ++x ) {
                putRaw( x, y0, 0xFFFFFFFF);
            }
            return;
        }

        //switching x0 with x1
        if( x0 > x1 ) {
            x0 = x0 + x1;
            x1 = x0 - x1;
            x0 = x0 - x1;

            y0 = y0 + y1;
            y1 = y0 - y1;
            y0 = y0 - y1;
        }

        FixP fy = FixP{ y0 };
        FixP fLimitY = FixP{ y1 };
        FixP fDeltatY = FixP{ y1 - y0 } / FixP{ x1 - x0 };

        for ( int x = x0; x <= x1; ++x ) {
            putRaw( x, static_cast<uint16_t >(fy), 0xFFFFFFFF);
            fy += fDeltatY;
        }
    }

    void CRenderer::render(long ms) {

        const static FixP two{2};

        if ( mSpeed.mX ) {
            mSpeed.mX /= two;
            mNeedsToRedraw = true;
        }

        if  ( mSpeed.mY ) {
            mSpeed.mY /= two;
            mNeedsToRedraw = true;
        }

        if  ( mSpeed.mZ ) {
            mSpeed.mZ /= two;
            mNeedsToRedraw = true;
        }

        mCamera += mSpeed;

        if ( mNeedsToRedraw ) {
            mNeedsToRedraw = false;

            if ( clearScr ) {
                clear();
            }

            FixP thirtyTwo{32};
            drawCubeAt( mCamera );
        }

        flip();
    }
}
