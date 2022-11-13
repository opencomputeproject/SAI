import logging
import os
import sys
from logging.handlers import RotatingFileHandler

ROOT_DIR = os.path.dirname(os.path.abspath(__file__))
LOG_FILE_DIR = os.path.join(ROOT_DIR, 'logs')
LOG_FILE_PATH = os.path.join(LOG_FILE_DIR, 'output.log')

global logger
logger = logging.getLogger()


class InfoFilter(logging.Filter):
    def filter(self, rec):
        return rec.levelno in (logging.DEBUG, logging.INFO, logging.WARN)

def set_logging():
    # Create a custom logger

    logger.setLevel(level=logging.DEBUG)

    # Create handlers
    console_debug_handler = logging.StreamHandler(sys.stdout)
    console_error_handler = logging.StreamHandler()
    if not os.path.exists(LOG_FILE_DIR):
        os.mkdir(LOG_FILE_DIR)

    file_handler = RotatingFileHandler(LOG_FILE_PATH, maxBytes=20000000, backupCount=5)

    if os.path.exists(LOG_FILE_PATH):
        '''rollover on each run'''
        file_handler.doRollover()


    console_debug_handler.setLevel(logging.INFO)
    console_debug_handler.addFilter(InfoFilter())
    file_handler.setLevel(logging.INFO)
    console_error_handler.setLevel(logging.ERROR)

    # Create formatters and add it to handlers
    c_debug_format = logging.Formatter(fmt='%(name)s - %(filename)s - %(levelname)s - %(message)s', datefmt='%m/%d/%Y %I:%M:%S %p')
    c_error_format = logging.Formatter(fmt='%(filename)s - %(levelname)s - %(message)s', datefmt='%m/%d/%Y %I:%M:%S %p')
    f_format = logging.Formatter(fmt='%(asctime)s - %(name)s - %(filename)s - %(levelname)s - %(message)s',
                                datefmt='%m/%d/%Y %I:%M:%S %p')

    console_error_handler.setFormatter(c_error_format)
    console_debug_handler.setFormatter(c_debug_format)
    file_handler.setFormatter(f_format)

    # Add handlers to the logger
    logger.addHandler(console_debug_handler)
    logger.addHandler(file_handler)
    logger.addHandler(console_error_handler)


def rollover_log():
    file_handler.doRollover()
