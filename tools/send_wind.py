import serial
import time

D_110_DEG_7_KT_A = b'\xFF\x70\x20\x01\x70\x8D\x48\x00\x8F\x50\x88\x02\xFB\x4E\x88\x03\x81\x51\x01\x00\x6E\x52\x08\x4E\x34\x57\x88\x07\x5B\x59\x01\x00\x9D\x7F\x88\xFA\xA9\x99'
D_110_DEG_7_KT_B = b'\xFF\x70\x0C\x01\x84\x4E\x88\x03\x45\x51\x01\x00\x6E\x52\x08\x4E\x34\xF0'

D_120_DEG_3_KT_A = b'\xFF\x70\x20\x01\x70\x8D\x48\x00\x8D\x50\x88\x01\x67\x4E\x88\x01\x42\x51\x01\x00\x78\x52\x08\x55\x50\x57\x88\x06\x52\x59\x01\x00\xA9\x7F\x88\xFA\x9A\x51'
D_120_DEG_3_KT_B = b'\xFF\x70\x0C\x01\x84\x4E\x88\x01\x74\x51\x01\x00\x78\x52\x08\x55\x50\x96'

jobb_90 =          b'\xFF\x70\x20\x01\x70\x8D\x48\x00\x93\x50\x88\x00\x00\x4E\x88\x00\x00\x51\x01\x00\x5A\x52\x08\x00\x00\x57\x88\x06\x21\x59\x01\x00\xB4\x7F\x88\xF9\xDF\x9B'
jobb_90 =          b'\xFF\x70\x0C\x01\x84\x4E\x88\x00\x00\x51\x01\x00\x5A\x52\x08\x00\x00\xCE'

jobb_91 =          b'\xFF\x70\x20\x01\x70\x8D\x48\x00\x93\x50\x88\x00\x00\x4E\x88\x00\x00\x51\x01\x00\x5B\x52\x08\x00\x00\x57\x88\x05\xE2\x59\x01\x00\xB4\x7F\x88\xFA\x1E\x9A'
jobb_91 =          b'\xFF\x70\x0C\x01\x84\x4E\x88\x00\x00\x51\x01\x00\x5B\x52\x08\x00\x00\xCD'

bal_90 =           b'\xFF\x70\x20\x01\x70\x8D\x48\x00\x92\x50\x88\x00\x00\x4E\x88\x00\x00\x51\x01\xFF\xA6\x52\x08\x00\x00\x57\x88\x05\xD3\x59\x01\x00\xB4\x7F\x88\xFA\x2D\x51'
bal_90 =           b'\xFF\x70\x0C\x01\x84\x4E\x88\x00\x00\x51\x01\xFF\xA6\x52\x08\x00\x00\x83'

bal_21 =           b'\xFF\x70\x20\x01\x70\x8D\x48\x00\x93\x50\x88\x00\x00\x4E\x88\x00\x00\x51\x01\xFF\xEB\x52\x08\x00\x00\x57\x88\x05\xCC\x59\x01\x00\xB4\x7F\x88\xFA\x34\x0B'
bal_21 =           b'\xFF\x70\x0C\x01\x84\x4E\x88\x00\x00\x51\x01\xFF\xEB\x52\x08\x00\x00\x3E'


# boat speed
# \xFF\x30\x23\x01\xAD\x41\x88\x00\x00\x42\x88\x00\x00\x65\x88\x00\x00\xCF\x48\x00\x00\x81\x48\x09\xC4\xD3\x01\x01\x09\x20\x04\x00\x45\x52\x52\x75\x03\x44\x32\x27\x7D
# \xFF\x30\x16\x01\xBA\x31\x04\x00\x20\x46\x20\xFE\x04\x00\x4F\x46\x46\xFF\x81\x00\x19\xC4\x04\x00\x2D\x2D\x2D\x2A
# \xFF\x30\x25\x01\xAB\x41\x88\x00\x00\x42\x88\x00\x00\xCD\x42\x00\x00\xA0\xE3\xCF\x48\x00\x00\x81\x48\x09\xC4\xD3\x01\x01\x09\x20\x04\x00\x45\x52\x52\x75\x03\x44\x32\x28\xD7


ser = serial.Serial('/dev/ttyUSB0', 10560,  parity=serial.PARITY_ODD)

while True:
    ser.write(D_110_DEG_7_KT_A)
    time.sleep(0.1)
    ser.write(D_110_DEG_7_KT_B)

    time.sleep(0.8)
