import paho.mqtt.publish as publish

import sys

print(sys.argv[1])

publish.single('esp/test', sys.argv[1], hostname="192.168.0.120")