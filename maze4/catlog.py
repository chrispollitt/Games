#!/usr/bin/env python3

import os
import sys
import time
import subprocess
import re
from collections import defaultdict

def wait_for_file_with_content(path):
    """Wait until the specified file exists and has content"""
    while not os.path.exists(path) or os.path.getsize(path) == 0:
        time.sleep(0.1)

def parse_nm_symbols(executable):
    """Extract symbols from the executable using nm"""
    symbols = {}
    try:
        output = subprocess.check_output(['nm', '-C', executable], text=True)
    except subprocess.CalledProcessError as e:
        print(f"Error running nm: {e}", file=sys.stderr)
        sys.exit(1)

    for line in output.splitlines():
        parts = line.strip().split()
        if len(parts) >= 3 and parts[1] == 'T':
            addr = int(parts[0], 16)
            symbol = ' '.join(parts[2:])
            symbols[addr] = symbol
    return symbols

def find_main_symbol(symbols):
    """Find the 'main' symbol or the lowest address as fallback."""
    for addr, name in symbols.items():
        if name == 'main':
            return addr
    return min(symbols.keys()) if symbols else None

def read_first_function_address(logfile):
    """Find the first function address from the trace"""
    with open(logfile, 'r') as f:
        for line in f:
            if line.startswith('#'):
                continue
            parts = line.strip().split(',')
            if len(parts) >= 4 and parts[1] == 'ENTER':
                return int(parts[3], 16)
    return None

def is_process_running(name):
    """Check if the target process is still running"""
    myname = 'catlog'
    # Linux or Cygwin with /proc
    if os.path.exists('/proc'):
        for pid in os.listdir('/proc'):
            if pid.isdigit():
                try:
                    with open(f'/proc/{pid}/cmdline', 'rb') as f:
                        cmdline = f.read().decode(errors='ignore').replace('\x00', ' ').strip()
                        if name in cmdline and myname not in cmdline:
                            return True
                except (FileNotFoundError, PermissionError):
                    continue
    # Fallback: use psutil if available
    else:
        try:
            import psutil
            for proc in psutil.process_iter(['name', 'cmdline']):
                try:
                    if (name in proc.info['name'] or 
                        any(name in arg for arg in proc.info['cmdline'])):
                        if not (myname in proc.info['name'] or 
                               any(myname in arg for arg in proc.info['cmdline'])):
                            return True
                except (psutil.NoSuchProcess, psutil.AccessDenied):
                    continue
        except ImportError:
            print("psutil not available - limited process detection", file=sys.stderr)
    return False

def tail_and_resolve(logfile, symbols, base_offset, resolved_path, traced_proc_name):
    """Process the trace file and resolve symbols"""
    in_count = 0
    out_count = 0
    call_stack = []
    timing_data = defaultdict(list)
    active_process = True

    with open(logfile, 'r') as f_log, open(resolved_path, 'w') as f_out:
        f_out.write("# timestamp,event,depth,duration,function\n")
        
        while active_process:
            line = f_log.readline()
            if not line:
                # Check if the target process is still running
                if not is_process_running(traced_proc_name):
                    print("\nTarget process has terminated", file=sys.stderr)
                    active_process = False
                    break
                time.sleep(0.1)
                continue

            in_count += 1
            if line.startswith('#'):
                continue
            
            parts = line.strip().split(',')
            if len(parts) < 4:
                print(f"Malformed line: {line}", file=sys.stderr)
                continue
                
            timestamp, event, depth, func_addr_hex = parts[:4]
            func_addr = int(func_addr_hex, 16)
            img_addr = func_addr - base_offset
            func_name = symbols.get(img_addr, f"0x{func_addr:x} (unresolved)")
            
            # Handle timing and call stack
            if event == 'ENTER':
                call_stack.append((timestamp, func_name))
                out_line = f"{timestamp},{event},{depth},0,{func_name}\n"
            elif event == 'EXIT' and call_stack:
                start_time, start_func = call_stack.pop()
                if start_func == func_name:
                    duration = int(timestamp) - int(start_time)
                    timing_data[func_name].append(duration)
                    out_line = f"{timestamp},{event},{depth},{duration},{func_name}\n"
                else:
                    out_line = f"{timestamp},{event},{depth},-1,{func_name}\n"
                    print(f"Call stack mismatch: entered {start_func} but exited {func_name}", 
                          file=sys.stderr)
            else:
                out_line = line
            
            f_out.write(out_line)
            out_count += 1
    
    # Generate summary statistics
    summary_path = f"{os.path.splitext(resolved_path)[0]}_summary.perf"
    with open(summary_path, 'w') as f_sum:
        f_sum.write("Function Timing Summary (μs):\n")
        f_sum.write("---------------------------------\n")
        for func, times in sorted(timing_data.items(), 
                                key=lambda x: sum(x[1])/len(x[1]), 
                                reverse=True):
            avg = sum(times) / len(times)
            total = sum(times)
            count = len(times)
            f_sum.write(f"{func}: calls={count} avg={avg:.1f} total={total}\n")
    
    return in_count, out_count

def main():
    if len(sys.argv) < 2:
        print("Usage: ./catlog.py <executable> [trace.perf]", file=sys.stderr)
        print("Default trace file: trace.perf", file=sys.stderr)
        sys.exit(1)

    executable = sys.argv[1]
    logfile = sys.argv[2] if len(sys.argv) > 2 else "trace.perf"
    resolved_log = f"{os.path.splitext(logfile)[0]}_resolved.perf"
    proc_name = os.path.basename(executable)

    print(f"Waiting for log file: {logfile}", file=sys.stderr)
    wait_for_file_with_content(logfile)

    print(f"Parsing symbols from: {executable}", file=sys.stderr)
    symbols = parse_nm_symbols(executable)
    if not symbols:
        print("Error: No symbols found in executable", file=sys.stderr)
        sys.exit(1)
    print(f"Found {len(symbols)} symbols", file=sys.stderr)

    print("Finding first function address...", file=sys.stderr)
    trace_addr = read_first_function_address(logfile)
    if trace_addr is None:
        print("Error: Couldn't find any function addresses in trace", file=sys.stderr)
        sys.exit(1)

    main_addr = find_main_symbol(symbols)
    base_offset = trace_addr - main_addr
    
    print(f"\nBase address calculation:", file=sys.stderr)
    print(f"  First trace address: 0x{trace_addr:x}", file=sys.stderr)
    print(f"  Main symbol address: 0x{main_addr:x}", file=sys.stderr)
    print(f"  Calculated offset:   0x{base_offset:x}\n", file=sys.stderr)

    print("Starting trace resolution...", file=sys.stderr)
    in_lines, out_lines = tail_and_resolve(logfile, symbols, base_offset, resolved_log, proc_name)
    
    print("\nTrace processing complete.", file=sys.stderr)
    print(f"Resolved trace saved to {resolved_log}", file=sys.stderr)
    print(f"Function timing summary saved to {resolved_log.replace('.perf', '_summary.perf')}", file=sys.stderr)
    print(f"Processed {in_lines} lines, wrote {out_lines} lines", file=sys.stderr)

if __name__ == "__main__":
    main()