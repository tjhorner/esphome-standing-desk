# ESPHome Standing Desk Configs

This repository contains everything you need to set up common standing desks with a controller board running ESPHome. Specifically:

- The `standing_desk_height` component, which reads the desk's height over UART
- A template package which uses the `standing_desk_height` component and allows you to set the desk's height
- Various packages that work with common standing desk brands

## Usage

### ESPHome Package

The quickest way to get started with this component is by using the included `configs/template.yaml` as a [package](https://esphome.io/guides/configuration-types.html#packages) in your ESPHome config. It includes all the configuration necessary to read the height from your desk as well as set a target height from Home Assistant (or whatever other frontend you use) right out of the box.

All you need to do is specify the correct pins, and the min/max height of your desk. For example:

```yaml
packages:
  standing_desk: github://tjhorner/esphome-standing-desk/configs/template.yaml

substitutions:
  # Required (replace these with your own values!)
  standing_desk_uart_rx_pin: D1
  standing_desk_up_pin: D3
  standing_desk_down_pin: D4
  standing_desk_min_height: "25.2"
  standing_desk_max_height: "50.8"

  # Optional (defaults shown)
  desk_height_name: "Desk Height"
  target_desk_height_name: "Target Desk Height"
  standing_desk_height_units: "in"
  standing_desk_variant: "auto" # jarvis, uplift, or omnidesk
```

This will expose two entities:

- `sensor.desk_height`: The actual height of the desk
- `number.target_desk_height`: The target height of the desk. This is an input and can be set from Home Assistant, or elsewhere in your config using `id(target_desk_height)`

I also created pre-made configs to use with various common standing desk brands. Check those out in the [`configs/desks`](configs/desks/README.md) directory.

### External Component

If you only need the component which reads the height from your desk, you can use the `standing_desk_height` component provided by this repo. Here's an example of how to do so:

```yaml
external_components:
  - source: github://tjhorner/esphome-standing-desk
    components: [ standing_desk_height ]

sensor:
  - platform: standing_desk_height
    id: desk_height
    variant: jarvis
```

By default, the component will report the height as inches, but if your desk reports its height in centimeters, you can simply set `units_of_measurement` to `"cm"` in the config like any other sensor.

### Addons

With addons, you can use ESPHome packages to add extra functionality outside of the core component.

#### Presets

You can add preset recall support with this package. It will create a button entity for each preset.

To use, add this line to your config:

```yaml
packages:
  presets_addon: github://tjhorner/esphome-standing-desk/configs/addons/presets.yaml
```

Then, if you are not using a pre-made desk config, you must add the following substitutions:

```yaml
substitutions:
  button_bit1_pin: D7
  button_bit2_pin: D8
  button_bit4_pin: D6
  button_m_pin: D1
```

## Hardware

You can find the open source designs I've made for a generic desk controller [here](https://github.com/tjhorner/wifi-desk-controller). It features two RJ45 jacks so you can passthrough to the original keypad. If you order the boards from JLCPCB and get the parts from DigiKey, the cost is about $5 for a single board.

But if you don't want to to order and assemble everything yourself, you can buy one from me [here](https://shop.horner.tj/things/desk-controller). And it even comes with a fancy 3D-printed case!

## Home Assistant Card

![image](https://user-images.githubusercontent.com/2646487/139561642-6913a43d-792b-47e1-9d63-a605dfa33bde.png)

This is the YAML config for the card I use on my Home Assistant dashboard. It shows the current and target desk height.

```yaml
type: entities
state_color: false
title: Standing Desk
entities:
  - entity: number.target_desk_height
    name: Target Height
    icon: mdi:target
  - entity: sensor.desk_height
    name: Actual Height
```