#include <SFML/Graphics.h>
#include <math.h>
#include <stdlib.h>
#include "my.h"
#include "screen.h"

int gen_tree_draw(gen_tree *root, float x, float y, framebuffer_t *fb, sfVector2f *pl)
{
    static float size = 6;
    static char returning = 0;
    static sfColor alive_c[] = {100, 100, 100, 255};
    static sfColor alive_child_c[] = {160, 160, 160, 255};
    //if (x > WINDOW_SIZE || y > WINDOW_SIZE)
    //    return (y);
    static float xe = 0;
    static float ye = 0;
    static float xo = DRAW_WINDOW_SIZE_X/2;
    static float yo = DRAW_WINDOW_SIZE_Y/2;
    static float focus = 0;
    if (x == -1 && y == -1){
        x = DRAW_WINDOW_SIZE_X/3 + xo - xe;
        y = DRAW_WINDOW_SIZE_Y/3 + yo - ye;
        x = x*0.03+xo*0.97;
        y = y*0.03+yo*0.97;
        xo = x;
        yo = y;
        pl->x = x;
        pl->y = y;
        xe = 0;
        focus = 0;
    }
    if ((root->c && root->child[0]) || (!root->c && lld_len((lld_t *)root->child))){

        returning = 0;
        sfVector2f p[] = {x, y};
        int child_nb = 0;

        if (root->c)
            for (int i = 0; root->child[i]; i++)
                root->child[i]->c == 0 ? child_nb++ : 0;
        else {
            lld_t *mv = (lld_t *)root->child;
            for (mv = mv->next; mv; mv = mv->next)
            ((gen_tree *)mv->data)->c == 0 ? child_nb++ : 0;
        }
        //amputate tree for better performance
        if (child_nb == 0 && root->c == 1){
            int p = 0;
            for (int i = 0; root->child[i]; i++)
                if (root->child[i]->child[0])
                    root->child[p++] = root->child[i];
            root->child[p] = 0;
            root->c = 1;
        }
        if (focus < child_nb){
            focus = child_nb;
            xe = x;
            ye = y;
        }
        if (x > 0 && y < DRAW_WINDOW_SIZE_Y && y > 0 && x < DRAW_WINDOW_SIZE_X){

            int tab[] = {(int)(x-size*2), (int)(y-size*2), (int)(size*4), (int)(size*4)};
            if (root->c)
                my_draw_rect(fb, tab, sfBlack);
            else
                my_draw_rect(fb, tab, *alive_c);
            tab[1] += size/3;
            tab[3] -= size/3*2;
            tab[0] += tab[2];
            tab[2] = child_nb;
            my_draw_rect(fb, tab, *alive_child_c);

            my_draw_line2(fb, pl, p, sfWhite);

            sfVector2f p1[] = {cos(0 - 135/180.0*PI)*size + x, sin(0 - 135/180.0*PI)*size + y};
            sfVector2f p2[] = {cos(0)*size + x, sin(0)*size + y};
            sfVector2f p3[] = {cos(0 + 135/180.0*PI)*size + x, sin(0 + 135/180.0*PI)*size + y};
            sfVector2f p4[] = {cos(0 - 135/180.0*PI)*size*2 + x, sin(0 - 135/180.0*PI)*size*2 + y};
            sfVector2f p5[] = {cos(0)*size*2 + x, sin(0)*size*2 + y};
            sfVector2f p6[] = {cos(0 + 135/180.0*PI)*size*2 + x, sin(0 + 135/180.0*PI)*size*2 + y};
            sfVector2f *lines[7] = {p1, p2, p3, p4, p5, p6, p1};

            sfColor color[] = {root->r, root->g, root->b, 255};
            my_draw_lines(fb, lines, 7, *color);
        }
        if (root->c)
            for (int i = 0; root->child[i]; i++)
                y = gen_tree_draw(root->child[i], x+size*8, y, fb, p);
        else {
            lld_t *mv = (lld_t *)root->child;
            for (mv = mv->next; mv; mv = mv->next)
                y = gen_tree_draw(mv->data, x+size*8, y, fb, p);
        }
        if (!returning){
            y += size*3.5;
            returning = 1;
        }
    }
    return (y);
}

void gen_tree_node_compile(gen_tree *tmp)
{
    gen_tree **tab = (gen_tree **)lld_lld_to_tab((lld_t *)tmp->child);
    lld_free((lld_t *)tmp->child);
    tmp->child = tab;
    tmp->c = 1;
}

gen_tree *gen_tree_node_new(gen_tree *parent, unsigned char r, unsigned char g, unsigned char b)
{
    gen_tree *node = malloc(sizeof(gen_tree));
    node->child = (gen_tree **)lld_init();
    node->r = r;
    node->g = g;
    node->b = b;
    node->c = 0;
    lld_insert((lld_t *)parent->child, lld_len((lld_t *)parent->child), node);
    return (node);
}
