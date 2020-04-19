# TunnelProxy
### Installing
1) Compile the program via make
```
make
```
2) After that you need to determine your default network device
```
chmod +x print_default_interface.sh
./print_default_interface.sh

(E.g output)
enp4s0 or eth0 (depends on your default network configuration
```
3) Next If you are setting up program for the first time configure.sh needs to be called

```
