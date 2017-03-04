
import os, sys, time
import numpy as np
from time import sleep

fifo = open('/var/run/mlx9062x.sock', 'r')

while True:
    sleep(1)        
    ir_raw = fifo.read()
    ir_trimmed = ir_raw[0:128]
    ir = np.frombuffer(ir_trimmed, np.uint16)

    print (ir)

fifo.close()
