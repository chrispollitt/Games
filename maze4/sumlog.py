#!/usr/bin/env python3
import sys
import re
from collections import defaultdict

# UTF-8 tree symbols
TREE_LAST  = "└─"
TREE_MID   = "├─"
TREE_VERT  = "│ "
TREE_SPACE = "  "
# Define width constants
FUNC_WIDTH  = 40  # Space for function name including tree branches
TOTAL_WIDTH = 10  # Width for total time column
AVG_WIDTH   = 10  # Width for average time column
CALLS_WIDTH =  6  # Width for calls column

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
    
    time_str = f" {total_time/1000:.1f}, {per_call_time/1000:.2f}, {calls}"

    branch = TREE_LAST if is_last else TREE_MID
    if not re.search(r"^(_|0x)", node):
      nodet = re.sub(r"\(.*$", "", node)
      func_column = f"{prefix}{branch}{nodet}"
      time_str = f"{total_time/1000:{TOTAL_WIDTH}.1f}{per_call_time/1000:{AVG_WIDTH}.2f}{calls:{CALLS_WIDTH}d}"
      print(f"{func_column:{FUNC_WIDTH}}{time_str}")
    new_prefix = prefix + (TREE_SPACE if is_last else TREE_VERT)
    children = call_dict.get(node, [])
    
    for i, child in enumerate(children):
        print_timed_tree(child, call_dict, timing_data, new_prefix, i == len(children) - 1)

def main():
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} trace_resolved.perf", file=sys.stderr)
        sys.exit(1)

    # timed tree
    root, call_dict, timing_data = build_timed_hierarchy(sys.argv[1])
    print("🌳 Timed Call Tree (Hierarchical View)")
    print("═" * 66)
    print(f"{'FUNCTION':<{FUNC_WIDTH}}{'TOTAL':>{TOTAL_WIDTH}}{'AVG/CALL':>{AVG_WIDTH}}{'CALLS':>{CALLS_WIDTH}}")
    print_timed_tree(root, call_dict, timing_data)
    
if __name__ == "__main__":
    main()
