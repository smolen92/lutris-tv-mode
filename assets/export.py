import sys

i = 0
size = 0

with open(sys.argv[1], "rb") as file:
    print("uint8_t data[] = {")
    c = file.read(1)
    print(f"0x{ord(c):02X}", end="")
    size = size + 1
    while True:
        c = file.read(1)
        if not c:
            break
        print(", ",end="")
        
        i = i + 1
        size = size + 1
        if( i > 15) :
            print()
            i = 0

        print(f"0x{ord(c):02X}",end="")
        
print("};")

print(f"uint64_t data_size = {size};")
