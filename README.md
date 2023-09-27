# MSI hardware management module for Linux

**This kernel module is designed to access the embedded controller via ACPI and expose it to userspace.**

**There's no DMI protection yet so DO NOT try this module on unsupported devices.**

Supported firmware(s):
 - 17F4EMS1



Supported functionalities:
 - Read/Write CPU/GPU fan speed configuration
 - Read/Write fan mode
 - Read/Write performance mode
 - Read/Write battery charging threshold
 - Toggle Cooler Boost
 - Read realtime CPU/GPU fan speed, fan RPM and temperature
 - Read keyboard backlight state


To install this module via DKMS, run:
`# make install`

To uninstall this module from DKMS build tree, run:
`# make uninstall`

To compile the kernel object manually, run:
`$ make module`

To clean generated files, run:
`$ make clean`
