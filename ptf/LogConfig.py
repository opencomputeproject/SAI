import logging
import os
import sys
from logging.handlers import RotatingFileHandler

ROOT_DIR = os.path.dirname(os.path.abspath(__file__))
LOG_FILE_DIR = os.path.join(ROOT_DIR, 'logs')
LOG_FILE_PATH = os.path.join(LOG_FILE_DIR, 'output.log')


def set_logging(loggerName='main'):
    """Set the logger.
    """

    # Create formatters and add it to handlers
    c_debug_format = '%(name)s - %(filename)s - %(levelname)s - %(message)s'
    c_error_format = '%(filename)s - %(levelname)s - %(message)s'
    f_format = '%(asctime)s - %(name)s - %(filename)s - %(levelname)s - %(message)s'
    datafmt = '%m/%d/%Y %I:%M:%S %p'

    if not os.path.exists(LOG_FILE_DIR):
        os.mkdir(LOG_FILE_DIR)

    # Create a custom logger
    logger = logging.getLogger()
    logger.name = loggerName
    logger.setLevel(logging.INFO)

    file_formatter = logging.Formatter(f_format, datafmt)

    # Create handlers
    # Add a new file handler
    file_handler = logging.FileHandler(LOG_FILE_PATH)
    file_handler.setLevel(logging.DEBUG)
    file_handler.setFormatter(file_formatter)    
    logger.addHandler(file_handler)


    # Add a new stream handler
    console_formatter = logging.Formatter(
        c_error_format, datafmt)
    stream_handler = logging.StreamHandler()
    stream_handler.setLevel(logging.ERROR)
    stream_handler.setFormatter(console_formatter)
    logger.addHandler(stream_handler)
