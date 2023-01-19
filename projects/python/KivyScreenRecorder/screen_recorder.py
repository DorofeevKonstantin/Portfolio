import pyautogui
import cv2
import numpy as np
import threading
import os

from functools import partial
from datetime import datetime
from mss import mss

from kivy.app import App
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.image import Image
from kivy.uix.button import Button
from kivy.uix.checkbox import CheckBox
from kivy.uix.label import Label
from kivy.clock import Clock
from kivy.graphics.texture import Texture

from frame_counter import Frame_Counter

class MainWindow(App):

    def build(self):
        self.title = "Python Kivy 2.0 screen recorder."
        self.font_size = 12
        self.main_image = Image(size_hint = (.9, 1))

        self.fps = 25.0
        self.frame_counter = Frame_Counter(self.fps)
        self.resolution = pyautogui.size()
        print("Resolution = ", self.resolution)
        self.clear_image()
        self.codec = cv2.VideoWriter_fourcc(*"DIVX")
        self.record_started = False
        self.need_recording = False

        self.record_button = Button(text = "Record")
        self.record_button.font_size = self.font_size
        self.record_button.bind(on_press = self.record_button_clicked)
        self.check_box = CheckBox()
        self.check_box.bind(active = self.checkbox_status)
        self.label_resolution = Label()
        self.label_resolution.text = str(self.resolution[0]) + " x " + str(self.resolution[1])
        self.label_info = Label()
        self.label_info.text = "Show ?"

        controls_layout = BoxLayout(orientation = "vertical", size_hint = (.1, 0.3))
        controls_layout.add_widget(self.label_resolution)
        controls_layout.add_widget(self.label_info)
        controls_layout.add_widget(self.check_box)
        controls_layout.add_widget(self.record_button)

        layout = BoxLayout(orientation = "horizontal")
        layout.add_widget(self.main_image)
        layout.add_widget(controls_layout)
        return layout

    def record_button_clicked(self, instance):
        if (self.record_started == False):
            now = datetime.now()
            directory_name = "records"
            if(not os.path.exists(directory_name)):
                os.mkdir(directory_name)
            full_name = os.path.join(directory_name, now.strftime("%d_%m_%Y_%H_%M_%S") + ".avi")
            self.filename = full_name
            self.out = cv2.VideoWriter(self.filename, self.codec, self.fps, self.resolution)
            instance.text = "Stop"

            self.need_recording = True
            self.thread = threading.Thread(target = self.thread_function)
            self.thread.start()
        else:
            instance.text = "Record"
            self.stop_thread()
        self.record_started = not self.record_started

    def thread_function(self):
        print("Thread started.")
        zone = {"top": 0, "left": 0, "width": self.resolution.width, "height": self.resolution.height}
        with mss() as sct:
            while self.need_recording == True:
                grabbed = np.array(sct.grab(zone))
                frame = np.flip(grabbed[:, :, :3], 2)
                frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
                count = self.frame_counter.needed_frames()
                for i in range(int(count)):
                    self.out.write(frame)
                if self.check_box.active == True:
                    buf1 = cv2.flip(frame, 0)
                    buf = buf1.tostring()
                    Clock.schedule_once(partial(self.set_main_texture, buf))
        print("Thread stopped.")

    def stop_thread(self):
        self.need_recording = False
        print("Try to stop recording thread.")
        self.thread.join()
        self.out.release()

    def set_main_texture(self, buf, dt):
        newTexture = Texture.create(size = (self.resolution[0], self.resolution[1]), colorfmt = 'bgr')
        newTexture.blit_buffer(buf, colorfmt = 'bgr', bufferfmt = 'ubyte')
        self.main_image.texture = newTexture

    def checkbox_status(self, checkbox_instance, is_active):
        if is_active:
            print("Checkbox checked.")
        else:
            self.clear_image()
            print("Checkbox unchecked.")

    def clear_image(self):
        texture = Texture.create(size = (self.resolution[1], self.resolution[0]), colorfmt = 'bgr')
        self.main_image.texture = texture

    def on_stop(self):
        if (self.record_started == True):
            self.stop_thread()
        print("MainWindow released.")