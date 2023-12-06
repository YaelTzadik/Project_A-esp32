import tkinter as tk
import ipaddress
import myServer


class Gui:
    """Gui class"""

    def __init__(self):
        self.root = tk.Tk()
        self.root.geometry("300x140")
        self.root.title(" START SERVER")
        self.l = tk.Label(text="enter URL")
        self.l.pack()
        self.ENTRY = tk.Entry(self.root, width=25, bg="light yellow")
        self.ENTRY.pack()
        self.Btn = tk.Button(self.root, height=2, width=20, text="Start", command=lambda: self.Take_input())
        self.Btn.pack()
        self.Btn2 = tk.Button(self.root, height=2, width=20, text="Help", command=lambda: self.Help_input())
        self.Btn2.pack()
        self.root.mainloop()

    def Take_input(self):
        entry_text = self.ENTRY.get()
        try:
            ipaddress.ip_address(entry_text)
            self.ENTRY.insert(tk.END, 'Valid IP')
            self.root.destroy()
            myServer.ruun(entry_text)
        except ValueError:
            self.ENTRY.delete(0, 'end')
            self.ENTRY.insert(tk.END, "Invalid IP")

    def Help_input(self):
        self.ENTRY.delete(0, 'end')
        self.ENTRY.insert(tk.END, "192.168.137.")


Gui()
