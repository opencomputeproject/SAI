
import unittest
import inspect

import LogConfig

"""
Base class of the unit test
"""

class BasicMockedTest(unittest.TestCase):
    """Basic test case, use to simualate the sai_base_test class

    """

    def setUp(self):
        unittest.TestCase.setUp(self)
    
    def set_logger_name(self):
        """
        Set Logger name as filename:classname
        """
        
        file_name = inspect.getfile(self.__class__)
        class_name = self.__class__.__name__
        logger_name = "{}:{}".format(file_name, class_name)
        LogConfig.set_logging(loggerName = logger_name)
