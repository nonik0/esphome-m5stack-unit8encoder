import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light
from esphome.const import (
    CONF_OUTPUT_ID,
)

from .. import CONF_M5STACK_UNIT8ENCODER_ID, M5StackUnit8Encoder, m5stack_unit8encoder_ns

CODEOWNERS = ["@nonik0"]
DEPENDENCIES = ["m5stack_unit8encoder"]

CONF_LED_1 = "led_1"
CONF_LED_2 = "led_2"
CONF_LED_3 = "led_3"
CONF_LED_4 = "led_4"
CONF_LED_5 = "led_5"
CONF_LED_6 = "led_6"
CONF_LED_7 = "led_7"
CONF_LED_8 = "led_8"

LEDS = [
    CONF_LED_1,
    CONF_LED_2,
    CONF_LED_3,
    CONF_LED_4,
    CONF_LED_5,
    CONF_LED_6,
    CONF_LED_7,
    CONF_LED_8,
]

LedLight = m5stack_unit8encoder_ns.class_("RgbLedLight", light.LightOutput)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_M5STACK_UNIT8ENCODER_ID): cv.use_id(M5StackUnit8Encoder),
        **{
            cv.Optional(LED): light.RGB_LIGHT_SCHEMA.extend({
                cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(LedLight),
            })
            for LED in LEDS
        }
    }
)

async def to_code(config):
    hub = await cg.get_variable(config[CONF_M5STACK_UNIT8ENCODER_ID])
    for i, key in enumerate(LEDS):
        if conf := config.get(key):
            led = cg.new_Pvariable(conf[CONF_OUTPUT_ID])
            await light.register_light(led, conf)
            cg.add(led.set_led_index(i))
            await cg.register_parented(led, hub)

