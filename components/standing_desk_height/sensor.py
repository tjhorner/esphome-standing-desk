import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.automation import maybe_simple_id
from esphome.components import uart, sensor
from esphome.const import CONF_ID, CONF_VARIANT

DEPENDENCIES = ["uart"]

standing_desk_height_ns = cg.esphome_ns.namespace("standing_desk_height")
StandingDeskHeightSensor = standing_desk_height_ns.class_("StandingDeskHeightSensor", cg.PollingComponent, uart.UARTDevice, sensor.Sensor)

DetectDecoderAction = standing_desk_height_ns.class_("DetectDecoderAction", automation.Action)

DETECT_DECODER_SCHEMA = maybe_simple_id(
    {
        cv.Required(CONF_ID): cv.use_id(StandingDeskHeightSensor),
    }
)

@automation.register_action("standing_desk_height.detect_decoder", DetectDecoderAction, DETECT_DECODER_SCHEMA)
async def detect_decoder_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    return cg.new_Pvariable(action_id, template_arg, paren)

DecoderVariants = standing_desk_height_ns.enum("DecoderVariants")
DECODER_VARIANTS = {
    "uplift": DecoderVariants.DECODER_VARIANT_UPLIFT,
    "jarvis": DecoderVariants.DECODER_VARIANT_JARVIS,
    "omnidesk": DecoderVariants.DECODER_VARIANT_OMNIDESK,
}

CONFIG_SCHEMA = (
    sensor.sensor_schema(
        unit_of_measurement="in",
        icon="mdi:human-male-height-variant",
        accuracy_decimals=1
    )
    .extend({
        cv.GenerateID(): cv.declare_id(StandingDeskHeightSensor),
        cv.Required(CONF_VARIANT): cv.enum(DECODER_VARIANTS, lower=True, space="_"),
    })
    .extend(cv.polling_component_schema("500ms"))
    .extend(uart.UART_DEVICE_SCHEMA)
)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    cg.add(var.set_decoder_variant(config[CONF_VARIANT]))

    yield cg.register_component(var, config)
    yield sensor.register_sensor(var, config)
    yield uart.register_uart_device(var, config)

