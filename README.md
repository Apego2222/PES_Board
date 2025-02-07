<!-- 
    Styling Rules:
    - When ever possible *.md [Links] should be used, and not just bold text
    - In a *.md [Link] all words start with a capital letter, e.g.: [Course setup]
    - A button or a pin or something similar is highlited bold: **USER BUTTON**, **PB_9**
    - Source code files are highlited bold and cursive: ***main.cpp***
    - Functions, objects and variables when not in a code snippet are formatted like this:
      - ``main()`` function
      - ``servo.enable()`` function
      - ``while()`` loop
      - ``if()`` statement
      - ``mechanical_button`` object
      - ``enable_motors`` variable
      - ``DCMotor.h`` driver
    Authors:
    - Maciej Szarek (szar)
    - Peter Michael (pichim/pmic)
 -->

<!-- link list -->
[0]: https://os.mbed.com/platforms/ST-Nucleo-F446RE/
[1]: https://www.st.com/en/microcontrollers-microprocessors/stm32f446re.html#documentation

# PES Board - Hardware and Driver Documentation

| Michael Peter | Michael Wüthrich | Camille Huber |
| ------------- | ---------------- | ------------- |
| pmic@zhaw.ch  | wuem@zhaw.ch     | hurc@zhaw.ch  |
| TE 307        | TE 301           | TE 307        |

Big shoutout to Maciej Szarek for his help and the support (https://github.com/szar99).

<p align="center">
    <img src="docs/images/fast_prototyping.jpg" alt="Fast Prototyping" width="650"/> </br>
    <center> <i>Fast Prototyping</i> </center>
</p>

Fast prototyping in robotics focuses on quickly building and testing a simple version of the system rather than aiming for perfection from the start. Through iterative development, each prototype helps identify and fix issues, leading to gradual improvements. This approach saves time and cost, encourages experimentation, and ensures that the final design is optimized based on real-world performance. Additionally, hardware and software evolve together, allowing adjustments to both as new challenges arise. Instead of spending too much time planning, build, test, and refine—learning from each iteration.

# Table of Contents
1. [Course Setup](#course-setup)
    * [Accounts](docs/markdown/general_info.md#accounts)
    * [Software](docs/markdown/general_info.md#software)
    * [GitHub](docs/markdown/general_info.md#github)
    * [Arm MBed](docs/markdown/general_info.md#arm-mbed)
2. [Hardware](#hardware)
    * [Nucleo F446RE](#nucleo-f446re)
        * [Nucleo Pinmap][0]
    * [PES Board](#pes-board)
        * [Pheripherals](#pheripherals)
        * [Pin-Mapping](#pin-mapping)
    * [Hardware Tutorials](#hardware-tutorials)
        * [IR Sensor](docs/markdown/ir_sensor.md)
        * [Ultrasonic Sensor](docs/markdown/ultrasonic_sensor.md)
        * [Servo](docs/markdown/servo.md)
        * [DC Motor](docs/markdown/dc_motor.md)
        * [Line Follower](docs/markdown/line_follower.md)
3. [Tips](#tips)
    * [Project Development](docs/markdown/tips.md#project-development)
    * [Programming](docs/markdown/tips.md#programming)
        * [Main file description](docs/markdown/main_description.md)
    * [Structuring a Robot Task](docs/markdown/tips.md#structuring-a-robot-task)
4. [Workshops, Solutions and Examples](#workshops)
    * [Workshop 1](docs/markdown/ws1.md)
    * [Workshop 2](docs/markdown/ws2.md)
    * [Workshop 3](docs/markdown/ws3.md)
5. [Build Mbed OS project with VScode](#build-mbed-os-project-with-vscode)
    * [Build on Windows](docs/markdown/build_mbed_windows.md)
    * [Build on Linux/WSL](docs/markdown/build_mbed_linux.md)
6. [Links](#links)

## Course Setup

In order to be able to complete the course, one must first register on the following platforms and obtain the appropriate tools. All the information needed to start the course can be found in [General Info](docs/markdown/general_info.md).

This document covers all the information about: 

- [Accounts](docs/markdown/general_info.md#accounts)
- [Software](docs/markdown/general_info.md#software)
- [GitHub](docs/markdown/general_info.md#github)
- [Arm MBed](docs/markdown/general_info.md#arm-mbed)
  
## Hardware

During the course, we will use the Nucleo-F446RE board from ST in combination with the PES board designed and developed at ZHAW. The basic hardware kid that students receive includes a variety of sensors and actuators for practical applications.

>**IMPORTANT NOTE:**
>
>- <b>When working with hardware (connecting, reconnecting etc.), it is recommended that all power sources are disconnected. This is a general safety measurement! So for us, the Nucleo is disconnected and the PES Board **Power Switch** is **OFF** when ever we change someting at the hardware setup.</b>
>- <b>The USB cable should only be connected to the computer after the power switch has been switched on.</b>
>- <b>To not connect the charger when the battery packs are not connected, otherwise the PES Board will be damaged.</b>
>- <b>Using the PES Board with power ON and hardware running while connected to your computer/laptop happens at your own risk. There was a case where the students laptop was damaged in the past.</b>
>- <b>Various Nucleo boards, PES Borads and even laptops have been damaged in the past, so make sure to stick to the rules above.</b>

### Nucleo F446RE

The Nucleo-F446RE is a microcontroller development board featuring the STM32F446RET6 microcontroller from STMicroelectronics. It provides a versatile platform for prototyping and developing embedded systems, offering a wide range of peripherals, connectivity options, and compatibility with the Arduino and ST Morpho ecosystems.

- [STM32F446RE Documentation][1] 

<p align="center">
    <img src="docs/images/nucleo_overview.png" alt="Nucleo F446RE Overview" width="600"/> </br>
    <i>Nucleo F446RE Overview</i>
</p>

Arm MBed provides a dedicated platform with essential information about the development board. Here you can find technical specifications and the pinmap.

- [Mbed ST-Nucleo-F446RE</font>][0]

### PES Board

The PES Board is a hardware board with additional sensors, devices and power electronics to work in combination with the Nucleo F446RE. It provides multiple pinouts for:

- 3 DC-Motor (brushed)
- 4 Servos (these occupy the 4 DI/O if used)
- 4 DI/O, 3.3V (5V tolerant)
- 4 AI/O, 3.3V (5V tolerant)
- 3 Encoder-Counter
- 9-axis IMU (accelerometer, gyroscope, magnetometer)

>**IMPORTANT NOTE:**
>- <b>The voltage of the DO (servos) is set via the switch behind the charging socket: 3.3 V or 5 V</b>
>- <b>Motor encoder soldering can be wrong. Do not assume that if you plug in one motor and everything works you can then also use the same connections with different motor. You have to make sure that the physical rotation is according to your assumption and that a positive input leads to a positive change of increments.</b>
>- <b>Depending on the PES Board version DCMotor M3 rotation direction might be inverted.</b>
>- <b>Depending on the PES Board version the Pin map might differ.</b>

#### Batteries

The kit includes two sets of 6V battery packs, which can be connected in series to provide 12 volts. The battery packs need to be connected to the back of the PES Board. The picture below illustrates the proper battery connection required to get a total voltage of 12V.

<p align="center">
    <img src="docs/images/battery_packs.png" alt="Battery Packs" width="700"/> </br>
    <i>Battery Packs</i>
</p>

The batteries enables the board to be powered independently of the connection to the computer/laptop, eliminating the need for a connection via the Mini USB cable. The board continues to receive a stable 5V supply while offering the option to use a 12V supply for the power electronics of the motors. To activate the external battery power, switch the slider on the PES Board to the ON position.

<b>Single battery pack</b> - if you are using a single battery pack, the remaining pins need to be bridged. If only 6 V is used, this must be parameterised accordingly in the firmware when parameterising motion controller class.

#### Charging the Batteries

<b>Using the Charger</b> - if you connect the charger to the PES Board, the battery packs need to be connected. If the battery packs (2 packs for 12 volts or one pack and a jumper for 6 volts) are not connected when you plug in the charger, the PES Board will be destroyed.

<b>Charging batteries</b> - the battery packs are only charged when the PES Board circuit breaker is set to OFF.

<b>Usage while charging</b> - don't use the PES Board while it is charging.

#### Resources

All additional technical information such as schematics and pin maps for the PES Board can be found in a folder <b>docs/datasheets/pes_board_data/</b>. Also included there are CAD files of the combined Nucleo F446RE and PES Board in .3dxml extensions (for 3Dexperience).

#### Pheripherals

<p align="center">
    <img src="docs/images/pes_board_peripherals.png" alt="PES Board Pheripherals" width="900"/> </br>
    <i>PES Board pheripherals</i>
</p>

- [pes_board_peripherals.pdf](docs/datasheets/pes_board_peripherals.pdf)

#### Pin-Mapping

- [pes_board_pin_mapping.pdf](docs/datasheets/pes_board_pin_mapping.pdf)

### Hardware Kit

- 2x Battery pack 6v / 2300mAh with charger
- 1x Servo – REELY S-0090
- 1x Servo – FUTABA S3001
- 1x LED green with resistor 2200/600mW/1%
- 1x Distance sensor SHARP GP2YOA41 analog 300mm with cable
- 1x DC Motor POLOLU (different gear ratios) D = 20 mm/ l = 43 mm/ 12V with encoder POLOLU 2.7 – 18V
- 1x Mechanical Button
- 1x Ultrasonic sensor GROVE ULTRASONIC RANGER V2.0 with cable

### Hardware Tutorials

The following links contain the hardware tutorials. The documents contain specifications and technical information about the hardware and information about how to use the hardware. The tutorials cover the software drivers, specific calibration procedures, controlling actuators and retrieving measurements from the sensors.

- [IR Sensor](docs/markdown/ir_sensor.md)
- [Ultrasonic Sensor](docs/markdown/ultrasonic_sensor.md)
- [Servo](docs/markdown/servo.md)
- [DC Motor](docs/markdown/dc_motor.md)
- [Line Follower Array](docs/markdown/line_follower.md)
- [IMU](docs/markdown/imu.md)

## Tips

Tips that you might find useful can be found in the document [Tips](docs/markdown/tips.md). Here you can find information about: 
- [Project Development](docs/markdown/tips.md#project-development)
- [Programming](docs/markdown/tips.md#programming)
- [Structuring a Robot Task](docs/markdown/tips.md#structuring-a-robot-task)

## Workshops, Solutions and Examples

The following links contain the workshops instructions:
- [Workshop 1](docs/markdown/ws1.md)
- [Workshop 2](docs/markdown/ws2.md)
- [Workshop 3](docs/markdown/ws3.md)

And here you find all the solutions, as well as some additional examples:
- [Solution Workshop 1](docs/solutions/main_ws1.cpp)
- [Solution Workshop 2, Part 1](docs/solutions/main_ws2_p1.cpp)
- [Solution Workshop 2, Part 2](docs/solutions/main_ws2_p2.cpp)
- [Solution Workshop 3, Part 1](docs/solutions/main_ws3_p1.cpp)
- [Solution Workshop 3, Part 2](docs/solutions/main_ws3_p2.cpp)
- [Example Line Follower](docs/solutions/main_line_follower.cpp)
- [Example Gimbal](docs/solutions/main_gimbal.cpp)
- [Example Line Follower Base Example](docs/solutions/main_line_follower_base_ss24.cpp)
- [Example Differential Drive Robot Kinematics Calibration](docs/solutions/main_calib_kinematic_ss24.cpp)
- TODO: Add stepper motor example here

In the folder (docs/cad/WS2) and (docs/cad/WS3) you will find the printable files needed to build the demonstration model for workshop 2 and workshop 3. For workshop 3, you need to edit the parts: rack and gear wheel because they are only available in STL print-ready format. Editing can be done in Prusa Slicer, which offers the possibility of minor modification.
- Rack - one hole should be added, in order to screw the rack to the carriage on which the sensor is located, or possibly lengthen this element (details available: https://www.printables.com/de/model/522220-universal-gear-rack-collection-different-modules)
- Make a hole in the element for the motor shaft (details available: https://www.printables.com/de/model/516177-universal-spur-gears-collection-module-15)

### Additional Information

- [Differential Drive Kinematics](docs/markdown/kinematics.md)

## Weblinks

### Some General Links

- Git and GitHub Tutorial for Beginners from 32:40 about Github: https://www.youtube.com/watch?v=tRZGeaHPoaw
- C++ step by step tutorials: https://www.w3schools.com/cpp/default.asp
- Detailed explanation of C++ programming language: https://cplusplus.com/
- C++ data types: https://www.tutorialspoint.com/cplusplus/cpp_data_types.htm
- Printf format convention: https://cplusplus.com/reference/cstdio/printf/
- Flowchart diagram maker: https://app.diagrams.net/

### Hardware Links

- https://www.pololu.com
- https://www.adafruit.com
- https://www.sparkfun.com
- https://www.seeedstudio.com
- https://www.robotshop.com
- https://boxtec.ch
- https://www.play-zone.ch
- http://farnell.ch
- https://www.mouser.ch
- https://www.digikey.com
- https://www.conrad.ch
- https://www.distrelec.ch

### Online C++ Compiler

- Online C++ Compiler: https://www.onlinegdb.com/online_c++_compiler
