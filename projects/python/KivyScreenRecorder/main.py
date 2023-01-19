from screen_recorder import MainWindow
import logging

if __name__ == '__main__':
    try:
        MainWindow().run()
    except Exception as ex:
       logging.error('EXCEPTION', exc_info=ex)