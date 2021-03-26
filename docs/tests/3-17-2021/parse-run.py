import os
from os import listdir
from os.path import isfile, join
import matplotlib.pyplot as plt
import struct

data = {}


def parse_byte(item):
    if len(item) == 1:
        item = "0" + item
    return bytearray.fromhex(item)


def show_data(plot, id, title, type="float",):
    global data
    parsed_data = []
    if (data.__contains__(id)):
        if (type == "float"):
            _data_ = data[id]
            for entry in _data_:
                __bytearray = b''.join(list(map(parse_byte, entry)))
                # __data = list(__bytearray)
                __data = struct.unpack("f", __bytearray)[0]
                parsed_data.append(__data)
    plot.plot(parsed_data)
    plot.set_title(title)


def parse_file(file):
    global data
    print("Parsing: " + file)
    f = open(file)
    for line in f.readlines():
        split = line.split(" ")
        try:
            id = split[0]
            len = int(split[1])
            _data_ = split[2:2+len]
            if (not data.__contains__(id)):
                data[id] = []
            else:
                data[id].append(_data_)
        except Exception as e:
            pass


if __name__ == "__main__":
    cwd = os.getcwd()
    print("Parsing files in: " + cwd)
    files = [f for f in listdir(cwd) if isfile(join(cwd, f))]
    for file in files:
        if (file.__contains__("LOG_")):
            parse_file(join(cwd, file))
    fig, (ax1, ax2, ax3) = plt.subplots(3)
    fig.suptitle(os.path.basename(cwd))
    # show_data("a3", "Engine RPM")
    # show_data("a4", "Engine RPM")
    show_data(ax1, "a3", "Rear Axel RPM")
    show_data(ax2, "a4", "Engine RPM")
    show_data(ax3, "a5", "Throttle")
    plt.show()
