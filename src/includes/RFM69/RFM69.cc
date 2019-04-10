#include "RFM69.hpp"
#include "../pins.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TIMEOUT_MODE_READY                                                     \
  100 ///< Maximum amount of time until mode switch [ms]
#define TIMEOUT_PACKET_SENT                                                    \
  100 ///< Maximum amount of time until packet must be sent [ms]
#define TIMEOUT_CSMA_READY                                                     \
  500 ///< Maximum CSMA wait time for channel free detection [ms]
#define CSMA_RSSI_THRESHOLD                                                    \
  -85 ///< If RSSI value is smaller than this, consider channel as free [dBm]


// Clock constants. DO NOT CHANGE THESE!
#define RFM69_XO 32000000       ///< Internal clock frequency [Hz]
#define RFM69_FSTEP 61.03515625 ///< Step width of synthesizer [Hz]

/**
 * RFM69 default constructor. Use init() to start working with the RFM69 module.
 *
 * @param spi Pointer to a SPI device
 * @param csGPIO GPIO of /CS line (ie. GPIOA, GPIOB, ...)
 * @param csPin Pin of /CS line (eg. GPIO_Pin_1)
 * @param highPowerDevice Set to true, if this is a RFM69Hxx device (default:
 * false)
 */
RFM69::RFM69(int mosi, int miso, int sclk, int ss, int rst,
             bool highPowerDevice) : serial(RADIO_TX, RADIO_RX) {
  _init = false;
  _mode = RFM69_MODE_STANDBY;
  _highPowerDevice = highPowerDevice;
  _powerLevel = 0;
  _rssi = -127;
  _ookEnabled = false;
  _autoReadRSSI = true;
  _dataMode = RFM69_DATA_MODE_PACKET;
  _highPowerSettings = false;
  _csmaEnabled = false;
  _rxBufferLength = 0;
}

/**
 * Reset the RFM69 module using the external reset line.
 *
 * @note Use setResetPin() before calling this function.
 */
void RFM69::reset() {
  _init = false;

  _mode = RFM69_MODE_STANDBY;

  while (serial.readable()) {
    serial.getc();
  }
}

/**
 * Initialize the RFM69 module.
 * A base configuration is set and the module is put in standby mode.
 *
 * @return Always true
 */
bool RFM69::init() {
  _init = true;

  return _init;
}

/**
 * Set the carrier frequency in Hz.
 * After calling this function, the module is in standby mode.
 *
 * @param frequency Carrier frequency in Hz
 */
void RFM69::setFrequency(unsigned int frequency) {
  // switch to standby if TX/RX was active
  if (RFM69_MODE_RX == _mode || RFM69_MODE_TX == _mode)
    setMode(RFM69_MODE_STANDBY);

  // calculate register value
  frequency /= RFM69_FSTEP;
}

/**
 * Set the FSK frequency deviation in Hz.
 * After calling this function, the module is in standby mode.
 *
 * @param frequency Frequency deviation in Hz
 */
void RFM69::setFrequencyDeviation(unsigned int frequency) {
  // switch to standby if TX/RX was active
  if (RFM69_MODE_RX == _mode || RFM69_MODE_TX == _mode)
    setMode(RFM69_MODE_STANDBY);

  // calculate register value
  frequency /= RFM69_FSTEP;
}

/**
 * Set the bitrate in bits per second.
 * After calling this function, the module is in standby mode.
 *
 * @param bitrate Bitrate in bits per second
 */
void RFM69::setBitrate(unsigned int bitrate) {
  // switch to standby if TX/RX was active
  if (RFM69_MODE_RX == _mode || RFM69_MODE_TX == _mode)
    setMode(RFM69_MODE_STANDBY);

  // calculate register value
  bitrate = RFM69_XO / bitrate;
}


/**
 * Acquire the chip.
 */

/**
 * Switch the mode of the RFM69 module.
 * Using this function you can manually select the RFM69 mode (sleep for
 * example).
 *
 * This function also takes care of the special registers that need to be set
 * when the RFM69 module is a high power device (RFM69Hxx).
 *
 * This function is usually not needed because the library handles mode changes
 * automatically.
 *
 * @param mode RFM69_MODE_SLEEP, RFM69_MODE_STANDBY, RFM69_MODE_FS,
 * RFM69_MODE_TX, RFM69_MODE_RX
 * @return The new mode
 */
RFM69Mode RFM69::setMode(RFM69Mode mode) {
  if ((mode == _mode) || (mode > RFM69_MODE_RX))
    return _mode;

  _mode = mode;

  return _mode;
}


/**
 * Enable/disable the power amplifier(s) of the RFM69 module.
 *
 * PA0 for regular devices is enabled and PA1 is used for high power devices
 * (default).
 *
 * @note Use this function if you want to manually override the PA settings.
 * @note PA0 can only be used with regular devices (not the high power ones!)
 * @note PA1 and PA2 can only be used with high power devices (not the regular
 * ones!)
 *
 * @param forcePA If this is 0, default values are used. Otherwise, PA settings
 * are forced. 0x01 for PA0, 0x02 for PA1, 0x04 for PA2, 0x08 for +20 dBm high
 * power settings.
 */
void RFM69::setPASettings(uint8_t forcePA) {

}

/**
 * Set the output power level of the RFM69 module.
 *
 * @param power Power level from 0 to 31.
 */
void RFM69::setPowerLevel(uint8_t power) {
  if (power > 31)
    power = 31;

  _powerLevel = power;
}

/**
 * Enable the +20 dBm high power settings of RFM69Hxx modules.
 *
 * @note Enabling only works with high power devices.
 *
 * @param enable true or false
 */
void RFM69::setHighPowerSettings(bool enable) {
  // enabling only works if this is a high power device
  if (true == enable && false == _highPowerDevice)
    enable = false;
}

/**
 * Reconfigure the RFM69 module by writing multiple registers at once.
 *
 * @param config Array of register/value tuples
 * @param length Number of elements in config array
 */
void RFM69::setCustomConfig(const uint8_t config[][2], unsigned int length) {

}

/**
 * Send a packet over the air.
 *
 * After sending the packet, the module goes to standby mode.
 * CSMA/CA is used before sending if enabled by function setCSMA() (default:
 * off).
 *
 * @note A maximum amount of RFM69_MAX_PAYLOAD bytes can be sent.
 * @note This function blocks until packet has been sent.
 *
 * @param data Pointer to buffer with data
 * @param dataLength Size of buffer
 *
 * @return Number of bytes that have been sent
 */
int RFM69::send(const void *data, unsigned int dataLength) {
  serial.write((uint8_t*)data, dataLength, NULL);
  return dataLength;
}


/**
 * Put the RFM69 module to sleep (lowest power consumption).
 */
void RFM69::sleep() { setMode(RFM69_MODE_SLEEP); }

/**
 * Put the RFM69 module in RX mode and try to receive a packet.
 *
 * @note The module resides in RX mode.
 *
 * @param data Pointer to a receiving buffer
 * @param dataLength Maximum size of buffer
 * @return Number of received bytes; 0 if no payload is available.
 */
int RFM69::receive(char *data, unsigned int dataLength) {
  // check if there is a packet in the internal buffer and copy it
  int i;
  for (i = 0; i < dataLength; i++) {
    if (!serial.readable()) break;
    data[i] = serial.getc();
  }
  return i;
}

/**
 * Enable and set or disable AES hardware encryption/decryption.
 *
 * The AES encryption module will be disabled if an invalid key or key length
 * is passed to this function (aesKey = 0 or keyLength != 16).
 * Otherwise encryption will be enabled.
 *
 * The key is stored as MSB first in the RF module.
 *
 * @param aesKey Pointer to a buffer with the 16 byte AES key
 * @param keyLength Number of bytes in buffer aesKey; must be 16 bytes
 * @return State of encryption module (false = disabled; true = enabled)
 */
bool RFM69::setAESEncryption(const void *aesKey, unsigned int keyLength) {
  return false;
}

/**
 * Transmit a high or low bit in continuous mode using the external data line.
 *
 * @note Use setDataPin() before calling this function.
 * @note Call setDataMode() before to enable continuous mode.
 *
 * @param bit true: high bit; false: low bit
 */
void RFM69::continuousBit(bool bit) {

}

/**
 * Debug function to dump all RFM69 registers.
 *
 * Symbol 'DEBUG' has to be defined.
 */
void RFM69::dumpRegisters(void) {

}

/**
 * Enable/disable OOK modulation (On-Off-Keying).
 *
 * Default modulation is FSK.
 * The module is switched to standby mode if RX or TX was active.
 *
 * @param enable true or false
 */
void RFM69::setOOKMode(bool enable) {

}

/**
 * Configure the data mode of the RFM69 module.
 *
 * Default data mode is 'packet'. You can choose between 'packet',
 * 'continuous with clock recovery', 'continuous without clock recovery'.
 *
 * The module is switched to standby mode if RX or TX was active.
 *
 * @param dataMode RFM69_DATA_MODE_PACKET, RFM69_DATA_MODE_CONTINUOUS_WITH_SYNC,
 * RFM69_DATA_MODE_CONTINUOUS_WITHOUT_SYNC
 */
void RFM69::setDataMode(RFM69DataMode dataMode) {

}

/**
 * Set the output power level in dBm.
 *
 * This function takes care of the different PA settings of the modules.
 * Depending on the requested power output setting and the available module,
 * PA0, PA1 or PA1+PA2 is enabled.
 *
 * @param dBm Output power in dBm
 * @return 0 if dBm valid; else -1.
 */
int RFM69::setPowerDBm(int8_t dBm) {
  return 0;
}

/** @}
 *
 */
