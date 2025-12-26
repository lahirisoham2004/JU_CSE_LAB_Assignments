from scapy.all import sniff, IP, TCP, UDP
from collections import defaultdict
import time

# Dictionary to store network flows
flows = defaultdict(lambda: {"packets": 0, "bytes": 0, "start_time": None, "end_time": None})
ip_activity = defaultdict(lambda: {"packets": 0, "bytes": 0})

# Packet processing function
def packet_analysis(pkt):
    if pkt.haslayer(IP):
        src_ip = pkt[IP].src
        dst_ip = pkt[IP].dst
        protocol = "TCP" if pkt.haslayer(TCP) else "UDP" if pkt.haslayer(UDP) else "Other"
        src_port = pkt[TCP].sport if pkt.haslayer(TCP) else pkt[UDP].sport if pkt.haslayer(UDP) else 0
        dst_port = pkt[TCP].dport if pkt.haslayer(TCP) else pkt[UDP].dport if pkt.haslayer(UDP) else 0
        packet_size = len(pkt)
        timestamp = time.time()

        # Define flow key
        flow_key = (src_ip, src_port, dst_ip, dst_port, protocol)

        # Update flow statistics
        if flows[flow_key]["start_time"] is None:
            flows[flow_key]["start_time"] = timestamp
        flows[flow_key]["end_time"] = timestamp
        flows[flow_key]["packets"] += 1
        flows[flow_key]["bytes"] += packet_size

        # Update IP activity
        ip_activity[src_ip]["packets"] += 1
        ip_activity[src_ip]["bytes"] += packet_size

# Capture packets for 15 minutes (900 seconds)
print("Capturing network traffic for 15 minutes...")
sniff(filter="ip", prn=packet_analysis, timeout=900)

# Convert flow data into a list and sort it
sorted_flows_bytes = sorted(flows.items(), key=lambda x: x[1]["bytes"], reverse=True)
sorted_flows_packets = sorted(flows.items(), key=lambda x: x[1]["packets"], reverse=True)

# Convert IP data into a list and sort it
sorted_ips_bytes = sorted(ip_activity.items(), key=lambda x: x[1]["bytes"], reverse=True)
sorted_ips_packets = sorted(ip_activity.items(), key=lambda x: x[1]["packets"], reverse=True)

# Display the top 10 largest flows by bytes
print("\n🔝 Top 10 Largest Flows (by bytes):")
print("Src IP -> Dst IP | Src Port | Dst Port | Protocol | Packets | Bytes | Duration (s)")
for flow, stats in sorted_flows_bytes[:10]:
    duration = stats["end_time"] - stats["start_time"]
    print(f"{flow[0]} -> {flow[2]} | {flow[1]} | {flow[3]} | {flow[4]} | {stats['packets']} | {stats['bytes']} | {duration:.2f}")

# Display the top 10 largest flows by packets
print("\n🔝 Top 10 Largest Flows (by packets):")
for flow, stats in sorted_flows_packets[:10]:
    duration = stats["end_time"] - stats["start_time"]
    print(f"{flow[0]} -> {flow[2]} | {flow[1]} | {flow[3]} | {flow[4]} | {stats['packets']} | {stats['bytes']} | {duration:.2f}")

# Display the top 10 talkers by bytes
print("\n🗣️ Top 10 Talkers (by bytes sent):")
for ip, stats in sorted_ips_bytes[:10]:
    print(f"{ip} | Packets: {stats['packets']} | Bytes: {stats['bytes']}")

# Display the top 10 talkers by packets
print("\n🗣️ Top 10 Talkers (by packets sent):")
for ip, stats in sorted_ips_packets[:10]:
    print(f"{ip} | Packets: {stats['packets']} | Bytes: {stats['bytes']}")

print("\n✅ Network traffic analysis completed!")

