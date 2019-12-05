from tkinter import *

# main window
window = Tk()
window.geometry('320x240+100+100')
window.overrideredirect(1)  # no top window title border

# pin window on top of everything
    # (there's no cross platform way to do this)
    # (need a windows dependant conditional for this method)
# window.wm_attributes("-topmost", 1)

# canvas (render to this, located within main window)
canvas = Canvas(window, width=320, height=240)
canvas.pack()

# callbacks (each callback must take an event argument)
def settime(e):
    print('set time')

def endprogram(e):
    print('ending program')
    window.destroy()

# canvas manipulations
def renderImageWithCallback():
    img = PhotoImage(file="img/setclock.png")
    obj_id = canvas.create_image(49, 47, image=img)
    canvas.tag_bind(obj_id, '<Button-1>', settime)

# why on earth does this not work
# --- 
renderImageWithCallback()
# ---

# and this does work
# ---
# img = PhotoImage(file="img/setclock.png")
# obj_id = canvas.create_image(49, 47, image=img)
# canvas.tag_bind(obj_id, '<Button-1>', settime)
# ---

# open the window
window.mainloop()