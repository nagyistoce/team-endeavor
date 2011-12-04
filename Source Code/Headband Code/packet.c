#include "packet.h"

Packet Incoming_Packet_Parser(int8 Current_Packet[], int size)
{
   Packet packetinput;

   packetinput.COM = 0;
   packetinput.Team = 0;
   packetinput.Player = 0;
   packetinput.Subroutine = 0;
   packetinput.Process = 0;
   packetinput.ReturnValue = 0;
   packetinput.Checksum = 0;


   if (size==8)
   {
      packetinput.COM = NULL;
      packetinput.Team = (Current_Packet[0]);
      packetinput.Player = Current_Packet[1];
      packetinput.Player = packetinput.Player*16;
      packetinput.Player = packetinput.Player + ((Current_Packet[2]&0b11110000)/16);
      packetinput.Subroutine = (Current_Packet[2]&0b00001111);
      packetinput.Process = (Current_Packet[3]);
      packetinput.Process = packetinput.Process*256 + Current_Packet[4];
      packetinput.ReturnValue = (Current_Packet[5]*256) + (Current_Packet[6]);
      packetinput.Checksum = Current_Packet[7];
   }

   if (size==9)
   {
      packetinput.COM = (Current_Packet[0]);
      packetinput.Team = (Current_Packet[1]);
      packetinput.Player = Current_Packet[2];
      packetinput.Player = packetinput.Player*16;
      packetinput.Player = packetinput.Player + ((Current_Packet[3]&0b11110000)/16);
      packetinput.Subroutine = (Current_Packet[3]&0b00001111);
      packetinput.Process = (Current_Packet[4]);
      packetinput.Process = packetinput.Process*256 + Current_Packet[5];
      packetinput.ReturnValue = (Current_Packet[6]*256) + (Current_Packet[7]);
      packetinput.Checksum = Current_Packet[8];
      
   }
   return packetinput;
}


int Integrity_Check(int8 * data)
{
   int checka = 0,checkb = 0;
   int8 IntegrityCHK = 0;
   
   checka = fletcher16(&data[0], 7);
   if((data[7]!=checka))
      return 0;
      
   else
      return 1;
   
}


void PacketBuilder(Packet CurrentPacket, int8 * PacketBuffer)
{
   int chka,chkb;
   
   if (CurrentPacket.COM != 0)
   {
      
      PacketBuffer[0]=CurrentPacket.COM;
      PacketBuffer[1]=CurrentPacket.Team;
      PacketBuffer[2]=((CurrentPacket.Player&0b0000111111110000)/16);
      PacketBuffer[3]=((CurrentPacket.Player&0b0000000000001111)*16);
      PacketBuffer[3]=PacketBuffer[3] + (CurrentPacket.Subroutine&0b00001111);
      PacketBuffer[4]=((CurrentPacket.Process&0b1111111100000000)/256);
      PacketBuffer[5]=((CurrentPacket.Process&0b0000000011111111));
      PacketBuffer[6]=((CurrentPacket.ReturnValue&0b1111111100000000)/256);
      PacketBuffer[7]=((CurrentPacket.ReturnValue&0b0000000011111111));   
      
      PacketBuffer[8]=fletcher16(&PacketBuffer[1], 7);
   }
   else if (CurrentPacket.COM == 0)
   {
      
      PacketBuffer[0]=CurrentPacket.Team;
      PacketBuffer[1]=((CurrentPacket.Player&0b0000111111110000)/16);
      PacketBuffer[2]=((CurrentPacket.Player&0b0000000000001111)*16);
      PacketBuffer[2]=PacketBuffer[2] + (CurrentPacket.Subroutine&0b00001111);
      PacketBuffer[3]=((CurrentPacket.Process&0b1111111100000000)/256);
      PacketBuffer[4]=((CurrentPacket.Process&0b0000000011111111));
      PacketBuffer[5]=((CurrentPacket.ReturnValue&0b1111111100000000)/256);
      PacketBuffer[6]=((CurrentPacket.ReturnValue&0b0000000011111111));
      
      PacketBuffer[7]=fletcher16(&PacketBuffer[0], 7);
   }

}

int8 fletcher16( int8 * data, size_t len )
{
   size_t tlen;
        int16 sum1 = 0xff, sum2 = 0xff;
        while (len) {
                if(len > 21)
                  tlen = 21;
                else
                  tlen = len;
                len -= tlen;
                do {
                        sum1 += *data++;
                        sum2 += sum1;
                } while (--tlen);
                sum1 = (sum1 & 0xff) + (sum1 >> 8);
                sum2 = (sum2 & 0xff) + (sum2 >> 8);
        }
        /* Second reduction step to reduce sums to 8 bits */
        sum1 = (sum1 & 0xff) + (sum1 >> 8);
        sum2 = (sum2 & 0xff) + (sum2 >> 8);
       // *checkA = (int8)sum1;
       // *checkB = (int8)sum2;
        return (int8)sum2;
}
//Packet Packet_Input()
//{
//   Packet packetinput;
//   char* temp[6];
//
//   fprintf(USB,"\n\r Please enter Team ID\n\r");
//   fgets(temp, USB);
//   if (temp[strlen(temp)-1] == '\n')
//      temp[strlen(temp)-1] = '\0';
//   packetinput.Team = (int8)strtoul(temp,null,0);
//   fprintf(USB,"  %u  ",packetinput.Team);
//
//   fprintf(USB,"\n\r Please enter Player ID\n\r");
//   fgets(temp, USB);
//   if (temp[strlen(temp)-1] == '\n')
//      temp[strlen(temp)-1] = '\0';
//   packetinput.Player = strtoul(temp,null,0);
//   fprintf(USB,"  %lu  ",packetinput.Player);
//
//   fprintf(USB,"\n\r Please enter Subroutine ID\n\r");
//   fgets(temp, USB);
//   if (temp[strlen(temp)-1] == '\n')
//      temp[strlen(temp)-1] = '\0';
//   packetinput.Subroutine = (int8)strtoul(temp,null,0);
//   fprintf(USB,"  %u  ",packetinput.Subroutine);
//
//   fprintf(USB,"\n\r Please enter Process ID\n\r");
//   fgets(temp, USB);
//   if (temp[strlen(temp)-1] == '\n')
//      temp[strlen(temp)-1] = '\0';
//   packetinput.Process = strtoul(temp,null,0);
//   fprintf(USB,"  %lu  ",packetinput.Process);
//
//   fprintf(USB,"\n\r Please enter Return Value\n\r");
//   fgets(temp, USB);
//   if (temp[strlen(temp)-1] == '\n')
//      temp[strlen(temp)-1] = '\0';
//   packetinput.ReturnValue = strtoul(temp,null,0);
//   fprintf(USB,"  %lu  ",packetinput.ReturnValue);
//
//   return packetinput;
//}

//void Print_Packet(int * buffer)
//{
//   fprintf(USB,"\n\r  current packet  \n\r");
//   fprintf(USB," %u  %u  %u  %u  %u  %u  %u  %u \n\r",buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7]);
//
//}




