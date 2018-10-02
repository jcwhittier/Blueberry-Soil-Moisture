#! /usr/bin/python3

# make sure to chmod 755
import serial
import os
import sys
import datetime
import requests
import time
from datetime import date

dir = os.path.dirname(os.path.abspath(__file__))


DummyData = "DummyData"
LiveData = "LiveData"

# Config options
data_mode = None
ser = None

DEBUG_MODE = False

url = None
key = None

UPDATE_FREQUENCY = 5
logFilePath = ""
logFilenamePart1 = 'Moisture_log'
bufferFilename = os.path.join(dir, 'Moisture_buffer.json.txt')
tempBufferFilename = os.path.join(dir,  'Moisture_buffer.json.txt.temp')
configFilename = os.path.join(dir, 'base_station_config.txt')


def write_log_file(json_data):
    log_string = "%s,%s,%s,%s,%s" % (
        json_data['node_id'], json_data['analog_value'], json_data['voltage'], json_data['moisture'],
        json_data['gateway_time'])

    date_today = date.today()

    log_file_name = logFilePath + logFilenamePart1 + "_{}_{}.csv".format(date_today.year, date_today.month)

    with open(log_file_name, 'a') as log_file:
        log_file.write(log_string + os.linesep)


def buffer_upload_json(json_data):
    lines_uploaded = 0
    upload_failed = False

    with open(bufferFilename, 'r') as buffer:
        for line in buffer:
            line = line.strip(os.linesep)

            # if could upload data
            if upload_json(eval(line)) is not None:
                lines_uploaded += 1
            else:
                upload_failed = True
                break

    # remove the uploaded lines from the head of the file
    if lines_uploaded > 0:
        with open(bufferFilename, 'r') as oldBuffer:

            # skip over lines uploaded and to be removed from file
            for i in range(lines_uploaded):
                oldBuffer.readline()

            # get to the first new line not uploaded
            line = oldBuffer.readline()

            # write any files not uploaded to new log file
            with open(tempBufferFilename, 'w') as newBuffer:
                while line:
                    newBuffer.write(line)
                    line = oldBuffer.readline()

        # remove the old buffer file and replace with temp file created
        if os.path.exists(bufferFilename):
            os.remove(bufferFilename)

        os.rename(tempBufferFilename, bufferFilename)

    # if the previous uploads did not fail and the new data was uploaded, return
    if (not upload_failed) and (upload_json(json_data) is not None):
        return
    else:
        # otherwise we append the new data to the buffer
        with open(bufferFilename, 'a') as buffer:
            buffer.write(str(json_data) + os.linesep)


def upload_json(json_data):
    headers = {'Accept': 'application/json'}

    try:
        print(json_data)
        r = requests.post(url, data=json_data, headers=headers)
        print(r.text)
        return json_data

    except:
        # unable to upload data -> write to file and try again later
        print("Unable to upload data: {} \n".format(datetime.datetime.now()))
        return None


# Read the serial port and generate and return a json dictionary
def read_serial():
    # print("Reading Serial...\n")
    chars_to_remove = ['[', ']', '\n', os.linesep]

    if ser is None:
        print("Serial port is null")
        return None

    # need to add try here
    try:
        line = ser.readline().decode("utf-8")

        line = line.strip()

        # print(line)

        for charToRemove in chars_to_remove:
            line = line.replace(charToRemove, "")

        line = line.split(",")

        if len(line) == 4:
            node_id = line[0].strip()
            analog_value = line[1].strip()
            voltage = line[2].strip()
            moisture = line[3].strip()

            json_data = {'node_id': node_id, 'analog_value': analog_value, 'voltage': voltage, 'moisture': moisture,
                        'key': key, 'gateway_time': datetime.datetime.now().isoformat(' ')}
            return json_data
        else:
            print("Bad input")
            print(line)
            return None
    except:
        print("Bad input")
        return None


def read_config():
    global data_mode
    global ser
    global logFilePath
    global key
    global url

    with open(configFilename, 'r') as configFile:

        for line in configFile:
            line = line.split(":::")

            print (line)
            # Set the data mode
            if len(line) == 2:
                line[0] = line[0].strip()
                line[1] = line[1].strip()

                print("line[0]:{} line[1]:{}".format(line[0],line[1]))

                if line[0] == "DataMode":
                    if line[1] == "DummyData":
                        data_mode = DummyData
                    elif line[1] == "LiveData":
                        data_mode = LiveData
                    else:
                        print("Bad data mode : %s\n" + line[1])

                elif line[0] == "SerialPort":
                    try:
                        ser = serial.Serial(line[1], 9600)
                        time.sleep(1)
                        ser.flushInput()
                        print("Configured serial port successfully")
                    except:
                        print("Failed to configure serial port:", sys.exc_info()[0])

                elif line[0] == "LogPath":
                    logFilePath = line[1]

                elif line[0] == "URL":
                    url = line[1]

                elif line[0] == "Key":
                    key = line[1]

                else:
                    print("Bad config line: %s\n", line)
            else:
                print("Bad config line: %s\n", line)


def main(argv):

    print(dir)
    print(configFilename)

    line_count = 0

    read_config()

    while True:
        line_count += 1
        json_data = None

        if data_mode == LiveData:
            json_data = read_serial()
        elif data_mode == DummyData:
            dummy_data = {'count': line_count, 'node_id': 99, 'analog_value': 1, 'voltage': 2, 'moisture': 3, 'key': key
                , 'gateway_time': datetime.datetime.now().isoformat(' ')}
            json_data = dummy_data
            time.sleep(5)
        else:
            print("Unsupported data_mode: {}\n".format(data_mode))
            break

        # if the dictionary is not empty.
        if json_data is not None:

            if DEBUG_MODE:
                print(json_data)
                print("Line count: %d\n", line_count)

            write_log_file(json_data)
            buffer_upload_json(json_data)
        else:
            time.sleep(1)
    pass


if __name__ == "__main__":
    main(sys.argv)
