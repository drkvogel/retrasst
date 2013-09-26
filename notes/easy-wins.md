today:
    chunking
sgBoxesFixedCellClick
    sortChunk    
    
get rid of samplechunk, boxchunk?    
    
sgwVials
    chunks[] // indexes to start/end of list
    
or

Chunk
    * list? // pointer to *part* of vector...hmmm
    * wrapper->sort
    
easy wins:
finish off chunk stringgridwrapper refactoring
sort out add first chunk mess:
    count the number of rows and put that as the end and size of the initial chunk
    double-clicking a row marks the start of a new chunk:
        set the previous row as the end of the previous chunk
        create a new chunk:
            set its start to that of the row clicked on
            set its end to the end of the whole list
    pressing incr increments the end of the current chunk (not in spec?)
    pressing decr decrements the end of the current chunk
    changing values in the chunk stringgrid changes chunk parameters? (not in spec?)
    
         
reject list
create list?

wrench -> settings -> advanced -> privacy -> content settings -> all cookies