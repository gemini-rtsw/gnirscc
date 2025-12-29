""" Remote access to VxWorks shell.

Allows user to connect via telnet to VxWorks shell.
An "external" variable can be read and written as if it were a local
variable.  Functions in the VxWorks code can be called in-line.

Connect to the VxWorks crate via the connect function.
"""


import telnetlib

connected = 0
tn = None

def connect(host):
    global tn
    tn = telnetlib.Telnet(host)

    
