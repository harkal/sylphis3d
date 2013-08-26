
##
## Sylphis Benchmark script
##
## run Sylphis with :
##          sylphis.exe -exec benchmark
##

import sylphis
import pyui
import mthread

##
## The function that does the measurement
##
def bench():
    mthread.sleep(0.1) # Sleep a little time to let things settle
    
    timer = sylphis.CTimer() # Generate a timer to count time
    startFrame = CEngine.getFrameCount()# Log the number of frames rendered by the engine at the start of the benchmark
    
    mthread.sleep(50.0) # Sleep for 50.0 seconds of game world time
    
    time = timer.getTime() # Read the real world time it took to render all the frames
    frames = float(CEngine.getFrameCount() - startFrame) # Calculate the number of frames the engine rendered
    print "Average FPS : %4.2f" % (frames / time)

    shutdown() # Shutdown the engine

CClient.loadMap('maps/last.cmap') # Load the benchmark map
pyui.deactivate() # Deactivate the GUI
CEngine.setFPS(30.0) # Set the engine to generate a constant frames per game world second

mthread.parallelize(bench) # Run bench() in parallel




