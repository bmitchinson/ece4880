import tkinter as tk


class Preset(object):
    def __init__(self, hour, minute, am_pm, temperature=0, active=False, **kwargs):
        self._active = active
        self._hour = hour
    
    def set_hour(self, hour):
        self._hour = hour

    def set_minute(self, minute):
        self._minute = minute

    def set_active(self):
        self._active = True
    
    def is_active(self):
        return self._active

class Page(tk.Frame):
    def __init__(self, *args, **kwargs):
        tk.Frame.__init__(self, *args, **kwargs)
    def show(self):
        self.lift()

class MainPage(Page):
   def __init__(self, mainview, *args, **kwargs):
       Page.__init__(self, *args, **kwargs)
       label = tk.Label(self, text="This is the MainPage")
       label.pack(side="top", fill="both", expand=True)

class SetClockPage(Page):
   def __init__(self, mainview, *args, **kwargs):
       Page.__init__(self, *args, **kwargs)
       label = tk.Label(self, text="This is the SetClockPage")
       label.pack(side="top", fill="both", expand=True)

class WeekdayWeekendPage(Page):
   def __init__(self, mainview, *args, **kwargs):
       Page.__init__(self, *args, **kwargs)
       label = tk.Label(self, text="This is  WeekdayWeekendPage")
       label.pack(side="top", fill="both", expand=True)

class PresetControlPage(Page):
   def __init__(self, mainview, *args, **kwargs):
       Page.__init__(self, *args, **kwargs)
       label = tk.Label(self, text="This is PresetControlPage")
       label.pack(side="top", fill="both", expand=True)

class MainView(tk.Frame):
    def __init__(self, *args, **kwargs):
        tk.Frame.__init__(self, *args, **kwargs)
        self._main_page = MainPage(self)
        self._set_clock_page = SetClockPage(self)
        self._weekend_weekday_page = WeekdayWeekendPage(self)
        self._preset_control_page = PresetControlPage(self)

        self._buttonframe = tk.Frame(self)
        self._container = tk.Frame(self)
        self._buttonframe.pack(side="top", fill="x", expand=False)
        self._container.pack(side="top", fill="both", expand=True)

        self._main_page.place(in_=self._container, x=0, y=0, relwidth=1, relheight=1)
        self._set_clock_page.place(in_=self._container, x=0, y=0, relwidth=1, relheight=1)
        self._weekend_weekday_page.place(in_=self._container, x=0, y=0, relwidth=1, relheight=1)
        self._preset_control_page.place(in_=self._container, x=0, y=0, relwidth=1, relheight=1)

        self._to_main_page_button = tk.Button(self._buttonframe, text="Main Page", command=self._main_page.lift)
        self._to_set_clock_page_button = tk.Button(self._buttonframe, text="Set Clock", command=self._set_clock_page.lift)
        self._to_weekend_weekday_page_button = tk.Button(self._buttonframe, text="Presets", command=self._weekend_weekday_page.lift)
        self._to_preset_control_page_button = tk.Button(self._buttonframe, text="Preset Control", command=self._preset_control_page.lift)

        self._to_main_page_button.pack(side="left")
        self._to_set_clock_page_button.pack(side="left")
        self._to_weekend_weekday_page_button.pack(side="left")
        self._to_preset_control_page_button.pack(side="left")

        self._main_page.show()

if __name__ == "__main__":
    root = tk.Tk()
    main = MainView(root)
    main.pack(side="top", fill="both", expand=True)
    root.wm_geometry('320x240+100+100')
    root.mainloop()