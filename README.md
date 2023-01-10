# elegooHidroFarm
Project to manage the hidropinic Farm

## Notes
This Project aims to manage a water pump base on timer and sensors attached to the Arduino / EleGoo.

### Logic

- Read temperature
- Read humidity
- Read simple Water level
- Start / Stop Pump every 3 hours for 15 minutes
    - reset timer if temperature is above 30 C
