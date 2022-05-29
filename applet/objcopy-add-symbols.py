#!/usr/bin/env python
from sys import argv

# TODO: This needs some better programming than this. It will break on any other potential flash and memory mapping

# TODO: Add function detection. Requires edits to the applet/src/symbols_* file format. An early attempt resolve the
# `branch and link relocation: R_ARM_THM_CALL to non STT_FUNC symbol` warning from lld by setting functions with the `,function` flag.
# This worked, but the device bootlooped :(

with open(argv[1]) as symbols:
    symbolstr = ""
    line = symbols.readline()
    while line:
        line = line.strip()
        if line:
            symbol = line.split("=")
            name = symbol[0].strip()
            value = symbol[1].removesuffix(";").strip()
            symbolstr += " --add-symbol " + name + "=" + value + ",global"
            if value.startswith("0x20"):
                symbolstr += ",weak"
        line = symbols.readline()
print(symbolstr)
