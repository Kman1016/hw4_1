from time import sleep
import erpc
from lcdtext_erpc import *
import sys

if __name__ == "__main__":

    if len(sys.argv) != 2:
        print("Usage: python led_test_client.py <serial port to use>")
        exit()

    # Initialize all erpc infrastructure
    xport = erpc.transport.SerialTransport(sys.argv[1], 9600)
    client_mgr = erpc.client.ClientManager(xport, erpc.basic_codec.BasicCodec)
    client = client.LCDServiceClient(client_mgr)

    # Blink LEDs on the connected erpc server
    client.col()
    sleep(0.5)
    client.row()
    sleep(0.5)
    client.cls()
    sleep(0.5)
    client.put(2)
    client.loc(4,1)
    sleep(0.5)
    client.put(3)

