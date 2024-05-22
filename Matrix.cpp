#include "Matrix.h"

namespace DT5EDU {

matrix_8 & matrix_8::Transmit(Address addr, uint8_t data) {
    uint16_t temp = (addr << 8) | data;
    cs = 0;
#ifdef _Matrix_8_Using_Simulated_SPI_
    uint16_t delay = 1.e9/SPI_Freq/2;
    for (int i=15; i>=0; i--) {
        mosi = (temp >> i) & 0x1;
        clk = 1;
        wait_ns(delay);
        clk = 0;
        wait_ns(delay);
    }
#else
    spi.write(temp);
#endif
    cs = 1;
    return *this;
}

matrix_8 & matrix_8::SendBuffer(void) {
    for (int i=1; i<=8; i++)
        Transmit(matrix_8::Address(i),buffer[i-1]);
    return *this;
}

matrix_8 & matrix_8::Set_Intensity(float intensity) {
    uint8_t value = 0b1111 * intensity;
    Transmit(Intensity,value);
    return *this;
}

matrix_8 & matrix_8::Init_Setting(void) {
    Transmit(DecodeMode);
    Set_MatrixTest(false);
    Set_Shutdown(false);
    Set_Intensity();
    Transmit(ScanLimit,0b111);
    return *this;
}

matrix_8 & matrix_8::Set_MatrixTest(bool mode) {
    Transmit(DisplayTest,mode);
    inDisplayTest = mode;
    return *this;
}

matrix_8 & matrix_8::Set_Shutdown(bool mode) {
    Transmit(Shutdown,!mode);
    inShutdown = mode;
    return *this;
}

inline matrix_8 & matrix_8::ClearBuffer(void) {
    buffer.fill(0);
    return *this;
}

matrix_8 & matrix_8::operator<<(const frame &data) {
    buffer = data;
    return SendBuffer();
}

}
