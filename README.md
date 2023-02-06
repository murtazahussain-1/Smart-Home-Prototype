# Smart-Home-Prototype
This system detects an intruder, abnormal temperature condition inside a house and any smoke that is hazardous to human life. On detecting any abnormal activity, an alarm turns on for few seconds to notify the resident. The system sends status of each sensor continuously to a PC connected with it. User can turn off the alarm from PC

### Inputs
* Smoke Sensor (MQ-2)
* Temperature Sensor (LM 35)
* IR-Sensor (MH-B)
* Computer Keyboard	

### Outputs
* Buzzer (MH-FMD)
* Computer Screen

# Method of interfacing
*	Smoke and Temperature sensor are analog devices. Hence, we used ADC modules with them. Moreover, we used ADC interrupts to obtain the output values
*	Buzzer was driven using the PWM module
*	IR-Sensor was just used as a digital input device which sent an interrupt to the PWM module to drive the buzzer
*	Computer was connected with the system using UART protocol at a baud rate of 115200. The status of each and every sensor was sent to the computer so that we could monitor their status on Tera Term and computer can send back data so that user can turn off and, On the buzzer.
*	Keyboard was used as an input to our system through UART.

# Flow Chart (main.c)
![image](https://user-images.githubusercontent.com/118768714/217047496-5685c569-b611-48ac-ac62-f02587d22e49.png)

# Block Diagram
![image](https://user-images.githubusercontent.com/118768714/217048353-02ec1eac-9c5b-4560-ba59-7c4f9627c515.png)

### Simulation of Code:
We were unable to simulate our code in a software due to these constraints:
*	TM4C module was unavailable in Proteus
*	No method of providing smoke to the smoke sensor, object to the IR sensor, heat to the temperature sensor in software

### Shortcomings:
*	IR Sensors have a very small range
*	This is supposed to be a home automation system but every device is fixed on a Veroboard
*	There is some issue in setting priority of the sensors.

### Improvements:
* Better IR sensors with a bigger range can be used to detect intruders instead of the one which we are currently using because the object has to come to a very close proximity to trigger it
*	An alarm should be used instead of a buzzer
*	Computer should be connected via wireless medium
*	There should be an application with Graphics for displaying of result
