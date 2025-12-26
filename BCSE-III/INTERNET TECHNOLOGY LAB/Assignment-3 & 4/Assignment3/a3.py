from scapy.all import IP, ICMP, sr1
import sys

def traceroute(destination, max_hops=30):
    print(f"Tracing route to {destination} with a maximum of {max_hops} hops:\n")
    
    for ttl in range(1, max_hops + 1):
        # Create ICMP packet with varying TTL
        packet = IP(dst=destination, ttl=ttl) / ICMP()
        
        # Send the packet and wait for a response
        reply = sr1(packet, timeout=1, verbose=False)
        
        if reply is None:
            print(f"{ttl}: *")  # No response
        elif reply.type == 11:  # ICMP Time Exceeded (intermediate router)
            print(f"{ttl}: {reply.src}")
        elif reply.type == 0:  # ICMP Echo Reply (destination reached)
            print(f"{ttl}: {reply.src} (Destination reached)")
            break
    else:
        print("Max hops reached without reaching destination.")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python traceroute.py <destination>")
        sys.exit(1)

    destination_ip = "8.8.8.8"
    traceroute(destination_ip)

