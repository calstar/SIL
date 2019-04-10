#pragma once

#include "common.h"
#include "../mbed.h"

#define RFM69_MAX_PAYLOAD 64 ///< Maximum bytes payload

/**
 * Valid RFM69 operation modes.
 */
typedef enum {
  RFM69_MODE_SLEEP = 0, //!< Sleep mode (lowest power consumption)
  RFM69_MODE_STANDBY,   //!< Standby mode
  RFM69_MODE_FS,        //!< Frequency synthesizer enabled
  RFM69_MODE_TX,        //!< TX mode (carrier active)
  RFM69_MODE_RX         //!< RX mode
} RFM69Mode;

/**
 * Valid RFM69 data modes.
 */
typedef enum {
  RFM69_DATA_MODE_PACKET = 0, //!< Packet engine active
  RFM69_DATA_MODE_CONTINUOUS_WITH_SYNC =
      2, //!< Continuous mode with clock recovery
  RFM69_DATA_MODE_CONTINUOUS_WITHOUT_SYNC =
      3, //!< Continuous mode without clock recovery
} RFM69DataMode;

/** RFM69 driver library for STM32 controllers. */
class RFM69 {
public:
  RFM69(int mosi, int miso, int sclk, int ss,
        int rst,
        bool highPowerDevice = false);

  /**
   * Set the data pin (DIO2) of the RFM69 module that is connected to the
   * controller.
   *
   * @param dataGPIO GPIO of DIO2 signal (ie. GPIOA, GPIOB, ...)
   * @param dataPin Pin of DIO2 signal (eg. GPIO_Pin_1)
   */
  void reset();

  bool init();

  void setFrequency(unsigned int frequency);

  void setFrequencyDeviation(unsigned int frequency);

  void setBitrate(unsigned int bitrate);

  RFM69Mode setMode(RFM69Mode mode);

  void setPowerLevel(uint8_t power);

  int setPowerDBm(int8_t dBm);

  void setHighPowerSettings(bool enable);

  void setCustomConfig(const uint8_t config[][2], unsigned int length);

  int send(const void *data, unsigned int dataLength);

  int receive(char *data, unsigned int dataLength);

  void sleep();

  /**
   * Gets the last "cached" RSSI reading.
   *
   * @note This only gets the latest reading that was requested by readRSSI().
   *
   * @return RSSI value in dBm.
   */
  int getRSSI() { return _rssi; }

  void setOOKMode(bool enable);

  void setDataMode(RFM69DataMode dataMode = RFM69_DATA_MODE_PACKET);

  /**
   * Enable/disable the automatic reading of the RSSI value during packet
   * reception.
   *
   * Default is off (no reading).
   *
   * @param enable true or false
   */
  void setAutoReadRSSI(bool enable) { _autoReadRSSI = enable; }

  /**
   * Enable/disable the CSMA/CA (carrier sense) algorithm before sending a
   * packet.
   *
   * @param enable true or false
   */
  void setCSMA(bool enable) { _csmaEnabled = enable; }

  void continuousBit(bool bit);

  void dumpRegisters();

  void setPASettings(uint8_t forcePA = 0);

  bool setAESEncryption(const void *aesKey, unsigned int keyLength);

private:
  Serial serial;

  bool _init;
  RFM69Mode _mode;
  bool _highPowerDevice;
  uint8_t _powerLevel;
  int _rssi;
  bool _autoReadRSSI;
  bool _ookEnabled;
  RFM69DataMode _dataMode;
  bool _highPowerSettings;
  bool _csmaEnabled;
  char _rxBuffer[RFM69_MAX_PAYLOAD];
  unsigned int _rxBufferLength;

};
