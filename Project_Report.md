
Project Title: Hotel Booking System (HBS)
Team Members: Bouabdallah Abderrahmene B4/ Attoui Amine B2
Institution: Higher National School of Advanced Technologies
Date: 17th April 2025

Table of Contents�:
1. Abstract
2. Introduction
3. Problem Statement
4. Objectives
5. System Design & Modeling
6. Data Structures Used
7. Algorithms Implemented
8. Screenshots
9. Challengs & solutions
10. Conclusion
11. Acknowledgment
1

1. Abstract
The Hotel Booking System (HBS) is a software application that replicates the operations of a hotel in the real world. It is designed to effectively manage rooms, guests, and reservations. The system enables hotel personnel to verify room availability, facilitate guest check-ins and check-outs, and manage reservation queues. Guests have the ability to explore available rooms, make or cancel reservations, and view their booking history. Aimed at improving the efficiency and organization of hospitality services, HBS is suitable for use in both small guesthouses and large multi-story hotels.
2. Introduction
The HBS is a modular application designed for console use, simulating the fundamental processes of hotel management. It is developed entirely in C, emphasizing clarity, modularity, and the effective utilization of data structures including trees, queues, stacks, and arrays.
Purpose: Replace error-prone manual hotel management with a structured, automated solution.
 Scope:  CRUD operations for rooms/guests - Real-time booking tracking - Visual hotel structure (tree view) - Data persistence.

3. Problem Statement
The first difficulty we faced was that the code was very long, exceeding hundreds of lines. This was our first experience working with such a large codebase. Searching for anything in the code or finding any function took a long time, and fixing any error also consumed a lot of time.
The second difficulty was that whenever we wanted to add a new feature to the code, it took us a long time to implement it across all the functions and throughout the entire code.
Third, we faced problems with input and output handling, as well as file operations and data saving.
We also encountered issues when trying to link between the different data structures.
Manual hotel management leads to errors, inefficiency, and poor guest service. HBS aims to digitalize and structure the workflow, ensuring easier tracking of rooms, guest status, and facilities, particularly in multi-floor hotel setups.
Finally, we faced a challenge in managing and testing the code effectively.

2
4. Objectives
* Implement a modular hotel management system in C.
* Use core data structures (Tree, Stack, Queue, Linked List).
* Support operations: add/view/update/delete guests & rooms.
* Display hotel structure (N-ary tree).
* Enable room booking, cancellation, check-in/out.
* Support save/load to file.
* Extend facility management per floor.

5. System Design & Modeling

6. Data Structures Used
* struct room, struct guest, struct Facility, struct Node
* enum RoomType, enum RoomStatus, enum GuestStatus
* Linked lists (facilities), Queue (waiting guests), Stack (checkout)
* Tree (hotel structure)
* 2D array for schedule, 1D arrays for room/guest lis
* N-ary Tree: Hotel > Floor > Rooms + Facilities
3
* 2D Array: RoomStatus schedule[FLOORS][ROOMS_PER_FLOOR]
* Stack: Checkout history
* Queue: Guest waiting list
* Linked List: Facilities per floor
* 1D Arrays: Guests, Rooms

7. Algorithms Implemented
* Tree traversal (print hotel structure)
* Bubble sort (rooms by index)
* Queue enqueue/dequeue
* Stack push/pop for undo checkout
* File for save/load system state
* Search and filter rooms/guests

8. Screenshots
 





             The Main menu                                                               Hotel Structure View (Tree)       





                 Room menu		     Booking Menu
4
9. challengs & solutions
Challenge�:Solution�:Memory leaks in treesRecursive freeTree() functionRebuilding BST from fileSave room counts first, then pre-order traversalQueue overflowCircular buffer with modulo arithmeticDynamic memory fragmentationUse memory pools or slab allocationDangling pointers after deletionSet pointers to NULL after free()Stack overflow in deep recursionReplace recursion with iterative approachInefficient search in linked listsUse hash table for fast lookup







10. Conclusion
The HBS project successfully applies data structures to solve real-world problems in hotel management. It strengthened our understanding of structured programming and system design by implementing CRUD operations, state persistence, and a structured hotel layout using the C language. Its modular design supports future extensions, such as a GTK-based graphical user interface, improved file export features, and advanced search functionality. This project highlights the effectiveness of C in building robust, extensible, and efficient management systems.
Beyond the technical aspects, I learned a lot from this project. I learned how to manage my time, how to break down a large program into smaller, organized parts, and how to stay patient and persistent throughout the development process. Through consistent effort, I was able to achieve my goal and create a system that organizes hotel operations efficiently�saving time, effort, and ultimately, money. The code can be adapted to any hotel simply by specifying the number of rooms, floors, available suites, and facilities. With further improvements, such as integrating a GTK-based GUI, the system could become a complete and user-friendly solution for hotel management.

11. Acknowledgment
I would like to express my sincere gratitude to Mr. GUELFOUT Nasreddine and Mrs. LAKHDARI Keira for their valuable guidance, support, and encouragement throughout the development of this project. Their insights and feedback were essential to the successful completion of this work .                             
5


