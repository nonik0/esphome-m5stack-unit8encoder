import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    ENTITY_CATEGORY_NONE,
    UNIT_EMPTY,
    DEVICE_CLASS_EMPTY,
    STATE_CLASS_MEASUREMENT,
    CONF_ID,
)

from . import CONF_M5STACK_UNIT8ENCODER_ID, M5StackUnit8Encoder


CODEOWNERS = ["@nonik0"]
DEPENDENCIES = ["m5stack_unit8encoder"]

CONF_ENCODER_1 = "encoder_1"
CONF_ENCODER_2 = "encoder_2"
CONF_ENCODER_3 = "encoder_3"
CONF_ENCODER_4 = "encoder_4"
CONF_ENCODER_5 = "encoder_5"
CONF_ENCODER_6 = "encoder_6"
CONF_ENCODER_7 = "encoder_7"
CONF_ENCODER_8 = "encoder_8"

ENCODERS = [
    CONF_ENCODER_1,
    CONF_ENCODER_2,
    CONF_ENCODER_3,
    CONF_ENCODER_4,
    CONF_ENCODER_5,
    CONF_ENCODER_6,
    CONF_ENCODER_7,
    CONF_ENCODER_8,
]

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_M5STACK_UNIT8ENCODER_ID): cv.use_id(M5StackUnit8Encoder),
        **{
            cv.Optional(encoder): sensor.sensor_schema(
                unit_of_measurement=UNIT_EMPTY,
                accuracy_decimals=0,
                device_class=DEVICE_CLASS_EMPTY,
                state_class=STATE_CLASS_MEASUREMENT,
                entity_category=ENTITY_CATEGORY_NONE,
            )
            for encoder in ENCODERS
        },
    }
)

async def to_code(config):
    hub = await cg.get_variable(config[CONF_M5STACK_UNIT8ENCODER_ID])
    for i, key in enumerate(ENCODERS):
        if conf := config.get(key):
            sens = await sensor.new_sensor(conf)
            cg.add(hub.set_encoder_delta_sensor(i, sens))
