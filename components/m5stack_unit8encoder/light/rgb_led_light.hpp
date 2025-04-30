#pragma once

#include "esphome/core/component.h"
#include "esphome/components/light/light_output.h"
#include "../m5stack_unit8encoder.h"

namespace esphome {
namespace m5stack_unit8encoder {

class RgbLedLight : public light::LightOutput, public Parented<M5StackUnit8Encoder> {
    public:
        RgbLedLight() = default;
        explicit RgbLedLight(uint8_t index) { led_num = index; }

        void set_led_index(uint8_t index) { this->led_num = index; }

        light::LightTraits get_traits() override {
            auto traits = light::LightTraits();
            traits.set_supported_color_modes({light::ColorMode::RGB});
            return traits;
        }

    protected:
        uint8_t led_num{0xFF};

        void write_state(light::LightState *state) override {
            this->parent_->set_encoder_led_state(this->led_num, state);
        }
};

}  // namespace m5stack_unit8encoder
}  // namespace esphome
