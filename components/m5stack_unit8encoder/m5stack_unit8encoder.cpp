#include "esphome/core/log.h"
#include "m5stack_unit8encoder.h"

namespace esphome {
namespace m5stack_unit8encoder {

static const char *TAG = "m5stack_unit8encoder";

void M5StackUnit8Encoder::setup() {
    ESP_LOGI(TAG, "Setting up M5Stack Unit8Encoder at address 0x%02x...", this->address_);

    this->read_register(FIRMWARE_VERSION_REG, &this->firmwareVersion, 1);
    ESP_LOGI(TAG, "M5Stack Unit8Encoder firmware version %d", this->firmwareVersion);

    for (int i = 0; i < ENCODER_COUNT; i++) {
        if (this->encoderButtons[i] != nullptr) {
            this->encoderButtons[i]->publish_initial_state(false);
        }

        if (this->encoderDeltas[i] != nullptr) {
            this->encoderDeltas[i]->publish_state(0.0f);
        }
    }

    if (this->toggle_switch != nullptr) {
        ESP_LOGD(TAG, "Publishing inital switch state");
        this->toggle_switch->publish_initial_state(false);
    }

    ESP_LOGI(TAG, "Done setting up M5Stack Unit8Encoder");
}

void M5StackUnit8Encoder::update() {
    if (this->read_buttons()) {
        for (uint8_t i = 0; i < ENCODER_COUNT; i++) {
            if (!this->buttonStates[i] && this->lastButtonStates[i]) {
                ESP_LOGD(TAG, "Pressed: %d", i);
                publish_state_(this->encoderButtons[i], true);
            }
            if (this->buttonStates[i] && !this->lastButtonStates[i]) {
                ESP_LOGD(TAG, "Released: %d", i);
                publish_state_(this->encoderButtons[i], false);
            }
        }
    }

    if (this->read_encoder_deltas()) {
        for (int32_t i = 0; i < ENCODER_COUNT; i++) {
            if (this->encDeltaStates[i] != 0 && this->encDeltaStates[i] != this->lastEncDeltaStates[i]) {
                ESP_LOGD(TAG, "Turned: %d -> %d", i, this->encDeltaStates[i]);
                publish_state_(this->encoderDeltas[i], (float)encDeltaStates[i]);
            }
        }
    }

    if (this->read_switch()) {
        ESP_LOGD(TAG, "Switch changed: %d", this->switchState);
        publish_state_(this->toggle_switch, this->switchState != 0);
    }
}

void M5StackUnit8Encoder::set_encoder_button_binary_sensor(uint8_t index, binary_sensor::BinarySensor *button_binary_sensor) {
    this->encoderButtons[index] = button_binary_sensor;
}

void M5StackUnit8Encoder::set_encoder_delta_sensor(uint8_t index, sensor::Sensor *encoder_delta_sensor) {
    this->encoderDeltas[index] = encoder_delta_sensor;
}

void M5StackUnit8Encoder::set_switch_binary_sensor(binary_sensor::BinarySensor *switch_binary_sensor) {
    this->toggle_switch = switch_binary_sensor;
}

void M5StackUnit8Encoder::set_encoder_led_state(uint8_t index, light::LightState *state) {
    if (index >= ENCODER_COUNT)
        return;

    float red, green, blue;
    state->current_values_as_rgb(&red, &green, &blue, false);

    auto to_uint8 = [](float c) -> uint8_t {
        c = std::max(0.0f, std::min(1.0f, c));  // clamp to [0.0, 1.0]
        return static_cast<uint8_t>(c * 255.0f + 0.5f);  // scale and round
    };
    constexpr uint8_t RGB_BYTES_PER_LED = 3;
    uint8_t colorData[RGB_BYTES_PER_LED] = { to_uint8(red), to_uint8(green), to_uint8(blue) };

    ESP_LOGD(TAG, "Setting LED %d color to [%3u,%3u,%3u]",  index, colorData[0], colorData[1], colorData[2]);
    this->write_register(RGB_LED_REG + index * RGB_BYTES_PER_LED, colorData, RGB_BYTES_PER_LED);
}

void M5StackUnit8Encoder::publish_state_(binary_sensor::BinarySensor *binary_sensor, const bool &state) {
    if (binary_sensor == nullptr)
        return;

    binary_sensor->publish_state(state);
}

void M5StackUnit8Encoder::publish_state_(sensor::Sensor *sensor, const float &state) {
    if (sensor == nullptr)
        return;

    sensor->publish_state(state);
}

bool M5StackUnit8Encoder::read_buttons() {
    memcpy(lastButtonStates, buttonStates, ENCODER_COUNT);

    bool update = false;
    for (int i = 0; i < ENCODER_COUNT; i++) {
        this->read_register(BUTTON_REG + i, &buttonStates[i], 1);
        update |= lastButtonStates[i] != buttonStates[i];
    }

    return update;
}

bool M5StackUnit8Encoder::read_encoder_deltas() {
    memcpy(lastEncDeltaStates, encDeltaStates, ENCODER_COUNT*4);

    bool update = false;
    for (int i = 0; i < ENCODER_COUNT; i++) {
        uint8_t valueBytes[4];
        this->read_register(INCREMENT_REG + i * 4, valueBytes, 4);
        encDeltaStates[i] =
            valueBytes[0] | (valueBytes[1] << 8) | (valueBytes[2] << 16) | (valueBytes[3] << 24);
        update |= lastEncDeltaStates[i] != encDeltaStates[i];
    }

    return update;
}

bool M5StackUnit8Encoder::read_switch() {
    lastSwitchState = switchState;

    this->read_register(SWITCH_REG, &switchState, 1);

    return lastSwitchState != switchState;
}

void M5StackUnit8Encoder::dump_config(){
    ESP_LOGCONFIG(TAG, "M5Stack Unit8Encoder Config: I2C Addr: 0x%02x", this->address_);
    ESP_LOGCONFIG(TAG, "M5Stack Unit8Encoder Config: Firmware Version: 0x%02x", this->firmwareVersion);
}

}  // namespace m5stack_unit8encoder
}  // namespace esphome