from setuptools import setup, Extension
import os

# Define the extension module
ext_modules = [
    Extension(
        'my_library.my_extension',  # Name of the extension
        sources=[os.path.join('src', 'my_extension.c')],  # Path to the C/C++ source file
        include_dirs=[],  # Include directories if any
        libraries=[],  # Libraries to link against if any
        library_dirs=[],  # Library directories if any
    )
]

setup(
    ext_modules=ext_modules,
)