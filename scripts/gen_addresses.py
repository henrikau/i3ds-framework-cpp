#! /usr/bin/python3

import sys

def output_addresses(ip, node_base, port_base):
    count = 20
    nodes = [node_base + i for i in range(count)]
    ports = [[port_base + 2*i, port_base + 2*i + 1] for i in range(count)]
    line ='{},{},{},tcp://{}:{}'

    for node, port in zip(nodes, ports):
        print(line.format(node, 'pub', 'b', ip, port[0]))
        print(line.format(node, 'sub', 'c', ip, port[0]))
        print(line.format(node, 'rep', 'b', ip, port[1]))
        print(line.format(node, 'req', 'c', ip, port[1]))

if __name__ == '__main__':
    if len(sys.argv) == 4:
        output_addresses(sys.argv[1], int(sys.argv[2]), int(sys.argv[3]))
    else:
        print('Usage: {} [ip] [node_base] [port_base]'.format(sys.argv[0]))
