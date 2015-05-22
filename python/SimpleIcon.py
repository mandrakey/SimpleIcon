# Copyright (C) Maurice Bleuel <mandrakey@lavabit.com>
# 
# This file is part of SimpleIcon, Python version.
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

import sys

class SimpleIcon:
    
    DELIM = ";;"
    
    def __init__(self, file = None):
        self.name = "EMPTY"
        self.fileVersion = 1
        self.width = 8
        self.height = 8
        self.data = []
        
        if file != None:
            self.loadFromFile(file)
    
    def loadFromFile(self, file):
        try:
            fp = open(file, "r")
        except IOError as e:
            print "Failed to load file: {0}".format(e)
            return
        
        fileContent = ""
        for line in fp:
            fileContent += line
        
        self.parseHeader(fileContent)
    
    def parseHeader(self, fileContent):
        if fileContent == None or fileContent.strip() == "":
            raise Exception("Cannot parse empty file content")
        
        tmp = fileContent.split(SimpleIcon.DELIM)
        if len(tmp) != 4:
            raise Exception("File content has illegal format (expect 4 fields)")
        
        self.name = tmp[0]
        self.fileVersion = int(tmp[1])
        fileData = tmp[3]
        
        tmp = tmp[2].split("x")
        self.width = int(tmp[0])
        self.height = int(tmp[1])
        
        # parse data
        if self.fileVersion == 1:
            self.parseData(fileData)
        elif self.fileVersion == 2:
            self.parseDataV2(fileData)
            pass
    
    def parseData(self, fileData):
        offset = 0
        data = []
        col = 0
        row = 0
        
        for col in range(self.height):
            data.append([])
            for row in range(self.width):
                data[col].append(fileData[offset])
                offset += 1
        
        self.data = data
    
    def parseDataV2(self, fileData):
        charPos = 0
        offset = 0
        data = []
        col = 0
        row = 0
        
        while offset <= self.width:
            for col in range(self.height):
                data.append([])
                for row in range(offset, offset + 8):
                    if row < self.width:
                        data[col].append(fileData[charPos])
                        charPos += 1
            
            offset += 8
        
        self.data = data
        
    def display(self):
        print("{0} ({1}x{2})\nVersion: {3}".format(self.name, self.width, self.height, self.fileVersion))
        
        col = 0
        row = 0
        for col in range(self.height):
            for row in range(self.width):
                if self.data[col][row] == "1":
                    sys.stdout.write("x")
                else:
                    sys.stdout.write(" ")
            
            print("")
        