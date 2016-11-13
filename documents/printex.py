#!/usr/bin/env python
# -*- coding: utf8 -*-
"""win32print 모듈 테스트 예제: 연결된 프린터 목록 얻기"""

import win32ui
import win32print
import win32con

Printers=win32print.EnumPrinters(win32print.PRINTER_ENUM_LOCAL, None, 2)
for Printer in Printers:
    print Printer['pPrinterName']



f = open("c:/test/test1.txt", 'r')
line = f.readline()
while True:
    line = f.readline()
    if not line:
        break
    print line,
f.close()

