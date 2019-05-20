#ifndef __TCP_SOCKET_H
#define   __TCP_SOCKET_H

#include <stdio.h>

#define	PACKETTYPE_FLOWMESSAGE			0x01
#define	PACKETTYPE_ERRORMESSAGE			0x02
#define	PACKETTYPE_BUTTONEVENT			0x03
#define	PACKETTYPE_BUSPRIORITYEVENT		0x04
#define	PACKETTYPE_PARKINGLOTDATA		0x05
#define	PACKETTYPE_RTCCHECKINIT			0x80
#define	PACKETTYPE_RTCCHECK				0x81

typedef struct __attribute__((packed)) Socket_Packet_Head
{
     unsigned char            CrossID[8];
     unsigned char            PacketType;
     unsigned int             PacketInfo;
}Socket_Packet_Head;

typedef struct __attribute__((packed)) Socket_Packet_Data
{
     unsigned char            DeviceType;
     unsigned short           Interval;
     unsigned char            LaneNo[3];
     unsigned char            DateTime[20];
     unsigned short           Volume;
     unsigned short           Volume1;
     unsigned short           Volume2;
     unsigned short           Volume3;
     unsigned short           Volume4;
     unsigned short           Volume5;
     unsigned short           AvgOccupancy;
     unsigned short           AvgHeadTime;
     float                    AvgLength;
     float                    AvgSpeed;
     unsigned char            Saturation;
     unsigned short           Density;
     unsigned short           Pcu;
     float                    AvgQueueLength;
}Socket_Packet_Data;

typedef struct __attribute__((packed)) Socket_Packet_Last
{
     unsigned char            ManuCode;
     unsigned char            CheckCode;
}Socket_Packet_Last;



































#endif
