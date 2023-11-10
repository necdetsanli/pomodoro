#include <stdlib.h>
#include <ncurses.h>
#include "pomodoro.h"

void init_ncurses()
{
    initscr();
    cbreak();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_RED);
    bkgd(COLOR_PAIR(1));
    curs_set(0);
}

wsize_t* init_wsize(void)
{
    wsize_t* wsize = (wsize_t*)malloc(sizeof(wsize_t));

    getmaxyx(stdscr, wsize->maxy, wsize->maxx);

    return wsize;
}

pomtime_t* init_time(void)
{
    pomtime_t* ptime = (pomtime_t*)malloc(sizeof(pomtime_t));

    ptime->minutes = get_user_minutes();
    ptime->restmins = get_user_restmins();
    ptime->sessions = get_user_sessions();
    
    ptime->hours = calc_hours(ptime);
    ptime->minutes -= ptime->hours * 60;
    ptime->seconds = 0;

    ptime->resthours = calc_resthours(ptime);
    ptime->restmins -= ptime->resthours * 60;
    ptime->restsecs = 0;

    return ptime;
}

int calc_hours(pomtime_t* ph)
{
    if (ph->minutes >= 60)
        return  ph->minutes / 60;
    
    return ph->minutes >= 60;
}

int calc_resthours(pomtime_t *ph)
{
    if (ph->restmins >= 60)
        return ph->restmins / 60;
    
    return ph->restmins >= 60;
}

int get_user_minutes(void)
{
    int minutes;

    printw("How many minutes are you going to study? : ");
    scanw("%d", &minutes);

    return minutes;
}

int get_user_restmins(void)
{
    int restmins;

    printw("How many minutes are you going to rest? : ");
    scanw("%d", &restmins);

    return restmins;
}

int get_user_sessions(void)
{
    int sessions;

    printw("How many sessions would you like to do? : ");
    scanw("%d", &sessions);

    return sessions;
}

int is_lazy(pomtime_t* ph)
{
    return !ph->minutes && !ph->hours;
}

void print_time(wsize_t* wh, int hours, int minutes, int seconds) 
{
    move(1, 0);
    clrtobot();
    mvprintw(wh->maxy / 2 - 4, wh->maxx / 2 - 10, "********************\n");
    mvprintw(wh->maxy / 2 - 3, wh->maxx / 2 - 5, "  %02d:%02d:%02d\n", hours, minutes, seconds);
    mvprintw(wh->maxy / 2 - 2, wh->maxx / 2 - 10, "********************\n");
    refresh();
}

void count_time(pomtime_t* ph, wsize_t* wh, activity_t act)
{
    int hours = ph->hours;
    int minutes = ph->minutes;
    int seconds = ph->seconds;
    int resthours = ph->resthours;
    int restmins = ph->restmins;
    int restsecs = ph->restsecs;


    if (act) 
       while (1) {
            if (hours > 0 && minutes == 0) {
                --hours;
                minutes = 59;
                seconds = 60;
            }
            else if(minutes > 0) {
                --minutes;
                seconds = 60;
            }
            else 
                break;
            while (seconds--) {
                print_time(wh, hours, minutes, seconds);
                napms(1000);
            }
        }

    else 
       while (1) {
            if(resthours > 0 && restmins == 0) {
                --resthours;
                restmins = 59;
                restsecs = 60;
            }
            else if (restmins > 0) {
                --restmins;
                restsecs = 60;
            }
            else 
                break;
            while (restsecs--) {
                print_time(wh, resthours, restmins, restsecs);
                napms(1000);
            }
        }
}

void run(void)
{
    init_ncurses();

    pomtime_t* phandle = init_time();
    wsize_t* whandle = init_wsize();

    if (is_lazy(phandle)) {
        clear();
        mvprintw(0, 0, "Get your lazy ass up!\n");
        give_back(phandle, whandle);
        exit(EXIT_FAILURE);
    }

    while (phandle->sessions--) {
        mvprintw(0, 0, "Studying...\n");
        mvchgat(0, 0, -1, A_BLINK, 1, NULL);
        count_time(phandle, whandle, STUDY);
        mvprintw(0, 0, "Study ime is up, please press enter to continue!\n");
        beep();
        getch();
        mvprintw(0, 0, "Resting...\n");
        mvchgat(0, 0, -1, A_BLINK, 1, NULL);
        count_time(phandle, whandle, REST);
        mvprintw(0, 0, "Rest time is up, please press enter to continue!\n");
        beep();
        getch();
    }

    printw("You completed all sessions, congrats! Keep up the good work!\n");
    printw("Please press enter to exit the program.\n");

    give_back(phandle, whandle);
}

void give_back(pomtime_t* ph, wsize_t* wh)
{
    free(ph);
    free(wh);

    refresh();
    getch();
    endwin();
}