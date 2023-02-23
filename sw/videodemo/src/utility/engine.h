#ifndef ENGINE_H
#define ENGINE_H

#include <stdbool.h>

/*
 * SDL's rect def'n
 * Ref: https://github.com/zielmicha/SDL2/blob/master/include/SDL_rect.h
*/
typedef struct SDL_Rect
{
    int x, y;
    int w, h;
} SDL_Rect;

/*
 * SDL's Intersection of rectangles code
 * Reference: https://github.com/spurious/SDL-mirror/blob/master/src/video/SDL_rect.c
 */
#define SDL_RectEmpty(X)    ((!(X)) || ((X)->w <= 0) || ((X)->h <= 0))
bool SDL_HasIntersection(const SDL_Rect * A, const SDL_Rect * B);

#endif

