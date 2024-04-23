import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.automation import maybe_simple_id
from esphome.components import uart, sensor, output
from esphome.const import CONF_ID, CONF_VARIANT

DEPENDENCIES = ["output"]

CONF_SDC_ID = "standing_desk_control_id"

standing_desk_control_ns = cg.esphome_ns.namespace("standing_desk_control")
StandingDeskControlComponent = standing_desk_control_ns.class_("StandingDeskControlComponent", cg.Component)

ParallelController = standing_desk_control_ns.class_("ParallelController")

CONF_PARALLEL = "parallel"

PARALLEL_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(ParallelController),

        cv.Required("bit_1"): cv.use_id(output.BinaryOutput),
        cv.Required("bit_2"): cv.use_id(output.BinaryOutput),
        cv.Required("bit_4"): cv.use_id(output.BinaryOutput),
        cv.Required("bit_m"): cv.use_id(output.BinaryOutput),

        cv.Required("up"): cv.use_id(output.BinaryOutput),
        cv.Required("down"): cv.use_id(output.BinaryOutput),
    }
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(StandingDeskControlComponent),

        cv.Exclusive(CONF_PARALLEL, "method"): PARALLEL_SCHEMA,
        cv.Exclusive("serial", "method"): {},
    }
).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])

    if CONF_PARALLEL in config:
        parallel_config = config[CONF_PARALLEL]

        parallel = cg.new_Pvariable(parallel_config[CONF_ID])
        cg.add(parallel.set_bit_1(await cg.get_variable(parallel_config["bit_1"])))
        cg.add(parallel.set_bit_2(await cg.get_variable(parallel_config["bit_2"])))
        cg.add(parallel.set_bit_4(await cg.get_variable(parallel_config["bit_4"])))
        cg.add(parallel.set_bit_m(await cg.get_variable(parallel_config["bit_m"])))
        cg.add(parallel.set_up(await cg.get_variable(parallel_config["up"])))
        cg.add(parallel.set_down(await cg.get_variable(parallel_config["down"])))

        cg.add(var.set_controller(parallel))

    await cg.register_component(var, config)

