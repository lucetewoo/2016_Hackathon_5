import win32api
import win32print
import win32con
import time

cmds = {'Pause':win32print.JOB_CONTROL_PAUSE, 'cancel':win32print.JOB_CONTROL_CANCEL, 
'resume':win32print.JOB_CONTROL_RESUME, 
'prior_low':win32print.MIN_PRIORITY,'prior_high':win32print.MAX_PRIORITY,'prior_normal':win32print.DEF_PRIORITY 
}

def win_print(filename, printer_name = None):
    if not printer_name:
        printer_name = win32print.GetDefaultPrinter()
    handle = win32print.OpenPrinter(printer_name, {'DesiredAccess':win32print.PRINTER_ALL_ACCESS})
    out = '/d:"%s"' % (printer_name)

    printer = win32print.GetPrinter(handle, 2)
    devmode = printer['pDevMode']
    devmode.Color = 2

    printer['pDevMode'] = devmode
    win32print.SetPrinter(handle, 2, printer, 0)
    """
    blank_page = PDFFileReader('test1.pdf').pages[0]
    dest = PDFFileWriter()
    for source in sources:
        PDF = PDFFileReader(source)
        dest.addPage(PDF.pages)
    if PDF.numPages % 2: #odd number of pages in source
        dest.addPage(blank_page)
    """
    win32print.DocumentProperties(None, handle, printer_name, devmode, devmode, win32con.DM_IN_BUFFER | win32con.DM_OUT_BUFFER)
    print "====\n"+str(win32api.ShellExecute(0, "print", filename, out, ".", 0))
    win32print.ClosePrinter(handle)

def print_job_checker():
    jobs = [1]
    while True:
        jobs = []
        for p in win32print.EnumPrinters(win32print.PRINTER_ENUM_LOCAL,
                                         None, 1):
            flags, desc, name, comment = p
            total_count = 0
            count = 0
            phandle = win32print.OpenPrinter(name)
            print_jobs = win32print.EnumJobs(phandle, 0, -1, 1)
            print print_jobs
            if len(print_jobs)>0:
                print print_jobs[0]['JobId']
            if len(print_jobs)>0:
                win32print.SetJob(phandle, print_jobs[0]['JobId'], 0, None, cmds['Pause'])
            if print_jobs:
                jobs.extend(list(print_jobs))
            for job in print_jobs:
                count = int(job['TotalPages'])
                total_count += count
                document = job["pDocument"]
            win32print.ClosePrinter(phandle)
        print total_count
        time.sleep(2)
    print "No more jobs!"
    
def test_print():
    win_print('2016_11_13_10_32_40.docx')



    #win_print('test1.txt', 'PDFCreator')
    

if __name__ == '__main__':
    test_print()
    print_job_checker()

