UNIT TESTING:

Ultrasound Sensor Testing:
1. Navigate to ultrasound_testing --> ultrasound_unit_testing and open the module in energia (ultrasound_unit_testing.ino)
2. Set up a launchpad with a ultra sound monitor (I used a breadoard)
3. Change the DEBUG_MODE macro at the top to true
4. Upload the code to your launch pad (in my case, I have a breadboard containing a ultrasound sensor and four jumper wires for the minimum neccessary connections)
5. Open the serial monitor to view how many of the tests have passed (I used the structure we covered in class) 
This tests for a valid distance measured, valid measurment based on parking spot size, occupation status, and parking status


Radio Testing:
1. For a non-modular version, we can use the TX/RX examples in the buffet repo
2. For a modular design, navigate to radio_testing --> radio_unit_testing and open the module in energia (radio_unit_testing.ino)
3. Set up two launchpads with a cc1101 radio (I also used two seperate computers)
2. Set up a TX launchpad upload the code with RECEIVING and DEBUG_MODE set to false
3. Set up a RX launchpad upload the code with RECEIVING and DEBUG_MODE set to true
6. If we are running the code from upload (on the receiving launchpad), you should see that the serial monitor prints "Entering Receiving Mode..."
7. Once this happens, plug in the transmitting launchpad
8. You can watch the packets being received by the launchpad under test until the point were the tests begin to run and you can see how many have passed
This tests to ensure that the correct number of packets were received, verifies the value of the last packet recevied, and the sum of all packets received

...
...

-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
INTEGRATION TESTING:

Radio Ultrasound Testing:
1. To test the combination of the radio and utlrasound modules, nagivate to integration_testing --> ultrasound_radio_testing and open the module in energia (ultrasound_radio_testing.ino)
2. Set up a TX launchpad with a radio and an ultrasound and upload the code with RECEIVING and DEBUG_MODE set to false
3. Set up a RX launchpad with a radio and upload the code with RECEIVING and DEBUG_MODE set to true
4. If we are running the code from upload (on the receiving launchpad), you should see that the serial monitor prints "Entering Receiving Mode..."
5. Once this happens, plug in the transmitting launchpad
6. You can watch the packets being received by the launchpad under test until the point were the tests begin to run and you can see how many have passed
This tests to make sure the correct number of packets are received and that the final parking state is accurately transmitted and calculated from the (valid) transmitted distances

...
...