// trace.c
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>

#define MAX_TRACE_ENTRIES 1000000

typedef struct {
    char type[10];          // "ENTER" or "EXIT"
    void* func_address;     // Function address
    long timestamp;         // Timestamp in microseconds
    int depth;              // Call depth
} TraceEntry;

static TraceEntry trace_buffer[MAX_TRACE_ENTRIES];
static int trace_buffer_index = 0;
static int trace_depth = 0;
static int logging_active = 0;
static struct timeval program_start_time;

static long get_timestamp();
static void init_trace();
static void flush_trace_data();
static void add_trace_entry(const char* type, void* func_address);
void __cyg_profile_func_enter(void *func_address, void *call_site);
void __cyg_profile_func_exit(void *func_address, void *call_site);

// Helper function to get current timestamp in microseconds
static long get_timestamp() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec - program_start_time.tv_sec) * 1000000 + 
           (tv.tv_usec - program_start_time.tv_usec);
}

// Initialize tracing (called automatically on first use)
static void init_trace() {
    gettimeofday(&program_start_time, NULL);
    atexit(flush_trace_data);  // Register flush on exit
}

// Write all buffered data to disk
static void flush_trace_data() {
    FILE *trace_file = fopen("trace.perf", "w");
    if (!trace_file) {
        fprintf(stderr, "CRITICAL ERROR: Could not open trace file trace.perf\n");
        perror("fopen error");
        return;
    }

    // Write perf-compatible header
    fprintf(trace_file, "# timestamp,event,depth,function_address\n");
    
    for (int i = 0; i < trace_buffer_index; i++) {
        TraceEntry *entry = &trace_buffer[i];
        fprintf(trace_file, "%ld,%s,%d,%p\n",
                entry->timestamp,
                entry->type,
                entry->depth,
                entry->func_address);
    }
    
    fclose(trace_file);
}

// Add a trace entry to the buffer
static void add_trace_entry(const char* type, void* func_address) {
    if (trace_buffer_index >= MAX_TRACE_ENTRIES) {
        // Buffer full - flush and reset
        flush_trace_data();
        trace_buffer_index = 0;
    }
    
    TraceEntry *entry = &trace_buffer[trace_buffer_index++];
    strncpy(entry->type, type, sizeof(entry->type));
    entry->func_address = func_address;
    entry->timestamp = get_timestamp();
    entry->depth = trace_depth;
}

// Function called upon entering a function
void __cyg_profile_func_enter(void *func_address, void *call_site) {
    if (trace_buffer_index == 0) {
        init_trace();  // Initialize on first use
    }

    if (logging_active) {
        return;
    }

    logging_active = 1;
    add_trace_entry("ENTER", func_address);
    trace_depth++;
    logging_active = 0;
}

// Function called upon exiting a function
void __cyg_profile_func_exit(void *func_address, void *call_site) {
    if (logging_active) {
        return;
    }

    logging_active = 1;
    if (trace_depth > 0) {
        trace_depth--;
    }
    add_trace_entry("EXIT", func_address);
    logging_active = 0;
}