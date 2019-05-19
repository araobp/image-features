import matplotlib
matplotlib.use('TkAgg')

import numpy as np
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
import sys
import tkinter as Tk
from datetime import datetime
import time
import os

import matplotlib.pyplot as plt

import interface
import image

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("port", help="serial port identifier")
parser.add_argument("-s", "--size", help="image sizes", default="s")
parser.add_argument("-e", "--effect", help="effect", default=None)
args = parser.parse_args()

if __name__ == '__main__':

    itfc = interface.Interface(port = args.port, size=args.size)
    gui = image.GUI(interface=itfc)

    PADX = 6
    PADX_GRID = 2
    PADY_GRID = 2

    root = Tk.Tk()
    root.wm_title("Image viewer")

    fig, ax = plt.subplots(1, 1, figsize=(4, 4))
    
    fig.subplots_adjust(bottom=0.15)

    frame = Tk.Frame(master=root)
    frame_row0 = Tk.Frame(master=frame)
    frame_row1 = Tk.Frame(master=frame)
    frame_row2 = Tk.Frame(master=frame)
    frame_row3 = Tk.Frame(master=frame)

    cnt = 0
    class_label_ = ''
    filename = None
    data = None

    repeat_action = False

    canvas = FigureCanvasTkAgg(fig, master=frame_row0)
    canvas.draw()
    
    def pixels():
        global data, ax
        ax.clear()
        if args.size == 'f':
            data = gui.plot(ax, interface.FULL_FRAME)
        elif args.size == 's' and args.effect == None:
            data = gui.plot(ax, interface.SQUARE_FRAME)
        elif args.size == 's' and args.effect == 'd':
            data = gui.plot(ax, interface.DIFF)

        '''
        if args.effect is None:
            data = gui.plot(ax, interface.FULL_FRAME)
        elif args.effect == 'd':
            data = gui.plot(ax, interface.DIFF)
        elif args.effect == 'g':
            data = gui.plot(ax, interface.GRAY, cmap='gray')
        elif args.effect == 'e':
            data = gui.plot(ax, interface.EDGE, cmap='gray')
        '''
        fig.tight_layout()
        canvas.draw()

    def pixels_continuous():
        pixels()
        repeat(pixels_continuous)

    def sccb_write():
        reg_addr = entry_sccb_reg_addr.get()
        reg_value = entry_sccb_reg_value.get()
        reg_value = int(reg_value, 2)
        print(reg_value)
        itfc.sccb_write(reg_addr, reg_value)

    def sccb_read():
        reg_addr = entry_sccb_reg_addr.get()
        reg_value = itfc.sccb_read(reg_addr)
        entry_sccb_reg_value.delete(0, Tk.END)
        entry_sccb_reg_value.insert(0, "0b{:08b}".format(reg_value))

    # Repeat an operation
    def repeat(func):
        if repeat_action:
            root.after(10, func)

    def repeat_toggle():
        global repeat_action
        if repeat_action == True:
            repeat_action = False
            button_continuous.configure(bg='lightblue')
        else:
            repeat_action = True
            button_continuous.configure(bg='red')
            pixels_continuous()

    def screenshot():
        global fig
        fig.savefig('screen_shot.png')

    def save():
        global class_label_, cnt, filename, data
        class_label = entry.get()
        dt = datetime.today().strftime('%Y%m%d%H%M%S')
        if class_label == '':
            filename = './data/{}'.format(dt)
        else:
            filename = './data/{}-{}'.format(entry.get(), dt)
            with open(filename+'.csv', "w") as f:
                flattend_data = data.flatten()
                f.write(','.join(flattend_data.astype(str)))
            #np.save(filename, data)
            if class_label_ != class_label:
                class_label_ = class_label
                cnt = 0
            cnt += 1
            counter.configure(text='({})'.format(str(cnt)))

    def remove():
        global filename, cnt
        if filename:
            os.remove(filename+'.csv')
            cnt -= 1
            counter.configure(text='({})'.format(str(cnt)))

    def on_key_event(event):
        c = event.key
        print(c)
        if c == 'up':
            pixels()
        elif c == 'down':
            save()

    def confirm():
        canvas._tkcanvas.focus_set()
        
    def _quit():
        itfc.close()
        root.quit()
        root.destroy() 
   
    canvas.mpl_connect('key_press_event', on_key_event)

    button_shutter = Tk.Button(master=frame_row1, text='Shutter', command=pixels, bg='turquoise', activebackground='grey', padx=PADX)
    button_continuous = Tk.Button(master=frame_row1, text='Continous', command=repeat_toggle, bg='lightblue', activebackground='grey', padx=PADX)
    button_confirm = Tk.Button(master=frame_row1, text='Confirm', command=confirm, bg='lightblue', activebackground='grey', padx=PADX)
    button_screenshot = Tk.Button(master=frame_row1, text='Screenshot', command=screenshot, bg='lightblue', activebackground='grey', padx=PADX)
    button_quit = Tk.Button(master=frame_row1, text='Quit', command=_quit, bg='yellow', activebackground='grey', padx=PADX)

    label_sccb = Tk.Label(master=frame_row2, text='SCCB')
    entry_sccb_reg_addr = Tk.Entry(master=frame_row2, width=5)
    entry_sccb_reg_value = Tk.Entry(master=frame_row2, width=11)
    button_sccb_read = Tk.Button(master=frame_row2, text='Read', command=sccb_read, bg='lightblue', activebackground='grey', padx=PADX)
    button_sccb_write = Tk.Button(master=frame_row2, text='Write', command=sccb_write, bg='lightblue', activebackground='grey', padx=PADX)

    label_class = Tk.Label(master=frame_row3, text='Class label:')
    entry = Tk.Entry(master=frame_row3, width=14)
    counter = Tk.Label(master=frame_row3)
    button_save = Tk.Button(master=frame_row3, text='Save', command=save, bg='lightblue', activebackground='grey', padx=PADX)
    button_remove = Tk.Button(master=frame_row3, text='Remove', command=remove, bg='lightblue', activebackground='grey', padx=PADX)

    ##### Place the parts on Tk #####

    frame.pack(expand=True, fill=Tk.BOTH)

    ### Row 0: main canvas
    canvas._tkcanvas.pack(expand=True, fill=Tk.BOTH)
    frame_row0.pack(expand=True, fill=Tk.BOTH)

    ### Row 1: operation ####
    button_shutter.grid(row=0, column=0, padx=PADX_GRID)
    button_continuous.grid(row=0, column=1, padx=PADX_GRID)
    button_confirm.grid(row=0, column=2, padx=PADX_GRID)
    button_screenshot.grid(row=0, column=3, padx=PADX_GRID)
    button_quit.grid(row=0, column=4, padx=PADX_GRID)
    frame_row1.pack(pady=PADY_GRID)

    ### Row 2: calibration ####
    label_sccb.grid(row=0, column=0, padx=PADX_GRID)
    entry_sccb_reg_addr.grid(row=0, column=1, padx=PADX_GRID)
    entry_sccb_reg_value.grid(row=0, column=2, padx=PADX_GRID)
    button_sccb_read.grid(row=0, column=3, padx=PADX_GRID)
    button_sccb_write.grid(row=0, column=4, padx=PADX_GRID)
    frame_row2.pack(pady=PADY_GRID)

    ### Row 3: save
    label_class.grid(row=0, column=0, padx=PADX_GRID)
    entry.grid(row=0, column=1, padx=PADX_GRID)
    counter.grid(row=0, column=2, padx=PADX_GRID)
    counter.configure(text='({})'.format(str(cnt)))
    button_save.grid(row=0, column=3, padx=PADX_GRID)
    button_remove.grid(row=0, column=4, padx=PADX_GRID)
    frame_row3.pack(pady=PADY_GRID)

    ##### loop forever #####
    Tk.mainloop()
