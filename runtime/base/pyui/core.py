# PyUI
# Copyright (C) 2001-2002 Sean C. Riley
# 
# This library is free software; you can redistribute it and/or
# modify it under the terms of version 2.1 of the GNU Lesser General Public
# License as published by the Free Software Foundation.
# 
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
# 

# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

import sys
import time
import pyui.locals
from pyui.themes import future

import colors
import string
import os

from desktop import Desktop, readTimer
from rendererBase import RendererBase
from renderer3d import Renderer3DBase

###########################################################
# Section: Globals
##########################################################

gRenderer = None
gDesktop = None
gVersion = 0.95

#########################################################
# Section: external/public functions
#
# External/Public Module level functions
#########################################################

def init(w, h, renderer = "sylphis", fullscreen = 0, title=""):
    """Initialize pyui. Will set to fullscreen if specified. default is to run in a window.
    This will return a Desktop Object.
    (public)
    """
    global gDesktop, gRenderer
    if renderer == "null":
        gRenderer = RendererBase(w, h, fullscreen, title)

    elif renderer == "null3d":
        gRenderer = Renderer3DBase(w, h, fullscreen, title)
        
    elif renderer == "2d":
        from renderers.pygame2D import Pygame2D
        gRenderer = Pygame2D(w, h, fullscreen, title)
        
    elif renderer == "gl":
        from renderers.openglGlut import OpenGLGlut
        gRenderer = OpenGLGlut(w, h, fullscreen, title)

    elif renderer == "p3d":
        from renderers.openglPygame import OpenGLPygame
        gRenderer = OpenGLPygame(w, h, fullscreen, title)
        
    elif renderer == "dx":
        from renderers import unseen
        gRenderer = unseen.Unseen(w, h, fullscreen, title)

    elif renderer == "nebula":
        from renderers import nebula
        gRenderer = nebula.RendererNeb(w, h, fullscreen)    
    elif renderer == "sylphis":
        from renderers import sylphisRenderer
        gRenderer = sylphisRenderer.SylphisRenderer(w, h, fullscreen, title)
    else:
        raise "Unsupported renderer type", renderer
        
    (w , h) = gRenderer.getScreenSize()
    # create the theme and desktop
    #theTheme = comic.ComicTheme(gRenderer)
    theTheme = future.FutureTheme(gRenderer)
    #theTheme = green.GreenTheme(gRenderer)
    #theTheme = win2k.Win2kTheme(gRenderer)
    
    gDesktop = Desktop(gRenderer, w, h, fullscreen, theTheme)
    colors.init(gRenderer)
    return gDesktop
    
def quit():
    """Sets the running flag so that the application knows to quit.
    (public)
    """
    global gDesktop
    gDesktop.quit()
    
def update():
    """Process events from the renderer, and events posted by users or widgets.
    Will return 1 if execution should continue, 0 if we should exit.
    (public)
    """
    global gDesktop
    return gDesktop.update()
    
def draw():    
    """
    fills the background and draws the widgets.
    (public)
    """
    global gDesktop
    gDesktop.draw()

def version():
    """return the version number of pyui"""
    global gVersion
    return gVersion


def run(callback=None):
    global gRenderer
    gRenderer.run(callback)
    
def activate():
    global gRenderer
    gRenderer.activate()
    
def deactivate():
    global gRenderer
    gRenderer.deactivate()
    
def isActive():
	global gRenderer
	return gRenderer.active

def loadPyuiImage(filename):
    """This loads an image file from the images directory in the pyui install.
    The directory pyui/images holds general images used in pyui that are not
    application specific.
    """
    path = pyui.__path__[0]
    pathElements = list(os.path.split(path)) # string.split(path, "\\")
    pathElements.pop( len(pathElements) -1)
    realName = string.join( pathElements, "/") + "/images/" + filename
    gRenderer.loadImage(realName, filename)

