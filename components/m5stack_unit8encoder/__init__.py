
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c
from esphome.const import CONF_ID

CODEOWNERS = ["@nonik0"]
DEPENDENCIES = ["binary_sensor", "light", "i2c"]
AUTO_LOAD = ["binary_sensor", "light"]
MULTI_CONF = True

m5stack_unit8encoder_ns = cg.esphome_ns.namespace("m5stack_unit8encoder")
M5StackUnit8Encoder = m5stack_unit8encoder_ns.class_(
    "M5StackUnit8Encoder", cg.PollingComponent, i2c.I2CDevice
)

CONF_M5STACK_UNIT8ENCODER_ID = "m5stack_unit8encoder_id"
CONFIG_SCHEMA = (
    cv.Schema({cv.GenerateID(): cv.declare_id(M5StackUnit8Encoder)})
    .extend(cv.polling_component_schema("100ms"))
    .extend(i2c.i2c_device_schema(0x41))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)
