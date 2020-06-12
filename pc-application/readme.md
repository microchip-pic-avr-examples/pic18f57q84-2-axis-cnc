<!-- Please do not change this html logo with link -->
<a href="https://www.microchip.com" rel="nofollow"><img src="../images/microchip.png" alt="MCHP" width="300"/></a>

# Python PC Application

This is the companion PC application written in Python responsible for
parsing the G-Code and controlling the CNC machine.

## Software Used

<!-- All software used in this example must be listed here. Use unbreakable links!
     - MPLAB® X IDE 5.30 or newer [(microchip.com/mplab/mplab-x-ide)](http://www.microchip.com/mplab/mplab-x-ide)
     - MPLAB® XC8 2.10 or a newer compiler [(microchip.com/mplab/compilers)](http://www.microchip.com/mplab/compilers)
     - MPLAB® Code Configurator (MCC) 3.95.0 or newer [(microchip.com/mplab/mplab-code-configurator)](https://www.microchip.com/mplab/mplab-code-configurator)
     - MPLAB® Code Configurator (MCC) Device Libraries PIC10 / PIC12 / PIC16 / PIC18 MCUs [(microchip.com/mplab/mplab-code-configurator)](https://www.microchip.com/mplab/mplab-code-configurator)
     - Microchip PIC18F-Q Series Device Support (1.4.109) or newer [(packs.download.microchip.com/)](https://packs.download.microchip.com/) -->

- Python 3.7 [(python.org/downloads)](https://www.python.org/downloads/) - must
be Python 3.7.x
- Pyserial 10.0.1 [(pypi.org/project/pyserial)](https://pypi.org/project/pyserial/) - `pip install pyserial`
- Developed and tested on Windows 10

## Setup

Run main.py

Select the comm port of the Nano in the dropdown menu

<img src="../images/disconnected.PNG" alt="MCHP" width="300"/>

Hit connect

<img src="../images/connecting.PNG" alt="MCHP" width="300"/>

The state will change to connected and you will be able to import a
G-code file. Click import

<img src="../images/connected.PNG" alt="MCHP" width="300"/>

Select the G-Code file

<img src="../images/select.PNG" alt="MCHP" width="300"/>

The G-Code will be loaded and parsed. The run option will become avaible,
click it to run the routine.

<img src="../images/run.PNG" alt="MCHP" width="300"/>

<!-- Explain how to connect hardware and set up software. Depending on complexity, step-by-step instructions and/or tables and/or images can be used -->

## Operation

<!-- Explain how to operate the example. Depending on complexity, step-by-step instructions and/or tables and/or images can be used -->

## Summary

<!-- Summarize what the example has shown -->
