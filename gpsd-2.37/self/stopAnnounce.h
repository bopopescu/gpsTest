#ifndef STOP_ANNOUNCE_H
#define STOP_ANNOUNCE_H
#include "dllist.h"
#include "lineMap.h"

#define ARRIVE 1
#define LEAVE   -1



typedef struct
{
    char *mp3Name;
}stopPendAction_t;


typedef struct
{
    U8  lng1;
    U8  lng2;
    U16  lng3;
    U8  lat1;
    U8  lat2;
    U16 lat3;
    U16 speed;//车速
    U16 azimuth;//方位角
    U8  vehicleStatus;//车辆状态
    U8  directMark;//上下行标志
    U8  stopId;
    U8  lineId[3];
    U8  *lineName;
    U8  *driverId;
}arrivedStopReport_t;

typedef struct
{
    U8  lng1;
    U8  lng2;
    U16  lng3;
    U8  lat1;
    U8  lat2;
    U16 lat3;
    U16 speed;//车速
    U16 azimuth;//方位角
    U8  vehicleStatus;//车辆状态
    U8  directMark;//上下行标志
    U8  stopId;
    U8  lineId[3];
    U8  *lineName;
    U8  *driverId;
}leaveStopReport_t;




void* stopAnnounce();
void addActionToActionPend(stopPendAction_t *action);
int getNextStop(int curStopId, int curUpOrDown, int lineId, stopPend_t *nextStop);
int getPrevStop(int curStopId, int curUpOrDown, int lineId, stopPend_t *prevStop);
int getDriveDirect();
int getLastStopId();
busStopMark_t *getNextStopAttr();
spotMark_t *getNextStopSpotAttr();
unsigned short getDistance2NextStop(struct gps_fix_t *current);
int getNextStopId();

#endif
