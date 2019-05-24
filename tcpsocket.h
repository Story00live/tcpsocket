#ifndef __TCP_SOCKET_H
#define   __TCP_SOCKET_H

#include <stdio.h>

/* 普通交通流量采集Socket */
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

/* 扩展交通流量采集SocketExtend */
#define	SOCKET_EXTEND_STARTX				0x88
#define	SOCKET_EXTEND_EXITX					0x99

#define	SOCKET_EXTEND_SERVERTYPE_RTCCHECKNONE	0x01
#define	SOCKET_EXTEND_SERVERTYPE_RTCCHECKINIT	0x04

#define	SOCKET_EXTEND_SERVERTYPE_CLIENT		0xC0
#define	SOCKET_EXTEND_SERVERTYPE_CROSS		0x40
#define	SOCKET_EXTEND_SERVERTYPE_SIGNAL		0x80
#define	SOCKET_EXTEND_SERVERTYPE_SERVER		0x00

typedef struct __attribute__((packed)) Socket_Extend_Packet_Head
{
     unsigned char            StartX;
     unsigned char            ServerType;
     unsigned char            CrossID[8];
     unsigned short           DataLength;
     unsigned short           Channel;
     unsigned char            ExitX;
     unsigned char            CheckCode;
     unsigned char            DataCount;
}Socket_Extend_Packet_Head;

typedef struct __attribute__((packed)) Socket_Extend_Packet_Data
{
     unsigned int             RecordNo;
     unsigned int             SendTime;
     
     unsigned short           VehicleCount;
     unsigned short           LargerVehicleCount;
     unsigned short           MidVehicleCount;
     unsigned short           SmallVehilceCount;
     unsigned short           MiniVehicleCount;
     unsigned short           Moto;
     
     unsigned short           ReserverType1;
     unsigned short           ReserverType2;
     unsigned short           ReserverType3;
     unsigned short           ReserverType4;
     unsigned short           ReserverType5;
     unsigned short           ReserverType6;
     
     unsigned int             MaxHeadInterval;
     unsigned int             MinHeadInterval;
     unsigned int             AddUpHeadInterval;
     
     float                    MaxSpeed;
     float                    MinSpeed;
     float                    AddUpSpeed;
     
     unsigned int             MaxOccupancy;
     unsigned int             MinOccupancy;
     unsigned int             AddUpOccupancy;
     
     unsigned int             MaxInterval;
     unsigned int             MinInterval;
     unsigned int             AddUpInterval;
     
     float                    MaxVehicleLength;
     float                    MinVehicleLength;
     float                    AddUpVehilcleLength;
     
     unsigned short           MaxQueueLength;
     unsigned short           MinQueueLength;
     unsigned int             AddUpQueueLength;
     
     unsigned short           AddUpQueueTime;
     unsigned short           RedLightCount;
     unsigned short           PrecedingWayFullTime;
     
     unsigned char            DirverWayCode[3];
     
     unsigned char            Status;
     unsigned char            Reserver1;
     unsigned char            Reserver2;
}Socket_Extend_Packet_Data;























#endif
