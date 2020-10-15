/*
** EPITECH PROJECT, 2019
** my
** File description:
** Prototype all functions of libmy.a & define macros
*/

//csfml
#include "my.h"
#include <SFML/Graphics.h>

#ifndef _screen_h_
#define _screen_h_


#define DRAW_WINDOW_SIZE_X 1920
#define DRAW_WINDOW_SIZE_Y 1080


typedef struct framebuffer {
    unsigned int width;
    unsigned int height;
    sfUint8 *pixels;
} framebuffer_t;

int my_draw_line2(framebuffer_t *buf, sfVector2f *vects0, sfVector2f *vects1,
    sfColor color);
int my_draw_line(framebuffer_t *buf, sfVector2f *vects0, sfVector2f *vects1,
    sfColor color);
framebuffer_t *framebuffer_create(unsigned int width, unsigned int height);
void framebuffer_destroy(framebuffer_t *framebuffer);
void my_put_pixel(framebuffer_t *framebuffer, int x, int y,
    sfColor color);
void my_fill_buffer(framebuffer_t *buf, sfColor color);
void my_blur_buffer(framebuffer_t *buf, float ratio);
void my_draw_circle(framebuffer_t *buf, sfVector2f position, unsigned int size,
    sfColor color);
void my_draw_circle2(framebuffer_t *buf, sfVector2f position, unsigned int size,
    sfColor color);
    void my_draw_lines(framebuffer_t *buf, sfVector2f **vects,
    int n, sfColor colo);
void my_draw_lines2(framebuffer_t *buf, sfVector2f **vects,
    int n, sfColor colo);
int my_draw_lld(framebuffer_t *buf, lld_t *lld);
void my_draw_square(framebuffer_t *buf, sfVector2u position, unsigned int size,
    sfColor color);
void my_draw_rect(framebuffer_t *buf, int *pos_size, sfColor color);
framebuffer_t *draw(void);

int gen_tree_draw(gen_tree *root, float x, float y, framebuffer_t *fb, sfVector2f *pl);

#endif
