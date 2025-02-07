DESCRIPTION:
A device that takes input from an arbitrary waveform generator and outputs that same waveform, but current boosted.  The purpose of this application is to simulate “bad”, external power rail conditions to the EUT (Equipment Under Test).  Often in the development of embedded electronics, the embedded device is just a sub-system of a larger system.  In many such cases the larger system provides power to that sub-system.  In many such cases the power provided to the sub-system is influenced negatively by the other components of that system.  Such influences include, but are not limited to:
•	Brown outs
•	Black outs
•	Power line ripple
•	Regenerative effects 
•	Weird harmonics created by heavy loading devices
Using an arbitrary waveform generator to recreate the negative effects of the supply rail this device can be used to power your EUT while it is on your desk and not installed within the unit itself.  The primary benefit of this device is singular in purpose.  As system development is often done as many sub-systems coming together, your sub-system can be tested independently of the system’s supply rail.  This allows the engineer to perform mitigation against the offending supply rial, or to study its impact on the EUT without actually having to be installed within the system.  A break through in the work flow of embedded system development.

This project is composed of a hardware and firmware component.  There are also supporting specification, design and other related documents 

BASIC SPECIFICATION:
PARAMETER	LIMIT	NOTES
Max capacitive loading	300uF	At the bandwidth limit
Max Output Swing	±15V	
Max Phase Lag	-5°	10KHz loaded to 300uF
Max Power 	TBD	
		

