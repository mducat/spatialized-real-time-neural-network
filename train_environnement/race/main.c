#include "include/my.h"
#include "include/screen.h"
#include "include/menu.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <SFML/Graphics.h>
#include <math.h>
#include <SFML/Window/Export.h>
#include <SFML/Window/Keyboard.h>
#include "include/evolve.h"
#include <string.h>

#include <sys/stat.h>
#include <fcntl.h>

typedef long int i64;


typedef struct segment
{
    float x, y;
    float rx, ry;
} ray_segment;

typedef struct road
{
    float sides_max_len;
    float sides_min_len;
    float road_with;
    float turn_power;
    float x;
    float y;
    float a;
    lld_t *road;
    lld_t *road_L;
    lld_t *road_R;
} rc_road;

void rc_generate_road(rc_road *road, int nb);


typedef struct player
{
    float x, y;
    float vx, vy;
    float va, a;
    float d;
    lld_t *mv_L;
    lld_t *mv_R;
    struct learning *AI;
    float score;
    float turning_power;
    float forward_power;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    float mem[32];
    gen_tree *gen_tree;
    int life;
} rc_player;

typedef struct game
{
    rc_player *player;
    rc_road *road;
    lld_t *lld_tmp;
} rc_game;

typedef struct ray_2f
{
    float x, y;
} ray_2f;

float ray_calc_t(ray_segment *p1, ray_segment *p2)
{
    return ((p1->x*-p2->ry + p1->y*p2->rx + p2->x*(p2->y+p2->ry) - p2->y*(p2->x+p2->rx)) / (p1->rx*p2->ry - p1->ry*p2->rx));
}

ray_2f *ray_calc_p(ray_segment *p1, float t)
{
    ray_2f *p = malloc(sizeof(ray_2f));
    p->x = p1->x + p1->rx * t;
    p->y = p1->y + p1->ry * t;
    return (p);
}

void ray_draw_seg2(framebuffer_t *fb, ray_segment *seg, sfColor color)
{
    sfVector2f p[] = {seg->x + seg->rx, seg->y + seg->ry};
    my_draw_line2(fb, (sfVector2f *)seg, p, color);
}

void ray_draw_seg(framebuffer_t *fb, ray_segment *seg, sfColor color)
{
    sfVector2f p[] = {seg->x + seg->rx, seg->y + seg->ry};
    my_draw_line(fb, (sfVector2f *)seg, p, color);
}

//int rays = 16;
int rays = 8;

int mem = 8;

float find_angle(sfVector2f *p, sfVector2f *p2);



float *raycasts(rc_game *game, rc_player *pl, framebuffer_t *fb, rc_player *best, int frame, int skip, rc_player *best_d, float best_x, float best_y)
{
    float view_d = DRAW_WINDOW_SIZE_Y/2.0;
    static float *out = 0; out == 0 ? out = malloc(sizeof(int)*(rays*2+3+mem)) : 0;

    for (int a = 0; a < rays; a++){
        float dx = cos(a*PI/rays + pl->a);
        float dy = sin(a*PI/rays + pl->a);
        ray_segment seg[] = {pl->x+dx, pl->y+dy, -dx*2, -dy*2};

        seg->x = pl->x;
        seg->y = pl->y;
        lld_t *mv_L = pl->mv_L;
        for (int i = 0; i < 20 && mv_L->prev; mv_L = mv_L->prev, i++);
        lld_t *mv_R = pl->mv_R;
        for (int i = 0; i < 20 && mv_R->prev; mv_R = mv_R->prev, i++);

        for (int i = -20; i <= 20; i++){
            float t1 = ray_calc_t(seg, mv_L->data);
            float t2 = ray_calc_t(mv_L->data, seg);

            if (t2 >= 0 && t2 <= 1){
                lld_insert(game->lld_tmp, 0, (void *)(i64)t1);
            }

            t1 = ray_calc_t(seg, mv_R->data);
            t2 = ray_calc_t(mv_R->data, seg);

            if (t2 >= 0 && t2 <= 1){
                lld_insert(game->lld_tmp, 0, (void *)(i64)t1);
            }

            if (!mv_L->next)
                rc_generate_road(game->road, 1);
            mv_L = mv_L->next;
            mv_R = mv_R->next;
        }

        //sort the results
        float tmax = view_d;
        float tmin = -view_d;
        for (lld_t *mv = game->lld_tmp->next; mv; mv = mv->next){
            if ((i64)mv->data > 0 && (i64)mv->data < tmax)
                tmax = (i64)mv->data;
            if ((i64)mv->data < 0 && (i64)mv->data > tmin)
                tmin = (i64)mv->data;
        }


        while (game->lld_tmp->data)
            lld_pop(game->lld_tmp, 0);

        if (pl == best_d && !(frame % skip) & 0)
            printf("%f %f\n", tmin, tmax);

        out[a] = -tmin;//view_d+tmin;
        out[a+rays] = tmax;//view_d-tmax;
        if (pl == best_d && !(frame % skip)){
            sfVector2f pos1 = {pl->x + tmin * seg->rx - best_x, pl->y + tmin * seg->ry - best_y};
            sfVector2f pos2 = {pl->x + tmax * seg->rx - best_x, pl->y + tmax * seg->ry - best_y};

            sfColor color = {122, 122, 122, 255};

            my_draw_line2(fb, &pos1, &pos2, color);
            my_draw_circle2(fb, pos1, 5, sfGreen);
            my_draw_circle2(fb, pos2, 5, sfGreen);
        }
    }
    sfVector2f p1[] = {pl->vx, pl->vy};
    sfVector2f p2[] = {cos(pl->a), sin(pl->a)};
    float a = find_angle(p1, p2);

    float d = sqrt(pow(pl->vx, 2) + pow(pl->vy, 2));

    float r_vx = d * cos(a);
    float r_vy = d * sin(a);

    out[rays*2] = r_vx;
    out[rays*2+1] = r_vy;
    out[rays*2+2] = pl->va;

    memcpy(out+rays*2+3, &pl->mem, sizeof(float)*mem);

    for (int i = 0; i < rays*2+3+mem; i++)
        if (out[i] == (1.0/0) || out[i] == -(1.0/0))
            out[i] = 0;

    if (pl == best_d  && !(frame % skip) && 0){
        for (int i = 0; i < 64; i++)
            printf("%0.1f |", out[i]);
        printf("\n");
    }

    /*float r = 0;
    for (int i = 0; i < rays*2; i++){
        r += out[i];
    }
    pl->score += -r/(view_d*rays*2*100);
    */




    return (out);
}


typedef struct node
{
    float x, y;
    float a;
} rc_node;

float find_angle(sfVector2f *p1, sfVector2f *p2)
{
    float dx = p1->x - p2->x;
    float dy = p1->y - p2->y;
    float d = sqrt(pow(dx, 2) + pow(dy, 2));
    float a = acos(dx/d);
    if (dy < 0)
        a *= -1;
    return (a);
}

void rc_generate_road(rc_road *road, int nb)
{

    for (int i = 0; i < nb; i++){

        gen:;
        road->a *= 0.95;

        int l = road->sides_min_len + rand() % (int)(road->sides_max_len - road->sides_min_len+1);
        float new_a = road->a + (rand()%2048/2048.0-0.5)*PI*road->turn_power;
        if (new_a == road->a)
            goto gen;
        ray_segment seg[] = {road->x, road->y, cos(new_a)*l, sin(new_a)*l};
        lld_t *mv = road->road->next;
        if (mv)
            for (; mv->next; mv = mv->next){
                rc_node *n1 = mv->data;
                rc_node *n2 = mv->next->data;
                ray_segment seg2[] = {n1->x, n1->y, n2->x - n1->x, n2->y - n1->y};
                float t1 = ray_calc_t(seg, seg2);
                float t2 = ray_calc_t(seg2, seg);
                if (t1 <= 1.5 && t1 >= 0 && t2 <= 1.25 && t2 >= -0.25){
                    sfVector2f p2[] = {seg->x+seg->rx-road->x + DRAW_WINDOW_SIZE_X/2, seg->y+seg->ry-road->y + DRAW_WINDOW_SIZE_Y/2};
                    sfVector2f n11[] = {n1->x - road->x + DRAW_WINDOW_SIZE_X/2, n1->y - road->y + DRAW_WINDOW_SIZE_Y/2};
                    sfVector2f n21[] = {n2->x - road->x + DRAW_WINDOW_SIZE_X/2, n2->y - road->y + DRAW_WINDOW_SIZE_Y/2};
                    ray_segment seg2[] = {seg->x - road->x + DRAW_WINDOW_SIZE_X/2, seg->y - road->y + DRAW_WINDOW_SIZE_Y/2, seg->rx, seg->ry};
                    sfVector2f *v = (sfVector2f *)ray_calc_p(seg2, t1);
                    goto gen;
                }
            }
        road->a = new_a;
        rc_node *node = malloc(sizeof(rc_node));
        node->x = road->x;
        node->y = road->y;
        node->a = road->a;

        lld_t *tmp = malloc(sizeof(lld_t));
        tmp->data = node;
        tmp->next = 0;
        tmp->prev = mv;
        mv->next = tmp;
        road->road->data++;

        road->x += seg->rx;
        road->y += seg->ry;

        ray_segment *seg_L = malloc(sizeof(ray_segment));
        ray_segment *seg_R = malloc(sizeof(ray_segment));

        rc_node *n1 = node;
        float with = road->road_with;
        float dx = cos(n1->a+PI/2)*with;
        float dy = sin(n1->a+PI/2)*with;

        float len_x = cos(n1->a)*road->sides_max_len;
        float len_y = sin(n1->a)*road->sides_max_len;

        seg_L->x = n1->x + dx;
        seg_L->y = n1->y + dy;
        seg_R->x = n1->x - dx;
        seg_R->y = n1->y - dy;

        seg_L->rx = len_x;
        seg_L->ry = len_y;
        seg_R->rx = len_x;
        seg_R->ry = len_y;

        lld_insert(road->road_L, (u64)road->road_L->data, seg_L);
        lld_insert(road->road_R, (u64)road->road_R->data, seg_R);
        if ((u64)road->road->data > 2){
            ray_segment *seg1 = road->road_L->prev->prev->data;
            ray_segment *seg2 = road->road_L->prev->data;

            float t1 = ray_calc_t(seg1, seg2);
            if (t1 == 1.0/0.0)
                continue;
            seg1->rx = seg1->rx*t1;
            seg1->ry = seg1->ry*t1;

            seg2->x = seg1->rx + seg1->x;
            seg2->y = seg1->ry + seg1->y;


            seg1 = road->road_R->prev->prev->data;
            seg2 = road->road_R->prev->data;

            t1 = ray_calc_t(seg1, seg2);

            seg1->rx = seg1->rx*t1;
            seg1->ry = seg1->ry*t1;

            seg2->x = seg1->rx + seg1->x;
            seg2->y = seg1->ry + seg1->y;
        }
    }
}

rc_road *rc_create_road()
{
    rc_road *road = malloc(sizeof(rc_road));
    road->sides_max_len = 50;
    road->sides_min_len = 25;
    road->road_with = 50;
    road->turn_power = 0;
    road->x = 0;
    road->y = DRAW_WINDOW_SIZE_Y/2;
    road->a = 0;
    road->road = lld_init();
    road->road_L = lld_init();
    road->road_R = lld_init();

    rc_node *node = malloc(sizeof(rc_node));
    node->x = road->x;
    node->y = road->y;
    node->a = 0;
    lld_insert(road->road, 0, node);

    road->turn_power = 0.02;
    rc_generate_road(road, 2);
    road->turn_power = 0.5;

    ray_segment *seg1 = road->road_L->next->data;
   ray_segment *seg2 = road->road_R->next->data;

    float dx = seg1->x - node->x;
    float dy = seg1->y - node->y;

    seg1->x -= dx;
    seg1->y -= dy;
    seg1->rx += dx;
    seg1->ry += dy;

    seg2->x += dx;
    seg2->y += dy;
    seg2->rx -= dx;
    seg2->ry -= dy;

    return (road);
}

void rc_draw_road(framebuffer_t *fb, rc_road *road, sfVector2f *off, sfColor color)
{
    lld_t *mv = road->road_L->next;
    for (; mv->prev; mv = mv->prev);

    for (; mv; mv = mv->next){
        ray_segment *seg = mv->data;
        seg->x -= off->x;
        seg->y -= off->y;
        ray_draw_seg2(fb, mv->data, color);
        seg->x += off->x;
        seg->y += off->y;
    }
    mv = road->road_R->next;
    for (; mv->prev; mv = mv->prev);
    for (; mv; mv = mv->next){
        ray_segment *seg = mv->data;
        seg->x -= off->x;
        seg->y -= off->y;
        ray_draw_seg2(fb, mv->data, color);
        seg->x += off->x;
        seg->y += off->y;
    }

    /*for (lld_t *mv = road->road->next; mv->next; mv = mv->next){
        sfVector2f *p1 = mv->data;
        sfVector2f *p2 = mv->next->data;
        p1->x -= off->x;
        p1->y -= off->y;
        p2->x -= off->x;
        p2->y -= off->y;
        sfColor color[] = {100, 0, 0, 255};
        my_draw_line2(fb, p1, p2, *color);
        p1->x += off->x;
        p1->y += off->y;
        p2->x += off->x;
        p2->y += off->y;
    }*/
}

int nb_players = 1024;

int skip = 1;


int main(int ac, char **av, char **env)
{
    srand(15);
    rc_road *road = rc_create_road();
    rc_generate_road(road, 5);
    rc_game game[1];

    game->player = malloc(sizeof(rc_player)*(nb_players+1));
    game->road = road;
    game->lld_tmp = lld_init();

    //create root of gen_tree
    gen_tree *root = malloc(sizeof(gen_tree));
    root->child = (gen_tree **)lld_init();
    root->r = 255;
    root->g = 255;
    root->b = 255;
    root->c = 0;

    //int levels[] = {66, 32, 32, 32, 0};
    int levels[] = {rays*2+3+mem, mem+rays, mem, 0};

    lld_t *tree_compil = lld_init();

    rc_node p_spawn[1];// = (rc_node *)game->road->road->next->next;

    p_spawn->x = 50;
    p_spawn->y = DRAW_WINDOW_SIZE_Y/2;
    p_spawn->a = 0;

    printf("%f %f %f\n", p_spawn->x, p_spawn->y, p_spawn->a);
    for (int i = 0; i < nb_players; i++){
        if (1){
            game->player[i].AI = learning_init(levels);
            learning_evolve(game->player[i].AI, 0.5, 0.5);
        } else {
            printf("loading...\n");
            int fd = open("save.eve", O_RDONLY);
            if (fd == -1)
                printf("load fail\n"), exit(84);
            game->player[i].AI = learning_read(fd);
            close(fd);
        }
        game->player[i].x = p_spawn->x;
        game->player[i].y = p_spawn->y;
        game->player[i].vx = 0;
        game->player[i].vy = 0;
        game->player[i].mv_L = road->road_L->next->next;
        game->player[i].mv_R = road->road_R->next->next;
        game->player[i].va = 0;
        game->player[i].a = p_spawn->a;
        game->player[i].d = 0;
        game->player[i].score = 1;
        game->player[i].life = 120 + rand()%120;
        game->player[i].turning_power = 0;
        game->player[i].forward_power = 0;
        game->player[i].r = rand()%128 + 64;
        game->player[i].g = rand()%128 + 64;
        game->player[i].b = rand()%128 + 64;
        memset(game->player[i].mem, 0, sizeof(float)*mem);

        game->player[i].gen_tree = malloc(sizeof(gen_tree));
        game->player[i].gen_tree->child = (gen_tree **)lld_init();
        game->player[i].gen_tree->r = game->player[i].r;
        game->player[i].gen_tree->g = game->player[i].g;
        game->player[i].gen_tree->b = game->player[i].b;
        game->player[i].gen_tree->c = 0;
        lld_insert((lld_t *)root->child, 0, game->player[i].gen_tree);
    }
    {
        //compile root gen_tree
        gen_tree **tab = (gen_tree **)lld_lld_to_tab((lld_t *)root->child);
        lld_free((lld_t *)root->child);
        root->child = tab;
        root->c = 1;
    }


    framebuffer_t *fb = draw();
    int d_max = 0;
    int d_max_prev = 0;

    rc_player *best_d;

    for (int frame = 0; 1; frame++) {
        rc_player *players = game->player;

        /*if (!((frame+1) % 6000)){
            printf("advincing spawns\n");
            for (int i = 0; nb_players > i; i++){
                rc_player *p = &players[i];
                p->x = p_spawn->x;
                p->y = p_spawn->y;
                p->vx = 0;
                p->vy = 0;
                p->a = ;
                p->life = 120;
                p->turning_power = 0;
                p->forward_power = 0;
                p->mv_L = road->road_L->next;
                p->mv_R = road->road_R->next;
            }
        }*/

        //find best player
        rc_player *best;
        int max = 0;
        float tot_score = 0;
        gen_tree *gen_tree_b;
        for (int i = 0; i < nb_players; i++){
            tot_score += players[i].score;
            if (players[i].score >= max){
                max = players[i].score;
                best = &players[i];
                gen_tree_b = best->gen_tree;
            }
        }

        //sfVector2f p1[] = {best->x, best->y};
        //my_draw_circle2(fb, *p1, 5, sfWhite);

        best_d = &players[0];
        {
            int max = 0;
            for (int i = 0; i < nb_players; i++){
                if (players[i].d >= max){
                    max = players[i].d;
                    best_d = &players[i];
                }
            }
        }

        static float best_x = 0;
        static float best_y = 0;

        if (0){
            if (sfKeyboard_isKeyPressed(sfKeyUp)){
                best_y -= 10;
            }
            if (sfKeyboard_isKeyPressed(sfKeyDown)){
                best_y += 10;
            }
            if (sfKeyboard_isKeyPressed(sfKeyLeft)){
                best_x -= 10;
            }
            if (sfKeyboard_isKeyPressed(sfKeyRight)){
                best_x += 10;
            }
        } else {
            best_x = best_x*0.97 + (best_d->x - DRAW_WINDOW_SIZE_X/2)*0.03;
            best_y = best_y*0.97 + (best_d->y - DRAW_WINDOW_SIZE_Y/2)*0.03;
        }
        if (!(frame % skip)){
            sfVector2f off[] = {best_x, best_y};

            rc_draw_road(fb , road, off, sfBlue);
            sfVector2f p1[] = {0, 0};
            gen_tree_draw(root, -1, -1, fb, p1);
            fb = draw();
            memset(fb->pixels, 0, fb->height * fb->width * 4);
        }

        float drift = 0.9   ;
        float drift_2 = 10;
        for (int i = 0; i < nb_players; i++){
            rc_player *p = &players[i];

            //player sime

            p->x += p->vx;
            p->y += p->vy;
            p->vx *= drift;
            p->vy *= drift;
            p->a += p->va;
            p->va *= 0.9;
            p->life--;
            //p->d += 0.000001;

            //draw player
            if (!(frame % skip)){
                p->x -= best_x;
                p->y -= best_y;
                float size = 10;
                sfVector2f p1[] = {cos(p->a - 135/180.0*PI)*size/2 + p->x, sin(p->a - 135/180.0*PI)*size/2 + p->y};
                sfVector2f p2[] = {cos(p->a)*size/2 + p->x, sin(p->a)*size/2 + p->y};
                sfVector2f p3[] = {cos(p->a + 135/180.0*PI)*size/2 + p->x, sin(p->a + 135/180.0*PI)*size/2 + p->y};
                sfVector2f p4[] = {cos(p->a - 135/180.0*PI)*size + p->x, sin(p->a - 135/180.0*PI)*size + p->y};
                sfVector2f p5[] = {cos(p->a)*size + p->x, sin(p->a)*size + p->y};
                sfVector2f p6[] = {cos(p->a + 135/180.0*PI)*size + p->x, sin(p->a + 135/180.0*PI)*size + p->y};
                sfVector2f *lines[7] = {p1, p2, p3, p4, p5, p6, p1};
                p->x += best_x;
                p->y += best_y;
                //printf("%i\n", lines[0]->x);
                float ratio = max(p->score*1.0/max, 0.2);
                ratio = 1;
                sfColor color[] = {ratio*p->r, ratio*p->g, ratio*p->b, 255};
                my_draw_lines2(fb, lines, 7, *color);
            }

            //player play
            float *action = learning_run(p->AI, raycasts(game, p, fb, best, frame, skip, best_d, best_x, best_y));

            for (int i = 0; i < mem; i++)
                if (action[i] == (1.0/0) || action[i] == -(1.0/0))
                    goto dead;



            memcpy(&p->mem, action, sizeof(float)*mem);

            if (1){
                p->forward_power = 2/drift_2;

                float move = (action[0] - action[1])/drift_2;

                if (fabsf(move) > p->forward_power)
                    if (move > 0)
                        move = p->forward_power;
                    else
                        move = p->forward_power;

                if (move < 0)
                    move /= 2;
                p->forward_power -= fabsf(move);
                p->vx += cos(p->a)*move*10;
                p->vy += sin(p->a)*move*10;


                p->turning_power = 0.05;
                //p->turning_power = 10 + p->turning_power * 0.9;

                float turn = action[2] - action[3];
                if (fabsf(turn) > p->turning_power)
                    if (turn > 0)
                        turn = p->turning_power;
                    else
                        turn = -p->turning_power;

                p->turning_power -= fabsf(turn);
                p->va += turn;

            } else {
                if (sfKeyboard_isKeyPressed(sfKeyUp)){
                    p->vx += cos(p->a)*1;
                    p->vy += sin(p->a)*1;
                }
                if (sfKeyboard_isKeyPressed(sfKeyDown)){
                    p->vx -= cos(p->a)*1;
                    p->vy -= sin(p->a)*1;
                }
                if (sfKeyboard_isKeyPressed(sfKeyLeft)){
                    p->a -= 0.1;
                }
                if (sfKeyboard_isKeyPressed(sfKeyRight)){
                    p->a += 0.1;
                }
            }

            //player score
            //score and advancement
            lld_t *mv_L = p->mv_L;
            for (int i = 0; i < 0 && mv_L->prev; mv_L = mv_L->prev, i++);
            lld_t *mv_R = p->mv_R;
            for (int i = 0; i < 0 && mv_R->prev; mv_R = mv_R->prev, i++);
            for (int i = 0; i <= 4 && mv_L->next; i++, mv_R = mv_R->next, mv_L = mv_L->next){
                ray_segment seg1[] = {p->x, p->y, p->vx, p->vy};
                ray_segment *sega = (ray_segment *)mv_L->data;
                ray_segment *segb = (ray_segment *)mv_R->data;
                ray_segment seg2[] = {sega->x, sega->y, segb->x - sega->x, segb->y - sega->y};

                float t1 = ray_calc_t(seg1, seg2);
                float t2 = ray_calc_t(seg2, seg1);
                if (best_d == p && !(frame % skip)){
                    seg2->x -= best_x;
                    seg2->y -= best_y;
                    ray_draw_seg2(fb, seg2, sfRed);
                }

                if (t1 >= 0 && t1 <= 1 && t2 >= 0 && t2 <= 1){
                    if (i < 0){
                        for (; i < 0 && p->mv_L->prev; i++){
                            p->mv_L = p->mv_L->prev;
                            p->mv_R = p->mv_R->prev;
                            p->score -= 2;
                            p->d -= 1;
                            p->life -= 60;
                        }
                    } else {
                        for (; i >= 0 && p->mv_L->next; i--){
                            p->mv_L = p->mv_L->next;
                            p->mv_R = p->mv_R->next;
                            p->d++;
                            p->score += 1;
                            p->life = 30;
                            break;
                        }
                    }
                }

            }
            //player dead?
            if (p->life < 0){
                goto dead;
            }
            ray_segment seg1[] = {p->x, p->y, p->vx, p->vy};

            mv_L = p->mv_L;
            for (int i = 0; i < 2 && mv_L->prev; mv_L = mv_L->prev, i++);
            mv_R = p->mv_R;
            for (int i = 0; i < 2 && mv_R->prev; mv_R = mv_R->prev, i++);
            for (int i = -2; i < 2 && mv_L->next; i++, mv_R = mv_R->next, mv_L = mv_L->next){
                float t1 = ray_calc_t(seg1, mv_L->data);
                float t2 = ray_calc_t(mv_L->data, seg1);
                if (t2 >= 0 && t2 <= 1 && t1 >= 0 && t1 <= 1){
                    goto dead;
                }

                t1 = ray_calc_t(seg1, mv_R->data);
                t2 = ray_calc_t(mv_R->data, seg1);
                if (t2 >= 0 && t2 <= 1 && t1 >= 0 && t1 <= 1){
                    goto dead;
                }
            }


            if (0){
                dead:;
                //add to the best
                float factor = (rand()%100000)/10000.0;///max(best->mem[4], 1.0);
                best->score *= 0.999;

                sfVector2f p1[] = {best->x - best_x, best->y - best_y};
                my_draw_circle2(fb, *p1, 5, sfWhite);


                learning_copy(p->AI, best->AI);
                learning_evolve(p->AI, factor, factor/2);
                p->x = p_spawn->x;
                p->y = p_spawn->y;
                p->vx = 0;
                p->vy = 0;
                p->va = 0;
                p->a = p_spawn->a;
                p->score = 1;
                p->life = 120;
                p->d = 0;
                p->turning_power = 0;
                p->forward_power = 0;
                p->mv_L = road->road_L->next;
                p->mv_R = road->road_R->next;
                p->r = (unsigned char)min(max(best->r + ((rand() % 10000)-5000)/200.0, 0), 255);
                p->g = (unsigned char)min(max(best->g + ((rand() % 10000)-5000)/200.0, 0), 255);
                p->b = (unsigned char)min(max(best->b + ((rand() % 10000)-5000)/200.0, 0), 255);
                memset(p->mem, 0, sizeof(float)*32);

                //create new tree_node
                lld_insert(tree_compil, 0, p->gen_tree);
                p->gen_tree = malloc(sizeof(gen_tree));
                p->gen_tree->child = (gen_tree **)lld_init();
                p->gen_tree->r = p->r;
                p->gen_tree->g = p->g;
                p->gen_tree->b = p->b;
                p->gen_tree->c = 0;
                lld_insert((lld_t *)gen_tree_b->child, lld_len((lld_t *)best->gen_tree->child), p->gen_tree);
            }
        }
        if (!(frame % 60)){
            printf("ADVANCE\n");
            road->road->next = road->road->next->next;
            road->road_L->next = road->road_L->next->next;
            road->road_R->next = road->road_R->next->next;
            rc_node *n = road->road->next->data;

            p_spawn->x = n->x + cos(n->a)*10;
            p_spawn->y = n->y + sin(n->a)*10;
            p_spawn->a = n->a;
            printf("%f %f %f\n", n->x, n->y, n->a);
        }

        //compile tree_nodes
        while (tree_compil->data){
            gen_tree *tmp = lld_pop(tree_compil, 0);
            gen_tree **tab = (gen_tree **)lld_lld_to_tab((lld_t *)tmp->child);
            lld_free((lld_t *)tmp->child);
            tmp->child = tab;
            tmp->c = 1;
        }
        if (!(frame % skip))
            printf("score %f dis %f mut %f\n", best->score, best_d->d, best->mem[4]);
    }


    sleep(500);
    return (0);
}
