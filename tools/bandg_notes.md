# B&G Pilot converter for "Network" devices

### Physical layer

Only the white is used. It carries async serial data at:
- **10560 baud**
- 8 data bit
- 1 stop bit
- **Odd parity**

### Data link layer

Data is grouped into frames, which being emitted by devices

- Header, 5 bytes:
    - Start mark of 0xFF
    - Transmitter id
        - 0x70 for wind module
        - 0x58 for pilot
        - 0x30 for quad module
    - Data length of N (total length = N + 6)
    - Command, 0x01 is general data send
    - Header Checksum (header bytes + checksum must end with 0x__00)

- N bytes of channel data, consists of M channels
    - 1 byte of channel id
    - 1 byte of format code
    - 2/4 bytes of channel data

- Channel data checksum (channel data bytes + checksum must end with 0x__AA)

### Data channels

Data is generally reprehended as big endian.

- Wind speed
    - ID: 0x4E
    - format: 0x88 -> uint16 knots multiplied by 100
    - e.g.:
        - 4E 88 03 45 -> 0x0345 = 837 dec -> 8.37 knots
        - 4E 88 01 74 -> 0x0174 = 372 dec -> 3.72 knots

- Wind angle
    - ID: 0x51
    - format: 0x01 -> int16 degree
    - e.g.:
        - 51 01 00 5A -> 0x005A =  90 dec -> right 90
        - 51 01 00 5B -> 0x005B =  91 dec -> right 91
        - 51 01 FF A6 -> 0xFFA6 = -90 dec -> left 90

#### Protocol description
- https://github.com/trlafleur/Fastnet/raw/master/Fastnet%20Specification.docx
- https://raw.githubusercontent.com/trlafleur/Fastnet/master/FastnetProtocol.pdf
- https://www.oppedijk.com/bandg/fastnet.html