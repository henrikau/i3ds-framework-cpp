#! /usr/bin/python3

import sys
import pandas as pd

def print_line(node, socket, bind, ip, port):
    print("{},{},{},tcp://{}:{}".format(node, socket, bind[0], ip, port))

def output(data, socket):
    for r in data.itertuples():
        node = str(r[1])
        print_line(node, socket[0], r[3], r[4], r[5])
        print_line(node, socket[1], r[6], r[7], r[8])

def main(filename):
    output(pd.read_excel(filename, sheetname='services'), ["rep", "req"])
    output(pd.read_excel(filename, sheetname='topics'), ["pub", "sub"])

if __name__ == '__main__':
    main(sys.argv[1])
