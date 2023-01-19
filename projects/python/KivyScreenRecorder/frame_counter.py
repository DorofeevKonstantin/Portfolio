import timeit
import math

class Frame_Counter:
    def __init__(self, fps):
        self.start_time = timeit.default_timer()
        self.writed_frames = 0
        self.fps = fps

    def needed_frames(self):
        current_time = timeit.default_timer()
        diff_time = (current_time - self.start_time) * 1000
        expected_frames = int(math.ceil((diff_time) / (1000 / self.fps)))
        need_frames = expected_frames - self.writed_frames
        self.writed_frames = self.writed_frames + need_frames
        return need_frames