//
// Created by monty on 27/08/16.
//

#ifndef BLANKSLATE_COMMON_H
#define BLANKSLATE_COMMON_H

namespace odb {
    enum class ECommand {
        kUp,
        kRight,
        kDown,
        kLeft,
        kFire1,
        kFire2
    };

    using FixP = fixed_point<int32_t, -16>;

    class Vec3 {
    public:
        FixP mX;
        FixP mY;
        FixP mZ;
    };

    Vec3 operator+(const Vec3& v1, const Vec3& v2 );
    Vec3 operator-(const Vec3& v1, const Vec3& v2 );

    class Vec2 {
    public:
        FixP mX;
        FixP mY;
    };

    using CControlCallback = std::function< void(ECommand) >;

    int wrap360(int i);

    const int xRes = 255;
    const int yRes = 128;
    const FixP fixedYRes = FixP{128};

}
#endif //BLANKSLATE_COMMON_H
