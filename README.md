CS 211: Computer Architecture: Fall 2013
Programming Assignment 1: Freqaddr
Author: Jeremy Balise
Instructor: Prof. Santosh Nagarakatte

I. Main Data Structures
This program uses two arrays of structs to store, sort, and ascertain the frequency of hexadecimal addresses read from a text 
file. Each struct is composed of a 64-bit hexadecimal number and two integers, and the length of each array is equal to the n 
non-unique addresses of which the text file consists.

II. Space Requirement Analysis
It should be noted that the actual number of items stored in the second array is equal to k, the number of unique addresses 
found in the text file. However, since the value of k is not known prior to the construction of this array, space on the order 
of n is allocated for both arrays in the case that k = n (that is, each address in the file is unique). The overall space 
requirements of the program are therefore on the order of n + n = 2*n, which is O(n).

III. Runtime Analysis
All processes undertaken within the program prior to the point at which the file is opened are constant-time operations. The 
first non-trivial operation to take place in the program is the determining of the number of lines, n, which are to be read 
from the file. This is a simple O(n) process; the more lines that are present, the longer this operation will take. By the same 
logic, building the array of n structs is also an O(n) process.

Next, this array is sorted using qsort from stdlib.h. The runtime of qsort on this list of n items, while not necessarily 
guaranteed to be an actual quicksort, is guaranteed by the C standard to be an O(n*log(n)) process.

After this array is sorted, the second array of k items is made via a scan of the first array of n items. This is an O(n) 
process. After this scan is complete, the new array is sorted using qsort, which is an O(k*log(k)) process because there are 
k items in the array. The overall runtime of the program is therefore on the order of n + n + n*log(n) + n + k*log(k), which 
is o(3*n + n*log(n) + k*log(k)) = O(n*log(n)). If k is small, the runtime is on the order of o(3*n + n*log(n) = O(n*log(n)). 
In the worst case, k=n and the runtime is on the order of o(3*n + 2*n*log(n)), which is also O(n*log(n)). The best case and 
worst case are therefore both O(n*log(n)).

IV. Challenges
This is the first code I have ever written using the C programming language; as a result, accounting for the possibility for 
C code to allow for buffer overflow was a significant challenge when coding, more than usual because I am not used to taking 
into account the possibility of non-secure code. In the same vein of thought, the main arrays used in this project were 
originally allocated to the call stack and were later allocated to the runtime heap due to the possibility that n is too 
large a number of structs for the call stack to accommodate. Finally, my original code did not preserve the original order of 
addresses with the same frequency; adding an integer to the struct indicating the original position of each address in the 
array and manipulating this number appropriately required critical thinking and careful consideration with regard to 
implementation.
