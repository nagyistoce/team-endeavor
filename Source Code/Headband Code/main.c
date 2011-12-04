#include "main.h"

void main()
{
   Packet CAN_packet;
   float ratio=0;
   int16 current_health = 1, max_health = 1;
   
   initialize();
    
   while(1)
   {  
      if(IR_data_ready) // if an IR packet has been read
      {       
         //output_toggle(YELLOW);
         //if(Integrity_Check(&IR_buffer[0])&&((IR_buffer[0]&IR_buffer[1]&IR_buffer[2]&IR_buffer[3]&IR_buffer[4]&IR_buffer[5]&IR_buffer[6]&IR_buffer[7])!=0xFF)) // if the IR packet was valid
         //{
            can_putd(0x08, &IR_buffer[0], 8,1,0,0); // put the packet on CANbus (using id 8 to signify ir packet, though this does not matter)
         //}
            
         IR_data_ready = 0; //clear ir ready flag
      }
      
      
      if (can_waiting)   //if a can packet with id 7 (signifying a char status packet) has been received
      {         
         CAN_packet = Incoming_Packet_Parser(&CAN_buffer[0],8);
         can_waiting = 0; //clear received flag immediately so another packet can be recieved
         
         if(Integrity_Check(&CAN_buffer[0])||1) // if the packet is valid (currently always accepted as invalid can packets are unlikely)
         {
            CAN_data_ready = 1; // set flag signifying valid packet
         }     
      
         if(CAN_data_ready) //if valid packet received
         {
            // if a packet containing current char health or max char health is received,
            // update lights according to the calculated ratio.
            
            if(CAN_packet.Process == 5)
            {
               current_health = CAN_packet.ReturnValue;
               ratio = (float)current_health/(float)max_health;
               update_lights(ratio);
            }
            
            if(CAN_packet.Process == 8)
            {
               max_health = CAN_packet.ReturnValue;
               ratio = (float)current_health/(float)max_health;
               update_lights(ratio);
            }
               
            CAN_data_ready = 0; //clear packet flag
         }         
      }
          
   } // End WHILE loop
 } //End main()



#int_canrx0
void canrx0_int ( ) 
{
   can_getd(rx_id, &CAN_buffer[0], rx_len, rxstat); //store any received packet
   //output_toggle(YELLOW);
   if (rx_id == 0x07) // if the packet is id 7
      can_waiting = 1; // set flag showing a can packet is ready to be processed
}

void update_lights(float rat)
{
   //update light sequence based on rat argument using change_lights();
   
   fprintf(USB, "\r\nratio: %f", rat);
   if (rat <= -0.1)
   {
      change_lights(0b00001000);
   }
   
   else if (rat <= 0.0)
   {
      change_lights(0b00001001);
   }
   
   else if (rat <= 0.1)
   {
      change_lights(0b00011000);
   } 
   
   else if (rat <= 0.25)
   {
      change_lights(0b00010010);
   } 
   
   else if (rat <= 0.5)
   {
      change_lights(0b00010000);
   } 
   
   else if (rat <= 0.75)
   {
      change_lights(0b00110000);
   } 
   
   else if (rat < 1.0)
   {
      change_lights(0b00100100);
   } 
   
   else if (rat >= 1.0)
   {
      change_lights(0b00100000);
   } 
}

void change_lights(int16 process)
{
   // change the lights based on process argument
   // follows the key shown below:
   // 0b X X G Y R Gf Yf Rf
   // where X is don't care, G is green on/off, Y is yellow on/off, Gf is Green flashing enabled/disabled, etc...
   
   output_low(GREEN);
   output_low(YELLOW);
   output_low(RED);
   green_countdown = 0;
   yellow_countdown = 0;
   red_countdown = 0;
   
      
   if ((process&0b00100000) > 0)
      output_high(GREEN);
      
   if ((process&0b00010000) > 0)
      output_high(YELLOW);
      
   if ((process&0b00001000) > 0)
      output_high(RED);
      
   if ((process&0b00000100) > 0)
      green_countdown = 1;
      
   if ((process&0b00000010) > 0)
      yellow_countdown = 1;
      
   if ((process&0b00000001) > 0)
      red_countdown = 1;
      
}

#int_TIMER0
void  TIMER0_isr(void) 
{
  // control the flashing of lights if currently enabled
  
  output_toggle(pin_c1);
   if(green_countdown > 0)
   {
      output_toggle(GREEN);
      
      
      if(green_countdown == 0)
      {
         output_high(GREEN);
      }
   }
   
   if(yellow_countdown > 0)
   {
      output_toggle(YELLOW);
      
      
      if(yellow_countdown == 0)
      {
         output_high(YELLOW);
      }
   }
   
   if(red_countdown > 0)
   {
      output_toggle(RED);
      
      
      if(red_countdown == 0)
      {
         output_high(RED);
      }
   }
   
}
