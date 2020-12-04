#include <cstdio>
#include <iostream>
#include <cmath>
#include <conio.h>
#include <ctime>
#include <graphics.h>
#include <dos.h>
using namespace std;

#define up 72
#define down 80
#define left 75
#define right 77
#define esc 27

#define point_plus 0
#define point_minus 1
#define time_plus 2
#define time_minus 3

int max_height = 768, max_width = 1366;
int face_x = 1366/2, face_y = max_height - 60, face_r = 30, k = 2, tot_powers = 4, face_color = WHITE;
int bul_bor = BLUE, bul_fill = BLUE, bul_r = 5;
int tot_time = 60;
int points;

struct baloon
{
    int cx, cy, cr;
    int power;
    clock_t tm;
    int border_color, fill_color;
} ara[1000];

int tot_baloon;
clock_t user_time;

void menu();
void frontpage();
void game();
void choice();
void check();
void high_scores();
void main_part();
void shoot();
void instructions();
void print_all();
void moving();
void balls();
bool check(int pos);
void time_check();
void set_ball_color(int power, int pos);
void erase_ball(int pos);
void face();
void print_ball(int pos);
int collison_check(int x, int y);
void power_effect(int p);
bool lagekina(int fir, int sec);
void pic(int x, int y);
bool valid_time(clock_t tm);
void print_time();
void show_score(int p);

int main()
{
    initwindow(max_width, max_height);

    game();

    closegraph();
    return 0;
}

void moving()
{
    if(GetAsyncKeyState(VK_RIGHT))
        face_x += k;
    else if(GetAsyncKeyState(VK_LEFT))
        face_x -= k;

    if(face_x < face_r)
        face_x = face_r;
    if(face_x + face_r > max_width)
        face_x = max_width - face_r;
}

void face()
{
    setcolor(face_color);
    circle(face_x, face_y, face_r);
}

bool lagekina(int fir, int sec)
{
    int x = ara[fir].cx - ara[sec].cx;
    int y = ara[fir].cy - ara[sec].cy;
    int z = ara[fir].cr + ara[sec].cr;
    x *= x, y *= y, z *= z;
    return (x + y) < z;
}

bool check(int pos)
{
    for(int i = 0; i < tot_baloon; i++)
    {
        if(lagekina(i, pos))
            return false;
    }
    return true;
}

void erase_ball(int pos)
{
    for(int i = pos; i < tot_baloon - 1; i++)
        ara[i] = ara[i + 1];
    tot_baloon--;
}

void time_check()
{
    for(int i = 0; i < tot_baloon; i++)
    {
        double sec = (clock() - ara[i].tm + 0.0)/CLOCKS_PER_SEC;
        if(sec > 3.00)
            erase_ball(i), i--;
    }
}

void set_ball_color(int power, int pos)
{
    if(power == 0)
    {
        ara[pos].border_color = GREEN;
        ara[pos].fill_color = GREEN;
    }
    else if(power == 1)
    {
        ara[pos].border_color = CYAN;
        ara[pos].fill_color = CYAN;
    }
    else if(power == 2)
    {
        ara[pos].border_color = YELLOW;
        ara[pos].fill_color = YELLOW;
    }
    else if(power == 3)
    {
        ara[pos].border_color = RED;
        ara[pos].fill_color = RED;
    }
}

void print_ball(int pos)
{
    setcolor(ara[pos].border_color);
    circle(ara[pos].cx, ara[pos].cy, ara[pos].cr);
    setfillstyle(SOLID_FILL, ara[pos].fill_color);
    floodfill(ara[pos].cx, ara[pos].cy, ara[pos].border_color);
}

void print_time()
{
    int sec = (clock() - user_time)/CLOCKS_PER_SEC;
    sec = tot_time - sec;
    char str[100];
    int minute = sec/60;
    sec %= 60;
    sprintf(str, "Time remaining  %.2d : %.2d", minute, sec);
    settextstyle(BOLD_FONT, HORIZ_DIR, 4);
    if(minute == 0 && sec <= 20)
        setcolor(RED);
    else
        setcolor(WHITE);
    outtextxy(900, 0, str);
}

void print_all()
{
    cleardevice();
    face();
    char str[100];
    sprintf(str, "points = %d", points);
    outtextxy(0, 0, str);
    print_time();
    for(int i = 0; i < tot_baloon; i++)
        print_ball(i);
}

void balls()
{
    time_check();

    if(rand() % 200 && tot_baloon != 0)
        return;

    ara[tot_baloon].cx = rand() % 1100 + 50;
    ara[tot_baloon].cy = rand() % 500 + 50;
    ara[tot_baloon].cr = rand() % 30 + 10;
    ara[tot_baloon].tm = clock();
    ara[tot_baloon].power = rand() % tot_powers;
    set_ball_color(ara[tot_baloon].power, tot_baloon);
    if(check(tot_baloon))
        tot_baloon++;
}

bool isCollide(int pos, int x1, int y1)
{
    baloon a;
    a = ara[pos];
    int x = abs(a.cx - x1);
    int y = abs(a.cy - y1);
    int z = abs(a.cr + bul_r);
    x *= x;
    y *= y;
    z *= z;
    return (x + y) < z;
}

int collison_check(int x, int y)
{
    for(int i = 0; i < tot_baloon; i++)
    {
        if(isCollide(i, x, y))
        {
            int k = ara[i].power;
            erase_ball(i);
            //cout << "k = " << endl;
            return k;
        }
    }
    return -1;
}

void power_effect(int p)
{
    if(p == 0)
        points += 10;
    else if(p == 1)
        points -= 5;
    else if(p == 2)
        user_time += 2*CLOCKS_PER_SEC;
    else if(p == 3)
        user_time -= 2*CLOCKS_PER_SEC;
}

void guli()
{
    for(int y = face_y - face_r - bul_r, x = face_x; y >= bul_r; y--)
    {
        setcolor(bul_fill);
        circle(x, y, bul_r);
        setfillstyle(SOLID_FILL, bul_fill);
        floodfill(x, y, bul_bor);
        //delay(50);
        int k = collison_check(x, y);
        //cout << x << " " << y << endl;
        //if(k + 1)
        //cout << "k = " << k << endl;
        //for(int i = 0; i < 10000000; i++);
        if(k != -1)
        {
            power_effect(k);
            return ;
        }
    }
}

void shoot()
{
    if(GetAsyncKeyState(VK_UP))
    {
        guli();
    }
    GetAsyncKeyState(VK_UP);
    GetAsyncKeyState(VK_UP);
}

bool valid_time(clock_t tm)
{
    double t = (clock() - tm)/CLOCKS_PER_SEC;
    return t <= tot_time;
}

void show_score(int p)
{
    int page = 1;
    while(1)
    {
        setactivepage(page);
        setvisualpage(page = 1 - page);
        cleardevice();
        settextstyle(BOLD_FONT, HORIZ_DIR, 4);
        char str[100];
        sprintf(str, "Your score is = %d", p);
        outtextxy(500, 100, str);
        getch();
        char c = getch();
        if(c == esc)
            return;
        cout << "c = " << (int) c << endl;
    }
}

void main_part()
{
    //readimagefile("space.jpg", 0, 0, max_width, max_height);
    user_time = clock(), points = 0;
    int page = 0, p = 0;
    while(valid_time(user_time))
    {
        setactivepage(page);
        setvisualpage(page = 1 - page);
        cleardevice();
        print_all();
        moving();
        balls();
        shoot();
        cout << p++ << endl;
        //cout << "time = " << (clock() - user_time)/CLOCKS_PER_SEC << endl;
    }
    show_score(points);
}

void frontpage()
{
    readimagefile("1.jpg", 0, 0, max_width, max_height);
    //cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
}

void menu()
{
    readimagefile("background.jpg", 0, 0, max_width, max_height);

}

int check(int x, int y)
{
    int ans = 0;
    if((x >= 900 && x <= 1188) && (y >= 113 && y <= 163))
        ans = 1;
    else if((x >= 900 && x <= 1177) && (y >= 213 && y <= 269))
        ans = 2;
    else if((x >= 900 && x <= 1183) && (y >= 311 && y <= 357))
        ans = 3;
    else if((x >= 900 && x <= 998) && (y >= 411 && y <= 456))
        ans = 4;
    return ans;
}

void high_scores()
{
    readimagefile("high.jpg", 0, 0, max_width, max_height);
    char str[1000], c, p = 0;
    FILE *in = fopen("high_scores.txt", "r");
    while(fscanf(in, "%c", &c) != EOF)
        str[p++] = c;
    str[p] = '\0';
    outtextxy(500, 500, str);
    getch();
    getch();
}

void instructions()
{

}

void pic(int x, int y)
{
    if((x >= 900 && x <= 1188) && (y >= 113 && y <= 163))
        readimagefile("New_game.jpg", 0, 0, max_width, max_height);
    else if((x >= 900 && x <= 1177) && (y >= 213 && y <= 269))
        readimagefile("High_score.jpg", 0, 0, max_width, max_height);
    else if((x >= 900 && x <= 1183) && (y >= 311 && y <= 357))
        readimagefile("Instruction.jpg", 0, 0, max_width, max_height);
    else if((x >= 900 && x <= 998) && (y >= 411 && y <= 456))
        readimagefile("Exit.jpg", 0, 0, max_width, max_height);
    else
        readimagefile("background.jpg", 0, 0, max_width, max_height);
}

void choice()
{
    int x, y, ans = 0;
    while(!(ans >= 1 && ans <= 4))
    {
        int x = mousex();
        int y = mousey();
        //cout << x << " " << y << endl;
        pic(x, y);

        if(!ismouseclick(WM_LBUTTONDOWN))
            continue;

        getmouseclick(WM_LBUTTONDOWN, x, y);
        ans = check(x, y);
        //cout << "ans  = " << ans << " x = " << x << " y = " << y << endl;
    }
    if(ans == 1)
        main_part(), printf("point = %d", points);
    else if(ans == 2)
        high_scores();
    else if(ans == 3)
        instructions();
    if(ans >= 1 && ans <= 3)
        game();
    exit(0);
}

void game()
{
    cleardevice();
    frontpage();

    while(!kbhit()); /** waiting for a key to be pressed **/

    getch();
    getch();

    menu();

    choice();
}
