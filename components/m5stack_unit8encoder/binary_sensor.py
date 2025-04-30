import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import (
    ENTITY_CATEGORY_NONE,
    #DEVICE_CLASS_BUTTON,
    CONF_ID,
)

from . import CONF_M5STACK_UNIT8ENCODER_ID, M5StackUnit8Encoder


CODEOWNERS = ["@nonik0"]
DEPENDENCIES = ["m5stack_unit8encoder"]

CONF_SWITCH = "switch"
CONF_BUTTON_1 = "button_1"
CONF_BUTTON_2 = "button_2"
CONF_BUTTON_3 = "button_3"
CONF_BUTTON_4 = "button_4"
CONF_BUTTON_5 = "button_5"
CONF_BUTTON_6 = "button_6"
CONF_BUTTON_7 = "button_7"
CONF_BUTTON_8 = "button_8"

BUTTONS = [
    CONF_BUTTON_1,
    CONF_BUTTON_2,
    CONF_BUTTON_3,
    CONF_BUTTON_4,
    CONF_BUTTON_5,
    CONF_BUTTON_6,
    CONF_BUTTON_7,
    CONF_BUTTON_8,
]

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_M5STACK_UNIT8ENCODER_ID): cv.use_id(M5StackUnit8Encoder),
        cv.Optional(CONF_SWITCH): binary_sensor.binary_sensor_schema(
            #device_class=DEVICE_CLASS_BUTTON,
            entity_category=ENTITY_CATEGORY_NONE,
        ),
        **{
            cv.Optional(BTN): binary_sensor.binary_sensor_schema(
                entity_category=ENTITY_CATEGORY_NONE,
            )
            for BTN in BUTTONS
        },
    }
)

async def to_code(config):
    hub = await cg.get_variable(config[CONF_M5STACK_UNIT8ENCODER_ID])
    if conf := config.get(CONF_SWITCH):
            sens = await binary_sensor.new_binary_sensor(conf)
            cg.add(hub.set_switch_binary_sensor(sens))
    for i, key in enumerate(BUTTONS):
        if conf := config.get(key):
            sens = await binary_sensor.new_binary_sensor(conf)
            cg.add(hub.set_encoder_button_binary_sensor(i, sens)) # creates 0-indexing for buttons internally
