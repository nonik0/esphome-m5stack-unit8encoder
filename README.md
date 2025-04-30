# esphome-m5stack-unit8encoder

ESPHome component to monitor and control the [M5Stack 8-Encoder Unit](https://shop.m5stack.com/products/8-encoder-unit-stm32f030) over I2C.

## Background

I wanted a decent integration into Home Assistant so I could use this to monitor and control different Home Assistant entities. I have no idea what the right "name" is for this thing. Unit 8Encoder, Encoder8, 8-Encoder Unit...

## Example Usage

See [enc8-minimal.yaml](/enc8-minimal.yaml) for an example minimal esphome YAML config that exposes all 8Encoder's buttons, LEDs, and encoder sensors.

## Work Left

- sensors for encoder absolute value
- support for firmware version v2 (more efficient I2C protocol)
- support for changing I2C address