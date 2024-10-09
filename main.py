import time
from ctypes import CDLL, POINTER, c_ubyte
from AES import *
from colorama import Fore, Style, init

# Initialize colorama
init()

KEYSIZE = 16
so_file = "./my_functions.so"
my_functions = CDLL(so_file)
Plaintext = "255044462d312e350a25d0d4c5d80a34"
Ciphertext = "d06bf9d0dab8e8ef880660d2af65aa82"
IV = "09080706050403020100a2b2c2d2e2f2"
date_time = "2018-04-17 22:00:00"
pl = bytes.fromhex(Plaintext)
ct = bytes.fromhex(Ciphertext)
iv = bytes.fromhex(IV)
unix_timestamp = int(time.mktime(time.strptime(date_time, "%Y-%m-%d %H:%M:%S")))

print(f"{Fore.BLUE}Plaintext: {pl} {Fore.YELLOW}Length: {len(pl)}{Style.RESET_ALL}")
print(f"{Fore.BLUE}Ciphertext: {ct} {Fore.YELLOW}Length: {len(ct)}{Style.RESET_ALL}")
print(f"{Fore.BLUE}IV: {iv} {Fore.YELLOW}Length: {len(iv)}{Style.RESET_ALL}")
print(f"{Fore.BLUE}Unix Timestamp: {unix_timestamp}{Style.RESET_ALL}")

my_functions.generateKey.restype = POINTER(c_ubyte * KEYSIZE)

# Start timing
start_time = time.time()

for i in range(0, 100000):
    key_ptr = my_functions.generateKey(unix_timestamp + i)
    key = bytes(key_ptr.contents)
    print(f"{Fore.RED}Key: {key.hex()} {Fore.YELLOW}")
    cypher = encrypt(pl, key, iv)
    if cypher == ct:
        print(f"{Fore.GREEN}Key found: {key.hex()}")
        break

# Stop timing
end_time = time.time()
execution_time = end_time - start_time

# Print execution time
print(f"{Fore.BLUE}Execution Time: {execution_time:.2f} seconds{Style.RESET_ALL}")

# Don't forget to free the memory allocated by the C function
libc = CDLL("libc.so.6")
libc.free(key_ptr)
