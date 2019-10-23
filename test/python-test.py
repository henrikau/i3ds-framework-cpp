#!/usr/bin/env python3
import sys

try:
    import i3ds_binding
except ModuleNotFoundError as e:
    print("Python bindings not found!")
    print(sys.path)
    sys.exit(1)

if __name__ == "__main__":
    print("Hello, world.")

sys.exit(0)
