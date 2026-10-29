#!/usr/bin/env python3
import sys
import re
import csv
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
CPL_WIDTH   =  8  # Width for calls per loop column

def build_timed_hierarchy(logfile):
    stack = []
    call_dict = defaultdict(list)
    timing_data = defaultdict(dict)
    root = None

    with open(logfile, 'r') as f:
        lines = f.readlines()
        lines_len = len(lines) - 1
        print(f"Debug: lines = {lines_len}", file=sys.stderr)
        
        for line_index, line in enumerate(lines):
            if line.startswith('#'):
                print(f"Debug: skipping comment {line}", end="", file=sys.stderr)
                continue
            parts = line.strip().split(',')
            if len(parts) < 5:
                print(f"Debug: skipping non-5 part line {line}", file=sys.stderr)
                continue
            ts, event, depth, dur, func = parts[:5]
            ts, depth, dur = int(ts), int(depth), int(dur)
            
            if event == "ENTER":
#                print(f"Debug: in ENTER {func}", file=sys.stderr)
                if stack:
                    parent = stack[-1][0]  # Get function name from stack tuple
                    if func not in call_dict[parent]:  # Avoid duplicates
                        call_dict[parent].append(func)
                else:
                    root = func
                stack.append((func, ts))
            elif event == "EXIT" and stack:
#                print(f"Debug: in EXIT {func}", file=sys.stderr)
                start_func, start_ts = stack.pop()
                if start_func == func:
#                    print(f"Debug: match EXIT {func}", file=sys.stderr)
                    # Use the duration field from the CSV directly
                    timing_data[func].setdefault('time', 0)
                    timing_data[func]['time'] += dur
                    timing_data[func]['calls'] = timing_data[func].get('calls', 0) + 1
        
        # Handle any remaining functions in the stack (including main)
        if stack and line_index == lines_len:
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

def calculate_loop_count(timing_data):
    # Look for update_monsters() function to determine loop count
    update_monsters_calls = 0
    for func, data in timing_data.items():
        if "update_monsters" in func:
            update_monsters_calls = data.get('calls', 0)
            print(f"Debug: Found update_monsters with {update_monsters_calls} calls", file=sys.stderr)
            break
    
    # Each loop calls update_monsters() once every 5 iterations
    loop_count = update_monsters_calls * 5 if update_monsters_calls > 0 else 1
    print(f"Debug: Calculated loop count: {loop_count}", file=sys.stderr)
    return loop_count

def print_timed_tree(node, call_dict, timing_data, loop_count, prefix="", is_last=True, flat_data=None):
    if not node:
        print(f"Debug: node is empty", file=sys.stderr)
        return
    
    # Format timing info
    time_info = timing_data.get(node, {'time': 0, 'calls': 0})
    total_time = time_info['time']
    calls = time_info['calls']
    
    # Calculate per-call time
    per_call_time = 0
    if calls > 0:
        per_call_time = total_time / calls
    
    # Calculate calls per loop
    calls_per_loop = calls / loop_count if loop_count > 0 else 0
    
    branch = TREE_LAST if is_last else TREE_MID
    if not re.search(r"^(_|0x)", node):
        nodet = re.sub(r"\(.*$", "", node)
        func_column = f"{prefix}{branch}{nodet}"
        time_str = f"{total_time/1000:{TOTAL_WIDTH}.1f}{per_call_time/1000:{AVG_WIDTH}.2f}{calls:{CALLS_WIDTH}d}"
        print(f"{func_column:{FUNC_WIDTH}}{time_str}")
        
        # Add to flat data for CSV export
        if flat_data is not None:
            # Strip tree symbols from function name for CSV
            clean_func = nodet.strip()
            # Add to flat_data if not already present
            func_key = (clean_func, total_time, per_call_time, calls, calls_per_loop)
            if func_key not in flat_data:
                flat_data.append(func_key)
    else:
        print(f"Debug: skipping non-standard func {node}", file=sys.stderr)
    
    new_prefix = prefix + (TREE_SPACE if is_last else TREE_VERT)
    children = call_dict.get(node, [])
    
    for i, child in enumerate(children):
        print_timed_tree(child, call_dict, timing_data, loop_count, new_prefix, i == len(children) - 1, flat_data)

def export_csv(data, output_file):
    """Export flat function data to CSV file"""
    with open(output_file, 'w', newline='') as csvfile:
        csv_writer = csv.writer(csvfile)
        # Add header
        csv_writer.writerow(['FUNCTION', 'TOTAL', 'AVG/CALL', 'CALLS', 'CALLS/LOOP'])
        # Sort by function name
        for func, total, avg, calls, calls_per_loop in sorted(data, key=lambda x: x[0]):
            csv_writer.writerow([
                func, 
                f"{total/1000:.1f}", 
                f"{avg/1000:.2f}", 
                calls,
                f"{calls_per_loop:.2f}"
            ])

def main():
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} trace_resolved.perf [output.csv]", file=sys.stderr)
        sys.exit(1)

    input_file = sys.argv[1]
    # Default output CSV filename based on input with .csv extension
    output_csv = sys.argv[2] if len(sys.argv) > 2 else input_file.rsplit('.', 1)[0] + '.csv'
    
    # Flat data structure for CSV export: [(func, total_time, avg_time, calls, calls_per_loop), ...]
    flat_data = []

    # timed tree
    try:
        root, call_dict, timing_data = build_timed_hierarchy(input_file)
        
        # Calculate loop count based on update_monsters() calls
        loop_count = calculate_loop_count(timing_data)
        
        print("🌳 Timed Call Tree (Hierarchical View)")
        print("═" * 66)
        print(f"{'FUNCTION':<{FUNC_WIDTH}}{'TOTAL':>{TOTAL_WIDTH}}{'AVG/CALL':>{AVG_WIDTH}}{'CALLS':>{CALLS_WIDTH}}")
        print_timed_tree(root, call_dict, timing_data, loop_count, flat_data=flat_data)
        
        # Export CSV file
        export_csv(flat_data, output_csv)
        print(f"\nCSV data exported to: {output_csv}")
        print(f"Loop count used for calculations: {loop_count}")
        
    except Exception as e:
        print(f"❌ Fatal error: {e}", file=sys.stderr)
        sys.exit(1)
    
if __name__ == "__main__":
    main()
