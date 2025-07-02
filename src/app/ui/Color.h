#pragma once

struct Color {
    Uint8 r, g, b, a;
    Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255)
        : r(r), g(g), b(b), a(a) {}
};