#ifndef IRENDERER_H
#define IRENDERER_H

#include <set>

namespace odb {

    class CRenderer {
        bool mNeedsToRedraw = true;
        void flip();
     public:
        CRenderer();
        void drawCubeAt(const Vec3& center);
        Vec2 project(const Vec3&  p );
        void render( long ms );
        void handleSystemEvents();
        void sleep( long ms );
        void putRaw(uint16_t x, uint16_t y, uint32_t pixel);
        void drawWall( FixP x0, FixP x1, FixP x0y0, FixP x0y1, FixP x1y0, FixP x1y1);
        void drawFloor(FixP y0, FixP y1, FixP x0y0, FixP x1y0, FixP x0y1, FixP x1y1 );
        void clear();
        Vec3 mCamera{1,1,1};
        Vec3 mSpeed{0,0,0};
        bool clearScr = true;
        void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
        void drawLine(const Vec2& p0, const Vec2& p1);
    };
}
#endif