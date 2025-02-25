# Desk Configs

Since not every standing desk has the same pinout, this directory includes substitutions for common desk brands that are verified to work with my [desk controller board](https://shop.horner.tj/things/desk-controller). Each file defines the correct pins and min/max height for each desk, so you don't need to.

## Usage

In your ESPHome config, you need to include the `template.yaml` from the parent directory as a package, as well as the desk-specific config. To make things easy, you can just copy and paste the snippets below into your config based on the desk you have.

The configs are assuming you're using `d1_mini` as your ESP8266 board, since that's what my desk controller board uses, i.e.:

```yaml
esphome:
  platform: ESP8266
  board: d1_mini
```

### UPLIFT v2

```yaml
packages:
  standing_desk: github://shadowkora/esphome-standing-desk/configs/template.yaml
  desk_config: github://shadowkora/esphome-standing-desk/configs/desks/uplift_v2.yaml
```

### Fully Jarvis

```yaml
packages:
  standing_desk: github://shadowkora/esphome-standing-desk/configs/template.yaml
  desk_config: github://shadowkora/esphome-standing-desk/configs/desks/fully_jarvis.yaml
```