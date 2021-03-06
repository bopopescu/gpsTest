#ifndef MANAGER_H
#define MANAGER_H

typedef enum
{
    NEXT_STOP_ANNOUNCE = 1,
    PREV_STOP_ANNOUNCE,
    INTERNET_CONNECT_HAND_SHAKE,
    END_MAIN_COMMAND
}mainCommand_t;

typedef enum
{
    PORT_ANNOUNCE = 9900,
}commandDispatchPort_t;

void disPatchCommand(int command, int port);

#endif