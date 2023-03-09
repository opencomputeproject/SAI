import logging
import argparse

"""
Convert the generated code in sai_header.py.
For the generated code from c++ enum, convert it to python enum class.

the code like
    ```C++
    /**
     * @brief SAI common API type
     */
    typedef enum _sai_common_api_t
    {
        SAI_COMMON_API_CREATE      = 0,
        SAI_COMMON_API_REMOVE      = 1,
        ...
    } sai_common_api_t;
    ```
    original converted by ctypesgen
    ```
    enum__sai_common_api_t = c_int# /usr/include/sai/saitypes.h: 183
    
    SAI_COMMON_API_CREATE = 0# /usr/include/sai/saitypes.h: 183
    
    SAI_COMMON_API_REMOVE = 1# /usr/include/sai/saitypes.h: 183
    ...
    ```
    new code
    ```
    class sai_common_api(SAIEnum):
    
        SAI_COMMON_API_CREATE = 0# /usr/include/sai/saitypes.h: 183
    
        SAI_COMMON_API_REMOVE = 1# /usr/include/sai/saitypes.h: 183
    ...
    
    SAI_COMMON_API_CREATE = sai_common_api.SAI_COMMON_API_CREATE
    SAI_COMMON_API_REMOVE = sai_common_api.SAI_COMMON_API_REMOVE
    ...
    ```
    
"""


ENUM_DEF = """
import enum
class SAIEnum(enum.IntEnum):
    def __str__(self):      
        return super().__str__().split(\".\")[1]\n"""
ENUM_PREFIX = "enum__sai_"
SAI_NAME  = "SAI"
ENUM_TYPE = "= c_int"
ENUM_END = " = enum__sai_"
ENUM_CLASS_TEMPLATE = "class {}(SAIEnum):\n"

def parse_param():
    '''
    Parse param.
    '''
    parser = argparse.ArgumentParser(
        description="""
        Convert python file after ctypesgen convert.
        """
    )

    parser.add_argument(
        "-i", type=str, dest="input_file",
        help="input file name", required=True)
    parser.add_argument(
        "-o", type=str, dest="output_file",
        help="output file name", required=True)
    return parser.parse_args()

def convert_enum_start_to_class(line):
    """
    Convert the enum to class.
    args:
        line: input
    """
    class_name = line.strip("enum__").split("_t = c_int", 1)[0]
    return class_name

def convert_file(input_file_name, output_file_name):
    """
    Convert the input file to a output file.

    """
    input_file = open(input_file_name, 'r')
    output_file = open(output_file_name, 'w')
    enum_times = 0
    enum_items = []
    enum_start = False
    class_name = None
    
    for line in input_file.readlines():
        if not enum_start and enum_times == 0 and ENUM_PREFIX in line and ENUM_TYPE in line:
            # first time hit a enum
            # add import
            output_file.writelines(line)
            output_file.write(ENUM_DEF)
            enum_times= enum_times + 1
            enum_start = True
            enum_items = []
            output_file.write("\n")
            class_name = convert_enum_start_to_class(line)
            output_file.writelines(ENUM_CLASS_TEMPLATE.format(class_name))
            output_file.flush()
            continue
        elif not enum_start and enum_times > 0 and ENUM_PREFIX in line and ENUM_TYPE in line:
            output_file.writelines(line)
            enum_times= enum_times + 1
            enum_start = True
            enum_items = []
            output_file.write("\n")
            class_name = convert_enum_start_to_class(line)
            output_file.writelines(ENUM_CLASS_TEMPLATE.format(class_name))
            output_file.flush()
            continue
        elif enum_start and ENUM_END not in line and SAI_NAME in line:
            line = line.split("\n", 1)[0]
            enum_name = line.split(" =", 1)[0]
            enum_items.append(enum_name)
            output_file.write("    {}\n".format(line))
            output_file.flush()
            continue
        elif ENUM_END in line:
            enum_start = False
            output_file.write("\n")
            for item in enum_items:
                output_file.write("{} = {}.{}".format(item, class_name, item))
                output_file.write("\n")
            output_file.write("\n")
            output_file.writelines(line)
            output_file.flush()
            continue
        else:
            output_file.writelines(line)
        

    output_file.flush()
    input_file.close()
    output_file.close()


if __name__ == "__main__":
    '''
    For the generated code from c++ enum, convert it to python enum class.
    '''
    args = parse_param()
    convert_file(args.input_file, args.output_file)
