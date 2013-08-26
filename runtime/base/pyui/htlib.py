import pyui

class HTMLPanel(pyui.widgets.Panel):
    pass


testText ="""

<HTML>

<HEAD>
  <TITLE> PyUI HTML Panel Test </TITLE>
</HEAD>

<BODY bgColor="#ffffd0" text="#000000">

<H1 color=#ff0000> PyUI HTML Test </H1>

<p>Some regular &quot; text in a normal font here. This should be
<b>bold</b> but the rest of this should be normal text.</p>

<img src="max.bmp"> <b>some text here <i>between the two images<img src="max.bmp" width=50 height=50>

<h2> Heading Two </h2>
</i>
<p color="#000055">Menus are instances of the class widgets.Menu. They have menu
items added to them with the addItem() method. The addItem method
takes a title, a <a href="http://www.news.com">handler method</a> and possibly a sub-menu. The <a>handler
method will be called with the menuItem instance as the argument.
The sub-menu <font color="#bb0000" face="arial" >argument can be used to add</font> sub-menu </a>to the menu
allowing hierarchies of menus.</p>

<pre>
Pre-formatted text:

class PyuiHTMLParser(HTMLParser):

    def __init__(self, &quot;panel&quot;):
        self.panel = panel
        
    def handle_starttag(self, &quot;tag&quot;, attrs):
        self.panel.handle_starttag(tag, attrs)

</pre>

<p> Below is a list of elements</p>
<OL>
  <li> List item 1 </li>
  <li> List item 2 </li>
  <li> List item 3 </li>
  <li> List item 4 </li>  
  <li> A very long List item goes here. </li>
</OL>

<p> Below is a horizontal line </p>
<HR>

<p color=#00ff00>Menus are instances of the class widgets.Menu. They have menu
items added to them with the addItem() method. The addItem method
takes a title, a handler method and possibly a sub-menu. The handler
method will be called with the menuItem instance as the argument.
The sub-menu argument can be used to add sub-menu to the menu
allowing hierarchies of menus.</p>

<img src="max.bmp" width=400 height=100>

<p>Menus are instances of the class widgets.Menu. They have menu
items added to them with the addItem() method. The addItem method
takes a title, a handler method and possibly a sub-menu. The handler
method will be called with the menuItem instance as the argument.
The sub-menu argument can be used to add sub-menu to the menu
allowing hierarchies of menus.</p>

<p>Menus are instances of the class widgets.Menu. They have menu
items added to them with the addItem() method. The addItem method
takes a title, a handler method and possibly a sub-menu. The handler
method will be called with the menuItem instance as the argument.
The sub-menu argument can be used to add sub-menu to the menu
allowing hierarchies of menus.</p>

<HR>

<p> Bottom of the page </p>

</BODY>
</HTML>
"""

import htmllib
import formatter

awriter = formatter.AbstractWriter()
aformatter = formatter.AbstractFormatter(awriter)
parser = htmllib.HTMLParser(aformatter)

parser.feed(testText)
