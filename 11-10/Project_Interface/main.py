import tkinter as tk
import ipaddress
import myServer


class Gui:
    """Gui class"""

    def __init__(self):
        self.root = tk.Tk()
        self.root.geometry("300x210")
        self.root.title(" START SERVER")
        self.l = tk.Label(text="enter URL")
        self.l.pack()
        self.ENTRY = tk.Entry(self.root, width=25, bg="light yellow")
        self.ENTRY.pack()
        self.Btn = tk.Button(self.root, height=2, width=20, text="Show", command=lambda: self.Take_input())
        self.Btn.pack()
        self.root.mainloop()

    def Take_input(self):
        entry_text = self.ENTRY.get()
        self.ENTRY.delete(0, 'end')
        try:
            ip_object = ipaddress.ip_address(entry_text)
            self.ENTRY.insert(tk.END, 'Valid IP')
            self.root.destroy()
            myServer.ruun(entry_text)
        except ValueError:
            self.ENTRY.insert(tk.END, "Invalid IP")


Gui()
