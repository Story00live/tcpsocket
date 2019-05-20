/**
  *********************************************************************************************************
  * @file    tcpsocket.c
  * @author  Movebroad -- KK
  * @version V0.0.1
  * @date    2019-05-17
  * @brief   1TAB = 5Speace
  *********************************************************************************************************
  * @attention
  *
  *
  *
  *********************************************************************************************************
  */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <winsock2.h>
#include <time.h>
#include "tcpsocket.h"

#pragma comment(lib,"ws2_32.lib")

/**********************************************************************************************************
 @Function			int main(int argc, char const *argv[])
 @Description			Main
 @Input				void
 @Return				int
**********************************************************************************************************/
int main(int argc, char const *argv[])
{
     WORD sockVersion = MAKEWORD(2, 2);
     WSADATA wsaData;
     time_t t;
     int listernport = 0;
     
     while (false) {
          printf("DEBUG:\r\n");
          printf("Socket_Packet_Head: %d\r\n", sizeof(Socket_Packet_Head));
          printf("Socket_Packet_Data: %d\r\n", sizeof(Socket_Packet_Data));
          printf("float: %d\r\n", sizeof(float));
          printf("argc:%d\r\n", argc);
          return 0;
     }
     
     if (argc < 2) {
          printf("Please Enter listen port. Example: 4001\r\n");
          scanf("%d", &listernport);
          printf("Listen port: %d\r\n", listernport);
          
     }
     else {
          sscanf(argv[1], "%d", &listernport);
          printf("Listen port: %d\r\n", listernport);
     }
     
     /* Init WSA */
     if (WSAStartup(sockVersion, &wsaData) != 0) {
          printf("WSA Init Error!\r\n");
          return 0;
     }
     
     /* Creat Socket */
     SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
     if (slisten == INVALID_SOCKET) {
          printf("Creat Socket Error!\r\n");
          return 0;
     }
     
     struct sockaddr_in sin;
     sin.sin_family = AF_INET;
     sin.sin_port = htons(listernport);
     sin.sin_addr.S_un.S_addr = INADDR_ANY;
     if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR) {
          printf("Bind Error!\r\n");
          return 0;
     }
     
     if (listen(slisten, 5) == SOCKET_ERROR) {
          printf("Listen Error!\r\n");
          return 0;
     }
     
     SOCKET sClient;
     struct sockaddr_in remoteAddr;
     int nAddrlen = sizeof(remoteAddr);
     unsigned char recvData[1024 * 4];
     
     while (true) {
          /* Connect socket */
          printf("Wait connect socket...\r\n");
          sClient = accept(slisten, (SOCKADDR*)&remoteAddr, &nAddrlen);
          if (sClient == INVALID_SOCKET) {
               printf("accept error !\r\n");
               continue;
          }
          printf("Connect socket: %s \r\n", inet_ntoa(remoteAddr.sin_addr));
          
          while (true) {
               /* Recvive Data */
               memset((void*)recvData, 0x00, sizeof(recvData));
               int ret = recv(sClient, recvData, 1024 * 4, 0);
               if (ret > 0) {
                    
                    #if 0
                    time(&t);
                    printf("%s\r\n", ctime(&t));
                    printf("Recvlen : %d \r\n", ret);
                    for (int i = 0; i < ret; i++) {
                         printf("%02X ", recvData[i]);
                    }
                    printf("\r\n");
                    #endif
                    
                    /* 普通交通流量采集Socket */
                    if (ret >= sizeof(Socket_Packet_Head)) {
                         Socket_Packet_Head* pHead = (Socket_Packet_Head*)&recvData;
                         if ((pHead->PacketType == PACKETTYPE_FLOWMESSAGE) || (pHead->PacketType == PACKETTYPE_ERRORMESSAGE) || (pHead->PacketType == PACKETTYPE_BUTTONEVENT) || \
                             (pHead->PacketType == PACKETTYPE_RTCCHECK) || (pHead->PacketType == PACKETTYPE_PARKINGLOTDATA) || (pHead->PacketType == PACKETTYPE_RTCCHECKINIT)) {
                              
                              unsigned char recvDataAll[1024 * 4];
                              int  recvDataAllLen = 0;
                              
                              time(&t);
                              printf("%s", ctime(&t));
                              printf("PacketType: %d\r\n", pHead->PacketType);
                              printf("PacketInfo: %d\r\n", pHead->PacketInfo);
                              
                              int mustRecvlen = pHead->PacketInfo * sizeof(Socket_Packet_Data) + sizeof(Socket_Packet_Head) + sizeof(Socket_Packet_Last);
                              int surplusRecvlen = mustRecvlen - ret;
                              
                              printf("mustRecvlen: %d\r\n", mustRecvlen);
                              printf("surplusRecvlen: %d\r\n", surplusRecvlen);
                              
                              memset((void*)recvDataAll, 0x00, sizeof(recvDataAll));
                              memcpy((void*)recvDataAll, recvData, ret);
                              recvDataAllLen += ret;
                              
                              while (surplusRecvlen > 0) {
                                   memset((void*)recvData, 0x00, sizeof(recvData));
                                   ret = recv(sClient, recvData, 1024 * 4, 0);
                                   if (ret > 0) {
                                        memcpy((void*)recvDataAll + recvDataAllLen, recvData, ret);
                                        recvDataAllLen += ret;
                                        surplusRecvlen -= ret;
                                        printf("surplusRecvlen: %d\r\n", surplusRecvlen);
                                   }
                              }
                              
                              Socket_Packet_Last* pLast = (Socket_Packet_Last*)&recvDataAll[(mustRecvlen - sizeof(Socket_Packet_Last))];
                              printf("ManuCode: %d\r\n", pLast->ManuCode);
                              printf("GetCheckCode: %d\r\n", pLast->CheckCode);
                              
                              unsigned char tcheckcode = 0;
                              unsigned int  tdataindex = sizeof(Socket_Packet_Head);
                              
                              tcheckcode = recvDataAll[tdataindex];
                              tdataindex += 1;
                              for (int i = tdataindex; i < recvDataAllLen - sizeof(Socket_Packet_Head) - sizeof(Socket_Packet_Last); i++) {
                                   tcheckcode = tcheckcode ^ recvDataAll[i];
                              }
                              
                              printf("MathCheckCode: %d\r\n", tcheckcode);
                              if (tcheckcode = pLast->CheckCode) {
                                   printf("Check success!\r\n");
                              }
                              else {
                                   printf("Check fail!\r\n");
                              }
                              
                              printf("Send Ack Data!!\r\n");
                              unsigned char sendDataAll[100];
                              int  snedDataAllLen = 0;
                              memset((void*)sendDataAll, 0x00, sizeof(sendDataAll));
                              Socket_Packet_Head* pSendHead = (Socket_Packet_Head*)&sendDataAll;
                              pHead = (Socket_Packet_Head*)&recvDataAll;
                              if ((pHead->PacketType == PACKETTYPE_RTCCHECKINIT) || (pHead->PacketType == PACKETTYPE_RTCCHECK)) {
                                   /* 对时 */
                                   memcpy(pSendHead->CrossID, pHead->CrossID, sizeof(pSendHead->CrossID));
                                   pSendHead->PacketType = pHead->PacketType;
                                   pSendHead->PacketInfo = 0x02;
                                   snedDataAllLen += sizeof(Socket_Packet_Head);
                                   
                                   t = time(NULL);
                                   unsigned int unixtime = time(&t);
                                   printf("Unix : %d\r\n", unixtime);
                                   sendDataAll[snedDataAllLen + 0] = (unixtime & 0xFF000000) >> 24;
                                   sendDataAll[snedDataAllLen + 1] = (unixtime & 0x00FF0000) >> 16;
                                   sendDataAll[snedDataAllLen + 2] = (unixtime & 0x0000FF00) >>  8;
                                   sendDataAll[snedDataAllLen + 3] = (unixtime & 0x000000FF) >>  0;
                                   snedDataAllLen += 4;
                                   
                                   printf("Send : ");
                                   for (int i = 0; i < snedDataAllLen; i++) {
                                        printf("%02X ", sendDataAll[i]);
                                   }
                                   printf("\r\n");
                              }
                              else {
                                   /* 不对时 */
                                   memcpy(pSendHead->CrossID, pHead->CrossID, sizeof(pSendHead->CrossID));
                                   pSendHead->PacketType = pHead->PacketType;
                                   pSendHead->PacketInfo = 0x02;
                                   snedDataAllLen += sizeof(Socket_Packet_Head);
                                   #if 0
                                   t = time(NULL);
                                   unsigned int unixtime = time(&t);
                                   printf("Unix : %d\r\n", unixtime);
                                   sendDataAll[snedDataAllLen + 0] = (unixtime & 0xFF000000) >> 24;
                                   sendDataAll[snedDataAllLen + 1] = (unixtime & 0x00FF0000) >> 16;
                                   sendDataAll[snedDataAllLen + 2] = (unixtime & 0x0000FF00) >>  8;
                                   sendDataAll[snedDataAllLen + 3] = (unixtime & 0x000000FF) >>  0;
                                   snedDataAllLen += 4;
                                   #endif
                                   printf("Send : ");
                                   for (int i = 0; i < snedDataAllLen; i++) {
                                        printf("%02X ", sendDataAll[i]);
                                   }
                                   printf("\r\n");
                              }
                              
                              send(sClient, sendDataAll, snedDataAllLen, 0);
                              
                              Socket_Packet_Data* pData;
                              pHead = (Socket_Packet_Head*)&recvDataAll;
                              pData = (Socket_Packet_Data*)&recvDataAll[sizeof(Socket_Packet_Head)];
                              pLast = (Socket_Packet_Last*)&recvDataAll[(mustRecvlen - sizeof(Socket_Packet_Last))];
                              
                              printf("\r\nCrossID:%c%c%c%c%c%c%c%c   PacketType:%d   PacketInfo:%d   ManufacturerCode:%d   CheckCode:%d\r\n", \
                              pHead->CrossID[0], pHead->CrossID[1], pHead->CrossID[2], pHead->CrossID[3], pHead->CrossID[4], pHead->CrossID[5], pHead->CrossID[6], pHead->CrossID[7], \
                              pHead->PacketType, pHead->PacketInfo, pLast->ManuCode, pLast->CheckCode);
                              
                              for (int i = 0; i < pHead->PacketInfo; i++) {
                                   printf("DeviceType:%c   ", pData->DeviceType);
                                   printf("Interval:%d   ",   pData->Interval);
                                   printf("LaneNo:%c%c%c   ", pData->LaneNo[0], pData->LaneNo[1], pData->LaneNo[2]);
                                   printf("DateTime:");
                                   for (int tt = 0; tt < sizeof(pData->DateTime); tt++) {
                                        printf("%c", pData->DateTime[tt]);
                                   }
                                   printf("   ");
                                   printf("Volume:%d   ",  pData->Volume);
                                   printf("Volume1:%d   ", pData->Volume1);
                                   printf("Volume2:%d   ", pData->Volume2);
                                   printf("Volume3:%d   ", pData->Volume3);
                                   printf("Volume4:%d   ", pData->Volume4);
                                   printf("Volume5:%d   ", pData->Volume5);
                                   printf("AvgOccupancy:%d   ", pData->AvgOccupancy);
                                   printf("AvgHeadTime:%d   ", pData->AvgHeadTime);
                                   printf("AvgLength:%f   ", pData->AvgLength);
                                   printf("AvgSpeed:%f   ", pData->AvgSpeed);
                                   printf("Saturation:%d   ", pData->Saturation);
                                   printf("Density:%d   ", pData->Density);
                                   printf("Pcu:%d   ", pData->Pcu);
                                   printf("AvgQueueLength:%f   ", pData->AvgQueueLength);
                                   printf("\r\n");
                                   pData = (Socket_Packet_Data*)&recvDataAll[sizeof(Socket_Packet_Head) + (i + 1) * sizeof(Socket_Packet_Data)];
                              }
                              
                              int newline = 0;
                              printf("RawData:\r\n");
                              for (int i = 0; i < recvDataAllLen; i++) {
                                   printf("%02X ", recvDataAll[i]);
                                   newline++;
                                   if (newline % 40 == 0) {
                                        printf("\r\n");
                                   }
                              }
                              printf("\r\n");
                              
                              printf("\r\n");
                         }
                    }
                    
                    /* 扩展交通流量采集SocketExtend */
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
               }
          }
     }
     
     closesocket(slisten);
     WSACleanup();
     
     return 0;
}





/********************************************** END OF FLEE **********************************************/
