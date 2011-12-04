00001 /**
00002  * @author Aaron Berk
00003  *
00004  * @section LICENSE
00005  *
00006  * Copyright (c) 2010 ARM Limited
00007  *
00008  * Permission is hereby granted, free of charge, to any person obtaining a copy
00009  * of this software and associated documentation files (the "Software"), to deal
00010  * in the Software without restriction, including without limitation the rights
00011  * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
00012  * copies of the Software, and to permit persons to whom the Software is
00013  * furnished to do so, subject to the following conditions:
00014  *
00015  * The above copyright notice and this permission notice shall be included in
00016  * all copies or substantial portions of the Software.
00017  *
00018  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
00019  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
00020  * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
00021  * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
00022  * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
00023  * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
00024  * THE SOFTWARE.
00025  *
00026  * @section DESCRIPTION
00027  *
00028  * ADXL345, triple axis, digital interface, accelerometer.
00029  *
00030  * Datasheet:
00031  *
00032  * http://www.analog.com/static/imported-files/data_sheets/ADXL345.pdf
00033  */  
00034 
00035 #ifndef ADXL345_H
00036 #define ADXL345_H
00037 
00038 /**
00039  * Includes
00040  */
00041 #include "mbed.h"
00042 
00043 /**
00044  * Defines
00045  */
00046 //Registers.
00047 #define ADXL345_DEVID_REG          0x00
00048 #define ADXL345_THRESH_TAP_REG     0x1D
00049 #define ADXL345_OFSX_REG           0x1E
00050 #define ADXL345_OFSY_REG           0x1F
00051 #define ADXL345_OFSZ_REG           0x20
00052 #define ADXL345_DUR_REG            0x21
00053 #define ADXL345_LATENT_REG         0x22
00054 #define ADXL345_WINDOW_REG         0x23
00055 #define ADXL345_THRESH_ACT_REG     0x24
00056 #define ADXL345_THRESH_INACT_REG   0x25
00057 #define ADXL345_TIME_INACT_REG     0x26
00058 #define ADXL345_ACT_INACT_CTL_REG  0x27
00059 #define ADXL345_THRESH_FF_REG      0x28
00060 #define ADXL345_TIME_FF_REG        0x29
00061 #define ADXL345_TAP_AXES_REG       0x2A
00062 #define ADXL345_ACT_TAP_STATUS_REG 0x2B
00063 #define ADXL345_BW_RATE_REG        0x2C
00064 #define ADXL345_POWER_CTL_REG      0x2D
00065 #define ADXL345_INT_ENABLE_REG     0x2E
00066 #define ADXL345_INT_MAP_REG        0x2F
00067 #define ADXL345_INT_SOURCE_REG     0x30
00068 #define ADXL345_DATA_FORMAT_REG    0x31
00069 #define ADXL345_DATAX0_REG         0x32
00070 #define ADXL345_DATAX1_REG         0x33
00071 #define ADXL345_DATAY0_REG         0x34
00072 #define ADXL345_DATAY1_REG         0x35
00073 #define ADXL345_DATAZ0_REG         0x36
00074 #define ADXL345_DATAZ1_REG         0x37
00075 #define ADXL345_FIFO_CTL           0x38
00076 #define ADXL345_FIFO_STATUS        0x39
00077 
00078 //Data rate codes.
00079 #define ADXL345_3200HZ      0x0F
00080 #define ADXL345_1600HZ      0x0E
00081 #define ADXL345_800HZ       0x0D
00082 #define ADXL345_400HZ       0x0C
00083 #define ADXL345_200HZ       0x0B
00084 #define ADXL345_100HZ       0x0A
00085 #define ADXL345_50HZ        0x09
00086 #define ADXL345_25HZ        0x08
00087 #define ADXL345_12HZ5       0x07
00088 #define ADXL345_6HZ25       0x06
00089 
00090 #define ADXL345_SPI_READ    0x80
00091 #define ADXL345_SPI_WRITE   0x00
00092 #define ADXL345_MULTI_BYTE  0x60
00093 
00094 #define ADXL345_X           0x00
00095 #define ADXL345_Y           0x01
00096 #define ADXL345_Z           0x02
00097 
00098 /**
00099  * ADXL345 triple axis, digital interface, accelerometer.
00100  */
00101 class ADXL345 {
00102 
00103 public:
00104 
00105     /**
00106      * Constructor.
00107      *
00108      * @param mosi mbed pin to use for MOSI line of SPI interface.
00109      * @param miso mbed pin to use for MISO line of SPI interface.
00110      * @param sck mbed pin to use for SCK line of SPI interface.
00111      * @param cs mbed pin to use for not chip select line of SPI interface.
00112      */
00113     ADXL345(PinName mosi, PinName miso, PinName sck, PinName cs);
00114 
00115     /**
00116      * Read the device ID register on the device.
00117      *
00118      * @return The device ID code [0xE5]
00119      */
00120     int getDevId(void);
00121 
00122     /**
00123      * Read the tap threshold on the device.
00124      *
00125      * @return The tap threshold as an 8-bit number with a scale factor of
00126      *         62.5mg/LSB.
00127      */
00128     int getTapThreshold(void);
00129 
00130     /**
00131      * Set the tap threshold.
00132      *
00133      * @param The tap threshold as an 8-bit number with a scale factor of
00134      *        62.5mg/LSB.
00135      */
00136     void setTapThreshold(int threshold);
00137 
00138     /**
00139      * Get the current offset for a particular axis.
00140      *
00141      * @param axis 0x00 -> X-axis
00142      *             0x01 -> Y-axis
00143      *             0x02 -> Z-axis
00144      * @return The current offset as an 8-bit 2's complement number with scale
00145      *         factor 15.6mg/LSB.
00146      */
00147     int getOffset(int axis);
00148 
00149     /**
00150      * Set the offset for a particular axis.
00151      *
00152      * @param axis 0x00 -> X-axis
00153      *             0x01 -> Y-axis
00154      *             0x02 -> Z-axis
00155      * @param offset The offset as an 8-bit 2's complement number with scale
00156      *               factor 15.6mg/LSB.
00157      */
00158     void setOffset(int axis, char offset);
00159 
00160     /**
00161      * Get the tap duration required to trigger an event.
00162      *
00163      * @return The max time that an event must be above the tap threshold to
00164      *         qualify as a tap event, in microseconds.
00165      */
00166     int getTapDuration(void);
00167 
00168     /**
00169      * Set the tap duration required to trigger an event.
00170      *
00171      * @param duration_us The max time that an event must be above the tap
00172      *                    threshold to qualify as a tap event, in microseconds.
00173      *                    Time will be normalized by the scale factor which is
00174      *                    625us/LSB. A value of 0 disables the single/double
00175      *                    tap functions.
00176      */
00177     void setTapDuration(int duration_us);
00178 
00179     /**
00180      * Get the tap latency between the detection of a tap and the time window.
00181      *
00182      * @return The wait time from the detection of a tap event to the start of
00183      *         the time window during which a possible second tap event can be
00184      *         detected in milliseconds.
00185      */
00186     float getTapLatency(void);
00187 
00188     /**
00189      * Set the tap latency between the detection of a tap and the time window.
00190      *
00191      * @param latency_ms The wait time from the detection of a tap event to the
00192      *                   start of the time window during which a possible
00193      *                   second tap event can be detected in milliseconds.
00194      *                   A value of 0 disables the double tap function.
00195      */
00196     void setTapLatency(int latency_ms);
00197 
00198     /**
00199      * Get the time of window between tap latency and a double tap.
00200      *
00201      * @return The amount of time after the expiration of the latency time
00202      *         during which a second valid tap can begin, in milliseconds.
00203      */
00204     float getWindowTime(void);
00205 
00206     /**
00207      * Set the time of the window between tap latency and a double tap.
00208      *
00209      * @param window_ms The amount of time after the expiration of the latency
00210      *                  time during which a second valid tap can begin,
00211      *                  in milliseconds.
00212      */
00213     void setWindowTime(int window_ms);
00214 
00215     /**
00216      * Get the threshold value for detecting activity.
00217      *
00218      * @return The threshold value for detecting activity as an 8-bit number.
00219      *         Scale factor is 62.5mg/LSB.
00220      */
00221     int getActivityThreshold(void);
00222 
00223     /**
00224      * Set the threshold value for detecting activity.
00225      *
00226      * @param threshold The threshold value for detecting activity as an 8-bit
00227      *                  number. Scale factor is 62.5mg/LSB. A value of 0 may
00228      *                  result in undesirable behavior if the activity
00229      *                  interrupt is enabled.
00230      */
00231     void setActivityThreshold(int threshold);
00232 
00233     /**
00234      * Get the threshold value for detecting inactivity.
00235      *
00236      * @return The threshold value for detecting inactivity as an 8-bit number.
00237      *         Scale factor is 62.5mg/LSB.
00238      */
00239     int getInactivityThreshold(void);
00240 
00241     /**
00242      * Set the threshold value for detecting inactivity.
00243      *
00244      * @param threshold The threshold value for detecting inactivity as an
00245      *                  8-bit number. Scale factor is 62.5mg/LSB.
00246      */
00247     void setInactivityThreshold(int threshold);
00248 
00249     /**
00250      * Get the time required for inactivity to be declared.
00251      *
00252      * @return The amount of time that acceleration must be less than the
00253      *         inactivity threshold for inactivity to be declared, in
00254      *         seconds.
00255      */
00256     int getTimeInactivity(void);
00257     
00258     /**
00259      * Set the time required for inactivity to be declared.
00260      *
00261      * @param inactivity The amount of time that acceleration must be less than
00262      *                   the inactivity threshold for inactivity to be
00263      *                   declared, in seconds. A value of 0 results in an
00264      *                   interrupt when the output data is less than the
00265      *                   threshold inactivity.
00266      */
00267     void setTimeInactivity(int timeInactivity);
00268     
00269     /**
00270      * Get the activity/inactivity control settings.
00271      *
00272      *      D7            D6             D5            D4
00273      * +-----------+--------------+--------------+--------------+
00274      * | ACT ac/dc | ACT_X enable | ACT_Y enable | ACT_Z enable |
00275      * +-----------+--------------+--------------+--------------+
00276      *
00277      *        D3             D2               D1              D0
00278      * +-------------+----------------+----------------+----------------+
00279      * | INACT ac/dc | INACT_X enable | INACT_Y enable | INACT_Z enable |
00280      * +-------------+----------------+----------------+----------------+
00281      *
00282      * See datasheet for details.
00283      *
00284      * @return The contents of the ACT_INACT_CTL register.
00285      */
00286     int getActivityInactivityControl(void);
00287     
00288     /**
00289      * Set the activity/inactivity control settings.
00290      *
00291      *      D7            D6             D5            D4
00292      * +-----------+--------------+--------------+--------------+
00293      * | ACT ac/dc | ACT_X enable | ACT_Y enable | ACT_Z enable |
00294      * +-----------+--------------+--------------+--------------+
00295      *
00296      *        D3             D2               D1              D0
00297      * +-------------+----------------+----------------+----------------+
00298      * | INACT ac/dc | INACT_X enable | INACT_Y enable | INACT_Z enable |
00299      * +-------------+----------------+----------------+----------------+
00300      *
00301      * See datasheet for details.
00302      *
00303      * @param settings The control byte to write to the ACT_INACT_CTL register.
00304      */
00305     void setActivityInactivityControl(int settings);
00306     
00307     /**
00308      * Get the threshold for free fall detection.
00309      *
00310      * @return The threshold value for free-fall detection, as an 8-bit number,
00311      *         with scale factor 62.5mg/LSB.
00312      */
00313     int getFreefallThreshold(void);
00314     
00315     /**
00316      * Set the threshold for free fall detection.
00317      *
00318      * @return The threshold value for free-fall detection, as an 8-bit number,
00319      *         with scale factor 62.5mg/LSB. A value of 0 may result in 
00320      *         undesirable behavior if the free-fall interrupt is enabled.
00321      *         Values between 300 mg and 600 mg (0x05 to 0x09) are recommended.
00322      */
00323     void setFreefallThreshold(int threshold);
00324     
00325     /**
00326      * Get the time required to generate a free fall interrupt.
00327      *
00328      * @return The minimum time that the value of all axes must be less than
00329      *         the freefall threshold to generate a free-fall interrupt, in
00330      *         milliseconds.
00331      */
00332     int getFreefallTime(void);
00333     
00334     /**
00335      * Set the time required to generate a free fall interrupt.
00336      *
00337      * @return The minimum time that the value of all axes must be less than
00338      *         the freefall threshold to generate a free-fall interrupt, in
00339      *         milliseconds. A value of 0 may result in undesirable behavior
00340      *         if the free-fall interrupt is enabled. Values between 100 ms 
00341      *         and 350 ms (0x14 to 0x46) are recommended.
00342      */
00343     void setFreefallTime(int freefallTime_ms);
00344     
00345     /**
00346      * Get the axis tap settings.
00347      *
00348      *      D3           D2            D1             D0
00349      * +----------+--------------+--------------+--------------+
00350      * | Suppress | TAP_X enable | TAP_Y enable | TAP_Z enable |
00351      * +----------+--------------+--------------+--------------+
00352      *
00353      * (D7-D4 are 0s).
00354      *
00355      * See datasheet for more details.
00356      *
00357      * @return The contents of the TAP_AXES register.
00358      */ 
00359     int getTapAxisControl(void);
00360     
00361     /**
00362      * Set the axis tap settings.
00363      *
00364      *      D3           D2            D1             D0
00365      * +----------+--------------+--------------+--------------+
00366      * | Suppress | TAP_X enable | TAP_Y enable | TAP_Z enable |
00367      * +----------+--------------+--------------+--------------+
00368      *
00369      * (D7-D4 are 0s).
00370      *
00371      * See datasheet for more details.
00372      *
00373      * @param The control byte to write to the TAP_AXES register.
00374      */
00375     void setTapAxisControl(int settings);
00376     
00377     /**
00378      * Get the source of a tap.
00379      *
00380      * @return The contents of the ACT_TAP_STATUS register.
00381      */
00382     int getTapSource(void);
00383     
00384     /**
00385      * Set the power mode.
00386      *
00387      * @param mode 0 -> Normal operation.
00388      *             1 -> Reduced power operation.
00389      */
00390     void setPowerMode(char mode);
00391     
00392     /**
00393      * Set the data rate.
00394      *
00395      * @param rate The rate code (see #defines or datasheet).
00396      */
00397     void setDataRate(int rate);
00398     
00399     /**
00400      * Get the power control settings.
00401      *
00402      * See datasheet for details.
00403      *
00404      * @return The contents of the POWER_CTL register.
00405      */
00406     int getPowerControl(void);
00407     
00408     /**
00409      * Set the power control settings.
00410      *
00411      * See datasheet for details.
00412      *
00413      * @param The control byte to write to the POWER_CTL register.
00414      */
00415     void setPowerControl(int settings);
00416     
00417     /**
00418      * Get the interrupt enable settings.
00419      *
00420      * @return The contents of the INT_ENABLE register.
00421      */
00422     int getInterruptEnableControl(void);
00423     
00424     /**
00425      * Set the interrupt enable settings.
00426      *
00427      * @param settings The control byte to write to the INT_ENABLE register.
00428      */
00429     void setInterruptEnableControl(int settings);
00430     
00431     /**
00432      * Get the interrupt mapping settings.
00433      *
00434      * @return The contents of the INT_MAP register.
00435      */
00436     int getInterruptMappingControl(void);
00437     
00438     /**
00439      * Set the interrupt mapping settings.
00440      *
00441      * @param settings The control byte to write to the INT_MAP register.
00442      */
00443     void setInterruptMappingControl(int settings);
00444     
00445     /**
00446      * Get the interrupt source.
00447      *
00448      * @return The contents of the INT_SOURCE register.
00449      */
00450     int getInterruptSource(void);
00451     
00452     /**
00453      * Get the data format settings.
00454      *
00455      * @return The contents of the DATA_FORMAT register.
00456      */
00457     int getDataFormatControl(void);
00458     
00459     /**
00460      * Set the data format settings.
00461      *
00462      * @param settings The control byte to write to the DATA_FORMAT register.
00463      */
00464     void setDataFormatControl(int settings);
00465     
00466     /**
00467      * Get the output of all three axes.
00468      *
00469      * @param Pointer to a buffer to hold the accelerometer value for the
00470      *        x-axis, y-axis and z-axis [in that order].
00471      */
00472     void getOutput(int* readings);
00473     
00474     
00475       /**
00476      * Get acceleration of X axis.
00477      *
00478      *@return Accelerometer value for the x-axis.
00479      */
00480      int getAx();
00481      
00482      
00483      /**
00484      * Get acceleration of Y axis.
00485      *
00486      *@return Accelerometer value for the y-axis
00487      */
00488      int getAy();
00489      
00490      
00491       /**
00492      * Get acceleration of Z axis.
00493      *
00494      *@return Accelerometer value for the z-axis
00495      */
00496      int getAz();
00497     
00498     
00499     
00500     
00501     
00502     /**
00503      * Get the FIFO control settings.
00504      *
00505      * @return The contents of the FIFO_CTL register.
00506      */
00507     int getFifoControl(void);
00508     
00509     /**
00510      * Set the FIFO control settings.
00511      *
00512      * @param The control byte to write to the FIFO_CTL register.
00513      */
00514     void setFifoControl(int settings);
00515     
00516     /**
00517      * Get FIFO status.
00518      *
00519      * @return The contents of the FIFO_STATUS register.
00520      */
00521     int getFifoStatus(void);
00522     
00523 private:
00524 
00525     SPI        spi_;
00526     DigitalOut nCS_;
00527 
00528     /**
00529      * Read one byte from a register on the device.
00530      *
00531      * @param address Address of the register to read.
00532      *
00533      * @return The contents of the register address.
00534      */
00535     int oneByteRead(int address);
00536 
00537     /**
00538      * Write one byte to a register on the device.
00539      *
00540      * @param address Address of the register to write to.
00541      * @param data The data to write into the register.
00542      */
00543     void oneByteWrite(int address, char data);
00544 
00545     /**
00546      * Read several consecutive bytes on the device.
00547      *
00548      * @param startAddress The address of the first register to read from.
00549      * @param buffer Pointer to a buffer to store data read from the device.
00550      * @param size The number of bytes to read.
00551      */
00552     void multiByteRead(int startAddress, char* buffer, int size);
00553 
00554     /**
00555      * Write several consecutive bytes on the device.
00556      *
00557      * @param startAddress The address of the first register to write to.
00558      * @param buffer Pointer to a buffer which contains the data to write.
00559      * @param size The number of bytes to write.
00560      */
00561     void multiByteWrite(int startAddress, char* buffer, int size);
00562 
00563 };
00564 
00565 #endif /* ADXL345_H */
