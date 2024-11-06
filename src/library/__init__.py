"""
My Library - High performance computations using Python and C.

This library provides optimized functions for [your purpose here].
"""

# Import key functions/classes from core.py to make them available at package level
from .core import (
    process_array,
    FastCalculator,
    optimize_matrix,
    DEFAULT_TOLERANCE
)

# Import the compiled C functions
from ._native import (
    fast_compute,
    matrix_multiply
)

# Define package-level variables
__version__ = "0.1.0"
__author__ = "Your Name"

# Define what gets imported with "from my_library import *"
__all__ = [
    'process_array',
    'FastCalculator',
    'optimize_matrix',
    'fast_compute',
    'matrix_multiply',
    'DEFAULT_TOLERANCE'
]
