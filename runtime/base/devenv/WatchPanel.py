#Boa:FramePanel:WatchPanel

from wxPython.wx import *
from wxPython.grid import *

[wxID_WATCHPANEL, wxID_WATCHPANELNOTEBOOK1, wxID_WATCHPANELWATCHES, 
] = map(lambda _init_ctrls: wxNewId(), range(3))

class WatchPanel(wxPanel):
    def _init_coll_watches_Columns(self, parent):
        # generated method, don't edit

        parent.InsertColumn(col=0, format=wxLIST_FORMAT_LEFT,
              heading='Variable', width=-1)
        parent.InsertColumn(col=1, format=wxLIST_FORMAT_LEFT, heading='Value',
              width=-1)

    def _init_coll_notebook1_Pages(self, parent):
        # generated method, don't edit

        parent.AddPage(imageId=-1, page=self.watches, select=True,
              text='Watches')

    def _init_utils(self):
        # generated method, don't edit
        pass

    def _init_ctrls(self, prnt):
        # generated method, don't edit
        wxPanel.__init__(self, id=wxID_WATCHPANEL, name='WatchPanel',
              parent=prnt, pos=wxPoint(692, 444), size=wxSize(269, 223),
              style=wxTAB_TRAVERSAL)
        self._init_utils()
        self.SetClientSize(wxSize(261, 189))
        self.SetAutoLayout(True)
        self.SetBackgroundColour(wxColour(236, 100, 216))
        EVT_SIZE(self, self.OnWatchpanelSize)

        self.notebook1 = wxNotebook(id=wxID_WATCHPANELNOTEBOOK1,
              name='notebook1', parent=self, pos=wxPoint(0, 0), size=wxSize(261,
              189), style=0)
        self.notebook1.SetAutoLayout(True)

        self.watches = wxListCtrl(id=wxID_WATCHPANELWATCHES, name='watches',
              parent=self.notebook1, pos=wxPoint(0, 0), size=wxSize(253, 163),
              style=wxLC_REPORT | wxLC_SINGLE_SEL | wxLC_VRULES | wxCLIP_CHILDREN,
              validator=wxDefaultValidator)
        self._init_coll_watches_Columns(self.watches)

        self._init_coll_notebook1_Pages(self.notebook1)

    def __init__(self, parent, id, pos, size, style, name):
        self._init_ctrls(parent)

    def OnWatchpanelSize(self, event):
        self.notebook1.SetSize(self.GetSize())

