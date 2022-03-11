from distutils.core import setup

long_description = """
saithrift is python client library to connect to sai thrift server.
"""

setup (name='saithrift',
        version='0.9',
        package_dir={'sai_thrift': 'gen-py/sai'},
        packages=['sai_thrift'],
        description='saithrift is python client library to connect to sai thrift server.',
        long_description=long_description,
        author='Guohan Lu',
        author_email='lguohan@gmail.com',
        url='https://github.com/opencomputeproject/SAI',
        license='Apache license',
        platforms='UNIX',
        classifiers = [
        'Development Status :: 5 - Production/Stable',
        'Environment :: Console',
        'Intended Audience :: Developers',
        'Intended Audience :: System Administrators',
        'License :: OSI Approved :: Apache License',
        'Operating System :: POSIX',
        'Operating System :: MacOS :: MacOS X',
        'Programming Language :: Python',
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: 3.7',
        'Topic :: Software Development',
        'Topic :: Software Development :: Libraries :: Python Modules',
        'Topic :: Software Development :: Quality Assurance',
        'Topic :: Software Development :: Testing',
        'Topic :: System',
        'Topic :: System :: Archiving :: Packaging',
        'Topic :: System :: Installation/Setup',
        'Topic :: System :: Shells',
        'Topic :: System :: Software Distribution',
        'Topic :: Terminals',
        ],
        )
