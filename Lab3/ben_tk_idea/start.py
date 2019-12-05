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

def printhey(e):
    print(e)

def endprogram(e):
    print('ending program')
    window.destroy()

# canvas manipulations
def renderImageWithCallback(imagePath, x, y, onClick):
    # create an image, render it to the canvas
    img = PhotoImage(file=imagePath)
    obj_id = canvas.create_image(x, y, image=img)
    # bind a callback to that object_id
    canvas.tag_bind(obj_id, '<Button-1>', onClick)

# fails with functions
# ----
# renderImageWithCallback("img/setclock.png", 49, 47, settime)
# renderImageWithCallback("img/exitbutton.png", 330, 18, endprogram)

# works without functions 
# :/ wtf
# ---
# imagePath = "img/setclock.png"
# x = 49
# y = 47
# onClick = settime
# # create an image, render it to the canvas
# img = PhotoImage(file=imagePath)
# obj_id = canvas.create_image(x, y, image=img)
# # bind a callback to that object_id
# canvas.tag_bind(obj_id, '<Button-1>', onClick)

imagePath = "img/exitbutton.png"
x = 60
y = 60
onClick = endprogram
# create an image, render it to the canvas
img = PhotoImage(file=imagePath)
obj_id = canvas.create_image(x, y, image=img)
# bind a callback to that object_id
canvas.tag_bind(obj_id, '<Button-1>', onClick)

# open the window
window.mainloop()

# working snippet start
# -------
# imagePath = "img/setclock.png"
# x = 49
# y = 47
# onClick = settime

# img = PhotoImage(file=imagePath)
# obj_id = canvas.create_image(img.width(), img.height(), image=img)
# canvas.tag_bind(obj_id, '<Button-1>', onClick)
# -------
# working snippet end