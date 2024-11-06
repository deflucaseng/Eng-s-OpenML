
# File: src/my_library/core.py
"""Core functionality for my_library.

This module provides high-level Python interfaces to the C implementations
and additional Python-only functionality.
"""

import numpy as np
from typing import Optional, Union, List, Tuple
from . import _native  # Import the C module

# Constants
DEFAULT_TOLERANCE = 1e-8
MAX_ITERATIONS = 1000

class FastCalculator:
    """High-performance calculator using C backend."""
    
    def __init__(self, tolerance: float = DEFAULT_TOLERANCE):
        self.tolerance = tolerance
        self._cache = {}

    def compute(self, data: np.ndarray) -> np.ndarray:
        """Perform fast computation using C implementation.
        
        Args:
            data: Input numpy array to process
            
        Returns:
            Processed numpy array
            
        Raises:
            ValueError: If data is not in the correct format
        """
        if not isinstance(data, np.ndarray):
            raise ValueError("Input must be a numpy array")
            
        # Prepare data for C function
        prepared_data = self._prepare_data(data)
        
        # Call C implementation
        result = _native.fast_compute(prepared_data, self.tolerance)
        
        # Post-process results if needed
        return self._post_process(result)
    
    def _prepare_data(self, data: np.ndarray) -> np.ndarray:
        """Prepare data for C processing."""
        # Ensure correct data type and memory layout
        return np.ascontiguousarray(data, dtype=np.float64)
    
    def _post_process(self, result: np.ndarray) -> np.ndarray:
        """Apply any Python-side post-processing."""
        return result

def process_array(
    data: np.ndarray,
    mode: str = 'fast',
    **kwargs
) -> np.ndarray:
    """High-level function combining Python and C processing.
    
    Args:
        data: Input data to process
        mode: Processing mode ('fast' or 'accurate')
        **kwargs: Additional arguments
        
    Returns:
        Processed data
    """
    if mode == 'fast':
        # Use C implementation for speed
        return _native.fast_compute(data, **kwargs)
    else:
        # Fall back to Python implementation for accuracy
        return _python_accurate_compute(data, **kwargs)

def optimize_matrix(
    matrix: np.ndarray,
    iterations: int = MAX_ITERATIONS
) -> Tuple[np.ndarray, float]:
    """Optimize a matrix using hybrid Python/C approach.
    
    This function demonstrates how to combine Python flexibility
    with C performance:
    1. Python handles high-level logic and validation
    2. C handles performance-critical computations
    3. Results are post-processed in Python
    """
    # Validation (Python)
    if matrix.size == 0:
        raise ValueError("Empty matrix provided")
    
    # Initial processing (Python)
    prepared_matrix = np.ascontiguousarray(matrix, dtype=np.float64)
    
    # Core computation (C)
    optimized = _native.matrix_multiply(prepared_matrix)
    
    # Calculate error (Python)
    error = np.mean(np.abs(optimized - matrix))
    
    return optimized, error

def _python_accurate_compute(
    data: np.ndarray,
    **kwargs
) -> np.ndarray:
    """Pure Python implementation for accuracy."""
    # Implement accurate but slower Python version here
    pass