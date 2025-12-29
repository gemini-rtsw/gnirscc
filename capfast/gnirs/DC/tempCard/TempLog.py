#FileName:    TempLog.py
#Parameters:  None
#Purpose:     Test program for the temperature card
#Description: This routine opens a socket to the gemini controller, reads temperature
#                 data and graphs it real time.  
# Previous requirements:  The logtemps routine must be run on the ioc that is connected
#                            to the temperature card.
#Author:      Peter Ruckle  1-29-99


#variables
class Vars:
    xp = 0

windowHeight=700
windowWidth=1000
apy = 100  #placement of y axis in pixels
apx = 250  # placement of x asix in pixels

sx=3       # time scale factor
sy=15       #temperature scale factor

tMin = 20    #Minimum limit for temperature scale
tMax = 60    #Maximum limit for temperature scale
tj =1      # degrees per division

pMin = 0     #Minimum limit for power scale
pMax = 801  #Maximum limit for power scale
pj = 25      #miliwatts per division

eMin = -2.4  #Minimum limit for temperature scale scale
eMax = 2.2  #Maximum limit for temperature scale scale
ej = .15      #degrees per division for temperature error

vMin = 1.0    #Minimum limit for voltage scale
vMax = 1.4   #Maximum limit for voltage scale
vj = .005     # volts per division 
 

import Tkinter 
import thread
import socket
import os
import string
import sys


def Mark():
    print 'Mark, xp = \n',vars.xp
    if vars.xp > 0:
	graph.create_line(vars.xp,windowHeight-apy,vars.xp,0,width=4,fill = 'green')
	graph2.create_line(vars.xp,windowHeight-apy,vars.xp,0,width=4,fill = 'green')




	
def plotPoint(s,mask):
	    
    data = s.recv(200)
    if data[0:3] == 'end':
	Tkinter.tkinter.deletefilehandler(s)
	s.close()
	
    else:
	print data
	p = 0
	
	#  current time
	str = ''
	while data[p] != ',':
	    str = str + data[p]
	    p = p+1
	time = string.atoi(str)
	
	
	# refference temperature
	str = ''
	p = p+1
	while data[p] != ',':
	    str =str+data[p]
	    p = p+1
	tref=string.atof(str)

			
	# foot temperature
	str = ''
	p = p+1
	while data[p] != ',':
	    str = str+data[p]
	    p = p+1
	tf=string.atof(str)

			
	# mount temperature
	str = ''
	p = p+1
	while data[p] != ',':
	    str = str+data[p]
	    p = p+1
	tm=string.atof(str)

			
	# temperature error
	str = ''
	p = p+1
	while data[p] != ',':
	    str = str+data[p]
	    p = p+1
	te=string.atof(str)/1000
			
	# power on the foot resistor
	str = ''
	p = p+1
	while data[p] != ',':
	    str = str+data[p]
	    p = p+1
	pf=string.atof(str)

			
	# power on the mount resistor
	str = ''
	p = p+1
	while data[p] != ',':
	    str = str+data[p]
	    p = p+1
	pm=string.atof(str)

			
	#Voltage across the temperature resistor on the foot
	str = ''
	p = p+1
	while data[p] != ',':
	    str = str+data[p]
	    p = p+1
	vf=string.atof(str)

	#Voltage across the temperature resistor on the mount
	str = ''
	p = p+1
	while data[p] != ',':
	    str = str+data[p]
	    p = p+1
	vm=string.atof(str)


	# x position in pixels for the current time
	vars.xp = apx + time*sx/60
	
	# plot points to the apropriate graph
	if tref >= tMin :
	    yp = windowHeight - apy - (tref-tMin)*sy
	    graph.create_oval(vars.xp-1,yp-1,vars.xp+1,yp+1)
	    graph2.create_oval(vars.xp-1,yp-1,vars.xp+1,yp+1)
	if tf >= tMin :
	    yp = windowHeight - apy - (tf-tMin)*sy
	    graph.create_oval(vars.xp-3,yp-3,vars.xp+3,yp+3)
	if te >= eMin :
	    yp = windowHeight - apy - (te-eMin)*sy*tj/ej
	    graph.create_oval(vars.xp-1,yp-1,vars.xp+1,yp+1,outline='blue')
	    graph2.create_oval(vars.xp-1,yp-1,vars.xp+1,yp+1,outline='blue')
	if pf >= pMin :
	    yp = windowHeight - apy - (pf-pMin)*tj/pj*sy
	    graph.create_oval(vars.xp-1,yp-1,vars.xp+1,yp+1,outline='red')
	if vf >= vMin :
	    yp = windowHeight - apy - (vf - vMin)*sy*tj/vj
	    graph.create_oval(vars.xp-1,yp-1,vars.xp+1,yp+1,outline='brown')
		


	if tm >= tMin :
	    yp = windowHeight - apy - (tm-tMin)*sy
	    graph2.create_oval(vars.xp-3,yp-3,vars.xp+3,yp+3)
	if pm >= pMin :
	    yp = windowHeight - apy - (pm - pMin)*sy*tj/pj
	    graph2.create_oval(vars.xp-1,yp-1,vars.xp+1,yp+1,outline='red')
	if vm >= vMin :
	    yp = windowHeight - apy - (vm - vMin)*sy*tj/vj
	    graph2.create_oval(vars.xp-1,yp-1,vars.xp+1,yp+1,outline='brown')
	    
	    








vars = Vars()
root = Tkinter.Tk() # main window for foot graph
#start event loop
#tempGraph = Graph(root)
#print 'created graph'
#class Graph:
#    def __init__(self,master):
# main program
win2 = Tkinter.Tk() # second window for mount graph

#set up windows
win2.title('Temperature Log mount')
root.title('Temperature Log foot')
frame = Tkinter.Frame(root)
graph2 = Tkinter.Canvas(win2,height=windowHeight,width=windowWidth,background = 'white')
graph = Tkinter.Canvas(frame,height=windowHeight,width=windowWidth,background='white')
quitB = Tkinter.Button(frame,text="QUIT",fg="red",command=graph.quit)
markB = Tkinter.Button(frame,text="MARK",fg="red",command=Mark)
quitB.grid(row=0,column=1)
markB.grid(row=0,column=0)
graph.grid(row=1,column=0)
graph2.pack()

frame.pack()
graph.create_line(apx,windowHeight-apy,windowWidth,windowHeight-apy,width=4)
graph.create_line(apx,windowHeight-apy,apx,0,width=4)
graph.create_text(windowWidth/2,windowHeight-30, anchor = 'nw',
		  text = 'Time (minutes)')
graph.create_text(2,windowHeight/2+50, anchor = 'nw',
		  text = 'Power (watts)',fill = 'red')
graph.create_text(2,windowHeight/2+90, anchor = 'nw',
		  text = 'Volts',fill='brown')
graph.create_text(2,windowHeight/2+130, anchor = 'nw',
		  text = 'Temp error',fill='blue')
graph.create_text(2,windowHeight/2, anchor = 'nw',
		  text = 'Temperature\n (degrees K)')
graph2.create_line(apx,windowHeight-apy,windowWidth,windowHeight-apy,width=4)
graph2.create_line(apx,windowHeight-apy,apx,0,width=4)
graph2.create_text(windowWidth/2,windowHeight-30, anchor = 'nw',
		   text = 'Time (minutes)')
graph2.create_text(2,windowHeight/2, anchor = 'nw',
		   text = 'Temperature\n (degrees K)')
graph2.create_text(2,windowHeight/2+50, anchor = 'nw',
		   text = 'Power (watts)',fill = 'red')
graph2.create_text(2,windowHeight/2+90, anchor = 'nw',
		   text = 'Volts',fill='brown')
graph2.create_text(2,windowHeight/2+130, anchor = 'nw',
		   text = 'Temp error',fill='blue')
# create time scale on x axis
for x in range(0,361,20):
    graph.create_line(apx+x*sx,windowHeight-apy+5,apx+x*sx,0)
    graph.create_text(apx+x*sx,windowHeight-apy+20, anchor = 'center',
		      text = str(x))
    graph2.create_line(apx+x*sx,windowHeight-apy+5,apx+x*sx,0)
    graph2.create_text(apx+x*sx,windowHeight-apy+20, anchor = 'center',
		       text = str(x))
    
#create power, voltage, temperature and temperature error scales on y axis
for y in range(tMin,tMax,tj):
    graph.create_line(apx-5,windowHeight-apy-(y-tMin)*sy,windowWidth,windowHeight-apy-(y-tMin)*sy)
    graph.create_text(apx-15,windowHeight-apy-(y-tMin)*sy, anchor = 'center',text = str(y))
    graph2.create_line(apx-5,windowHeight-apy-(y-tMin)*sy,windowWidth,windowHeight-apy-(y-tMin)*sy)
    graph2.create_text(apx-15,windowHeight-apy-(y-tMin)*sy, anchor = 'center',text = str(y))
    if ((y-tMin)/tj*pj + pMin) < pMax:
	graph.create_text(apx-50,windowHeight-apy-(y-tMin)*sy, anchor = 'center', text = str((y-tMin)/tj*pj + pMin),fill = 'red')
	graph2.create_text(apx-50,windowHeight-apy-(y-tMin)*sy, anchor = 'center', text = str((y-tMin)/tj*pj + pMin),fill = 'red')
    if ((y-tMin)/tj*vj + vMin) < vMax:
	graph.create_text(apx-90,windowHeight-apy-(y-tMin)*sy, anchor = 'center', text = str((y-tMin)/tj*vj + vMin),fill = 'brown')
	graph2.create_text(apx-90,windowHeight-apy-(y-tMin)*sy, anchor = 'center', text = str((y-tMin)/tj*vj + vMin),fill = 'brown')    
    if ((y-tMin)/tj*ej + eMin) < eMax:
	graph.create_text(apx-130,windowHeight-apy-(y-tMin)*sy, anchor = 'center', text = str((y-tMin)/tj*ej + eMin),fill = 'blue')
	graph2.create_text(apx-130,windowHeight-apy-(y-tMin)*sy, anchor = 'center', text = str((y-tMin)/tj*ej + eMin),fill = 'blue')
	
# Host to connect to
PORT = 5546
HOST = 'arthur.tuc.noao.edu'
# Open socket to host
s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)

# change HOST to reflect the propper
if(sys.argv[1] == 'nirs'):
	HOST = 'romeo.tuc.noao.edu'

elif (sys.argv[1] == 'niri'):
	HOST = 'tristan.tuc.noao.edu'
elif (sys.argv[1] == 'naac'):
	HOST = 'arthur.tuc.noao.edu'
else :
	HOST = 'arthur.tuc.noao.edu'
print HOST
print PORT
s.connect((HOST,PORT))
print'connected to socket'
# register callback routine for socket reading
Tkinter.tkinter.createfilehandler(s,Tkinter.tkinter.READABLE,plotPoint)
print'register done'
# This routine is registered with the os.  Whenever there is data to be 
#read, this gets called.  The data is read parsed and graphed


root.mainloop()
