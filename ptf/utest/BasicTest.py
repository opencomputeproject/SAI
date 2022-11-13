
import unittest
import inspect

from LogConfig import logger
import LogConfig



class BasicMockedTest(unittest.TestCase):
    """Basic test case, use to simualate the sai_base_test class

    """

    def setUp(self):
        unittest.TestCase.setUp(self)
        self.set_logger_name()
        
    
    def set_logger_name(self):
        """
        Set Logger name as filename:classname
        """
        LogConfig.set_logging()
        file_name = inspect.getfile(self.__class__)
        class_name = self.__class__.__name__
        logger.name = "{}:{}".format(file_name, class_name)
