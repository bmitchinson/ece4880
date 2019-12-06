import tkinter as tk
from tkinter import *
from Preset import Preset

# small preset test
#
# p = Preset(1,12,True,True,80)
# print(p)
# p.set_pm()
# print(p)
# p.set_hour(3)
# print(p)

# credit: Page class and current MainView logic, however logic will change as we are using canvas to draw
# https://stackoverflow.com/questions/14817210/using-buttons-in-tkinter-to-navigate-to-different-pages-of-the-application
class Page(tk.Frame):
    def __init__(self, mainview, *args, **kwargs):
        tk.Frame.__init__(self, *args, **kwargs)
        self.main_view = mainview

    def show(self):
        self.lift()


class MainPage(Page):
    def __init__(self, mainview, *args, **kwargs):
        Page.__init__(self, mainview, *args, **kwargs)
        label = tk.Label(self, text="This is the MainPage")
        label.pack(side="top", fill="both", expand=True)


class SetClockPage(Page):
    def __init__(self, mainview, *args, **kwargs):
        Page.__init__(self, mainview, *args, **kwargs)
        label = tk.Label(self, text="This is the SetClockPage")
        label.pack(side="top", fill="both", expand=True)


class WeekdayWeekendPage(Page):
    def __init__(self, mainview, *args, **kwargs):
        Page.__init__(self, mainview, *args, **kwargs)
        label = tk.Label(self, text="This is  WeekdayWeekendPage")
        label.pack(side="top", fill="both", expand=True)


class PresetControlPage(Page):
    def __init__(self, mainview, *args, **kwargs):
        Page.__init__(self, mainview, *args, **kwargs)
        label = tk.Label(self, text="This is PresetControlPage")
        label.pack(side="top", fill="both", expand=True)


class MainView(tk.Frame):
    def __init__(self, root, *args, **kwargs):
        tk.Frame.__init__(self, *args, **kwargs)

        # attributes used to render canvas attributes
        self._root = root
        self._canvas = Canvas(self._root, width=320, height=240)
        self._canvas.pack()
        self._imgs = []
        self._obj_ids = []

        # pages
        self._main_page = MainPage(self)
        self._set_clock_page = SetClockPage(self)
        self._weekend_weekday_page = WeekdayWeekendPage(self)
        self._preset_control_page = PresetControlPage(self)

        self._buttonframe = tk.Frame(self)
        self._container = tk.Frame(self)
        self._buttonframe.pack(side="top", fill="x", expand=False)
        self._container.pack(side="top", fill="both", expand=True)

        self._main_page.place(in_=self._container,
                              x=0,
                              y=0,
                              relwidth=1,
                              relheight=1)
        self._set_clock_page.place(in_=self._container,
                                   x=0,
                                   y=0,
                                   relwidth=1,
                                   relheight=1)
        self._weekend_weekday_page.place(in_=self._container,
                                         x=0,
                                         y=0,
                                         relwidth=1,
                                         relheight=1)
        self._preset_control_page.place(in_=self._container,
                                        x=0,
                                        y=0,
                                        relwidth=1,
                                        relheight=1)

        # buttons to navigate between pages
        # these will eventuall be callbacks linked to images, but for now they are here as an example
        self._to_main_page_button = tk.Button(self._buttonframe,
                                              text="Main Page",
                                              command=self._main_page.lift)
        self._to_set_clock_page_button = tk.Button(
            self._buttonframe,
            text="Set Clock",
            command=self._set_clock_page.lift)
        self._to_weekend_weekday_page_button = tk.Button(
            self._buttonframe,
            text="Presets",
            command=self._weekend_weekday_page.lift)
        self._to_preset_control_page_button = tk.Button(
            self._buttonframe,
            text="Preset Control",
            command=self._preset_control_page.lift)

        self._to_main_page_button.pack(side="left")
        self._to_set_clock_page_button.pack(side="left")
        self._to_weekend_weekday_page_button.pack(side="left")
        self._to_preset_control_page_button.pack(side="left")

        self._main_page.show()

        # working image render with callback
        self.renderImageWithCallback("img/setclock.png", 49, 47, self.settime)

    # render image w/ callback
    def renderImageWithCallback(self, fname, xloc, yloc, callback):
        self._imgs.append(PhotoImage(file=fname))
        self._obj_ids.append(
            self._canvas.create_image(xloc, yloc, image=self._imgs[-1]))
        self._canvas.tag_bind(self._obj_ids[-1], '<Button-1>', callback)

    def settime(self, e1):
        print(f"set time {e1}")


if __name__ == "__main__":
    root = tk.Tk()
    main = MainView(root)
    main.pack(side="top", fill="both", expand=True)
    root.wm_geometry('320x240+100+100')
    root.mainloop()