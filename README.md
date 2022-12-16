# Neotec NT7108 Demonstration Code
This is Arduino sample code for any display that uses the Neotec NT7108 controller, but has been written specifically for the 128x64 size displays. The code has to be slightly modified for the 192x64 sized displays.

These displays are a part of the graphics family and operate at 5V.

## Connection Guide
```
//  ARD      | Port  | Display pin |  Function - 8080 Parallel   |
//-----------+-------+-------------+-----------------------------+
// 3.3V/5V   |       |             |  POWER 3.3V                 |
// GND       |       |             |  GROUND                     |
//-----------+-------+-------------+-----------------------------+
// D8        | PORTB |             |  Data/Instruction   (D/I)   |
// D9        | PORTB |             |  Read/Write         (R/W)   |
// D10       | PORTB |             |  Enable signal        (E)   |
// D11       | PORTB |             |  Column select 1    (CS1)   |
// D12       | PORTB |             |  Column select 2    (CS2)   |
// D13       | PORTB |             |  Reset              (RES)   |
//-----------+-------+-------------+-----------------------------+
//Data Lines
//-----------+-------+-------------+-----------------------------+
// D0        | PORTD |             |  DATA BUS LINE (DB0)        |
// D1        | PORTD |             |  DATA BUS LINE (DB1)        |
// D2        | PORTD |             |  DATA BUS LINE (DB2)        |
// D3        | PORTD |             |  DATA BUS LINE (DB3)        |
// D4        | PORTD |             |  DATA BUS LINE (DB4)        |
// D5        | PORTD |             |  DATA BUS LINE (DB5)        |
// D6        | PORTD |             |  DATA BUS LINE (DB6)        |
// D7        | PORTD |             |  DATA BUS LINE (DB7)        |
//-----------+-------+-------------+-----------------------------+
```


Crystalfontz Part Numbers:\
[CFAG12864A-CFH-VN](https://www.crystalfontz.com/products/CFAG12864A-CFH-VN)\
[CFAG12864A-TMI-VN](https://www.crystalfontz.com/products/CFAG12864A-TMI-VN)\
[CFAG12864A-YYH-VN](https://www.crystalfontz.com/products/CFAG12864A-YYH-VN)\
[CFAG12864B-TFH-V](https://www.crystalfontz.com/products/CFAG12864B-TFH-V)\
[CFAG12864B-TMI-V](https://www.crystalfontz.com/products/CFAG12864B-TMI-V)\
[CFAG12864B-YYH-N](https://www.crystalfontz.com/products/CFAG12864B-YYH-N)\
[CFAG12864B-YYH-V](https://www.crystalfontz.com/products/CFAG12864B-YYH-V)\
[CFAG12864C-TMI-TN](https://www.crystalfontz.com/products/CFAG12864C-TMI-TN)\
[CFAG12864C-YYH-TN](https://www.crystalfontz.com/products/CFAG12864C-YYH-TN)\
[CFAG12864I-STI-TN](https://www.crystalfontz.com/products/CFAG12864I-STI-TN)\
[CFAG12864I-YYH-TN](https://www.crystalfontz.com/products/CFAG12864I-YYH-TN)\
[CFAG12864K-STI-TN](https://www.crystalfontz.com/products/CFAG12864K-STI-TN)\
[CFAG12864K-TMI-TN](https://www.crystalfontz.com/products/CFAG12864K-TMI-TN)\
[CFAG12864K-YYH-TN](https://www.crystalfontz.com/products/CFAG12864K-YYH-TN)\
[CFAG12864M-TMI-TN](https://www.crystalfontz.com/products/CFAG12864M-TMI-TN)\
[CFAG19264A-TMI-TN](https://www.crystalfontz.com/products/CFAG19264A-TMI-TN)\
[CFAG19264A-YYH-TN](https://www.crystalfontz.com/products/CFAG19264A-YYH-TN)\
[CFAG19264D-TFH-VN](https://www.crystalfontz.com/products/CFAG19264D-TFH-VN)\
[CFAG19264D-TMI-VN](https://www.crystalfontz.com/products/CFAG19264D-TMI-VN)\
[CFAG19264D-YYH-VN](https://www.crystalfontz.com/products/CFAG19264D-YYH-VN)
