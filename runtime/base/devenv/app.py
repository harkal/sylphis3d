#!/usr/bin/env python
#Boa:App:BoaApp

from wxPython.wx import *

import wxMainFrame

modules ={'MDIChild': [0, '', 'MDIChild.py'],
 'MDISourceFrame': [0, '', 'MDISourceFrame.py'],
 'wxMainFrame': [1, 'Main frame of Application', 'wxMainFrame.py']}

class BoaApp(wxApp):
    def OnInit(self):
        wxInitAllImageHandlers()
        self.main = wxMainFrame.create(None)
        # needed when running from Boa under Windows 9X
        self.SetTopWindow(self.main)
        self.main.Show();self.main.Hide();self.main.Show()
        return True

def main():
    application = BoaApp(0)
    application.MainLoop()

if __name__ == '__main__':
    import sys
    sys.path.insert(0, '..\\..\\stdlib\\')
    sys.path.insert(0, '..\\core\\')
    main()
