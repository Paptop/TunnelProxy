
if [ "$#" -ne 3 ]; then
	echo "Usage: $0 [default interface name] [proxy ip] [proxy port]"
	echo ""
	echo "      first argument is default interface name"
	echo "      second argument is ip of the proxy"
	echo "      third argument is proxies udp port"
	exit 1
fi

# create tap interface
sudo ip tuntap add name tap0 mode tap 
sudo ifconfig tap0 0.0.0.0 promisc up
sudo ifconfig tap0 up

#setup bridge
sudo ip link add name tapbr type bridge
sudo ip link set tapbr up

#assign ip address to bridge
sudo ip addr add 192.168.0.2/24 brd + dev tapbr

`sudo ip link set $1 up`
`sudo ip link set $1 master tapbr`
sudo ip link set tap0 master tapbr

#add to route
sudo route add default gw 192.168.0.1 dev tapbr 

#forward packets to udp port
`sudo iptables -A INPUT -i tap0 -p udp --dport $3 -j ACCEPT`
`sudo iptables -t nat -A PREROUTING -i tapbr -p udp -j DNAT --to-destination $2:$3`

#first time run
sudo build/bin/tunproxy $2 $3
