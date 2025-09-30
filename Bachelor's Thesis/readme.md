### BSc Thesis
#### Development of an Obstacle Avoidance System for FPV Drones

This engineering thesis presents the design and implementation of an experimental obstacle detection system for FPV drones, aimed at supporting beginner pilots during the learning process.

##### The solution integrates:

- Arduino platform (as an intermediary microcontroller),
- ultrasonic distance sensors (HC-SR04),
- MSP protocol for communication with the drone’s flight controller,
- custom 3D-printed housing for protection and modularity.

##### Key Features

- Obstacle detection up to 2 meters.
- Dual feedback system: acoustic signal + on-screen display (OSD) warning in FPV goggles.
- Configurable operation modes: active, semi-active, or deactivated by the pilot.
- Compact, modular, and low-cost design.

##### Testing

- Laboratory tests confirmed stable sensor readings with occasional noise.
- Experimental flights proved the system effective at low speeds (<10 km/h), allowing beginner pilots to react in time.

##### Conclusions

- The system successfully improves pilot safety and reduces the risk of crashes during training.
- It is not an autonomous obstacle avoidance system but an early-warning tool (similar to a car’s parking sensor).

Future development could include LiDAR/ToF sensors, dynamic thresholds based on flight speed, and multi-sensor configurations.

Note: This is an experimental project intended for educational purposes and early-stage FPV training.