# Parksharks Team Members:

- Chloe Crozier | [cscrozi@clemson.edu](mailto:cscrozi@clemson.edu)
- Eliza Sorber | [esorber@clemson.edu](mailto:esorber@clemson.edu)
- Charlie Medlock | [medloc5@clemson.edu](mailto:medloc5@clemson.edu)
- Harrison Langpaul | [hlangpa@clemson.edu](mailto:hlangpa@clemson.edu)

**Git Buffet Link:** [buffet@buffet.cs.clemson.edu:esorber/parksharks](buffet@buffet.cs.clemson.edu:esorber/parksharks)

---
Our board's ultrasound sensor detects an object within half of a pre-set 'parking spot' distance. Once the object has been detected for a given number of cycles, it is considered parked, and the LEDs toggle from blue (unoccupied) to red (occupied). After an object is parked, we turn on microphone polling to detect low-frequency noises that suggest the object may be leaving the spot (such as engine rumble or footsteps). We speed up the ultrasound sensor's polling rate when the yellow LED indicates noise above the set threshold. When the ultrasound no longer detects an object, we turn off the microphone polling and decrease the ultrasound polling rate back to the original slow cycle. We have a button to turn the system on/off and a button to turn debug mode on/off (LED indicators). We use a switch to select the potentiometer path when correcting envelope distortions in our microphone filtering. We utilize radio communication to transmit information regarding the status of a parking spot (whether it's empty or occupied) and the duration for which an object has been parked.


<p align="center">
  <img alt="assembled_board" src="https://github.com/ChloeCrozier/parking_sensor/assets/40836363/cc4f9a4d-c46a-4614-bfc9-053440b35001">
</p>

---

## UNIT TESTING:

### Ultrasound Sensor Testing:

1. Navigate to `ultrasound_testing --> ultrasound_unit_testing` and open the module in Energia (`ultrasound_unit_testing.ino`).
2. Set up a Launchpad with an ultrasound monitor (e.g., a breadboard).
3. Change the `DEBUG_MODE` macro at the top to `true`.
4. Upload the code to your Launchpad.
5. Open the serial monitor to view how many of the tests have passed. These tests validate a valid distance measured, valid measurement based on parking spot size, occupation status, and parking status.

### Radio Testing:

1. For a non-modular version, use the TX/RX examples in the Buffet repository.
2. For a modular design, navigate to `radio_testing --> radio_unit_testing` and open the module in Energia (`radio_unit_testing.ino`).
3. Set up two Launchpads with a CC1101 radio.
4. Set up a TX Launchpad and upload the code with `RECEIVING` and `DEBUG_MODE` set to `false`.
5. Set up a RX Launchpad and upload the code with `RECEIVING` and `DEBUG_MODE` set to `true`.
6. Plug in the transmitting Launchpad after the code starts running on the receiving Launchpad. Verify that the correct number of packets were received, and that the value of the last packet received and the sum of all packets received are accurate.

### Microphone Testing:

1. Navigate to `microphone_unit_testing` and open the module in Energia (`microphone_unit_testing.ino`).
2. Set up a Launchpad with a microphone and upload the code.
3. Set `DEBUG_MODE` to `true` and run the code. The serial monitor will print which tests have passed.
4. For the tests verifying that the LED does/doesn't turn on, manually observe the LED behavior. Ensure that it blinks exactly as expected for each test case. These tests measure the bounds of the microphone thresholds and detect noise above a specified level.

---

## INTEGRATION TESTING:

### Radio Ultrasound Testing:

1. Navigate to `integration_testing --> ultrasound_radio_testing` and open the module in Energia (`ultrasound_radio_testing.ino`).
2. Set up a TX Launchpad with a radio and an ultrasound and upload the code with `RECEIVING` and `DEBUG_MODE` set to `false`.
3. Set up a RX Launchpad with a radio and upload the code with `RECEIVING` and `DEBUG_MODE` set to `true`.
4. Plug in the transmitting Launchpad after the code starts running on the receiving Launchpad. Verify that the correct number of packets are received and that the final parking state is accurately transmitted and calculated from the valid transmitted distances.

### Radio Microphone Ultrasound Testing:

1. Navigate to `integration_testing --> mic_radio_ultrasound_testing` and open the module in Energia (`mic_radio_ultrasound_testing.ino`).
2. Set up a TX Launchpad with a radio and an ultrasound and upload the code with `RECEIVING` and `DEBUG_MODE` set to `false`.
3. Set up a RX Launchpad with a radio and upload the code with `RECEIVING` and `DEBUG_MODE` set to `true`.
4. Plug in the transmitting Launchpad after the code starts running on the receiving Launchpad. Verify that the correct number of packets are received and that the final parking state is accurately transmitted and calculated from the valid transmitted distances. Ensure that the microphone responds to the specified intervals.

---

## FINAL DEMO TESTING:

### Parking Sensor Demo:

1. Navigate to the `parking_sensor_demo` folder and open the module in Energia (`parking_sensor_demo.ino`).
