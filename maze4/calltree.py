#!/usr/bin/env python3

import subprocess
import re
import sys
import os

def extract_functions_from_source(file_path):
    """
    Extract function names defined in the source file.
    This is a simplistic approach and may not catch all C++ functions.
    """
    defined_functions = set()
    
    try:
        with open(file_path, 'r') as f:
            content = f.read()
            
        # Look for function definitions (simplistic pattern)
        # This pattern looks for function declarations followed by opening braces
        pattern = r'(?:^|\n)(?:\w+\s+)+(\w+)\s*\([^)]*\)\s*(?:const\s*)?\s*\{'
        matches = re.finditer(pattern, content)
        
        for match in matches:
            function_name = match.group(1)
            defined_functions.add(function_name)
            
        # Also look for class methods
        class_pattern = r'(?:^|\n)(?:\w+\s+)*(\w+)::(\w+)\s*\([^)]*\)\s*(?:const\s*)?\s*\{'
        class_matches = re.finditer(class_pattern, content)
        
        for match in class_matches:
            class_name = match.group(1)
            method_name = match.group(2)
            defined_functions.add(method_name)
            defined_functions.add(f"{class_name}::{method_name}")
            
    except Exception as e:
        print(f"Error extracting functions from source: {e}", file=sys.stderr)
    
    return defined_functions

def is_external_function(func_name, defined_functions):
    """Check if a function is external (not defined in our source)."""
    # Handle class methods and simple function names
    base_name = func_name.split('::')[-1] if '::' in func_name else func_name
    return base_name not in defined_functions and func_name not in defined_functions

def process_cflow_output(cflow_output, defined_functions):
    """Process cflow output, filtering out external functions."""
    lines = cflow_output.split('\n')
    result = []
    
    # Keep track of the current line's indentation and if we should skip branches
    skip_levels = []
    
    for line in lines:
        if not line.strip():
            continue
            
        # Calculate indentation level
        indent = len(line) - len(line.lstrip())
        level = indent // 4  # cflow typically indents by 4 spaces
        
        # Skip this line if we're in a branch we're skipping
        if any(skip_level < level for skip_level in skip_levels):
            continue
            
        # Clean the skip_levels list for levels we've already passed
        skip_levels = [l for l in skip_levels if l >= level]
        
        # Extract function name
        match = re.search(r'(\w+(?:::\w+)?)\(\)', line.strip())
        if match:
            func_name = match.group(1)
            
            if is_external_function(func_name, defined_functions):
                # Mark this level to skip all its children
                skip_levels.append(level)
                continue
        
        result.append(line)
    
    return '\n'.join(result)

def run_cflow_and_filter(file_path):
    """Run cflow command and filter the output."""
    try:
        # Get list of defined functions
        defined_functions = extract_functions_from_source(file_path)
        
        # Run cflow command
        command = ["cflow", "--tree", "--brief", "--omit-arguments", "--no-cpp", file_path]
        result = subprocess.run(command, capture_output=True, text=True, check=True)
        
        # Process and filter the output
        filtered_output = process_cflow_output(result.stdout, defined_functions)
        
        # Print the filtered output
        print(filtered_output)
        
    except subprocess.CalledProcessError as e:
        print(f"Error running cflow: {e}", file=sys.stderr)
        print(f"Error output: {e.stderr}", file=sys.stderr)
        sys.exit(1)
    except FileNotFoundError:
        print("Error: cflow command not found. Please make sure it is installed.", file=sys.stderr)
        sys.exit(1)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} <filename>", file=sys.stderr)
        sys.exit(1)
    
    file_path = sys.argv[1]
    
    if not os.path.isfile(file_path):
        print(f"Error: File '{file_path}' not found.", file=sys.stderr)
        sys.exit(1)
        
    run_cflow_and_filter(file_path)
