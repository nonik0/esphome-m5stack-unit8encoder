
// ported from https://github.com/adafruit/m5stack_unit8encoder_Library
#pragma once

#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/light/light_output.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/switch/switch.h"
#include "esphome/components/i2c/i2c.h"

#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif

// I2C register lookup tables
#define ENCODER_ADDR         0x41
#define ENCODER_REG          0x00
#define INCREMENT_REG        0x20
#define BUTTON_REG           0x50
#define SWITCH_REG           0x60
#define RGB_LED_REG          0x70
#define RESET_COUNTER_REG    0x40
#define FIRMWARE_VERSION_REG 0xFE
#define I2C_ADDRESS_REG      0xFF

static const uint8_t ENCODER_COUNT = 8;

namespace esphome {
namespace m5stack_unit8encoder {

class M5StackUnit8Encoder : public i2c::I2CDevice, public PollingComponent {
  public:
    void setup() override;
    void update() override;
    void dump_config() override;

    void set_encoder_button_binary_sensor(uint8_t index, binary_sensor::BinarySensor *button_binary_sensor);
    void set_encoder_delta_sensor(uint8_t index, sensor::Sensor *encoder_delta_sensor);
    void set_switch_binary_sensor(binary_sensor::BinarySensor *switch_binary_sensor);

    void set_encoder_led_state(uint8_t index, light::LightState *state);

  protected:
    binary_sensor::BinarySensor *encoderButtons[ENCODER_COUNT];
    sensor::Sensor *encoderDeltas[ENCODER_COUNT];
    binary_sensor::BinarySensor *toggle_switch{nullptr};

    void publish_state_(binary_sensor::BinarySensor *binary_sensor, const bool &state);
    void publish_state_(sensor::Sensor *sensor, const float &state);

  private:
    uint8_t firmwareVersion{0xFF};
    uint8_t buttonStates[ENCODER_COUNT], lastButtonStates[ENCODER_COUNT];
    int32_t encDeltaStates[ENCODER_COUNT], lastEncDeltaStates[ENCODER_COUNT];
    uint8_t switchState, lastSwitchState;

    bool read_buttons();
    bool read_encoder_deltas();
    bool read_switch();
};


}  // namespace m5stack_unit8encoder
}  // namespace esphome