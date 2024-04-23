import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.automation import maybe_simple_id
from esphome.components import uart, sensor, output, button
from esphome.const import CONF_ID, CONF_VARIANT, CONF_NAME

from .. import CONF_SDC_ID, StandingDeskControlComponent, standing_desk_control_ns

DEPENDENCIES = ["standing_desk_control"]

RecallPresetButton = standing_desk_control_ns.class_("RecallPresetButton", button.Button)
SetPresetButton = standing_desk_control_ns.class_("SetPresetButton", button.Button)

def _make_recall_preset_button(preset_number):
  return button.button_schema(RecallPresetButton, icon=f"mdi:numeric-{preset_number}-box")

def _make_set_preset_button(preset_number):
  return button.button_schema(SetPresetButton, icon=f"mdi:numeric-{preset_number}-box-multiple")

CONFIG_SCHEMA = cv.Schema(
  {
    cv.GenerateID(CONF_SDC_ID): cv.use_id(StandingDeskControlComponent),

    cv.Optional("preset_1"): _make_recall_preset_button(1),
    cv.Optional("preset_2"): _make_recall_preset_button(2),
    cv.Optional("preset_3"): _make_recall_preset_button(3),
    cv.Optional("preset_4"): _make_recall_preset_button(4),

    cv.Optional("set_preset_1"): _make_set_preset_button(1),
    cv.Optional("set_preset_2"): _make_set_preset_button(2),
    cv.Optional("set_preset_3"): _make_set_preset_button(3),
    cv.Optional("set_preset_4"): _make_set_preset_button(4),
  }
)

async def _generate_button(config, preset_number):
  btn = await button.new_button(config)
  cg.add(btn.set_preset_number(preset_number))
  return btn

async def to_code(config):
  for i in range(1, 5):
    if btn_config := config.get(f"preset_{i}"):
      btn = await _generate_button(btn_config, i)
      await cg.register_parented(btn, config[CONF_SDC_ID])
    
    if btn_config := config.get(f"set_preset_{i}"):
      btn = await _generate_button(btn_config, i)
      await cg.register_parented(btn, config[CONF_SDC_ID])