false = False
true = True
#-------------------------------------------------------------------------------
# wxStyledTextCtrl default settings, edited on a seperate config node.
# Docs from the Scintilla web page

# Makes end-of-line characters visible or not.
STCViewEOL = false
# Determines whether indentation should be created out of a mixture of tabs and
# space or be based purely on spaces.
STCUseTabs = false
# Sets the size of a tab as a multiple of the size of a space character in the
# style of the language's default style definition.
STCTabWidth = 4
# Sets the size of indentation in terms of characters.
STCIndent = 4
# Margin width used for line numbering
STCLineNumMarginWidth = 28
# Margin width used by symbols such as breakpoints and line pointers
STCSymbolMarginWidth = 15
# Margin width used for line folding, set to 0 to 'disable' folding
STCFoldingMarginWidth = 13
# Turns buffered drawing on or off. Buffered drawing draws each line into a bitmap
# rather than directly to the screen and then copies the bitmap to the screen.
# This avoids flickering although it does take slightly longer.
STCBufferedDraw = true
# Indentation guides are dotted vertical lines that appear within indentation
# whitespace every indent size columns. They make it easy to see which constructs
# line up especially when they extend over multiple pages.
STCIndentationGuides = false

from wxPython.stc import wxSTC_WS_INVISIBLE, wxSTC_WS_VISIBLEALWAYS, wxSTC_WS_VISIBLEAFTERINDENT
# White space can be made visible. Space characters appear as small centred dots
# and tab characters as light arrows pointing to the right.
# With the SCWS_VISIBLEAFTERINDENT option, white space used for indentation is
# invisible but after the first visible character, it is visible.
## options: wxSTC_WS_INVISIBLE, wxSTC_WS_VISIBLEALWAYS, wxSTC_WS_VISIBLEAFTERINDENT
STCViewWhiteSpace = wxSTC_WS_INVISIBLE

from wxPython.stc import wxSTC_CARET_SLOP, wxSTC_CARET_STRICT
wxSTC_CARET_SLOP_STRICT = wxSTC_CARET_SLOP | wxSTC_CARET_STRICT
# Can be set to a combination of the flags CARET_SLOP and CARET_STRICT to change
# the automatic vertical positioning of the view when ensuring a position is visible.
# If CARET_SLOP is off then the caret is centred within the view.
# When CARET_STRICT is set then caret policy is rechecked even if the caret is completely visible.
# Setting this value to 0 will leave the policy at startup default
## options: 0, wxSTC_CARET_SLOP, wxSTC_CARET_STRICT, wxSTC_CARET_SLOP_STRICT
STCCaretPolicy = 0
# If CARET_SLOP is on then the slop value determines the number of lines at top
# and bottom of the view where the caret should not go.
STCCaretPolicySlop = 0
# Sets rate at which the caret blinks, this determines the time in milliseconds
# that the caret is visible or invisible before changing state.
# Setting the period to 0 stops the caret blinking.
STCCaretPeriod = 500

from wxPython.stc import wxSTC_EDGE_NONE, wxSTC_EDGE_LINE, wxSTC_EDGE_BACKGROUND
# This mechanism marks lines that are longer than a specified length in one of two ways.
# A vertical line can be displayed at the specified column number (EDGE_LINE) or
# characters after that column can be displayed with a specified background colour
# (EDGE_BACKGROUND). The vertical line works well for monospaced fonts but not for
# proportional fonts which should use EDGE_BACKGROUND.
## options: wxSTC_EDGE_NONE, wxSTC_EDGE_LINE, wxSTC_EDGE_BACKGROUND
STCEdgeMode = wxSTC_EDGE_LINE
STCEdgeColumnWidth = 80

# Colours
STCCallTipBackColour = wxColour(255, 255, 240)
STCSyntaxErrorColour = wxColour(255, 0, 0)
STCCodeBrowseColour = wxColour(0, 0, 255)
STCDebugBrowseColour = wxColour(255, 0, 0)

# Markers
from wxPython.stc import wxSTC_MARK_CIRCLE, wxSTC_MARK_ROUNDRECT, \
      wxSTC_MARK_ARROW, wxSTC_MARK_SMALLRECT, wxSTC_MARK_SHORTARROW, \
      wxSTC_MARK_EMPTY, wxSTC_MARK_ARROWDOWN, wxSTC_MARK_MINUS, wxSTC_MARK_PLUS

STCLinePointer = wxSTC_MARK_SHORTARROW, 'BLACK', 'BLUE'
STCBreakpointMarker = wxSTC_MARK_CIRCLE, 'BLACK', 'RED'
STCTmpBreakpointMarker = wxSTC_MARK_CIRCLE, 'BLACK', 'BLUE'
STCDisabledBreakpointMarker = wxSTC_MARK_CIRCLE, 'BLACK', wxColour(0xCC, 0xCC, 0xCC)
STCMarkPlaceMarker = wxSTC_MARK_SHORTARROW, 'BLACK', 'YELLOW'

STCDiffAddedMarker = wxSTC_MARK_PLUS, 'BLACK', 'WHITE'
STCDiffRemovedMarker = wxSTC_MARK_MINUS, 'BLACK', 'WHITE'
STCDiffChangesMarker = wxSTC_MARK_SMALLRECT, 'BLACK', 'WHITE'

STCFoldingOpen = wxSTC_MARK_MINUS, 'BLACK', 'WHITE'
STCFoldingClose = wxSTC_MARK_PLUS, 'BLACK', 'WHITE'

