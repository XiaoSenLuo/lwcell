# GSM AT commands parser for RTOS systems

GSM-AT Library commands parser is a generic, platform independent, library for communicating with SIMCOM based modules SIM800/SIM900 or SIM70xx. Module is written in C99 and is independent from used platform. Its main targets are embedded system devices like ARM Cortex-M, AVR, PIC and others, but can easily work under `Windows`, `Linux` or `MAC` environments.

<h3><a href="https://majerle.eu/documentation/gsm_at/html/index.html">Documentation</a></h3>

## Features

* Supports ``SIM800/SIM900 (2G)`` and ``SIM7000/SIM7020 (NB-Iot LTE)`` modules
* Platform independent and very easy to port
    * Development of library under Win32 platform
    * Provided examples for ARM Cortex-M or Win32 platforms
* Written in C language (C99)
* Allows different configurations to optimize user requirements
* Supports implementation with operating systems with advanced inter-thread communications
    * Currently only OS mode is supported
    * 2 different threads handling user data and received data
        * First (producer) thread (collects user commands from user threads and starts the command processing)
        * Second (process) thread reads the data from ESP device and does the job accordingly
* Allows sequential API for connections in client and server mode
* Includes several applications built on top of library:
    * MQTT client for MQTT connection
* User friendly MIT license

## Contribute

We always welcome new contributors. To be as efficient as possible, we recommend:

1. Fork Github repository
2. Respect C style & coding rules used by the library
3. Make a pull request to develop branch with new features or bug fixes

Alternatively you may:

1. Report a bug
2. Ask for a feature request