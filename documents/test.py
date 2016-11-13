import win32api
import win32print
import win32con
import pyPdf

def win_print(filename, printer_name = None):
    if not printer_name:
        printer_name = win32print.GetDefaultPrinter()
       # handle = win32print.OpenPrinter(printer_name)
    out = '/d:"%s"' % (printer_name)

    # Get the default properties for the printer
    #properties = win32print.GetPrinter(handle, 2)
    #devmode = properties['pDevMode']
    #print papersize.parse_papersize("A4","cm")
    #devmode.Color = win32con.DMCOLOR_MONOCHROME

    """
    blank_page = PDFFileReader('test1.pdf').pages[0]
    dest = PDFFileWriter()
    for source in sources:
        PDF = PDFFileReader(source)
        dest.addPage(PDF.pages)
    if PDF.numPages % 2: #odd number of pages in source
        dest.addPage(blank_page)
        """
    #win32print.DocumentProperties(None, handle, printer_name, devmode, devmode, win32con.DM_IN_BUFFER | win32con.DM_OUT_BUFFER)
    win32api.ShellExecute(0, "print", filename, out, ".", 0)


def test_print():
    win_print('test3.hwp')
    #win_print('test1.txt', 'PDFCreator')
    

if __name__ == '__main__':
    test_print()

 gh h 
