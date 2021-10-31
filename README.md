# ESPHome Standing Desk Component

![image](https://user-images.githubusercontent.com/2646487/139561642-6913a43d-792b-47e1-9d63-a605dfa33bde.png)

This is an ESPHome component that can be used to read the current height from standing desk controllers which communicate over UART. It has been tested to work with Uplift Desk controllers.

## Usage

Copy the `standing_desk.h` file to the root of your ESPHome config directory. (I'll maybe make it an [external component](https://esphome.io/components/external_components.html) later, but I'm lazy.)

Then in your node's configuration, add the following:

```yaml
uart:
  id: uart_bus
  rx_pin: D1 # Change this to whatever you're using for RX
  baud_rate: 9600

sensor:
  - platform: custom
    lambda: |-
      auto standing_desk = new StandingDeskComponent(id(uart_bus));
      App.register_component(standing_desk);
      return {standing_desk};
    sensors:
      id: desk_height
      name: "Standing Desk Height"
      icon: mdi:human-male-height-variant
      unit_of_measurement: in # Change to cm if your controller is set to output in cm
      accuracy_decimals: 1
```

Additionally, if you're using Home Assistant and have pins connected to the up and down buttons, you can add the following to your node's configuration to add a service which will set the desk height to the specified value:

```yaml
api:
  services:
    - service: set_height
      variables:
        height_in: float
      then:
        - if:
            condition:
              lambda: "return static_cast<StandingDeskComponent *>(id(desk_height))->get_last_read() > height_in;"
            then:
              - output.turn_on: down_pin
              - wait_until:
                  lambda: "return static_cast<StandingDeskComponent *>(id(desk_height))->get_last_read() <= height_in + 0.4;"
              - output.turn_off: down_pin
            else:
              - output.turn_on: up_pin
              - wait_until:
                  lambda: "return static_cast<StandingDeskComponent *>(id(desk_height))->get_last_read() >= height_in - 0.4;"
              - output.turn_off: up_pin
```

Et voilà. You can now control your standing desk via the internet.

## Hardware

For more information on how to wire up the hardware, see [this great blog post](https://embedded-elixir.com/post/2019-01-18-nerves-at-home-desk-controller/). I used it as reference for my own, but it turns out my pin layout was slightly different. So make sure to check with a multimeter before doing anything.