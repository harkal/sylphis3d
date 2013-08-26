"""
This file is part of the "Sylphis" 3D engine
Copyright (c) 2002 - 2007 by Harry Kalogirou

"Sylphis3D Game Engine" is free software; you can redistribute it and/or 
modify it under the terms of the GNU General Public License as 
published by the Free Software Foundation; either version 2 of the 
License, or (at your option) any later version, 
plus a special exception:
  Linking "Sylphis3D Game Engine" statically or dynamically with other 
modules is making a combined work based on this library. Thus, the 
terms and conditions of the GNU General Public License cover the whole 
combination. As a special exception, the copyright holders of this 
library give you permission to link this library with independent 
modules to produce an executable, regardless of the license terms of 
these independent modules, and to copy and distribute the resulting 
executable under terms of your choice, provided that you also meet, 
for each linked independent module, the terms and conditions of the 
license of that module. An independent module is a module which is 
not derived from or based on this program. If you modify this program,
you may extend this exception to your version of the library, but you 
are not obligated to do so. If you do not wish to do so, delete this 
exception statement from your version.

"Sylphis3D Game Engine" is distributed in the hope that it will be 
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "Sylphis3D Game Engine"; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
"""

from xmlrpclib import ServerProxy, _Method, dumps, getparser

class Transport:
    """Handles an HTTP transaction to an XML-RPC server."""

    # client identifier (may be overridden)
    user_agent = "CRPCProxy.py (by HarKal)"

    def request(self, host, handler, request_body, verbose=0, response=False):
        # issue XML-RPC request

        h = self.make_connection(host)
        if verbose:
            h.set_debuglevel(1)

        self.send_request(h, handler, request_body)
        self.send_host(h, host)
        self.send_user_agent(h)
        self.send_content(h, request_body)

        if response:
            errcode, errmsg, headers = h.getreply()

            if errcode != 200:
                raise ProtocolError(
                    host + handler,
                    errcode, errmsg,
                    headers
                    )

            self.verbose = verbose

            return self.parse_response(h.getfile())
        else:
            return

    def getparser(self):
        # get parser and unmarshaller
        return getparser()

    def make_connection(self, host):
        # create a HTTP connection object from a host descriptor
        import httplib
        return httplib.HTTP(host)

    def send_request(self, connection, handler, request_body):
        connection.putrequest("POST", handler)

    def send_host(self, connection, host):
        connection.putheader("Host", host)

    def send_user_agent(self, connection):
        connection.putheader("User-Agent", self.user_agent)

    def send_content(self, connection, request_body):
        connection.putheader("Content-Type", "text/xml")
        connection.putheader("Content-Length", str(len(request_body)))
        connection.endheaders()
        if request_body:
            connection.send(request_body)

    def parse_response(self, f):
        # read response from input file, and parse it

        p, u = self.getparser()

        while 1:
            response = f.read(1024)
            if not response:
                break
            if self.verbose:
                print "body:", repr(response)
            p.feed(response)

        f.close()
        p.close()

        res = u.close()[0]
        try:
            return res
        except TypeError:
            return (res,)

class CRPCProxy:
    blocking = True
    def __init__(self, uri, transport=None, encoding=None, verbose=0):
        # establish a "logical" server connection
        # get the url
        import urllib
        type, uri = urllib.splittype(uri)
        if type not in ("http",):
            raise IOError, "unsupported XML-RPC protocol"
        self.__host, self.__handler = urllib.splithost(uri)
        if not self.__handler:
            self.__handler = "/RPC2"

        if transport is None:
            if type == "https":
                transport = SafeTransport()
            else:
                transport = Transport()
        self.__transport = transport

        self.__encoding = encoding
        self.__verbose = verbose

    def __request(self, methodname, params):
        # call a method on the remote server

        request = dumps(params, methodname, encoding=self.__encoding)

        response = self.__transport.request(
            self.__host,
            self.__handler,
            request,
            verbose=self.__verbose,
            response = self.blocking
            )

        if self.blocking:
            if len(response) == 1:
                response = response[0]

            return response

    def __repr__(self):
        return (
            "<ServerProxy for %s%s>" %
            (self.__host, self.__handler)
            )

    __str__ = __repr__

    def __getattr__(self, name):
        # magic method dispatcher
        return _Method(self.__request, name)
      
    def setBlocking(self, block):
        self.blocking = block
        
    def callNonBlocking(self, func, *args):
        self.blocking = False
        self.__request('nb__' + func, args)
        self.blocking = True
        



