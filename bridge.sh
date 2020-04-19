# create tap interface
sudo ip tuntap add name tap0 mode tap 
ifconfig tap0 0.0.0.0 promisc up
ifconfig tap0 up

#setup bridge
ip link add name tapbr type bridge
ip link set tapbr up

#assign ip address
ip addr add 192.168.0.2/24 brd + dev tapbr

ip link set enp4s0 up
ip link set enp4s0 master tapbr
ip link set tap0 master tapbr

#add to route
route add default gw 192.168.0.1 dev tapbr 

#forward packets to udp port
sudo iptables -t nat -A PREROUTING -i tapbr -p udp -j DNAT --to-destination 10.10.10.10:1080
