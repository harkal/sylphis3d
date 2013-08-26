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

"""Useful local constants used by pyui
"""

TEXT_HEIGHT  = 16

# Border Layout options
WEST   = 1
EAST   = 2
NORTH  = 3
SOUTH  = 4
CENTER = 5

## Message IDs - these are default that may be populated by a renderer
EVENT_MASK          = 0xF000
NONE                = 0
EVENT_MOUSE         = 0x1000
MOUSEMOVE           = 0x1000
LMOUSEBUTTONDOWN    = 0x1001
LMOUSEBUTTONUP      = 0x1002
RMOUSEBUTTONDOWN    = 0x1003
RMOUSEBUTTONUP      = 0x1004
MMOUSEBUTTONDOWN    = 0x1005
MMOUSEBUTTONUP      = 0x1006
MOUSEWHEEL          = 0x1007
LMOUSEDBLCLICK      = 0x1008
RMOUSEDBLCLICK      = 0x1009
MMOUSEDBLCLICK      = 0x100A

EVENT_KEYBOARD      = 0x2000
CHAR                = 0x2000
KEYDOWN             = 0x2001
KEYUP               = 0x2002

EVENT_SYSTEM        = 0x3000
CLICKED             = 0x3000
SCROLLPOS           = 0x3001
DIALOGCLOSED        = 0x3002
LIST_SELECTED       = 0x3003
TREENODE_SELECTED   = 0x3004
QUIT                = 0x3005
MENU_EXIT           = 0x3006
LIST_DBLCLICK       = 0x3007


USEREVENT           = 0x4000


# Theme constants
THEME_SCREEN_WIDTH         = "SCREENWIDTH"
THEME_SCREEN_HEIGHT        = "SCREENHEIGHT"

# Hit region IDs
HIT_FRAME_MOVE                  = "FRAME_MOVE"
HIT_FRAME_CLOSE                 = "FRAME_CLOSE"
HIT_FRAME_RESIZE_RIGHT          = "FRAME_RESIZE_RIGHT"
HIT_FRAME_RESIZE_BOTTOM         = "FRAME_RESIZE_BOTTOM"
HIT_FRAME_RESIZE_BOTTOM_RIGHT   = "FRAME_RESIZE_BOTTOM_RIGHT"

# primative drawing commands
RECT     = 0
GRADIENT = 1
IMAGE    = 2
TEXT     = 3
VIEW     = 4
CLIP     = 5
LINE     = 6
IMAGE_ROT = 7

# Controller Types
CONTROLLER_UNINITIALIZED    = 0
CONTROLLER_MOVE             = 1
CONTROLLER_REACH            = 2
CONTROLLER_ANIMATION        = 3
CONTROLLER_2DWINDOW         = 4


# Key codes - may be populated by a renderer
K_BACKSPACE = 0x08
K_TAB       = 0x09
K_RETURN    = 0x0D
K_SHIFT     = 0x10
K_CONTROL   = 0x11
K_ALT       = 0x12
K_PAUSE     = 0x13
K_CAPITAL   = 0x14
K_ESCAPE    = 0x1B

K_SPACE     = 0x20
K_PAGEUP    = 0x21
K_PAGEDOWN  = 0x22
K_END       = 0x23
K_HOME      = 0x24
K_LEFT      = 0x25
K_UP        = 0x26
K_RIGHT     = 0x27
K_DOWN      = 0x28
K_INSERT    = 0x2D
K_DELETE    = 0x2E

K_0         = 0x30
K_1         = 0x31
K_2         = 0x32
K_3         = 0x33
K_4         = 0x34
K_5         = 0x35
K_6         = 0x36
K_7         = 0x37
K_8         = 0x38
K_9         = 0x39

K_A         = 0x41
K_B         = 0x42
K_C         = 0x43
K_D         = 0x44
K_E         = 0x45
K_F         = 0x46
K_G         = 0x47
K_H         = 0x48
K_I         = 0x49
K_J         = 0x4A
K_K         = 0x4B
K_L         = 0x4C
K_M         = 0x4D
K_N         = 0x4E
K_O         = 0x4F
K_P         = 0x50
K_Q         = 0x51
K_R         = 0x52
K_S         = 0x53
K_T         = 0x54
K_U         = 0x55
K_V         = 0x56
K_W         = 0x57
K_X         = 0x58
K_Y         = 0x59
K_Z         = 0x5A

K_PAD0      = 0x60
K_PAD1      = 0x61
K_PAD2      = 0x62
K_PAD3      = 0x63
K_PAD4      = 0x64
K_PAD5      = 0x65
K_PAD6      = 0x66
K_PAD7      = 0x67
K_PAD8      = 0x68
K_PAD9      = 0x69

K_PADTIMES  = 0x6A
K_PADPLUS   = 0x6B
K_PADMINUS  = 0x6D
K_PADDECIMAL= 0x6E
K_PADDIVIDE = 0x6F
K_PADENTER  = 0x0D

K_F1        = 0x70
K_F2        = 0x71
K_F3        = 0x72
K_F4        = 0x73
K_F5        = 0x74
K_F6        = 0x75
K_F7        = 0x76
K_F8        = 0x77
K_F9        = 0x78
K_F10       = 0x79
K_F11       = 0x7A
K_F12       = 0x7B

K_NUMLOCK   = 0x90
K_SCROLL    = 0x91


# Key Modifiers
MOD_NONE        = 0
MOD_LSHIFT      = 1
MOD_RSHIFT      = 2
MOD_LCONTROL    = 64
MOD_RCONTROL    = 128
MOD_LALT        = 256
MOD_RALT        = 512
MOD_SHIFT       = MOD_LSHIFT | MOD_RSHIFT
MOD_CONTROL     = MOD_LCONTROL | MOD_RCONTROL
MOD_ALT         = MOD_LALT | MOD_RALT

# Font effects
BOLD = 1
ITALIC = 2
UNDERLINE = 4
