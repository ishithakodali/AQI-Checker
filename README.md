# AQI-Checker
*Code and test file for an AQI Checker implemented as a Simple Reflex Agent.*  
The program reads environmental sensor values from a .txt file and determines the **Air Quality Index (AQI)** category for the region.

# Project Description
This project simulates a simple reflex agent in artificial intelligence.  
The agent:  
1. Reads environmental parameters from sensors (simulated using a text file).
2. Applies predefined rules to the sensor values.
3. Determines the Air Quality Index (AQI) level of the environment.
4. The agent reacts only to the current percept, without memory of past states.

# Input Format
PM2.5:<value>  
PM10:<value>  
CO:<value>  
SO2:<value>  
NO2:<value>  
O3:<value>  
TEMP:<value>  
HUMIDITY:<value>  

# How to Compile
Use GCC to compile the program  
`gcc aqi_checker.c -o aqi`

# How to Run
`./aqi sensor_data.txt `  
where sensor_data.txt is the file containing the environmental parameters. The .txt file can have any name.

# Output
After reading the sensor data from the input file, the program displays:  
1. The sensor readings
2. The calculated AQI value
3. The AQI category
4. The AQI scale position
5. Recommended actions based on the air quality level

# Files in the Repo
aqi_checker.c        -> main program
sensor_data.txt      -> sample sensor input
README.md            -> project documentation

# Concepts Used
1. Simple Reflex Agent (Artificial Intelligence)
2. Rule-based decision making
3. File handling in C 
