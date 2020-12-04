#include <cstdio>
#include <iostream>
#include <cmath>
#include <conio.h>
#include <ctime>
#include <graphics.h>
#include <dos.h>
using namespace std;

int max_height = 736, max_width = 1300;

void menu();
void frontpage();
void game();
void game();
void choice();
void check();
void high_scores();
void shooting();
void instructions();


int main()
{
    initwindow(max_width, max_height);

    game();

    closegraph();
    return 0;
}

void frontpage()
{
    readimagefile("press_key.jpg", 0, 0, max_width, max_height);
}

void menu()
{
    readimagefile("1.jpg", 0, 0, max_width, max_height);
    while(1)

    {
        int x = mousex();
        int y = mousey();
        int ans = 0;
        if(x < 861 || x > 1128)
            readimagefile("1.jpg", 0, 0, max_width, max_height);

        else if(y >= 115 && y <= 158)
            readimagefile("New_game.jpg", 0, 0,max_width,max_height);
        else if(y >= 205 && y <= 248)
            readimagefile("High_score.jpg", 0, 0,max_width,max_height);
        else if(y >= 300 && y <= 343)
            readimagefile("Instruction.jpg", 0, 0, max_width, max_height);
        else if(y >= 395 && y <= 438)
            readimagefile("Exit.jpg", 0, 0, max_width, max_height);
        else
            readimagefile("1.jpg", 0, 0, max_width, max_height);
    }


}

int check(int x, int y)
{
    int ans = 0;
    if(x < 861 || x > 1128)
        ans = -1;
    else if(y >= 115 && y <= 158)
        ans = 1;
    else if(y >= 205 && y <= 248)
        ans = 2;
    else if(y >= 300 && y <= 343)
        ans = 3;
    else if(y >= 395 && y <= 438)
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

void choice()
{
    int x, y, ans = 0;
    while(!(ans >= 1 && ans <= 4))
    {
        if(!ismouseclick(WM_LBUTTONDOWN))
            continue;

        getmouseclick(WM_LBUTTONDOWN, x, y);
        ans = check(x, y);
        //cout << "ans  = " << ans << " x = " << x << " y = " << y << endl;
    }
    if(ans == 1);
    //    shooting();
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
    frontpage();

    while(!kbhit()); /** waiting for a key to be pressed **/
    cleardevice();
    menu();

    choice();
}
