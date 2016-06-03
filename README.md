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

To build:

1) Ensure g++ is available on your distribution.
2) Compile: `g++ p37-ec.c -o p37-ec`

To run and view current EC values (and enable the EC fan controller):
`./p37-ec`

To modify an EC register (Ex: turn on fan to full speed):
`./p37-ec 0x64 0x7`

Note that even after executing the above example, the EC will detect that the 
temperature is cool enough to lower the fan speed and will do so in steps until
it reaches the corresponding speed suitable for the current temperature values.

*This project is not affiliated with GIGA-BYTE Technology Co. Ltd.*