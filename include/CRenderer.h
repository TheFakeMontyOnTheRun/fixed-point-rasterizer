#ifndef IRENDERER_H
#define IRENDERER_H

#include <set>

namespace odb {

    class CRenderer {
        bool mCached = false;
        bool mNeedsToRedraw = true;
        int zBuffer[ 320 ];
        void fill( int x1, int y1, int w, int h, const array<uint8_t,4>& colour );
        void put( int x, int y, uint32_t pixel);
        void flip();
     public:
        CRenderer();
        void drawCubeAt(const Vec3& center);
        Vec2 project(const Vec3&  p );
        void render( long ms );
        void handleSystemEvents();
        void sleep( long ms );
        void putRaw(int x, int y, uint32_t pixel);
        void drawWall( FixP x0, FixP x1, FixP x0y0, FixP x0y1, FixP x1y0, FixP x1y1);
        void drawFloor(FixP y0, FixP y1, FixP x0y0, FixP x1y0, FixP x0y1, FixP x1y1 );
        void clear();
        int32_t x = 64;
        int32_t y = -96;
        int32_t z = 1;
        int32_t speedX = 0;
        int32_t speedY = 0;
        int32_t speedZ = 0;
        bool clearScr = true;
        void drawLine(int x0, int y0, int x1, int y1);
        void drawLine(const Vec2& p0, const Vec2& p1);
        int wrap(int i);
    };
}
#endif