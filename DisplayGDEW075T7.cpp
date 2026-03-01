/**
 * Display driver for 075RW-Z08 e-paper display (800x480 7.5" B/W/Red)
 * Based on Waveshare epd7in5b_V2 reference driver
 * Adapted for Waveshare Universal ESP32 Driver Board (no PWR pin)
 */

#include "DisplayGDEW075T7.h"
#include "config.h"

#define BUSY_TIMEOUT 30000

DisplayGDEW075T7::~DisplayGDEW075T7() {
    #ifndef HEADLESS
    _spi->endTransaction();
    _spi->end();
    delete _spi;
    #endif
};

DisplayGDEW075T7::DisplayGDEW075T7(
    uint8_t spi_bus,
    uint8_t sck_pin,
    uint8_t copi_pin,
    uint8_t cs_pin,
    uint8_t reset_pin,
    uint8_t dc_pin,
    uint8_t busy_pin
) {
    _resetPin = reset_pin;
    _dcPin = dc_pin;
    _csPin = cs_pin;
    _busyPin = busy_pin;

    pinMode(_csPin, OUTPUT);
    pinMode(_resetPin, OUTPUT);
    pinMode(_dcPin, OUTPUT);
    pinMode(_busyPin, INPUT);

    #ifndef HEADLESS
    _spi = new SPIClass(spi_bus);
    _spi->begin(sck_pin, -1, copi_pin, cs_pin);
    _spi->beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
    #endif
};

void DisplayGDEW075T7::reset()
{
    digitalWrite(_resetPin, HIGH);
    delay(200);
    digitalWrite(_resetPin, LOW);
    delay(2);
    digitalWrite(_resetPin, HIGH);
    delay(200);
}

void DisplayGDEW075T7::wakeup()
{
    digitalWrite(_csPin, LOW);

    reset();

    sendCommand(0x01);  // POWER SETTING
    sendData(0x07);
    sendData(0x07);     // VGH=20V, VGL=-20V
    sendData(0x3F);     // VDH=15V
    sendData(0x3F);     // VDL=-15V

    sendCommand(0x04);  // POWER ON
    delay(100);
    waitUntilIdle();

    sendCommand(0x00);  // PANEL SETTING
    sendData(0x0F);     // KW mode

    sendCommand(0x61);  // RESOLUTION SETTING
    sendData(0x03);     // source 800
    sendData(0x20);
    sendData(0x01);     // gate 480
    sendData(0xE0);

    sendCommand(0x15);  // DUAL SPI
    sendData(0x00);

    sendCommand(0x50);  // VCOM AND DATA INTERVAL SETTING
    sendData(0x11);
    sendData(0x07);

    sendCommand(0x60);  // TCON SETTING
    sendData(0x22);

    sendCommand(0x65);  // RESOLUTION SETTING
    sendData(0x00);
    sendData(0x00);
    sendData(0x00);
    sendData(0x00);
}

void DisplayGDEW075T7::sendCommand(uint8_t command)
{
    #ifndef HEADLESS
    digitalWrite(_dcPin, LOW);
    _spi->transfer(command);
    #endif
}

void DisplayGDEW075T7::sendData(uint8_t data)
{
    #ifndef HEADLESS
    digitalWrite(_dcPin, HIGH);
    _spi->transfer(data);
    #endif
}

void DisplayGDEW075T7::waitUntilIdle()
{
    #ifndef HEADLESS
    unsigned long start = millis();
    uint8_t busy;
    do {
        sendCommand(0x71);
        busy = digitalRead(_busyPin);
        busy = !(busy & 0x01);
        delay(5);
    } while (busy && millis() - start < BUSY_TIMEOUT);
    #endif
    delay(200);
}

void DisplayGDEW075T7::refresh(const FrameBuffer *frameBuffer)
{
    wakeup();

    size_t i;
    uint8_t j;
    uint16_t chunk;
    uint8_t output;
    size_t len = frameBuffer->getLength();
    const uint8_t *data = frameBuffer->data;

    // DTM1: black channel
    // FrameBuffer 2-bit: WHITE=00 -> 1 (white), anything else -> 0 (black)
    sendCommand(0x10);
    for (i = 0; i < len; i += 2) {
        output = 0;
        chunk = (data[i] << 8) | data[i + 1];
        for (j = 0; j < 8; ++j) {
            output |= (((chunk >> (j * 2)) & 0b11) == 0 ? 1 : 0) << j;
        }
        sendData(output);
    }

    // DTM2: red channel - unused, send all 0x00
    sendCommand(0x13);
    for (i = 0; i < len; i += 2) {
        sendData(0x00);
    }

    sendCommand(0x12);  // REFRESH
    delay(100);
    waitUntilIdle();
    sleep();
}

void DisplayGDEW075T7::fastClear(bool black)
{
    wakeup();

    size_t len = NATIVE_WIDTH * NATIVE_HEIGHT / 8;

    sendCommand(0x10);
    for (size_t i = 0; i < len; ++i) {
        sendData(black ? 0x00 : 0xFF);
    }

    sendCommand(0x13);
    for (size_t i = 0; i < len; ++i) {
        sendData(0x00);
    }

    sendCommand(0x12);
    delay(100);
    waitUntilIdle();
    sleep();
}

void DisplayGDEW075T7::sleep()
{
    sendCommand(0x02);  // POWER OFF
    waitUntilIdle();
    sendCommand(0x07);  // DEEP SLEEP
    sendData(0xA5);
    digitalWrite(_csPin, HIGH);
}
