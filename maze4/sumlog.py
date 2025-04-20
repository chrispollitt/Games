#!/usr/bin/env python3
import sys
from collections import defaultdict

# UTF-8 tree symbols
TREE_LAST = "└── "
TREE_MID  = "├── "
TREE_VERT = "│   "
TREE_SPACE = "    "

def build_timed_hierarchy(logfile):
    stack = []
    call_dict = defaultdict(list)
    timing_data = defaultdict(dict)
    root = None

    with open(logfile, 'r') as f:
        lines = f.readlines()
        
        for line_index, line in enumerate(lines):
            if line.startswith('#'):
                continue
            parts = line.strip().split(',')
            if len(parts) < 5:
                continue
            ts, event, depth, dur, func = parts[:5]
            ts, depth, dur = int(ts), int(depth), int(dur)

            if event == "ENTER":
                if stack:
                    parent = stack[-1][0]  # Get function name from stack tuple
                    if func not in call_dict[parent]:  # Avoid duplicates
                        call_dict[parent].append(func)
                else:
                    root = func
                stack.append((func, ts))
            elif event == "EXIT" and stack:
                start_func, start_ts = stack.pop()
                if start_func == func:
                    # Use the duration field from the CSV directly
                    timing_data[func].setdefault('time', 0)
                    timing_data[func]['time'] += dur
                    timing_data[func]['calls'] = timing_data[func].get('calls', 0) + 1
        
        # Handle any remaining functions in the stack (including main)
        if stack and line_index == len(lines) - 1:
            # Get timestamp from the last line for calculating remaining durations
            last_ts = ts
            
            # Process remaining stack in reverse order (innermost to outermost)
            while stack:
                func, start_ts = stack.pop()
                # Calculate duration from start to last timestamp
                duration = last_ts - start_ts
                
                # Update timing data
                timing_data[func].setdefault('time', 0)
                timing_data[func]['time'] += duration
                timing_data[func]['calls'] = timing_data[func].get('calls', 0) + 1

    return root, call_dict, timing_data

def print_timed_tree(node, call_dict, timing_data, prefix="", is_last=True):
    if not node:
        return
    
    # Format timing info
    time_info = timing_data.get(node, {'time': 0, 'calls': 0})
    total_time = time_info['time']
    calls = time_info['calls']
    
    # Calculate per-call time
    per_call_time = 0
    if calls > 0:
        per_call_time = total_time / calls
    
    time_str = f" [{total_time/1000:.1f}ms total, {per_call_time/1000:.2f}ms per call, {calls} calls]"

    branch = TREE_LAST if is_last else TREE_MID
    print(f"{prefix}{branch}{node}{time_str}")

    new_prefix = prefix + (TREE_SPACE if is_last else TREE_VERT)
    children = call_dict.get(node, [])
    
    for i, child in enumerate(children):
        print_timed_tree(child, call_dict, timing_data, new_prefix, i == len(children) - 1)

def build_call_hierarchy(logfile):
    stack = []
    call_dict = defaultdict(list)
    root = None

    with open(logfile, 'r') as f:
        for line in f:
            if line.startswith('#'):
                continue
            parts = line.strip().split(',')
            if len(parts) < 5:
                continue
            _, event, depth, _, func = parts[:5]
            depth = int(depth)

            if event == "ENTER":
                if stack:
                    parent = stack[-1]
                    if func not in call_dict[parent]:  # Avoid duplicates
                        call_dict[parent].append(func)
                else:
                    root = func
                stack.append(func)
            elif event == "EXIT" and stack:
                stack.pop()

    return root, call_dict

def print_tree(node, call_dict, prefix="", is_last=True):
    if not node:
        return
    
    # Current node line
    branch = TREE_LAST if is_last else TREE_MID
    print(f"{prefix}{branch}{node}")

    # Children handling
    new_prefix = prefix + (TREE_SPACE if is_last else TREE_VERT)
    children = call_dict.get(node, [])
    
    for i, child in enumerate(children):
        print_tree(child, call_dict, new_prefix, i == len(children) - 1)

def main():
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} trace_resolved.perf", file=sys.stderr)
        sys.exit(1)

    # timed tree
    root, call_dict, timing_data = build_timed_hierarchy(sys.argv[1])
    print("🌳 Timed Call Tree (Hierarchical View)")
    print("═" * 50)
    print_timed_tree(root, call_dict, timing_data)
    
    # bare tree
#    root, call_dict = build_call_hierarchy(sys.argv[1])
#    print("🌳 Bare Call Tree (Hierarchical View)")
#    print("═" * 50)
#    print_tree(root, call_dict)

if __name__ == "__main__":
    main()
