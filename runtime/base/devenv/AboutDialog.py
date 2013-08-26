#Boa:Dialog:AboutDialog

from wxPython.wx import *

def create(parent):
    return AboutDialog(parent)

[wxID_ABOUTDIALOG, wxID_ABOUTDIALOGALLRIGHTS, wxID_ABOUTDIALOGBUTTON1, 
 wxID_ABOUTDIALOGSTATICTEXT1, wxID_ABOUTDIALOGSTATICTEXT2, 
] = map(lambda _init_ctrls: wxNewId(), range(5))

class AboutDialog(wxDialog):
    def _init_utils(self):
        # generated method, don't edit
        pass

    def _init_ctrls(self, prnt):
        # generated method, don't edit
        wxDialog.__init__(self, id=wxID_ABOUTDIALOG, name='AboutDialog',
              parent=prnt, pos=wxPoint(597, 303), size=wxSize(431, 217),
              style=wxDEFAULT_DIALOG_STYLE, title='About SDE')
        self._init_utils()
        self.SetClientSize(wxSize(423, 183))
        self.Center(wxBOTH)

        self.button1 = wxButton(id=wxID_ABOUTDIALOGBUTTON1, label='Close',
              name='button1', parent=self, pos=wxPoint(144, 128),
              size=wxSize(136, 24), style=0)
        self.button1.SetToolTipString('Closes this dialog')
        EVT_BUTTON(self.button1, wxID_ABOUTDIALOGBUTTON1, self.OnButton1Button)

        self.staticText1 = wxStaticText(id=wxID_ABOUTDIALOGSTATICTEXT1,
              label='Sylphis Development Environment', name='staticText1',
              parent=self, pos=wxPoint(128, 24), size=wxSize(161, 13), style=0)

        self.staticText2 = wxStaticText(id=wxID_ABOUTDIALOGSTATICTEXT2,
              label='Copyright (c) 2003 Harry Kalogirou', name='staticText2',
              parent=self, pos=wxPoint(128, 72), size=wxSize(161, 13), style=0)

        self.allrights = wxStaticText(id=wxID_ABOUTDIALOGALLRIGHTS,
              label='All Rights Reserved', name='allrights', parent=self,
              pos=wxPoint(160, 88), size=wxSize(93, 13), style=0)

    def __init__(self, parent):
        self._init_ctrls(parent)

    def OnButton1Button(self, event):
        self.Close()
