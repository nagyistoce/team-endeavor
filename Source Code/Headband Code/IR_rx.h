/*
This header enables support for the MAGE IR protocol. This code assumes a PIC18F2480 or 2580
is being used with a 20MHz clock for receiving and a PIC16F688 is being used for transmitting.
Also the IR receiver should be a TSOP36256. Some other receivers
may work, but this one has been found to have good range and do a very good job of blocking
noise.

Author: Bruce Lebold
Date: 07/18/2010

!!! Any hardware changes may affect the performance of the IR protocol.!!!

If IR data becomes inaccurate due to hardware changes, you will most likely need
to adjust timer2. This will affect the timing of the period which is sampled for each bit.

setup_timer_2(T2_DIV_BY_16,141,4); is the original setting using the assumed hardware.

*/



// IR global vars
int1 ir_start = 0;
int8 ir_bit[64];
int8 IR_buffer[8];
int8 ir_bit_count = 0;
int1 IR_data_ready = 0;

void decode_IR()
{
   //decode the 64 values stored into an 8 byte packet
   int8 i = 0;
   int8 h = 0;
   int8 index = 0;
   
   for(i=0;i<64;i++) //for all 64 elements in the IR_bit array
   {
      if(IR_bit[i] > 2) // if that element is > 2, the bit is 0
      {
         IR_bit[i] = 0;
      }
      else //if that element is 2 or less, the bit is a 1
      {  
         IR_bit[i] = 1;
      }  
   }
   
   for(h = 0; h<8; h++) // convert the 64 bits into 8 bytes
   {
       for (i = 0; i < 8; i++)
       {
          IR_buffer[h] = IR_buffer[h]*2 + IR_bit[index+i];
       }
       
       index+=8;
   }
   
   for(i=0;i<64;i++) //clear the bit values
   {
      IR_bit[i] = 0;    
   }
}

#int_EXT
void  EXT_isr(void) 
{
   //IR_data_ready = 1;
   //output_toggle(RED);

   if(ir_start == 0) //if a start bit has not yet been received
   {
      ir_start = 1; //set flag
      delay_us(10); //delay
      set_timer2(0); //get ready to store data using timer2 to time bits
   }
   
   else //if the start bit has already been received
   {
      ir_bit[ir_bit_count]++; //increase the counter for # of interrupts caused by IR during this bit's time period
   }

}

#int_TIMER2
void  TIMER2_isr(void) 
{
   
   if(ir_start == 1) //change ir_bit_count every ? us so ext_isr can know what bit to tally interrupts for
   {
      if(ir_bit_count < 64)
      {
        ir_bit_count++;
      }
      else // if all 64 bits have been tallied, decode the IR packet
      {
         disable_interrupts(INT_TIMER2);
         decode_IR();
         IR_data_ready = 1;
         ir_start = 0;
         ir_bit_count = 0;
         enable_interrupts(INT_TIMER2);
      }
   }
}




