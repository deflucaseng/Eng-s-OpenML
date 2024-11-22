import os
from pathlib import Path
import argparse

def generate_tree(directory, prefix="", ignore_patterns=None, max_depth=None, current_depth=0):
    """
    Generate a text representation of the directory structure.
    
    Args:
        directory (str): The root directory to start from
        prefix (str): Current prefix for line formatting
        ignore_patterns (list): List of patterns to ignore
        max_depth (int): Maximum depth to traverse
        current_depth (int): Current recursion depth
    """
    if ignore_patterns is None:
        ignore_patterns = ['.git', '__pycache__', 'node_modules', '.env']
        
    if max_depth is not None and current_depth > max_depth:
        return ""
    
    output = ""
    directory = Path(directory)
    
    # Get and sort directory contents
    contents = list(directory.iterdir())
    contents.sort(key=lambda x: (not x.is_dir(), x.name.lower()))
    
    # Process each item
    for i, item in enumerate(contents):
        # Skip ignored patterns
        if any(pattern in str(item) for pattern in ignore_patterns):
            continue
            
        is_last = i == len(contents) - 1
        connector = "└── " if is_last else "├── "
        
        # Add item to output
        output += f"{prefix}{connector}{item.name}\n"
        
        # Recursively process directories
        if item.is_dir():
            next_prefix = prefix + ("    " if is_last else "│   ")
            output += generate_tree(
                item,
                prefix=next_prefix,
                ignore_patterns=ignore_patterns,
                max_depth=max_depth,
                current_depth=current_depth + 1
            )
    
    return output

def main():
    parser = argparse.ArgumentParser(description='Generate a tree representation of a directory structure')
    parser.add_argument('path', nargs='?', default='.', help='Path to the root directory')
    parser.add_argument('--ignore', '-i', nargs='+', help='Patterns to ignore')
    parser.add_argument('--max-depth', '-d', type=int, help='Maximum depth to traverse')
    parser.add_argument('--output', '-o', help='Output file (optional)')
    
    args = parser.parse_args()
    
    # Generate tree
    tree_output = generate_tree(
        args.path,
        ignore_patterns=args.ignore,
        max_depth=args.max_depth
    )
    
    # Handle output
    if args.output:
        with open(args.output, 'w', encoding='utf-8') as f:
            f.write(tree_output)
    else:
        print(tree_output)

if __name__ == "__main__":
    main()