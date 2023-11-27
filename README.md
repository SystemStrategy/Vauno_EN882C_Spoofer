# Vauno_EN882C_Spoofer
RF443 Decoded Vauno EN882C Protocol Retransmitter

Pin 18 goes to tx on STX882 transmitter
Pin 16 goes to ds18b20 sensor with 4.7k ohm pullup to 3.3v

Temperature is from the DS18B20
Humidity is an hourly uptime counter resetting to 1 every 127 hours
Battery status is associated with the DS18B20 connectivity state


Created this project because I don't want to use wifi for everything and this sensor was reporting hot tub temperatures and not readily accessable to replace batteries.

Used https://github.com/merbanan/rtl_433 to aid in encoding the transmission 
