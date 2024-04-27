Parksharks
Team Members:

Charlie Medlock | medloc5@clemson.edu
Eliza Sorber | esorber@clemson.edu
Chloe Crozier | cscrozi@clemson.edu
Harrison Langpaul | hlangpa@clemson.edu
Git Buffet Link: buffet@buffet.cs.clemson.edu:esorber/parksharks

-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
UNIT TESTING:

Ultrasound Sensor Testing:
1. Navigate to ultrasound_testing --> ultrasound_unit_testing and open the module in energia (ultrasound_unit_testing.ino)
2. Set up a launchpad with an ultrasound monitor (I used a breadboard)
3. Change the DEBUG_MODE macro at the top to true
4. Upload the code to your launch pad (in my case, I have a breadboard containing an ultrasound sensor and four jumper wires for the minimum necessary connections)
5. Open the serial monitor to view how many of the tests have passed (I used the structure we covered in class) 
These tests for a valid distance measured, valid measurement based on parking spot size, occupation status, and parking status


Radio Testing:
1. For a non-modular version, we can use the TX/RX examples in the buffet repo
2. For a modular design, navigate to radio_testing --> radio_unit_testing and open the module in energia (radio_unit_testing.ino)
3. Set up two launchpads with a cc1101 radio (I also used two separate computers)
2. Set up a TX launchpad and upload the code with RECEIVING and DEBUG_MODE set to false
3. Set up a RX launchpad and upload the code with RECEIVING and DEBUG_MODE set to true
6. If we run the code from upload (on the receiving launchpad), you should see that the serial monitor prints "Entering Receiving Mode..."
7. Once this happens, plug in the transmitting launchpad
8. You can watch the packets being received by the launchpad under test until the point where the tests begin to run, and you can see how many have passed
These tests to ensure that the correct number of packets were received, verifies the value of the last packet received, and the sum of all packets received.


Microphone Testing:
1. Navigate to microphone_unit_testing and open the module in Energia (microphone_unit_testing.ino).
2. Set up a Launchpad with a microphone and upload the code.
3. Set DEBUG_MODE to true and run the code. The serial monitor will print which tests have passed.
4. For the tests verifying that the LED does/doesn't turn on, manually observe the LED behavior. Ensure that it blinks exactly as expected for each test case.
These tests measure the bounds of the microphone thresholds and detect noise above a specified level. We will later use this threshold to change the polling intervals of our ultrasound.

-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
INTEGRATION TESTING:

Radio Ultrasound Testing:
1. To test the combination of the radio and ultrasound modules, navigate to integration_testing --> ultrasound_radio_testing and open the module in energia (ultrasound_radio_testing.ino)
2. Set up a TX launchpad with a radio and an ultrasound and upload the code with RECEIVING and DEBUG_MODE set to false
3. Set up a RX launchpad with a radio and upload the code with RECEIVING and DEBUG_MODE set to true
4. If we are running the code from upload (on the receiving launchpad), you should see that the serial monitor prints "Entering Receiving Mode..."
5. Once this happens, plug in the transmitting launchpad
6. You can watch the packets being received by the launchpad under test until the point where the tests begin to run, and you can see how many have passed
These tests to make sure the correct number of packets are received and that the final parking state is accurately transmitted and calculated from the (valid) transmitted distances

Radio Microphone Ultrasound Testing:
1. To test the combination of the radio, microphone, and ultrasound modules, navigate to integration_testing --> mic_radio_ultrasound_testing and open the module in energia (mic_radio_ultrasound_testing.ino)
2. Set up a TX launchpad with a radio and an ultrasound and upload the code with RECEIVING and DEBUG_MODE set to false
3. Set up a RX launchpad with a radio and upload the code with RECEIVING and DEBUG_MODE set to true
4. If we are running the code from upload (on the receiving launchpad), you should see that the serial monitor prints "Entering Receiving Mode..."
5. Once this happens, plug in the transmitting launchpad
6. You can watch the packets being received by the launchpad under test until the point where the tests begin to run, and you can see how many have passed. You can also see the microphone tests run from our previous unit testing module
These tests to make sure the correct number of packets are received and that the final parking state is accurately transmitted and calculated from the (valid) transmitted distances and ensures that the microphone responds to the specificed intervals


-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
FINAL DEMO TESTING:

Parking Sensor Demo
1. Navigate to the parking_sensor_demo folder and open the module in energia (parking_sensor_demo.ino)
