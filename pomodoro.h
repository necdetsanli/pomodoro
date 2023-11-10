#ifndef POMODORO_H
#define POMODORO_H

#define REST    0
#define STUDY   1

typedef int activity_t;

typedef struct pomtime {
    int hours;
    int minutes;
    int seconds;
    int resthours;
    int restmins;
    int restsecs; 
    int sessions;
}pomtime_t;

typedef struct wsize {
    int maxy;
    int maxx;
}wsize_t;

void init_ncurses();
wsize_t* init_wsize(void);
pomtime_t* init_time(void);
int calc_hours(pomtime_t* ph);
int calc_resthours(pomtime_t *ph);
int get_user_minutes(void);
int get_user_restmins(void);
int get_user_sessions(void);
int is_lazy(pomtime_t* ph);
void print_time(wsize_t* wh, int hours, int minutes, int seconds);
void count_time(pomtime_t* ph, wsize_t* wh, activity_t act);
void run(void);
void give_back(pomtime_t* ph, wsize_t* wh);

#endif
