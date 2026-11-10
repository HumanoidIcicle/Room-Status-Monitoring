# Room-Status-Monitoring
## Description
The materials used in the system were SCD40 sensors, two DHT11 sensors, two active buzzers, two RGB LEDs, two jumper wires, three ESP32-WROOM32 microprocessors and Arduino IDE to program the processors.

Two ESP32 are used as the sensor nodes in different group rooms throughout the day and one as the sink node receiving the data from the sensor nodes for analysis. 
I2C communication is being used between the sensor node ESPs and SCD40 sensors, and the data is forwarded to the sink node through MQTT broker.  The sink node further analyses the carbon dioxide by comparing to threshold values and sets an appropriate rating on the room. 
The rating, 1, 2 or 3, is sent back to the source node.

Furthermore, the sink node handles the visualisation of the room’s status, i.e. received sensor data, along with final rating, on the serial monitor in the Arduino IDE. 
To provide real-time feedback to users, a special alert subsystem is included. On receipt of the rating from the sink node, the sensor node controls an RGB light and a buzzer. The RGB colour represents the comfort status of the room; Red (Rating 3) for poor quality, Blue (Rating 2) for moderate and Green (Rating 1) for good air quality. The buzzer goes off when the quality rated 3, reminding the students to take a break and get some fresh air. 

## Connection diagram
<img width="794" height="360" alt="image" src="https://github.com/user-attachments/assets/c878423e-9b0f-4a90-b5aa-12d3020c4aa3" />

## Implementation
The code for the system is written in the Arduino IDE which uses a variant of C++ programming language and is divided into modules separated by concern and for easy management and troubleshooting. 

### Sensor node
The sensor nodes do most of the work and are divided into two header files, “sensors.h” and “mqtt.h”. 

#### Sensors.h /.cpp
Declares four functions: init_scd40, DHT11_values, SCD40_values and data_to_send.

Firstly, a Json object is created to hold the data from all the sensors.
DHT11_values declares and initializes key: value pairs of the sensor’s humidity and temperature and adds them to the data object. 
Init_SCD40 function sets up the I2C communication between SCD40 sensor and the ESP32 and starts the SCD40. 
The SCD40_values function gets the individual CO2, humidity and temperature readings and appends them to the data object. 

Finally, the data_to_send function calls all the above functions, so that a new data object is created every time it’s called. After all the parameters are added, the data object is serialized and converted into a format that can be transmitted over MQTT. 

#### Mqtt.h /.cpp
This header file contains all the functions needed to setup smooth and efficient communication between the sensor nodes and the sink node. 
With functions connect_wifi () and reconnect_mqtt () handling the initialization and connection of the ESP32 to Wi-Fi and MQTT broker respectively. 

Since sensor nodes need to receive a rating from the sink node, the reconnect_mqtt function also subscribes each node to a separate topic for example results/room1and results/room2.

For this reason, a callback function is also defined to interpret the rating received. Inside the callback, RGB_setup function is called to control the RGB light and the buzzer based on the received rating passed as argument. 

### Sink node
Besides the connect_wifi and reconnect_mqtt functions, the sink node module has two other crucial functions; air_analysis function that handles the data analysis and feedback to the sensor nodes, and callback function to interpret the data from the sensor topics.

#### Air_analysis function
Taking the sensor’s measured air quality as parameter, the function compares the data with three threshold values and gives the room a rating, accompanied by a small phrase of recommended action.
The ratings (as described in the System Overview) and action are defined as a global variable since they are not only used in this function. 

#### Callback function
The function begins by creating a Json object that will hold the published data from the sensor nodes and proceeds to deserialize and initialize the objects fields.

To better handle data from multiple nodes, the data specifies the node from which it comes, in this case Room 1 and Room 2.

After checking for the name of the node, the function sends the rating to the appropriate results topic that the sensor node reads to control the RGB light and buzzer.
Then the air_analysis function is called with the air_quality passed as argument and finally all the rooms details are printed neatly on the serial monitor.

## Example Visualisation
<img width="504" height="431" alt="image" src="https://github.com/user-attachments/assets/5606f0c9-3519-4bee-8942-581213e2bebd" />

