#Boa:MDIParent:MDISourceFrame

from wxPython.wx import *
from wxPython.lib.anchors import LayoutAnchors

from Debugger.DebuggerProxy import CDebuggerProxy, EVT_DEBUGGER_STOPPED

from WatchPanel import *

import MDIChild

def create(parent):
    return MDISourceFrame(parent)

[wxID_MDISOURCEFRAME, wxID_MDISOURCEFRAMEFILEEXPLORER, 
 wxID_MDISOURCEFRAMENOTEBOOK1, wxID_MDISOURCEFRAMESASHDEBUGGER, 
 wxID_MDISOURCEFRAMESASHLAYOUTWINDOW1, wxID_MDISOURCEFRAMESTATUSBAR, 
 wxID_MDISOURCEFRAMETOOLBARDEBUG, wxID_MDISOURCEFRAMETREECTRL1, 
] = map(lambda _init_ctrls: wxNewId(), range(8))

[wxID_MDISOURCEFRAMEMENU1ITEMS0, wxID_MDISOURCEFRAMEMENU1ITEMS1, 
] = map(lambda _init_coll_menu1_Items: wxNewId(), range(2))

[wxID_MDISOURCEFRAMETOOLBARDEBUGTOOLS_CONTINUE, 
 wxID_MDISOURCEFRAMETOOLBARDEBUGTOOLS_PAUSE, 
 wxID_MDISOURCEFRAMETOOLBARDEBUGTOOLS_RETURN, 
 wxID_MDISOURCEFRAMETOOLBARDEBUGTOOLS_STEP_IN, 
 wxID_MDISOURCEFRAMETOOLBARDEBUGTOOLS_STEP_OVER, 
 wxID_MDISOURCEFRAMETOOLBARDEBUGTOOLS_STOP_DEBUG, 
] = map(lambda _init_coll_toolBarDebug_Tools: wxNewId(), range(6))

[wxID_MDISOURCEFRAMEWATCHES] = map(lambda _init_ctrls: wxNewId(), range(1))

class MDISourceFrame(wxMDIParentFrame):
    def _init_coll_menu1_Items(self, parent):
        # generated method, don't edit

        parent.Append(helpString='Items0', id=wxID_MDISOURCEFRAMEMENU1ITEMS0,
              item='New child window')
        parent.Append(helpString='', id=wxID_MDISOURCEFRAMEMENU1ITEMS1,
              item='Open', kind=wxITEM_NORMAL)
        EVT_MENU(self, wxID_MDISOURCEFRAMEMENU1ITEMS0, self.OnMenu1items0Menu)
        EVT_MENU(self, wxID_MDISOURCEFRAMEMENU1ITEMS1, self.OnMenu1items1Menu)

    def _init_coll_menuBar1_Menus(self, parent):
        # generated method, don't edit

        parent.Append(menu=self.menu1, title='&File')

    def _init_coll_notebook1_Pages(self, parent):
        # generated method, don't edit

        parent.AddPage(imageId=-1, page=self.fileExplorer, select=False,
              text='Sylphis Files')
        parent.AddPage(imageId=-1, page=self.treeCtrl1, select=True,
              text='Sylphis Classes')

    def _init_coll_toolBarDebug_Tools(self, parent):
        # generated method, don't edit

        parent.AddTool(bitmap=wxBitmap('base/devenv/Images/Debug/Debug.png',
              wxBITMAP_TYPE_PNG),
              id=wxID_MDISOURCEFRAMETOOLBARDEBUGTOOLS_CONTINUE, isToggle=false,
              longHelpString='', pushedBitmap=wxNullBitmap,
              shortHelpString='Continue')
        parent.AddTool(bitmap=wxBitmap('base/devenv/Images/Debug/Pause.png',
              wxBITMAP_TYPE_PNG), id=wxID_MDISOURCEFRAMETOOLBARDEBUGTOOLS_PAUSE,
              isToggle=false, longHelpString='', pushedBitmap=wxNullBitmap,
              shortHelpString='Pause')
        parent.AddTool(bitmap=wxBitmap('base/devenv/Images/Debug/Stop.png',
              wxBITMAP_TYPE_PNG),
              id=wxID_MDISOURCEFRAMETOOLBARDEBUGTOOLS_STOP_DEBUG,
              isToggle=false, longHelpString='', pushedBitmap=wxNullBitmap,
              shortHelpString='Stop debug')
        parent.AddTool(bitmap=wxBitmap('base/devenv/Images/Debug/Step.png',
              wxBITMAP_TYPE_PNG),
              id=wxID_MDISOURCEFRAMETOOLBARDEBUGTOOLS_STEP_IN, isToggle=false,
              longHelpString='', pushedBitmap=wxNullBitmap,
              shortHelpString='Step in')
        parent.AddTool(bitmap=wxBitmap('base/devenv/Images/Debug/Over.png',
              wxBITMAP_TYPE_PNG),
              id=wxID_MDISOURCEFRAMETOOLBARDEBUGTOOLS_STEP_OVER, isToggle=false,
              longHelpString='', pushedBitmap=wxNullBitmap,
              shortHelpString='Step over')
        parent.AddTool(bitmap=wxBitmap('base/devenv/Images/Debug/Out.png',
              wxBITMAP_TYPE_PNG),
              id=wxID_MDISOURCEFRAMETOOLBARDEBUGTOOLS_RETURN, isToggle=false,
              longHelpString='', pushedBitmap=wxNullBitmap,
              shortHelpString='Return')
        EVT_TOOL(self, wxID_MDISOURCEFRAMETOOLBARDEBUGTOOLS_CONTINUE,
              self.OnToolbardebugtools0Tool)
        EVT_TOOL(self, wxID_MDISOURCEFRAMETOOLBARDEBUGTOOLS_PAUSE,
              self.OnToolbardebugtools1Tool)
        EVT_TOOL(self, wxID_MDISOURCEFRAMETOOLBARDEBUGTOOLS_STOP_DEBUG,
              self.OnToolbardebugtools2Tool)
        EVT_TOOL(self, wxID_MDISOURCEFRAMETOOLBARDEBUGTOOLS_STEP_IN,
              self.OnToolbardebugtools3Tool)
        EVT_TOOL(self, wxID_MDISOURCEFRAMETOOLBARDEBUGTOOLS_STEP_OVER,
              self.OnToolbardebugtools4Tool)
        EVT_TOOL(self, wxID_MDISOURCEFRAMETOOLBARDEBUGTOOLS_RETURN,
              self.OnToolbardebugtools5Tool)

        parent.Realize()

    def _init_utils(self):
        # generated method, don't edit
        self.menuBar1 = wxMenuBar()

        self.menu1 = wxMenu(title='')
        self._init_coll_menu1_Items(self.menu1)

        self._init_coll_menuBar1_Menus(self.menuBar1)

    def _init_ctrls(self, prnt):
        # generated method, don't edit
        wxMDIParentFrame.__init__(self, id=wxID_MDISOURCEFRAME,
              name='MDISourceFrame', parent=prnt, pos=wxPoint(593, 368),
              size=wxSize(621, 509),
              style=wxDEFAULT_FRAME_STYLE | wxVSCROLL | wxHSCROLL,
              title='Editor')
        self._init_utils()
        self.SetMenuBar(self.menuBar1)
        self.SetAutoLayout(True)
        self.SetClientSize(wxSize(613, 475))
        EVT_SIZE(self, self.OnWxmdiparentframe1Size)

        self.sashLayoutWindow1 = wxSashLayoutWindow(id=wxID_MDISOURCEFRAMESASHLAYOUTWINDOW1,
              name='sashLayoutWindow1', parent=self, pos=wxPoint(0, 59),
              size=wxSize(296, 232),
              style=wxMINIMIZE_BOX | wxCLIP_CHILDREN | wxSW_3D)
        self.sashLayoutWindow1.SetOrientation(wxLAYOUT_VERTICAL)
        self.sashLayoutWindow1.SetAlignment(wxLAYOUT_LEFT)
        self.sashLayoutWindow1.SetSashVisible(wxSASH_RIGHT, true)
        self.sashLayoutWindow1.SetDefaultSize(wxSize(296, 232))
        EVT_SASH_DRAGGED(self.sashLayoutWindow1,
              wxID_MDISOURCEFRAMESASHLAYOUTWINDOW1,
              self.OnSashlayoutwindow1SashDragged)

        self.notebook1 = wxNotebook(id=wxID_MDISOURCEFRAMENOTEBOOK1,
              name='notebook1', parent=self.sashLayoutWindow1, pos=wxPoint(0,
              0), size=wxSize(293, 232), style=0)

        self.treeCtrl1 = wxTreeCtrl(id=wxID_MDISOURCEFRAMETREECTRL1,
              name='treeCtrl1', parent=self.notebook1, pos=wxPoint(0, 0),
              size=wxSize(285, 206), style=wxTR_HAS_BUTTONS,
              validator=wxDefaultValidator)

        self.fileExplorer = wxGenericDirCtrl(defaultFilter=0,
              dir='c:\\sylphis_runtime\\', filter='',
              id=wxID_MDISOURCEFRAMEFILEEXPLORER, name='fileExplorer',
              parent=self.notebook1, pos=wxPoint(0, 0), size=wxSize(285, 206),
              style=wxDIRCTRL_SELECT_FIRST | wxDIRCTRL_SHOW_FILTERS | wxDIRCTRL_3D_INTERNAL | wxSUNKEN_BORDER)
        self.fileExplorer.SetToolTipString('File explorer')

        self.toolBarDebug = wxToolBar(id=wxID_MDISOURCEFRAMETOOLBARDEBUG,
              name='toolBarDebug', parent=self, pos=wxPoint(0, 0),
              size=wxSize(613, 27), style=wxTB_HORIZONTAL | wxNO_BORDER)
        self.toolBarDebug.SetConstraints(LayoutAnchors(self.toolBarDebug, True,
              True, False, False))
        self.toolBarDebug.SetAutoLayout(True)
        self._init_coll_toolBarDebug_Tools(self.toolBarDebug)
        self.SetToolBar(self.toolBarDebug)

        self.statusBar = wxStatusBar(id=wxID_MDISOURCEFRAMESTATUSBAR,
              name='statusBar', parent=self, style=0)
        self.statusBar.SetSize(wxSize(613, 23))
        self.statusBar.SetPosition(wxPoint(0, 389))
        self.SetStatusBar(self.statusBar)

        self.sashDebugger = wxSashLayoutWindow(id=wxID_MDISOURCEFRAMESASHDEBUGGER,
              name='sashDebugger', parent=self, pos=wxPoint(255, 85),
              size=wxSize(200, 100), style=wxCLIP_CHILDREN | wxSW_3D)
        self.sashDebugger.SetDefaultSize(wxSize(200, 100))
        self.sashDebugger.SetAlignment(wxLAYOUT_BOTTOM)
        self.sashDebugger.SetSashVisible(wxSASH_BOTTOM, False)
        self.sashDebugger.SetOrientation(wxLAYOUT_HORIZONTAL)
        self.sashDebugger.SetLabel('Debugger')
        self.sashDebugger.SetSashVisible(wxSASH_RIGHT, False)
        self.sashDebugger.SetConstraints(LayoutAnchors(self.sashDebugger, False,
              True, False, False))
        self.sashDebugger.SetSashVisible(wxSASH_TOP, True)
        self.sashDebugger.SetAutoLayout(False)
        EVT_SASH_DRAGGED(self.sashDebugger, wxID_MDISOURCEFRAMESASHDEBUGGER,
              self.OnSashdebuggerSashDragged)

        self._init_coll_notebook1_Pages(self.notebook1)

    def __init__(self, parent):
        self._init_ctrls(parent)
        
        self.watches = WatchPanel(id=wxID_MDISOURCEFRAMEWATCHES, 
              name='watches', parent=self.sashDebugger, pos=wxPoint(32, 48),
              size=wxSize(75, 23), style = 0)
              
        from rpcproxy import CRPCProxy
        self.rs = CRPCProxy("http://127.0.0.1:8000")
        
        self.debugProxy = CDebuggerProxy(self)
        try:
            import entry
            entry.debugger.setDebuggerProxy(self.debugProxy)
        except ImportError:
            pass
        
        EVT_DEBUGGER_STOPPED(self, wxID_MDISOURCEFRAME, self.onDebuggerStoped)
        
    def clearStepMarker(self):
        if hasattr(self, 'lastStepEditor'):
            self.lastStepEditor.clearStepMarker(self.lastStepLine)
        
    def onDebuggerStoped(self, event):
        status = self.rs.getExecStatus()
        filename = status[0]
        line = status[1] - 1
        edit = self.openFile(filename)
        edit.setStepPos(line)
        edit.gotoLine(line - 1)
        self.lastStepEditor = edit
        self.lastStepLine = line

    def OnMenu1items0Menu(self, event):
        MDIChild.create(self).Show(true)

    def OnWxmdiparentframe1Size(self, event):
        wxLayoutAlgorithm().LayoutMDIFrame(self)

    def OnSashlayoutwindow1SashDragged(self, event):
        if event.GetDragStatus() == wxSASH_STATUS_OUT_OF_RANGE:
            return

        eID = event.GetId()
        if eID == wxID_MDISOURCEFRAMESASHLAYOUTWINDOW1:
            self.sashLayoutWindow1.SetDefaultSize(wxSize(event.GetDragRect().width, 0))

        wxLayoutAlgorithm().LayoutMDIFrame(self)
        self.GetClientWindow().Refresh()

    def OnSashdebuggerSashDragged(self, event):
        if event.GetDragStatus() == wxSASH_STATUS_OUT_OF_RANGE:
            return

        eID = event.GetId()
        if eID == wxID_MDISOURCEFRAMESASHDEBUGGER:
            self.sashDebugger.SetDefaultSize(wxSize(0, event.GetDragRect().height))

        wxLayoutAlgorithm().LayoutMDIFrame(self)
        self.GetClientWindow().Refresh()

        
    openFiles = {}
    def openFile(self, fileName):
        if fileName in self.openFiles.keys():
            return self.openFiles[fileName]
        
        n = MDIChild.create(self, fileName)
        n.Show()
        self.last = n
        self.openFiles[fileName] = n
        return n

    def OnMenu1items1Menu(self, event):
        dlg = wxFileDialog(self, 'Choose a file', '.', '', '*.*', wxOPEN)
        try:
            if dlg.ShowModal() == wxID_OK:
                filename = dlg.GetPath()
                self.openFile(filename)
        finally:
            dlg.Destroy()

 
    def OnToolbardebugtools0Tool(self, event):
        self.clearStepMarker()
        self.rs.callNonBlocking('cont')

    def OnToolbardebugtools1Tool(self, event):
        self.clearStepMarker()
        self.rs.callNonBlocking('break')

    def OnToolbardebugtools2Tool(self, event):
        event.Skip()

    def OnToolbardebugtools3Tool(self, event):
        self.clearStepMarker()
        self.rs.callNonBlocking('step')

    def OnToolbardebugtools4Tool(self, event):
        self.clearStepMarker()
        self.rs.callNonBlocking('next')

    def OnToolbardebugtools5Tool(self, event):
        self.clearStepMarker()
        self.rs.callNonBlocking('ret')

        
            


if __name__ == '__main__':
    app = wxPySimpleApp()
    wxInitAllImageHandlers()
    frame = create(None)
    # needed when running from Boa under Windows 9X
    frame.Show();frame.Hide();frame.Show()

    app.MainLoop()
