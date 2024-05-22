/**
 * @file Matrix.h
 * @author CATEDS
 * @brief A class designed for the 8*8 LED matrix_8 module
 *        with the connection of SPI and drived by MAX7219
 * @version 0.1
 * @date 2024-05-14
 * 
 * @copyright Copyright (c) 2024 DT5EDU
 * @namespace DT5EDU
 * @class matrix_8
 */

#ifndef _CATEDS_MS_Matrix_8_
#define _CATEDS_MS_Matrix_8_
 
/**
 * @brief It's a macro which decides how would you like to transmit the signal.
 *        If this macro is defined, then the program will use simulated SPI to transmit data.
 *        While if the macro is be commented or undefined, the program will use hardware SPI to do such things.
 *        More info would be in README.md
*/
#define _Matrix_8_Using_Simulated_SPI_

#include "mbed.h"
#include "array"
#include "cstdint"

namespace DT5EDU {

/**
 * @brief A class designed for the 8*8 LED matrix_8 module
 *        with the connection of SPI and drived by MAX7219
 */
class matrix_8 {
private:
    enum Address {
        NoOperation = 0b0000,
        Digit0 = 1, Digit1 = 2, Digit2 = 3, Digit3 = 4,
        Digit4 = 5, Digit5 = 6, Digit6 = 7, Digit7 = 8,
        DecodeMode  = 0b1001,
        Intensity   = 0b1010,
        ScanLimit   = 0b1011,
        Shutdown    = 0b1100,
        DisplayTest = 0b1111
    };

    int SPI_Freq = 1000000;

    #ifdef _Matrix_8_Using_Simulated_SPI_
    DigitalOut mosi,clk;
    #else
    SPI spi;
    #endif
    DigitalOut cs;

    bool inDisplayTest;
    bool inShutdown;
    
    /**
     * @brief   The basic function to transmit the data to the slave device
     *          according to the input variables
     * 
     * @param addr  the address defined in enum Address
     * @param data  the data of the corresponding address
     * @return matrix_8&  the reference of the object itself
     */
    matrix_8 & Transmit(Address addr = NoOperation, uint8_t data = 0x0);
public:
    array<uint8_t,8> buffer;

    #ifdef _Matrix_8_Using_Simulated_SPI_
    /**
     * @brief Construct a new matrix_8 object.
     *        CAUTION: You could choose the pins without SPI function.
     *                 The only need of the pin is supporting DigitalOut.
     * @param Matrix_IO_MOSI    Pin for MOSI port for simulated SPI.
     * @param Matrix_IO_CLK     Pin for CLK port for simulated SPI.
     * @param Matrix_IO_CS      Pin for CS/SS port for simulated SPI.
     */
    matrix_8( PinName Matrix_IO_MOSI, PinName Matrix_IO_CLK,  PinName Matrix_IO_CS ) : 
            mosi(Matrix_IO_MOSI), clk(Matrix_IO_CLK), cs(Matrix_IO_CS) {
        cs = 1;
        clk = 0;
        buffer.fill(0);
        inDisplayTest = false;
        inShutdown = false;
        Init_Setting();
    }
    #else 
    /**
     * @brief Construct a new matrix_8 object
     *        CAUTION: You should choose the pins with hardware SPI function.
     *                 Because you chose the hardware SPI choice.
     * @param Matrix_SPI_MOSI    Pin for MOSI port for SPI of the board.
     * @param Matrix_SPI_CLK     Pin for CLK port for SPI of the board.
     * @param Matrix_IO_CS       Pin for CS/SS port for SPI.
     */
    matrix_8( PinName Matrix_SPI_MOSI, PinName Matrix_SPI_CLK,  PinName Matrix_IO_CS ) : 
            spi(Matrix_SPI_MOSI,NC,Matrix_SPI_CLK), cs(Matrix_IO_CS) {
        cs = 1;
        spi.frequency(SPI_Freq);
        spi.format(16,0);
        buffer.fill(0);
        inDisplayTest = false;
        inShutdown = false;
        Init_Setting();
    }
    #endif
    ~matrix_8() {}
    
    /**
     * @brief Define the alias of std::array<uint8_t,8>
     * 
     */
    using frame = array<uint8_t,8>;

    /**
     * @brief  Display one frame oon the matrix_8. This function would send the buffer automatically.
     * 
     * @param data the frame of the display you want wo display in the type of array<uint8-t,8>
     * @return matrix_8&  The reference of the object itself.
     */
    matrix_8 & operator<<(const frame &data);

    /**
     * @brief Send the data in the array object buffer to the matrix
     * 
     * @return matrix_8&  The reference of the object itself.
     */
    matrix_8 & SendBuffer(void);

    /**
     * @brief Initialized the default settings of the matrix_8;
     * 
     * @return matrix_8&  The reference of the object itself.
     */
    matrix_8 & Init_Setting(void);
    
    /**
     * @brief Set the DisplayTest mode of MAX7219. 
     * 
     * @param mode THe choice whether you want to turn on the DisplayTest mode. 
     *             true or 1: Turn on the DisplayTest mode. 
     *             false or 0: Turn off the DisplayTest mode. 
     * @return matrix_8&  The reference of the object itself.
     */
    matrix_8 & Set_MatrixTest(bool mode);

    /**
     * @brief Check if the matrix_8 is in DisplayTest mode.
     * 
     * @return whether the matrix_8 is in DisplayTest mode.
     * @retval true :   It is in DisplayTest mode.
     * @retval false :  It isn't in DisplayTest mode.
     */
    bool is_In_MatrixTest(void) { return inDisplayTest; }

    /**
     * @brief Set the Shutdown mode of MAX7219. 
     * 
     * @param mode THe choice whether you want to turn on the Shutdown mode. 
     *             true or 1: Turn on the Shutdown mode. 
     *             false or 0: Turn off the Shutdown mode. 
     * @return matrix_8&  The reference of the object itself.
     */
    matrix_8 & Set_Shutdown(bool mode);

    /**
     * @brief Check if the matrix_8 is in Shutdown mode.
     * 
     * @return whether the matrix_8 is in Shutdown mode.
     * @retval true :   It is in Shutdown mode.
     * @retval false :  It isn't in Shutdown mode.
     */
    bool is_In_Shutdown(void) { return inShutdown; }

    /**
     * @brief clear the buffer array inside the object.
     *        This function won't send the buffer automatically.
     * 
     * @return matrix_8&  The reference of the object itself.
     */
    matrix_8 & ClearBuffer(void);

    /**
     * @brief Set the intensity (or called brightness) of the matrix_8.
     * 
     * @param intensity The target brightness of the matrix_8.
     *                  The input value should be in the range of 1.0f and 0.0f
     * @return matrix_8&  The reference of the function itself.
     */
    matrix_8 & Set_Intensity(float intensity = 1.f);

    /**
     * @brief A test function to display a smile on your matrix_8 to test whether
     *         your harware could work. Therefore it's named as HelloWorld.
     * 
     * @return matrix_8&  The reference of the object itself.
     */
    matrix_8 & HelloWorld(void) {
        return *this << frame {
            0,0b100,0b1100010,0b10,
            0b10,0b1100010,0b100,0
        };
    }
};

using matrix = matrix_8;

}

using DT5EDU::matrix;


#endif //_CATEDS_MS_Matrix_8_