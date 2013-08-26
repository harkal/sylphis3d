#Boa:MDIChild:MDIChild

from wxPython.wx import *
from wxPython.stc import *

def create(parent, filename):
    return MDIChild(parent, filename)

[wxID_MDICHILD] = map(lambda _init_ctrls: wxNewId(), range(1))

from StyledTextCtrls import wxStyledTextCtrl, FoldingStyledTextCtrlMix, PythonStyledTextCtrlMix
from StyledTextCtrls import DebuggingViewSTCMix

[wxID_MDICHILDEDITOR] = map(lambda __init__: wxNewId(), range(1))

brkPtMrk, tmpBrkPtMrk, disabledBrkPtMrk, stepPosMrk = range(3, 3 + 4)
class PythonText(wxStyledTextCtrl, FoldingStyledTextCtrlMix, PythonStyledTextCtrlMix,
                DebuggingViewSTCMix):
    def __init__(self, parent, id, filename, *args):
        wxStyledTextCtrl.__init__(self, id = id, parent = parent, *args)
##        StyledTextCtrls.CallTipCodeHelpSTCMix.__init__(self)

##        StyledTextCtrls.AutoCompleteCodeHelpSTCMix.__init__(self)
        self.filename = filename
        FoldingStyledTextCtrlMix.__init__(self, id, 2)

        PythonStyledTextCtrlMix.__init__(self, id, (0, 30))
        DebuggingViewSTCMix.__init__(self, (brkPtMrk, tmpBrkPtMrk,

              disabledBrkPtMrk, stepPosMrk))
      
        self.setupDebuggingMargin(1) # the margin number (0 is the first)
        
        self.SetIndentationGuides(True)
        
        EVT_STC_MARGINCLICK(self, id, self.OnMarginClick)
        self.LoadFile(filename)
        self.active = True # Investigate
        
    def getDebugger(self):
        import entry
        return entry.debugger.proxy
            
    def OnMarginClick(self, event):

        if event.GetMargin() == 1:

            DebuggingViewSTCMix.OnMarginClick(self, event)

        else:

            FoldingStyledTextCtrlMix.OnMarginClick(self, event)



class MDIChild(wxMDIChildFrame):
    def _init_utils(self):
        # generated method, don't edit
        pass

    def _init_ctrls(self, prnt):
        # generated method, don't edit
        wxMDIChildFrame.__init__(self, id=wxID_MDICHILD, name='MDIChild',
              parent=prnt, pos=wxPoint(457, 277), size=wxSize(565, 496),
              style=wxDEFAULT_FRAME_STYLE, title='Child')
        self._init_utils()
        self.SetClientSize(wxSize(557, 462))
        self.Center(wxBOTH)

    def __init__(self, parent, filename):
        self._init_ctrls(parent)
        self.editor = PythonText(id=wxID_MDICHILDEDITOR, parent=self, filename = filename)
        self.editor.SetPrintColourMode(wxSTC_PRINT_COLOURONWHITEDEFAULTBG)
        self.SetTitle(filename)
        
    def setStepPos(self, pos):
        self.editor.setStepPos(pos)
        
    def clearStepMarker(self, pos):
        self.editor.clearStepPos(pos)
        
    def gotoLine(self, line):
        self.editor.GotoLine(line)
        
      
       
