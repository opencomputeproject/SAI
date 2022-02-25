from setuptools import setup

with open('README.md') as readme_file:
    readme = readme_file.read()

setup(
    name='ptf',
    version='0.9',
    description='PTF is a Python based dataplane test framework.',
    long_description=readme,
    author='Antonin Bas',
    author_email='antonin@barefootnetworks.com',
    url='https://github.com/p4lang/ptf',
    packages=[
        'ptf', 'ptf.platforms',
    ],
    package_dir={'': 'src'},
    scripts=['ptf'],
    zip_safe=False,
    license='Apache License',
    keywords='ptf',
    classifiers=[
        'Development Status :: 2 - Pre-Alpha',
        'Intended Audience :: Developers',
        'License :: OSI Approved :: Apache License',
        'Natural Language :: English',
        "Programming Language :: Python :: 2",
        'Programming Language :: Python :: 2.7',
    ]
)
