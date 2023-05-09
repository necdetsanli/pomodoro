#include <ncurses.h>

int study, rest, hour, minute, second, sessions;

int y, x;

int print_time() 
{
    erase();
    mvprintw(y / 2, x / 2 - 17, "********************************\n");
    mvprintw(y / 2 + 1, x / 2 - 5, "  %d:%d:%d\n", hour, minute, second);
    mvprintw(y / 2 + 2, x / 2 - 17, "********************************\n");
    refresh();
}

void count_time(int selection)
{
    hour = minute = second = 0;
    while (minute < selection) {
        if (minute > 59) {
            minute = 0;
            ++hour;
        }
        if (second > 59) {
            second = 0;
            ++minute;
        }
        print_time();
        napms(1000);
        ++second;
    }
}

void set_study(void)
{
    printw("Kac dakika calisacaksin: ");
    scanw("%d", &study);
}

void set_rest(void)
{
    printw("Kac dakika mola vereceksin: ");
    scanw("%d", &rest);
}

void set_sessions(void)
{
    printw("How many sessions would you like to do: ");
    scanw("%d", &sessions);
}

void run(void)
{
    initscr();
    cbreak();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_RED);
    bkgd(COLOR_PAIR(1));

    set_study();
    set_rest();
    set_sessions();
    curs_set(0);
    getmaxyx(stdscr, y, x);

    while (sessions--) {
        count_time(study);
        printw("Study ime is up, please press enter to continue!\n");
        beep();
        getch();
        count_time(rest);
        printw("Rest time is up, please press enter to continue!\n");
        beep();
        getch();
    }

    printw("You completed all sessions, congrats! Keep up the good work!\n");
    printw("Please press enter to exit the program.\n");
    refresh();
    getch();
    endwin();
}

int main(void)
{   
    run();
    return 0;    
}
