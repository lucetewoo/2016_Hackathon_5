#!/usr/bin/env python
#-*- coding: utf-8 -*-

try:
    # For Python 3.0 and later
    from urllib.request import urlopen
except ImportError:
    # Fall back to Python 2's urllib2
    from urllib2 import urlopen

import json
import time
import wget
import win32print
import win32api
import win32con
import time


def get_jsonparsed_data(url):
    response = urlopen(url)
    data = response.read().decode("utf-8")
    return json.loads(data)

def fileCheckLoop():
	while True:
		url = "http://192.168.43.247/ajax/getUploadedFileList.php?printer=1"
		file_list = get_jsonparsed_data(url)
		if len(file_list)>0 and file_list[0]['status']=='waiting':
			downloadFile(file_list)
			
		print "."
		time.sleep(2)

def downloadFile(d):
	for file in d:
		result = wget.download("http://192.168.43.247/"+file['file_path'], '.\\test3\\')
		print result + " is downloaded."
		url = "http://192.168.43.247/ajax/setUploadedFileStatus.php?file="+file['no']
		response = urlopen(url)
		printing(file['file_path'].split('/')[-1])

def printing(filename, printer_name = None):
    if not printer_name:
        printer_name = win32print.GetDefaultPrinter()
    out = '/d:"%s"' % (printer_name)
    print filename, out
    win32api.ShellExecute(0, "print", ".\\test3\\"+filename, out, ".", 0)
    
def print_job_checker():
    """
    Prints out all jobs in the print queue every 5 seconds
    """
    jobs = [1]
    while jobs:
        jobs = []
        for p in win32print.EnumPrinters(win32print.PRINTER_ENUM_LOCAL, None, 1):
            flags, desc, name, comment = p
 
            phandle = win32print.OpenPrinter(name)
            print_jobs = win32print.EnumJobs(phandle, 0, -1, 1)
            if print_jobs:
                jobs.extend(list(print_jobs))
            for job in print_jobs:
                print "printer name => " + name
                print win32print.EnumForms(phandle)
                document = job["pDocument"]
                print "Document name => " + document
            win32print.ClosePrinter(phandle)
 
        time.sleep(5)
    print "No more jobs!"
    

fileCheckLoop()
print_job_checker()
    
