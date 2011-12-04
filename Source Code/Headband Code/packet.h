/*
 * packet.h
 *
 *  Created on: Oct 6, 2009
 *      Author: steven
 */

#ifndef PACKET_H_
#define PACKET_H_
//Packet.h
// this header file contains the subroutines to create and handle packets
//    that interact with the HIU(Hardware Interface Unit)
//
// Author: Jonathan Holden
// Date: 10/16/2008
// Compliant with Developer Code Version 0.3
//    do not use older developer code as some functionality has changed
//
//
//
// Incoming_Packet_Parser(Current_Packet[8])
//
// This subroutine will take an incoming packet from the global array (Current_Packet[8])
// and break it into the correct parts and place it in the global variables.
// it will then perform a checksum on the data to insure that the data is not corrupted.
//
//
// PACKET_BUILDER(int TeamID, int16 PlayerID, int SubID, int16 ProcessID, int ReturnID)
//
// This routine takes the values in the global variables and forms them into a packet that is ready for communication
// The packet is stored in Packet_Buffer[8]
//
// Packet_Input()
//
// This routine will query the user for information and place it in the global variables
// The IO stream that you are printing to must be named USB
//
//
// Print_Packet()
//
// This routine will print the current packet that is in Packet_Buffer[8]
// The IO stream that you are printing to must be named USB

#ifdef __GNUC__

typedef int int16;
typedef unsigned char int8;

#endif


#define IRCOMMID        1
#define RFCOMMID        2
#define RFIDCOMMID      3
#define CANCOMMID       4

typedef struct
{
   int8 COM;
   int8 Team;
   int16 Player;
   int8 Subroutine;
   int16 Process;
   int16 ReturnValue;
   int8 Checksum;
}Packet;




//subroutines

//Parses and Incoming Packet and returns the result in a Packet structure
Packet Incoming_Packet_Parser(int8 Current_Packet[], int size);

//Input a Packet and an array of data is returned ready to be transmitted.
void PacketBuilder(Packet CurrentPacket, int8 PacketBuffer[]);

int8 fletcher16( int8 * data, size_t len );

//Packet Packet_Input();


//void Print_Packet(int * buffer); // modify for 8 & 9 byte packets use size of array

//Verifies the checksum of the packet
int Integrity_Check(int8 * data);

#endif /* PACKET_H_ */


