## MAXIM



**(i)**



* 512KB Flash Memory
* 128 KB SRAM
* 16KB Cache
* 
* CNN Data RAM -> 512 KB
* 
* 17 Direct General Input Output pins
* NO Bluetooth, No Wifi
* 1.8V/3.3V constraints
* => Many pins are shared with onboard components
* &#x09;-> Causes capacitive Loading -> which limits maximum clock frequencies for high speed signals
* 
* Compatible with Adafruit FeatherWings for adding WiFi, BLE, or LoRa.
* Some tasks require operations other than the ones done by the CNN accelerator which can be done on the M4 CPU. This causes lost parallelism and increases power input. 



## ESP32

**(i)**



* 520KB SRAM
* 320 KB DRAM -> 160 KB usable for storing values, remaining used for runtime allocation as heap
* 200 KB IRAM -> Stores executable code for fast access 
* 
* 2.4 GHz WiFi Model
* Dual Mode Bluetooth ( Classic + BLE )
* Configure the radio mode (Station vs. Access Point) in code.
* It excels in tasks where data needs to move across networks
* Environment Setup:

&#x09;Arduino IDE: You simply install the ESP32 board manager and use C++ libraries.



&#x09;ESP-IDF: The official professional framework from Espressif. It is based on FreeRTOS, allowing you to manage tasks across the two cores 		manually.

###### Limitations:

WiFi vs. ADC2: If you turn on WiFi, you lose access to all ADC2 analog pins. You must use ADC1 pins (GPIO 32–39).

