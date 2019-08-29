

# Page replacement algorithms2

This document serves as a small index of page-replacement algorithms implemented in C.

## Not Recently Used

Favours keeping pages that have been recently used by categorizing pages into four different classes. The classes are defined by two bits: referenced (R-bit) and modified (M-bit). 

- Class 0: not referenced, not modified (0, 0)
- Class 1: not referenced,  modified (0, 1)
- Class 2: referenced, not modified (1, 0)
- Class 3: referenced, modified (1, 1)

A timer interrupt is used to update the bits accordingly, usually once every 20ms. The timer interrupt clears the referenced bits of all the pages. Only pages referenced within the current timer interval have their referenced bit set to 1.

Whenever a page needs to be replaced, the algorithm a page by randomly picking one from the lowest class that contains pages. 

At every interrupt, the modified and referenced bits of each page gets updated.

Take note of the fact that a class 2 page can occur when the referenced bit of a modified page is cleared by the timer interrupt.

#### Timer interrupt & modified/referenced implementation

The timer in this implementation is a bit different than a unit of time. Instead of checking every Xms, it checks every time 5 pages are referenced. 

Similarly, the program modifies every other page that gets referenced to simulate processes modifying their pages.