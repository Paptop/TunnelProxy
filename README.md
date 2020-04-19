# TunnelProxy

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
