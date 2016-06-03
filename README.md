# Overview

This project provides a capability to monitor and control the embedded controller 
(EC) on Gigabyte laptops, specifically models P37Xv5 and P37Wv5, and potentially 
others.

The EC on these laptops is responsible for auto-adjusting the fan speed, however,
this functionality is not activated by default. Further, installing Linux on these
laptops will result in the fan being set to a medium 
speed, which is fairly noisy. Using this program you can adjust the fan settings
similarly to what is available via Gigabyte's SmartManager tool on the Windows 
operating system. 

Additionally, there are other values that can be read and adjusted, such as the
ambient light sensor, keyboard backlight setting, temperatures, as well as wifi,
bluetooth, camera, touchpad, and even USB charge capabilities during sleep and
hibernate.

However, the source code in this project is command-line accessible only, and users
must be cautious of any modifications to the EC registers. Because this program
allows any value to be written to any register it is possible to cause undesired
results, with potentially physically damaging results. Therefore, do not use this
program unless you accept the risks mentioned above.

# Instructions

First, ensure g++ is available on your distribution. Then compile via the 
following command:
`g++ p37-ec.c -o p37-ec`

To run and view current EC values (and enable the EC fan controller):
`./p37-ec`

Once the above command is execute you will see output similar to the following:
  Usage: sudo ./p37-ec [<hex-offset[.bit]> <hex-value>]
    Ex: sudo ./p37-ec 0x01.6 0x07

  Current Embedded Controller Values:
    USB Charge During Sleep     [0x01.5]: 1
    USB Charge During Hibernate [0x07.2]: 0
    Camera Enabled              [0x01.6]: 0
    Bluetooth Enabled           [0x01.7]: 1
    WiFi Enabled                [0x02.6]: 1
    Touchpad Enabled            [0x03.5]: 1
    Ambient Light               [0x66]:   1%
    Screen Enabled              [0x09.3]: 0
    Keyboard Backlight Mode     [0xD7]:   1
    CPU Temp                    [0x60]:   32 C
    GPU Temp                    [0x61]:   0 C
    MLB Temp                    [0x62]:   25 C
    Fan0 Speed                  [0xFC]:   2301 RPM
    Fan1 Speed                  [0xFE]:   2269 RPM
    Fan Control Enabled         [0x13.3]: 1
    Fan Quiet Mode Enabled      [0x08.6]: 1
    Fan Gaming Mode Enabled     [0x12.4]: 0
    Fan Custom Mode Enabled     [0x13.0]: 0
    Fan0 Custom Speed Setting   [0xB0]:   35%
    Fan1 Custom Speed Setting   [0xB1]:   35%
    Current Speed Setting       [0x64]:   0

The hexadecimal value in the square brackets is the offset within the EC for that 
register. The 0x13.3 notation means bit 3 (starting at 0 from right to left) of 
register 0x13.

To modify an EC register (Ex: turn on fan to full speed):
`sudo ./p37-ec 0x64 0x7`

Note that even after executing the above example, the EC will detect that the 
temperature is cool enough to lower the fan speed and will do so in steps until
it reaches the corresponding speed suitable for the current temperature values.

*This project is not affiliated with GIGA-BYTE Technology Co. Ltd.*