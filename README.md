# Comp222Assignment4

This program has a representation of 1 level of processor cache & a representation for main
memory. The user is able to read & write values to cache which in turn reads & writes the values to
the main memory if needed. When a value is read or written, the program lets the user know what &
where that value is stored in cache. The cache is represented as a dynamic array of structs. Main
memory is represented by a dynamic array of ints. A cache line is represented by the "line" struct.
The line_block_pointer points to a block of words. That block can take on many different contiguous
memory addresses from main memory. 
