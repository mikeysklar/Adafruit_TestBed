/*
  ESP32BootROM - part of the Firmware Updater for the
  Arduino MKR WiFi 1010, Arduino MKR Vidor 4000, and Arduino UNO WiFi Rev.2.

  Copyright (c) 2018 Arduino SA. All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <Arduino.h>

#define ESP32_DEFAULT_TIMEOUT 3000

class ESP32BootROMClass {
public:
  ESP32BootROMClass(HardwareSerial &hwSerial, int gpio0Pin, int resetnPin);

  int begin(unsigned long baudrate);
  void end();

  bool read_reg(uint32_t regAddr, uint32_t *regValue,
                uint32_t timeout_ms = ESP32_DEFAULT_TIMEOUT);
  bool read_MAC(uint8_t mac[6]);
  uint32_t read_chip_detect(void);

  bool isRunningStub(void);
  uint32_t getFlashWriteSize(void);

  int beginFlash(uint32_t offset, uint32_t size, uint32_t chunkSize);
  int dataFlash(const void *data, uint32_t length);
  int endFlash(uint32_t reboot);
  bool md5Flash(uint32_t offset, uint32_t size, uint8_t *result);

private:
  int sync();
  int changeBaudrate(uint32_t baudrate);
  int spiAttach();

  void command(uint8_t opcode, const void *data, uint16_t length,
               const void *data2 = NULL, uint16_t len2 = 0);
  int response(uint8_t opcode, uint32_t timeout_ms, void *body = NULL,
               uint16_t maxlen = 4);

  uint16_t readBytes(void *buf, uint16_t length, uint32_t timeout_ms);
  bool readSLIP(uint32_t timeout_ms);
  void writeEscapedBytes(const uint8_t *data, uint16_t length);

  bool beginMem(uint32_t offset, uint32_t size, uint32_t chunkSize);
  bool dataMem(const void *data, uint32_t length);
  bool endMem(uint32_t entry);

  // only needed for ESP32
  bool uploadStub(void);
  bool syncStub(uint32_t timeout_ms);

private:
  HardwareSerial *_serial;
  int _gpio0Pin;
  int _resetnPin;
  bool _supports_encrypted_flash;
  bool _stub_running;

  uint32_t _flashSequenceNumber;
  uint32_t _chunkSize;
};
