import pyui
import copy

from HTMLParser import HTMLParser

from htmlElements import *

class PyuiHTMLParser(HTMLParser):

    def __init__(self, panel):
        self.panel = panel
        
    def handle_starttag(self, tag, attrs):
        self.panel.handle_starttag(tag, attrs)

    def handle_endtag(self, tag):
        self.panel.handle_endtag(tag)
        
    def handle_data(self, data):
        self.panel.handle_data(data)

    def handle_charref(self, name):
        self.panel.handle_charref(name)

    def handle_entityref(self, name):
        self.panel.handle_entityref(name)
        
class HTMLError(Exception):
    pass

class HTMLDone(Exception):
    pass

class HTMLToken:
    def __init__(self):
        self.children = []
        self.parent = None

    def addChild(self, child):
        self.children.append(child)
        child.parent = self

    def __repr__(self):
        return "ROOT"
        
class HTMLTag(HTMLToken):
    def __init__(self, tag, attrs):
        self.tag = tag
        self.attrs = attrs
        HTMLToken.__init__(self)

    def __repr__(self):
        return "TAG: <%s> %s" % (self.tag, self.attrs)

class HTMLEndTag(HTMLToken):
    def __init__(self, tag):
        self.tag = tag
        HTMLToken.__init__(self)

    def __repr__(self):
        return "ENDTAG: <%s>" % (self.tag)
        
class HTMLData(HTMLToken):
    def __init__(self, data):
        self.data = data
        HTMLToken.__init__(self)

    def __repr__(self):
        return "DATA: <%s>" % self.data


"""Strategy:

on creation and resize, build a set of primatives that can draw themselves. this includes
wrapping text with appropriate fonts. these primative will be arranged into lines in drawing
order left to right.

primatives in a line do not overlap to the right of the panel.

each line has a height. primatives may be aligned vertically within the line's height.




on drawing, 

"""
        
class HTMLPanel(pyui.base.Panel):
    """Panel that displays html.
    """
    
    def __init__(self):
        self.htmlText = ""
        pyui.base.Panel.__init__(self)
        self.parser = PyuiHTMLParser(self)
        self.root = None
        
        self.currentToken = None
        self.formatStack = None
        self.attrsStack = []
        self.formatElements = {}
        self.visibleElements = {}
        self.formatStateStack = []
        self.areas = []
        
        self.bgColor = pyui.colors.white
        self.textColor = pyui.colors.black
        self.scrollbar = pyui.widgets.VScroll()
        self.addChild(self.scrollbar)
        self.loadFormatElements(pyui.desktop.getRenderer())
        self.loadVisibleElements()
        
    def loadFormatElements(self, renderer):
        """Load the format elements"""
        for tag, before, after, effects, name in formatElements:
            newElement = FormatElement(tag, name, before, after, effects)
            self.formatElements[tag] = newElement

    def loadVisibleElements(self):
        """Load the visible elements"""
        for tag, before, after, instanceClass, name in visibleElements:
            newElement = VisibleElement(tag, name, before, after, instanceClass)
            self.visibleElements[tag] = newElement
        
    def feed(self, htmlText):
        self.htmlText = htmlText
        self.root = HTMLToken()
        self.currentToken = self.root
        self.parser.reset()
        self.parser.feed(htmlText)

        self.buildPrimatives()
        

    ##############  handlers for parser ################
        
    def handle_starttag(self, tag, attrs):
        """Add the new tag to the tree
        """
        newToken = HTMLTag(tag, attrs)
        self.currentToken.addChild(newToken)
        self.currentToken = newToken
    
    def handle_endtag(self, tag):
        """move back up the tree
        """
        newToken = HTMLEndTag(tag)
        self.currentToken.addChild(newToken)        
        self.currentToken = self.currentToken.parent
    
    def handle_data(self, data):
        if len(data) == 0:
            return 
        newToken = HTMLData(data)
        self.currentToken.addChild(newToken)

    def handle_charref(self, name):
        print "handling charref: <%s>" % name

    def handle_entityref(self, name):
        #print "handling entityref: <%s>" % name
        if name == "quot":
            newToken = HTMLData('"')
            self.currentToken.addChild(newToken)

    ########### construct primatives lines #######

    def resize(self, width, height):
        pyui.widgets.Panel.resize(self, width, height)
        self.buildPrimatives()

        h = self.windowRect[3]
        count = 0
        pos = 0
        for l in self.areas[-1].lines:
            if count >= self.scrollbar.currentItem:
                pos += l.height
            if pos >= self.windowRect[3]:
                break
            count += 1
            
        self.scrollbar.resize(10, self.windowRect[3])
        #print "SCROLL:", self.windowRect[0] + self.width - 10, self.windowRect[1]
        self.scrollbar.moveto(self.windowRect[0] + self.width - 10, 0)
        #print "NUMITEMS:", len(self.lines), count - self.scrollbar.currentItem
        self.scrollbar.setNumItems(self.areas[0].height-self.height+24, self.height)

    def buildPrimatives(self):
        """This constructs the set of primatives for drawing the HTML
        elements in this panel.
        """
        self.areas = [PrimativeArea([], self.windowRect[2], self.windowRect[3], self.fgColor)]
        self.formatStack = []
        self.attrsStack = []
        self.currentLine = PrimativeLine()
        if self.root:
            self.expandToken(self.root)

        for line in self.areas[0].lines:
            if len(line.primatives):
                print line.primatives[0].text
                if isinstance(line.primatives[0], PrimativeTable):
                    for l2 in line.primatives[0].lines:
                        for p in l2.primatives:
                            print "    ", p.text
                        
            else:
                print "empty"

    def expandToken(self, token):
        """This does the work of constructing lines and primatives from
        the set of tokens for the current size of the panel.

        This is recursive through the token tree
        """
        if isinstance(token, HTMLTag):
            #check if it is a format tag
            formatElement = self.formatElements.get(token.tag)
            if formatElement:
                self.formatStack.append(formatElement)
                self.attrsStack.append(token.attrs)
                if formatElement.newLineBefore:
                    print "new element", formatElement.name
                    self.areas[-1].newLine()
                self.pushCurrentFormat()

            # check if it is a visible tag
            visibleElement = self.visibleElements.get(token.tag)
            if visibleElement:
                newPrim = visibleElement.instanceClass(token.attrs, self.width)
                if token.tag == 'table':
                    print "TABLE OPEN"
                    self.areas.append(newPrim)
                else:
                    self.areas[-1].addPrimative(newPrim)

            # check for special tag
            if token.tag == "body":
                self.processBodyTag(token)
                                             
        elif isinstance(token, HTMLEndTag):
            # pop last format tag off
            if len(self.formatStack) > 0:
                current = self.formatStack[-1]
                if current.tag == token.tag:
                    formatElement = self.formatStack.pop(-1)
                    if formatElement.newLineAfter:
                        print "end tag", formatElement.name
                        self.areas[-1].newLine()
                    self.attrsStack.pop(-1)
                    self.popCurrentFormat()
                else:
                    #print "ERROR: end tag doesnt match for ", token.tag
                    pass
            if token.tag == 'table':
                table = self.areas.pop(-1)                
                print "TABLE CLOSE height is ", table.height

                self.areas[-1].addPrimative(table)
                
        elif isinstance(token, HTMLData):
            if self.formatStack:
                format = self.formatStack[-1]
                attrs = self.attrsStack[-1]
                #print self.formatStateStack

                print "STATE:", self.formatStateStack[-1].name, self.formatStateStack[-1].face, self.formatStateStack[-1].face, self.formatStateStack[-1].size, self.formatStateStack[-1].color, self.formatStateStack[-1].bold
                print "DATA:", token.data            
                if format.tag == 'li':
                    constructSingleLine(self, token, self.formatStateStack[-1]) #format, attrs)
                elif format.tag == 'pre':
                    constructPreformatted(self, token, self.formatStateStack[-1]) #format, attrs)
                else:
                    constructWrapped(self, token, self.formatStateStack[-1]) #format, attrs)
            
        for child in token.children:
            self.expandToken(child)

    def pushCurrentFormat(self):
        """This builds a new format state based on the last format state and puts
        it on the stack.
        """
        if not len(self.formatStateStack):
            self.formatStateStack.append( FormatState())
            return
        currentState = self.formatStateStack[-1]
        newFormatState = copy.copy(currentState)
        
        format = self.formatStack[-1]
        attrs = self.attrsStack[-1]


        for att in currentState.attrs:
            # set values from latets format            
            if hasattr(format, att):
                setattr(newFormatState, att, getattr(format, att) )

            # set values from latest attrs
            for name, value in attrs:
                if name == att and name in currentState.attrs:
                    setattr(newFormatState, att, value)
        newFormatState.name = format.name

        self.formatStateStack.append(newFormatState)

    def popCurrentFormat(self):
        if not len(self.formatStateStack):
            return
        
        self.formatStateStack.pop(-1)


    ################## process special tags #################

    def processBodyTag(self, token):
        b = Primative(token.attrs, 0, 0, 0)

        bgColor = b.findAttr("bgcolor")
        if bgColor:
            self.bgColor = parseColor(bgColor)

        textColor = b.findAttr("text")
        if textColor:
            self.fgColor = parseColor(textColor)
    
    def draw(self, renderer):
        renderer.drawRect(self.bgColor, self.windowRect)
        self.areas[0].draw(renderer, self.windowRect[0], self.windowRect[1], self.scrollbar.currentItem, self.height)
        self.scrollbar.draw(renderer)

class FormatState:
    ### List of attributes that can stack with default values

    attrs = ("bgcolor","align","valign","size","face","color","italic","underline","bold")
    
    def __init__(self):
        self.name="empty"
        self.bgcolor   = "#dddddd"
        self.align     = 0
        self.valign    = 0
        self.size      = 10
        self.face      = "times"
        self.italic    = 0
        self.underline = 0
        self.bold      = 0
        self.color     = None
    
class FormatElement:
    """An element that sets the formatting mode. eg. <b> or <H1>.
    An element can require that it starts on a new line, and that it is ended by a new line.
    """
    def __init__(self,  tag, name, newlineBefore, newlineAfter, effects):
        self.tag = tag
        self.name = name
        self.newLineAfter = newlineAfter
        self.newLineBefore = newlineBefore
        self.font = None
        self.__dict__.update(effects)

    def getTextSize(self, text):
        return self.renderer.getTextSize(text, self.font)
    
formatElements = [
    #tag  before after effects                              name
    ('p',   1,  1,   {},                                    "Paragraph"),
    ('h1',  1,  1,   {'face':'times', 'size':32, 'bold':1,'italic':0}, "Heading 1"),
    ('h2',  1,  1,   {'face':'times', 'size':26, 'bold':1,'italic':0}, "Heading 2"),
    ('h3',  1,  1,   {'face':'times', 'size':18, 'bold':1,'italic':0}, "Heading 3"),
    ('b',   0,  0,   {'bold':1},                            "Bold"),
    ('i',   0,  0,   {'italic':1},                          "Italic"),
    ('br',  1,  0,   {},                                    "Break"),    
    ('pre', 1,  1,   {'face':'courier','size':10,'bold':0,'italic':0}, "Preformatted"),
    ('li',  0,  1,   {'italic':0, 'bold':0},                "List Item"),
    ('a',   0,  0,   {'underline':1},                       "Link"),
    ('font',0,  0,   {},                                    "Font"),
    ('tr',  1,  1,   {},                                    "Table Row"),
    ('td',  0,  0,   {},                                    "Table Cell")        
    ]


class SpecialElement:
    """An element with special, non-local effects. these elements dont actually draw anything.
    """
    
specialElements = [
    # tag
    ('body',),
    ('title',),
    ('head',),
    ('table',),
    ('html',)
    ]

class VisibleElement:
    """Elements that display non-textual things to the screeen.
    """
    def __init__(self,  tag, name, newlineBefore, newlineAfter, instanceClass):
        self.tag = tag
        self.name = name
        self.newLineAfter = newlineAfter
        self.newLineBefore = newlineBefore
        self.instanceClass = instanceClass

visibleElements = [
    # tag  before  after class 
    ('img', 0,     0,    ImagePrimative, "Image"),
    ('hr',  1,     1,    HRPrimative,    "Horizontal Line"),
    ('table',1,    1,    PrimativeTable, "Table"),
    
    ]
    
    
    
