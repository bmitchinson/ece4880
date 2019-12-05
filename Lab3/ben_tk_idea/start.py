from tkinter import *


def printhey(nope):
    print(nope)

# TODO: window on top


# main window
window = Tk()
window.geometry('320x240+100+100')
window.overrideredirect(1)  # no top window title border

# canvas (render to this, located within main window)
canvas = Canvas(window, width=320, height=240)
canvas.pack()

# create an image, render it to the canvas
setclock_img = PhotoImage(file="img/setclock.png")
setclock_id = canvas.create_image(setclock_img.width(),
                                  setclock_img.height(), image=setclock_img)

# bind a callback to that object_id
canvas.tag_bind(setclock_id, '<Button-1>', printhey)

# open the window
window.mainloop()
