# TunnelProxy

## Issues
**Only works with wired connection for now. Don't test on wifi. Because wifi has problems bridging. More time needed**
## Getting started

### Installing first time
1) Compile the program via make
```
make
```
2) After that you need to determine your default network device
```
chmod +x print_default_interface.sh
./print_default_interface.sh

(E.g output)
enp4s0 or eth0 (depends on your default network configuration)
```
3) Next If you are setting up program for the first time configure.sh needs to be called
  It will create bridge with the default network device and configure routing.
  
  It will automatically run the c++ program which will print the incoming packets
  
  **it needs to run in kernel mode**  
  **first parameter is your default network device, see the step 2**

```
sudo ./configure.sh enp4s0 11.11.11.11 1080
```
### Running program
1) **You need to configure first time installation first. See above.**  
2) After that you can simply run
```
sudo ./tunproxy 11.11.11.11 1080

```
### After running program you should see following udp packet flow
```
*------------------------------------------------*
|          RECEIVED PACKET: 0                    |
*------------------------------------------------*
|                   SIZE: 173                    |
*------------------------------------------------*
|4b 41 4e 4e 4f 55 25 4e 00 00 00 00 00 50 c7 bf |
|1b 9d dc 41 72 63 68 65 72 20 43 31 32 30 30 00 |
|00 00 00 41 72 63 68 65 72 20 43 31 32 30 30 00 |
|00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |
|00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |
|00 00 00 00 00 00 00 31 2e 30 32 2e 38 30 00 00 |
|00 00 00 00 00 00 00 00 00 00 00 00 00 01 00 00 |
|00 01 00 00 00 02 00 00 00 02 00 00 00 00 00 00 |
|00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |
|00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |
|00 00 00 00 00 00 00 00 00 00 00 00 00
*------------------------------------------------*
| K  A  N  N  O  U  .  N  .  .  .  .  .  P  .  . |
| .  .  .  A  r  c  h  e  r  .  C  1  2  0  0  . |
| .  .  .  A  r  c  h  e  r  .  C  1  2  0  0  . |
| .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . |
| .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . |
| .  .  .  .  .  .  .  1  .  0  2  .  8  0  .  . |
| .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . |
| .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . |
| .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . |
| .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . |
| .  .  .  .  .  .  .  .  .  .  .  .  .
*------------------------------------------------*
```
