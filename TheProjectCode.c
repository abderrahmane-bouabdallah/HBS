
//WELCOM ;)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <conio.h>  // already done
#define UP 72
#define DOWN 80
#define ENTER 13


//Constants
#define MAX_GUESTS 1000
#define MAX_ROOMS 200
#define FLOORS 4
#define ROOMS_PER_FLOOR 50
#define MAX_HISTORY 200
#define MAX_WAITING 200


//enumators
typedef enum { AVAILABLE = 1, BOOKED, UNDER_MAINTENANCE } RoomStatus;
typedef enum { WAITING = 1, UNDER_TREATMENT, DISCHARGED } GuestStatus;
typedef enum { SINGLE = 1, DOUBLE, SUITE , DELUXE} RoomType;

//rooms structur
typedef struct { 
    int number;
    int floor ;
    int index;
    int geustid;
    char guestname[50];
    RoomType type;
    RoomStatus status;
} room;

//guests structur
typedef struct { 
    int id;
    char name[50];
    int room_number;
    GuestStatus status;
} guest;

// 1D array for rooms and guests
room rooms[MAX_ROOMS];
guest guests[MAX_GUESTS];
//track rooms and guests count
int room_count = 0;
int guest_count = 0;

//stack structur for check out history
typedef struct { 
    int guestId;
    int roomNumber;
    char guestName[50];
} CheckoutRecord;

CheckoutRecord checkoutHistory[MAX_HISTORY];
int historyTop = -1;

// Struct for hotel facilities (linked list)
typedef struct Facility {
    char name[50];             
    char description[100];      
    struct Facility* next;      
} Facility;



// Struct for facilities (linked list)
typedef struct FloorFacility {
    int floorNumber;
    Facility* facilities;
} FloorFacility;
FloorFacility floorFacilities[FLOORS]; // array to store floor-specific facilities



//queue structur for WaitingGuests
typedef struct { 
    int guestId;
    RoomType preferredType;
    char name[50];
} WaitingGuest;
WaitingGuest waitingQueue[MAX_WAITING];
int front = 0, rear = -1, waitingCount = 0;



// Function prototypes
const char* roomTypeToString(RoomType type);
const char* roomstatus(RoomStatus status);
const char* gueststatus(GuestStatus status);

void viewavailableRooms();
void viewbookedRooms();
void viewSchedule();

void mainMenu();
void manageRoomsMenu();
void manageGuestMenu();
void bookingMenu();
void SearchRoomGuest();

void addroom();
void addguest();
void viewrooms();
void viewguests();
void updateRoom();
void bookroom();
void cancelbooking();
void checkInGuest();
void checkOutGuest();
void freeTree();
void pushCheckoutHistory();



typedef struct Node { //struct node -> Node
    char label[50];         // Label to display
    int isRoom;             // 1 if it's a room, 0 for other items like facilities    
    int roomnumber;
    int childCount;
    struct Node* children[ROOMS_PER_FLOOR];
} Node;


// Function to create a new node in the tree
Node* createNode(const char* label, int isRoom, int roomNumber) {
    Node* newNode = (Node*)malloc(sizeof(Node)); // Allocate memory for the node
    if (newNode == NULL) {
        printf("Memory allocation failed for new node.\n");
        exit(1); // Exit if memory allocation fails
    }
    strcpy(newNode->label, label);       // Set the display name
    newNode->isRoom = isRoom;            // Set whether this node is a room
    newNode->roomnumber = roomNumber;    // Save room number if applicable
    newNode->childCount = 0;             // No children yet

    // Initialize all child pointers to NULL
    for (int i = 0; i < ROOMS_PER_FLOOR; i++) {
        newNode->children[i] = NULL;
    }

    return newNode;
}


// Function to create the hotel structure tree
Node* createHotelStructureTree() {
    Node* hotel = createNode("Hotel", 0, -1);

    for (int f = 0; f < FLOORS; f++) {
        char floorLabel[20];
        sprintf(floorLabel, "Floor %d", f);
        Node* floorNode = createNode(floorLabel, 0, -1);

        // Add rooms for this floor
        int roomIndex = 0;
        for (int i = 0; i < room_count; i++) {
            if (rooms[i].floor == f) {
                char roomLabel[20];
                sprintf(roomLabel, "Room %d", rooms[i].number);
                Node* roomNode = createNode(roomLabel, 1, rooms[i].number);
                floorNode->children[roomIndex++] = roomNode;
                floorNode->childCount++;
            }
        }

        // Add facilities for this floor
        Facility* current = floorFacilities[f].facilities;
        if (current != NULL) {
            Node* facilitiesNode = createNode("Facilities", 0, -1);
            int fIndex = 0;
            while (current != NULL && fIndex < ROOMS_PER_FLOOR) {
                Node* fNode = createNode(current->name, 0, -1);
                facilitiesNode->children[fIndex++] = fNode;
                facilitiesNode->childCount++;
                current = current->next;
            }
            floorNode->children[floorNode->childCount++] = facilitiesNode;
        }

        // Add floor to hotel
        hotel->children[hotel->childCount++] = floorNode;
    }

    return hotel;
}


//function to free tree
void freeTree(Node* root) { 
    if (root == NULL) return;
    
    for (int i = 0; i < root->childCount; i++) {
        freeTree(root->children[i]);
    }
    
    free(root);
}

//function to print tree with indentation
void printTree(Node* root, int depth) {
    if (root == NULL) return;
    
    // Print indentation
    for (int i = 0; i < depth; i++) {
        printf("    ");
    }
    
    if (strcmp(root->label, "Hotel") == 0) {
        printf("Hotel\n");
    } else if (root->isRoom) {
        // Print room details
        for (int i = 0; i < room_count; i++) {
            if (rooms[i].number == root->roomnumber) {
                printf("Room %d (%s) - %s\n",
                    rooms[i].number,
                    roomTypeToString(rooms[i].type),
                    roomstatus(rooms[i].status));
                    if (rooms[i].status==BOOKED)
                    {
                        printf("           guest id: %d\n           name: %s\n",rooms[i].geustid,rooms[i].guestname);
                    }
                    
                break;
            }
        }
    } else {
        // Non-room node: Floor label, Facility, etc
        printf("%s\n", root->label);
    }
    
    
    // Print children
    for (int i = 0; i < root->childCount; i++) {
        printTree(root->children[i], depth + 1);
    }
}

// Function to display the hotel structure tree
void displayHotelStructureTree() {
    printf("\n=== Hotel Structure Tree View ===\n");
    
    // Create the tree structure
    Node* hotelTree = createHotelStructureTree();
    
    // Print the tree
    printTree(hotelTree, 0);
    freeTree(hotelTree);
    printf("\nPress Enter to continue...");
    while (getchar() != '\n');
    getchar();
}

void initFloorFacilities() {
    for (int i = 0; i < FLOORS; i++) {
        floorFacilities[i].floorNumber = i;
        floorFacilities[i].facilities = NULL;
    }
}

//function to show availlable ROOMS
void viewavailableRooms() {
    if (room_count == 0) {                
        printf("NO ROOMS AVAILABLE IN SYSTEM!\n");
        return;
    }

    printf("\n*** Available Rooms List by Floor (Sorted by Index) ***\n");

    for (int f = 0; f < FLOORS; f++) {
        printf("\nFloor %d:\n", f);
        room floorRooms[ROOMS_PER_FLOOR];
        int count = 0;

        // Filter only AVAILABLE rooms on this floor
        for (int i = 0; i < room_count; i++) {
            if (rooms[i].floor == f && rooms[i].status == AVAILABLE) {
                floorRooms[count++] = rooms[i];
            }
        }

        // Sort the available rooms by index
        for (int i = 0; i < count - 1; i++) {
            for (int j = 0; j < count - i - 1; j++) {
                if (floorRooms[j].index > floorRooms[j + 1].index) {
                    room temp = floorRooms[j];
                    floorRooms[j] = floorRooms[j + 1];
                    floorRooms[j + 1] = temp;
                }
            }
        }

        if (count == 0) {
            printf("No available rooms on this floor.\n");
        } else {
            for (int i = 0; i < count; i++) {
                printf("Room Number: %d\n", floorRooms[i].number);
                printf("Type: %s\n", roomTypeToString(floorRooms[i].type));
                printf("Status: %s\n", roomstatus(floorRooms[i].status));
                printf("Index in Floor: %d\n", floorRooms[i].index);
                printf("-----------------------------\n");
            }
        }
    }
}


// function to show booked ROOMS
void viewbookedRooms() { 
    if (room_count == 0) {
        printf("NO ROOMS !\n");
        return;
    }

    printf("\n*** booked Rooms List by Floor (Sorted by Index) ***\n");

    for (int f = 0; f < FLOORS; f++) {
        printf("\nFloor %d:\n", f);
        room floorRooms[ROOMS_PER_FLOOR];
        int count = 0;

        // Filter only booked rooms on this floor
        for (int i = 0; i < room_count; i++) {
            if (rooms[i].floor == f && rooms[i].status == BOOKED) {
                floorRooms[count++] = rooms[i];
            }
        }

        // Sort the booked rooms by index
        for (int i = 0; i < count - 1; i++) {
            for (int j = 0; j < count - i - 1; j++) {
                if (floorRooms[j].index > floorRooms[j + 1].index) {
                    room temp = floorRooms[j];
                    floorRooms[j] = floorRooms[j + 1];
                    floorRooms[j + 1] = temp;
                }
            }
        }

        if (count == 0) {
            printf("No booked rooms on this floor.\n");
        } else {
            for (int i = 0; i < count; i++) {
                printf("Room Number: %d\n", floorRooms[i].number);
                printf("Type: %s\n", roomTypeToString(floorRooms[i].type));
                printf("Status: %s\n", roomstatus(floorRooms[i].status));
                printf("room guest id: %d\n", floorRooms[i].geustid);
                printf("room guest name: %s\n", floorRooms[i].guestname);
                printf("Index in Floor: %d\n", floorRooms[i].index);
                printf("-----------------------------\n");
            }
        }
    }
}


//function to convert a RoomType enum value to a corresponding string
const char* roomTypeToString(RoomType type) { 
    switch (type) {
        case SINGLE: return "SINGLE";
        case DOUBLE: return "DOUBLE";
        case SUITE: return "SUITE";
        case DELUXE: return "DELUXE";
        default: return "UNKNOWN";
    }
}

//function to convert a RoomType enum value to a corresponding string
const char* roomstatus(RoomStatus status) {
    switch (status) {
        case AVAILABLE: return "AVAILABLE";
        case BOOKED: return "BOOKED";
        case UNDER_MAINTENANCE: return "UNDER_MAINTENANCE";
        default: return "UNKNOWN";
    }
}

//function to convert a RoomType enum value to a corresponding string
const char* gueststatus(GuestStatus status) {
    switch (status) {
        case WAITING: return "WAITING";
        case UNDER_TREATMENT: return "UNDER_TREATMENT";
        case DISCHARGED: return "DISCHARGED";
        default: return "UNKNOWN";
    }
}


//2D Array Room Occupancy Schedule
RoomStatus schedule[FLOORS][ROOMS_PER_FLOOR] ;


//function to display Main Menu
void mainMenu() {
    printf("\n*** Hotel Booking System ***\n");
    printf("1. Manage Rooms\n");
    printf("2. Manage Guests\n");
    printf("3. Booking System\n");
    printf("4. Search Room / Guest\n");
    printf("5. Hotel Structure View(tree) \n");
    printf("6. View Booking Schedule \n");
    printf("7. Save Data\n");
    printf("8. Load Data\n");
    printf("9. Manage Facilities\n");    
    printf("10.Exit\n");
    printf("Choose an option: ");
}

//function to display manage rooms menu
void manageRoomsMenu(){
    printf("\n*** Room Menu ***\n");
    printf("1. Add Room\n");
    printf("2. View Rooms\n");
    printf("3. Update Room\n");
    printf("4. Delete Room\n");
    printf("5. Back\n");
    printf("enter your choice: ");
}

//function to display manage geust menu
void manageGuestMenu(){
    printf("\n*** guest Menu ***\n");
    printf("1. Add guest\n");
    printf("2. View guests\n");
    printf("3. Update guest\n");
    printf("4. Delete guest\n");
    printf("5. Back\n");
    printf("enter your choice: ");
}

//function to display boking menu
void bookingMenu(){
    printf("\n*** Booking Menu ***\n");
    printf("1. Book a Room \n");
    printf("2. Cancel a Booking \n");
    printf("3. Check-in Guest \n");
    printf("4. Check-out Guest \n");
    printf("5. Check-out history \n");
    printf("6. Undo last Check-out \n");
    printf("7. Back\n");
    printf("enter your choice: ");
}

////function to display search menu
void  SearchRoomGuest(){
    printf("\n*** Search Room / Guest ***\n");
    printf("1. Search Room by Number \n");
    printf("2. Search Guest by Name\n");
    printf("3. Back\n");
    printf("enter your choice: ");
}

//function to book a ROOM
void bookroom(){
    //Check room availability
    if (room_count == 0) {
        printf("\nNo rooms!\n");
        return;
    }
    
    // Option to view rooms first
    char viewFirst;
    printf("View all availabel rooms first? (y/n): ");
    scanf(" %c", &viewFirst);
    if (viewFirst == 'y' || viewFirst == 'Y') {
        viewavailableRooms();
    }
    int number;
    printf("\nEnter a room number to book: ");
    scanf("%d", &number);
    //Check the room number
    for (int i = 0; i < room_count; i++) { 
        if (rooms[i].number == number || rooms[i].status==AVAILABLE) {
            rooms[i].status=BOOKED;
            printf("enter guest id: ");
            scanf("%d",&rooms[i].geustid);
            printf("enter guest name: ");
            scanf(" %[^\n]",rooms[i].guestname);
            schedule[rooms[i].floor][rooms[i].index - 1] = BOOKED;
            printf("Room %d booked successfully!\n", number);
            return;
        }
    }
        printf("Room not found.\n");
}

// Function to display all hotel facilities
void viewFacilities() {
    printf("\n*** Hotel Facilities by Floor ***\n");
    for (int f = 0; f < FLOORS; f++) {
        printf("\nFloor %d:\n", f);
        Facility* current = floorFacilities[f].facilities;
        if (current == NULL) {
            printf("  No facilities on this floor.\n");
        }
        while (current != NULL) {
            printf("  Name: %s\n", current->name);
            printf("  Description: %s\n", current->description);
            printf("  ------------------------\n");
            current = current->next;
        }
    }
}

//function to cancel a ROOM booking
void cancelbooking(){
    //Check room availability
    if (room_count == 0) {
        printf("\nNo rooms!\n");
        return;
    }
    
    // Option to view rooms first
    char viewFirst;
    printf("View all booked rooms first? (y/n): ");
    scanf(" %c", &viewFirst);
    if (viewFirst == 'y' || viewFirst == 'Y') {
        viewbookedRooms();
    }
    int number;
    printf("\nEnter room number to cancel booking: ");
    scanf("%d", &number);
    for (int i = 0; i < room_count; i++) {
        if (rooms[i].number == number) {
            rooms[i].status=AVAILABLE;
            rooms[i].geustid=-1;
            strcpy(rooms[i].guestname, "empty");
            schedule[rooms[i].floor][rooms[i].index - 1] = AVAILABLE;
            printf("Room %d booking canceled successfully!\n", number);
            return;
        }
    }
        printf("Room not found.\n");
}


//function to check in a guest
void checkInGuest() {
    if (guest_count == 0) {
        printf("\nNo guests in the system!\n");
        return;
    }
    // Option to view rooms first
    char viewFirst;
    printf("View all booked rooms first? (y/n): ");
    scanf(" %c", &viewFirst);
    if (viewFirst == 'y' || viewFirst == 'Y') {
        viewbookedRooms();
    }
    int roomNumber;
    printf("Enter room number for check-in: ");
    scanf("%d", &roomNumber);

    // Find the room
    int roomFound = 0;
    for (int i = 0; i < room_count; i++) {
        if (rooms[i].number == roomNumber) {
            roomFound = 1;
            if (rooms[i].status != BOOKED) {
                printf("Room %d is not booked. Cannot check-in.\n", roomNumber);
                return;
            }

            // Match guest ID from room
            int guestId = rooms[i].geustid;
            for (int j = 0; j < guest_count; j++) {
                if (guests[j].id == guestId) {
                    guests[j].status = UNDER_TREATMENT;
                    printf("Guest '%s' has been checked in successfully to room %d.\n", guests[j].name, roomNumber);
                    return;
                }
            }

            // If guest was not found using ID
            printf("No matching guest found for room %d. Cannot check-in.\n", roomNumber);
            return;
        }
    }

    if (!roomFound) {
        printf("Room %d not found.\n", roomNumber);
    }
}

// function to check out aguest
void checkOutGuest() {
    if (guest_count == 0 || room_count == 0) {
        printf("\nNo rooms or guests in the system!\n");
        return;
    }
    // Option to view rooms first
    char viewFirst;
    printf("View all booked rooms first? (y/n): ");
    scanf(" %c", &viewFirst);
    if (viewFirst == 'y' || viewFirst == 'Y') {
        viewbookedRooms();
    }
    int roomNumber;
    printf("Enter room number for check-out: ");
    scanf("%d", &roomNumber);

    // Find the room
    for (int i = 0; i < room_count; i++) {
        if (rooms[i].number == roomNumber) {
            if (rooms[i].status != BOOKED) {
                printf("Room %d is not booked.\n", roomNumber);
                return;
            }

            // Find the guest using guest ID
            int guestId = rooms[i].geustid;
            for (int j = 0; j < guest_count; j++) {
                if (guests[j].id == guestId) {
                    if (guests[j].status != UNDER_TREATMENT) {
                        printf("Guest '%s' is not currently checked in.\n", guests[j].name);
                        return;
                    }
                    pushCheckoutHistory(guests[j].id, roomNumber, guests[j].name);
                    
                    guests[j].status = DISCHARGED;
                    rooms[i].status = AVAILABLE;
                    rooms[i].geustid = -1;
                    printf("Guest '%s' has been checked out and room %d is now available.\n", guests[j].name, roomNumber);
                    return;
                }
            }

            printf("Guest not found for room %d.\n", roomNumber);
            return;
        }
    }

    printf("Room %d not found.\n", roomNumber);
}

//history of check out
void displayCheckoutHistory() {
    //Check history availability
        if (historyTop < 0) {
        printf("No check-out history available.\n");
        return;
    }

    printf("\n*** Checkout History ***\n");
    for (int i = historyTop; i >= 0; i--) {
        printf("Guest ID: %d\n", checkoutHistory[i].guestId);
        printf("Guest Name: %s\n", checkoutHistory[i].guestName);
        printf("Room Number: %d\n", checkoutHistory[i].roomNumber);
        printf("-----------------------------\n");
    }
}


//display the schedule
void viewSchedule() {
    printf("\n*** Booking Schedule ***\n");
    for (int i = 0; i < FLOORS; i++) {
    printf("Floor %d: ", i );
    for (int j = 0; j < ROOMS_PER_FLOOR; j++) {
        switch (schedule[i][j]) {
            case AVAILABLE:
                printf("[A] ");
                break;                
            case BOOKED:
                printf("[B] ");
                break;
            case UNDER_MAINTENANCE:
                printf("[M] ");
                break;               
                 default:
                printf(" "); //empty space
            }
        }
        printf("\n");
    }
}

//Waiting Queue Management
void enqueueWaitingGuest(int guestId, const char* name, RoomType type) {
    if (waitingCount >= MAX_WAITING) {
        printf("Waiting queue is full!\n");
        return;
    }
    rear = (rear + 1) % MAX_WAITING;
    waitingQueue[rear].guestId = guestId;
    waitingQueue[rear].preferredType = type;
    strcpy(waitingQueue[rear].name, name);
    waitingCount++;
    printf("Guest '%s' added to the waiting list for %s room.\n", name, roomTypeToString(type));
}

//Waiting Queue Management
void dequeueWaitingGuest() {
    if (waitingCount == 0) {
        printf("Waiting queue is empty.\n");
        return;
    }

    WaitingGuest wg = waitingQueue[front];
    front = (front + 1) % MAX_WAITING;
    waitingCount--;

    // Try to assign an available room of preferred type
    for (int i = 0; i < room_count; i++) {
        if (rooms[i].status == AVAILABLE && rooms[i].type == wg.preferredType) {
            rooms[i].status = BOOKED;
            rooms[i].geustid = wg.guestId;
            strcpy(rooms[i].guestname, wg.name);
            schedule[rooms[i].floor][rooms[i].index - 1] = BOOKED;

            // Update guest if exists
            for (int j = 0; j < guest_count; j++) {
                if (guests[j].id == wg.guestId) {
                    guests[j].room_number = rooms[i].number;
                    guests[j].status = UNDER_TREATMENT;
                    break;
                }
            }

            printf("Assigned Room %d to waiting guest '%s'.\n", rooms[i].number, wg.name);
            return;
        }
    }

    printf("No available %s room for guest '%s'. Re-enqueued.\n", roomTypeToString(wg.preferredType), wg.name);
    enqueueWaitingGuest(wg.guestId, wg.name, wg.preferredType); // re-enqueue if not fulfilled
}


//Waiting Queue Management
void viewWaitingQueue() {
    if (waitingCount == 0) {
        printf("Waiting queue is empty.\n");
        return;
    }

    printf("\n=== Waiting Queue ===\n");
    for (int i = 0; i < waitingCount; i++) {
        int idx = (front + i) % MAX_WAITING;
        printf("Guest ID: %d | Name: %s | Preferred Room: %s\n",
               waitingQueue[idx].guestId,
               waitingQueue[idx].name,
               roomTypeToString(waitingQueue[idx].preferredType));
    }
}


void pushCheckoutHistory(int guestId, int roomNumber, const char* guestName) {
    if (historyTop >= MAX_HISTORY - 1) {
        printf("Checkout history stack is full!\n");
        return;
    }
    historyTop++;
    checkoutHistory[historyTop].guestId = guestId;
    checkoutHistory[historyTop].roomNumber = roomNumber;
    strcpy(checkoutHistory[historyTop].guestName, guestName);
}

void undoLastCheckout() {
    if (historyTop < 0) {
        printf("No checkout history to undo!\n");
        return;
    }

    CheckoutRecord last = checkoutHistory[historyTop];
    historyTop--;

    // Restore room status
    for (int i = 0; i < room_count; i++) {
        if (rooms[i].number == last.roomNumber) {
            rooms[i].status = BOOKED;
            rooms[i].geustid = last.guestId;
            strcpy(rooms[i].guestname, last.guestName);
            schedule[rooms[i].floor][rooms[i].index - 1] = BOOKED;
            break;
        }
    }

    // Restore guest status
    for (int j = 0; j < guest_count; j++) {
        if (guests[j].id == last.guestId) {
            guests[j].status = UNDER_TREATMENT;
            guests[j].room_number = last.roomNumber;
            break;
        }
    }

    printf("Undo successful: Guest '%s' has been restored to room %d.\n", last.guestName, last.roomNumber);
}


//function to add room in the hotel
void addroom() {
    if (room_count >= MAX_ROOMS) {
     printf("Room list is full!\n");
        return;
    }
 // auto number
    rooms[room_count].number = room_count + 1;


     int   type;
    printf("Enter room type {1-SINGLE, 2-DOUBLE, 3-SUITE, 4-DELUXE}:  ");
     scanf("%d", &type);

     if (type < 1||type> 4) {
        printf("Invalid room type\n");
        return;
    }

 rooms[room_count].type = (RoomType)type;

  int status;
     printf("Enter room status {1-AVAILABLE, 2-BOOKED, 3-UNDER_MAINTENANCE}: ");
     scanf("%d", &status);

    if (status>= 1 &&status <= 3) {
        rooms[room_count].status = (RoomStatus)status;

// default guest info
        if (status == AVAILABLE || status == UNDER_MAINTENANCE) {
            rooms[room_count].geustid = -1;
            strcpy(rooms[room_count].guestname, "empty");
        }
        else if   (status  == BOOKED) {
            printf("Enter guest ID: ");
            scanf("%d", &rooms[room_count].geustid);

// this loop checks if guest exists
            int valid = 0;
            for (int i = 0; i < guest_count; i++) {
                if (guests[i].id == rooms[room_count].geustid) {
                    strcpy(rooms[room_count].guestname, guests[i].name);
                valid = 1;
                    break;
                }
            }


            if (!valid) {
                printf("Guest ID not found. Room not added.\n");
                return;
        
            }
        }


    } else {
        printf("Invalid room status.\n");
        return;
    }


// floor and index
    printf("Enter floor number (0 to %d): ", FLOORS - 1);
    scanf("%d", &rooms[room_count].floor);


    if (rooms[room_count].floor < 0 || rooms[room_count].floor >= FLOORS) {
        printf("Invalid floor.\n");
        return;
    }

    printf("Enter room index (1 to %d): ", ROOMS_PER_FLOOR);
    scanf("%d", &rooms[room_count].index);

    if (rooms[room_count].index <= 0 || rooms[room_count].index > ROOMS_PER_FLOOR) {
        printf("Invalid index.\n");
        return;
    }

// make sure no duplicate room
    for (int i = 0; i < room_count; i++) {
        if (rooms[i].floor == rooms[room_count].floor &&
        rooms[i].index == rooms[room_count].index) {
        printf("Room already exists at this location.\n");
            return;
        }

    }
// add to schedule
   schedule[rooms[room_count].floor][rooms[room_count].index - 1] = rooms[room_count].status;
    printf("Room %d added successfully!\n", rooms[room_count].number);
    room_count++;
}

//function to add a geust
void addguest() {
    if (guest_count >= MAX_GUESTS) {
        printf("Guest list is full!\n");
        return;
    }
    guests[guest_count].id = guest_count + 1;
    printf("Enter guest full name: ");
    getchar();
    fgets(guests[guest_count].name, sizeof(guests[guest_count].name), stdin);
    guests[guest_count].name[strcspn(guests[guest_count].name, "\n")] = '\0';
    
    int status;
    printf("Enter guest status {1-WAITING, 2-UNDER_TREATMENT, 3-DISCHARGED}: ");
    scanf("%d", &status);
    if (status < 1 || status > 3) {
        printf("Invalid guest status. Guest not added.\n");
        return;
    }
    guests[guest_count].status = (GuestStatus)status;

    switch (status)
    {
    case WAITING:
        guests[guest_count].room_number=-1;
        int type;
        printf("Enter preferred room type {1-SINGLE, 2-DOUBLE, 3-SUITE, 4-DELUXE}: ");
        scanf("%d", &type);
        if (type < 1 || type > 4) { //check
        printf("Invalid room type. Guest not added to waiting queue.\n");
        } else {
        enqueueWaitingGuest(guests[guest_count].id, guests[guest_count].name, (RoomType)type);
        }
        break;
    case UNDER_TREATMENT:
    printf("Enter guest room number: ");
    scanf("%d", &guests[guest_count].room_number);
    int validRoom = 0;
    for (int i = 0; i < room_count; i++) {
        if (rooms[i].number == guests[guest_count].room_number && rooms[i].status == BOOKED) {
            validRoom = 1;
            break;
        }
    }

    if (!validRoom) {
        printf("Invalid or unavailable room number. Guest not added.\n");
        return;
    }
    break;
    default:
        guests[guest_count].room_number=-1;
        break;
    }
    
    printf("Guest added successfully!\n");
    guest_count++;
}


//display all rooms
void viewrooms() {
    if (room_count == 0) {
        printf("\n There are no rooms!\n");
        return;
    }
    printf("\n*** Room List by Floor (Sorted by Index) ***\n"); 
    for (int f = 0; f < FLOORS; f++) { //sorting
        printf("\nFloor %d:\n", f);
        room floorRooms[ROOMS_PER_FLOOR];
        int count = 0;
        for (int i = 0; i < room_count; i++) {
            if (rooms[i].floor == f) {
                floorRooms[count++] = rooms[i];
            }
        }
        for (int i = 0; i < count - 1; i++) {
            for (int j = 0; j < count - i - 1; j++) {
                if (floorRooms[j].index > floorRooms[j + 1].index) {
                    room temp = floorRooms[j];
                    floorRooms[j] = floorRooms[j + 1];
                    floorRooms[j + 1] = temp;
                }
            }
        }
        if (count == 0) {
            printf("No rooms found on this floor.\n");
        } else {
            for (int i = 0; i < count; i++) {
                printf("\tRoom Number: %d\n", floorRooms[i].number);
                printf("\t\tType: %s\n", roomTypeToString(floorRooms[i].type));
                printf("\t\tStatus: %s\n", roomstatus(floorRooms[i].status));
                printf("\t\tIndex in Floor: %d\n", floorRooms[i].index);
                printf("-----------------------------\n");
            }
        }
    }
}

//function to update a room
void updateRoom() {
    if (room_count == 0) {
        printf("\nNo rooms to update!\n");
        return;
    }
    // Option to view rooms first
    char viewFirst;
    printf("View all rooms first? (y/n): ");
    scanf(" %c", &viewFirst);
    if (viewFirst == 'y' || viewFirst == 'Y') {
        viewrooms();
    }
    int number;
    printf("\nEnter room number to update: ");
    scanf("%d", &number);
    for (int i = 0; i < room_count; i++) {
        if (rooms[i].number == number) {
            int type;
            printf("Current type: %s\n",roomTypeToString(rooms[i].type));
            printf("Enter room type {1-SINGLE, 2-DOUBLE, 3-SUITE, 4-DELUXE}: ");
            scanf("%d", &type);
            if (type < 1 || type > 4) { //check
                printf("Invalid room type. Room not added.\n");
                return;
            }
            rooms[i].type = (RoomType)type;            
            printf("Current status: %s\n", roomstatus(rooms[i].status));
            int status;
            printf("Enter new status {1-AVAILABLE, 2-BOOKED, 3-UNDER_MAINTENANCE}: ");
            scanf("%d", &status);
            if (status < 1 || status > 3) {
                printf("Invalid status. Update cancelled.\n");
                return;
            }
            int floor = rooms[i].floor;
            int index = rooms[i].index;
            rooms[i].status = (RoomStatus)status;
            schedule[floor][index-1] = rooms[i].status;
            printf("Room %d updated successfully!\n", number);
            return;
        }
    }
        printf("Room not found.\n");
}

//delete ROOM
void deleteRoom(){
    if (room_count == 0) {
        printf("\nNo rooms to delete!\n");
        return;
    }
    // Option to view rooms first
    char viewFirst;
    printf("View all rooms first? (y/n): ");
    scanf(" %c", &viewFirst);
    if (viewFirst == 'y' || viewFirst == 'Y') {
        viewrooms();
    }
    int number;
    printf("\nEnter room number to delete: ");
    scanf("%d", &number);
    for (int i = 0; i < room_count; i++) {
        //ceck
        if (rooms[i].number == number){
            int floor = rooms[i].floor;
            int index = rooms[i].index;
            if (index > 0 && index <= ROOMS_PER_FLOOR) {
                schedule[floor][index - 1] = 0;
            } else {
                printf("Invalid room index. Cannot modify schedule.\n");
            }            
            int j ;
            for (int j = i; j < room_count - 1; j++){
                rooms[j] = rooms[j + 1];
            }
            room_count--;
            printf("Room %d deleted successfully!\n", number);
            return;
        }
    }
        printf("Room not found.\n");
}

//function to update a geust
void updateGuest() {
    if (guest_count == 0) {
        printf("\nNo guests to update!\n");
        return;
    }
    // Option to view guests first
    char viewFirst;
    printf("View all guests first? (y/n): ");
    scanf(" %c", &viewFirst);
    if (viewFirst == 'y' || viewFirst == 'Y') {
        viewguests();
    }
    int id;
    printf("\nEnter guest ID to update: ");
    scanf("%d", &id);
    for (int i = 0; i < guest_count; i++) {
        if (guests[i].id == id) { //check
            printf("Current name: %s\n", guests[i].name);
            getchar();
            printf("Enter new name: ");
            fgets(guests[i].name, sizeof(guests[i].name), stdin);
            guests[i].name[strcspn(guests[i].name, "\n")] = '\0';

            printf("Current room number: %d\n", guests[i].room_number);
            printf("Enter new room number: ");
            scanf("%d", &guests[i].room_number);

            int status;
            printf("Current status: %s\n", gueststatus(guests[i].status));
            printf("Enter new status {1-WAITING, 2-UNDER_TREATMENT, 3-DISCHARGED}: ");
            scanf("%d", &status);
            if (status < 1 || status > 3) {
                printf("Invalid status. Update cancelled.\n");
                return;
            }
            guests[i].status = (GuestStatus)status;
            printf("Guest ID %d updated successfully!\n", id);
            return;
        }
    }
    printf("Guest not found.\n");
}

//delete geust
void deleteGuest() {
    if (guest_count == 0) { //Check guest availability
        printf("\nNo guests to delete!\n");
        return;
    }
    // Option to view guests first
    char viewFirst;
    printf("View all guests first? (y/n): ");
    scanf(" %c", &viewFirst);
    if (viewFirst == 'y' || viewFirst == 'Y') {
        viewguests();
    }
    int id;
    printf("\nEnter guest ID to delete: ");
    scanf("%d", &id);
    for (int i = 0; i < guest_count; i++) {
        if (guests[i].id == id) {
            for (int j = i; j < guest_count - 1; j++) {
                guests[j] = guests[j + 1];
            }
            guest_count--;
            printf("Guest ID %d deleted successfully!\n", id);
            return;
        }
    }
    printf("Guest not found.\n");
}

//display all guests 
void viewguests() {
    if (guest_count == 0) {//Check geust availability
        printf("\nThere are no guests!\n");
        return;
    }

    printf("\n--- Guest List ---\n");
    for (int i = 0; i < guest_count; i++) {
        printf("Guest ID: %d\n", guests[i].id);
        printf("Name: %s\n", guests[i].name);
        printf("Room Number: %d\n", guests[i].room_number);
        printf("Status: %s\n\n", gueststatus(guests[i].status));
    }
}

//search geust using name
void searchGuestByName() {
    if (guest_count == 0) {
        printf("There are no guests!\n");
        return;
    }

    char name[50];
    printf("Enter guest name to search: ");
    getchar();
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';

    bool found = false;
    for (int i = 0; i < guest_count; i++) { //check
        if (strcasecmp(guests[i].name, name) == 0) {
            printf("Guest ID: %d\n", guests[i].id);
            printf("Room Number: %d\n", guests[i].room_number);
            printf("Status: %s\n", gueststatus(guests[i].status));
            found = true;
            break;
        }
    }

    if (!found) {
        printf("Guest not found.\n");
    }
}

//search guest using ID
void searchGuestByID() {
    if (guest_count == 0) {
        printf("There are no guests!\n");
        return;
    }

    int id;
    printf("Enter guest ID to search: ");
    scanf("%d", &id);

    bool found = false;
    for (int i = 0; i < guest_count; i++) {
        if (guests[i].id == id) {
            printf("Guest Found:\n");
            printf("Name: %s | Room number: %d | Status   : %s | ID: %d\n",guests[i].name, guests[i].room_number, gueststatus(guests[i].status), guests[i].id);
            found = true;
            break;
        }
    }

    if (!found) {
        printf("Guest not found.\n");
    }
}

//search Room using its Number
void searchRoomByNumber() {
    if (room_count == 0) {
        printf("There are no rooms!\n");
        return;
    }

    int number;
    printf("Enter room number to search: ");
    scanf("%d", &number);

    bool found = false;
    for (int i = 0; i < room_count; i++) {
        if (rooms[i].number == number) {
            printf("Room Number: %d\n", rooms[i].number);
            printf("Type: %s\n", roomTypeToString(rooms[i].type));
            printf("Status: %s\n", roomstatus(rooms[i].status));
            printf("Floor: %d, Index: %d\n", rooms[i].floor, rooms[i].index);
            found = true;
            break;
        }
    }

    if (!found) {
        printf("Room not found.\n");
    }
}

//by type
void searchRoomByType() {
    if (room_count == 0) {
        printf("There are no rooms!\n");
        return;
    }

    int type;
    printf("Enter room type to search {1-SINGLE, 2-DOUBLE, 3-SUITE, 4-DELUXE}: ");
    scanf("%d", &type);

    int found = 0;
    for (int i = 0; i < room_count; i++) {
        if (rooms[i].type == type) {
            printf("Room Number: %d | Floor: %d | Index: %d | Status: %s\n",
                   rooms[i].number, rooms[i].floor, rooms[i].index, roomstatus(rooms[i].status));
            found = 1;
        }
    }

    if (!found) {
        printf("No rooms found with this type.\n");
    }
}

//by availability
void searchRoomByAvailability() {
    if (room_count == 0) {
        printf("There are no rooms!\n");
        return;
    }

    int status;
    printf("Enter status to search {1-AVAILABLE, 2-BOOKED, 3-UNDER_MAINTENANCE}: ");
    scanf("%d", &status);

    int found = 0;
    for (int i = 0; i < room_count; i++) {
        if (rooms[i].status == status) {
            printf("Room Number: %d | Type: %s | Floor: %d | Index: %d\n",
                   rooms[i].number, roomTypeToString(rooms[i].type), rooms[i].floor, rooms[i].index);
            found = 1;
        }
    }

    if (!found) {
        printf("No rooms found with this status.\n");
    }
}


// displa the search menu
 void searchMenu() {
    int choiceType, choiceMethod;

    printf("\n*** Search Menu ***\n");
    printf("1. Search Guest\n");
    printf("2. Search Room\n");
    printf("3. Back\n");
    printf("Choose an option: ");
    scanf("%d", &choiceType);
    printf("\033[2J\033[H");

      
    switch (choiceType) {
        case 1:  // Guest
        printf("\n*** Search Guest ***\n");
        printf("1. By Name\n");
        printf("2. By ID\n");
        printf("Choose a method: ");
        scanf("%d", &choiceMethod);
        printf("\033[2J\033[H");
        switch (choiceMethod) {
            case 1:
                searchGuestByName();
                break;
            case 2:
                searchGuestByID();
                break;
                default:
                printf("Invalid choice for guest search.\n");
            }
            break;
        
    
        case 2:  // Room
            printf("\n*** Search Room ***\n");
            printf("1. By Room Number\n");
            printf("2. By Availability (optional)\n");
            printf("3. By Type (optional)\n");
            printf("Choose a method: ");
            scanf("%d", &choiceMethod);
            printf("\033[2J\033[H");

            switch (choiceMethod){
                case 1:
                searchRoomByNumber();
                break;
                case 2:
                searchRoomByAvailability();
                break;
                case 3:
                searchRoomByType();                    
                break;
                default:
                printf("Invalid choice for room search.\n");
            break;
            }
        case 3:
            printf("Back to Main Menu...\n");
            break;

        default:
            printf("Invalid choice. Try again.\n");
 }}


 //stor the data
void saveData() {
    // Open file for writing in binary mode
    FILE *file = fopen("hotel_data.dat", "wb");
    if (file == NULL) {
        printf(" Error: Could not open file for saving.\n");
        return;
    }

      // Save numbeer of rooms and room array
    fwrite(&room_count, sizeof(int), 1, file);
     fwrite(rooms, sizeof(room), room_count, file);

    // Save number of guests and guest array
     fwrite(&guest_count, sizeof(int), 1, file);
    fwrite(guests, sizeof(guest), guest_count, file);

    // Save waiting queue details
    fwrite(&front, sizeof(int), 1, file);
    fwrite(&rear, sizeof(int), 1, file);
    fwrite(&waitingCount, sizeof(int), 1, file);
    fwrite(waitingQueue, sizeof(WaitingGuest), waitingCount, file);

    // Save checkout history stack
    fwrite(&historyTop, sizeof(int), 1, file);
    fwrite(checkoutHistory, sizeof(CheckoutRecord), historyTop + 1, file);

    // Save room occupancy schedule (2D array)
    fwrite(schedule, sizeof(RoomStatus), FLOORS * ROOMS_PER_FLOOR, file);

    // Close the file
    fclose(file);
    printf(" Data saved successfully to 'hotel_data.dat'.\n");
    // Save facilities for each floor
for (int f = 0; f < FLOORS; f++) {
    // Count how many facilities in this floor
    int count = 0;
    Facility* current = floorFacilities[f].facilities;
    while (current != NULL) {
        count++;
        current = current->next;
    }

    // Save count
    fwrite(&count, sizeof(int), 1, file);

    // Save each facility
    current = floorFacilities[f].facilities;
    while (current != NULL) {
        fwrite(current, sizeof(Facility), 1, file);
        current = current->next;
    }
}

}


//load data
void loadData() {
    // Open file for reading in binary mode
    FILE *file = fopen("hotel_data.dat", "rb");
    if (file == NULL) {
         printf("No saved file found. Starting with empty data.\n");
        return;
    }

    // Load number of rooms and room array
    fread(&room_count, sizeof(int), 1, file);
     fread(rooms, sizeof(room), room_count, file);

    // Load number of guests and guest array
    fread(&guest_count, sizeof(int), 1, file);
   fread(guests, sizeof(guest), guest_count, file);

    // Load waiting queue data
    fread(&front, sizeof(int), 1, file);
    fread(&rear, sizeof(int), 1, file);
    fread(&waitingCount, sizeof(int), 1, file);
     fread(waitingQueue, sizeof(WaitingGuest), waitingCount, file);

    // Load checkout history stack
    fread(&historyTop, sizeof (int), 1, file);
    fread(checkoutHistory, sizeof(CheckoutRecord), historyTop + 1, file);

    // Load room occupancy schedule
    fread(schedule, sizeof(RoomStatus), FLOORS * ROOMS_PER_FLOOR, file);

    // Close the file
    fclose(file);
    printf(" Data loaded successfully from 'hotel_data.dat'.\n");
    // Load facilities for each floor
for (int f = 0; f < FLOORS; f++) {
    int count = 0;
    fread(&count, sizeof(int), 1, file);
    floorFacilities[f].facilities = NULL;

    for (int i = 0; i < count; i++) {
        Facility* newFacility = (Facility*)malloc(sizeof(Facility));
        fread(newFacility, sizeof(Facility), 1, file);
        newFacility->next = floorFacilities[f].facilities;
        floorFacilities[f].facilities = newFacility;
    }
}

}

// Function to add a new facility to the linked list
void addFacility(const char* name, const char* description) {
    Facility* newFacility = (Facility*)malloc(sizeof(Facility));
    if (newFacility == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    strcpy(newFacility->name, name);
    strcpy(newFacility->description, description);
    newFacility->next = floorFacilities[FLOORS].facilities;
    floorFacilities[FLOORS].facilities = newFacility;    

    printf("Facility '%s' added successfully.\n", name);
}


void addFacilityToFloor(int floor, const char* name, const char* description) {
    if (floor < 0 || floor >= FLOORS) {
        printf("Invalid floor number!\n");
        return;
    }

    Facility* newFacility = (Facility*)malloc(sizeof(Facility));
    if (newFacility == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    strcpy(newFacility->name, name);
    strcpy(newFacility->description, description);
    newFacility->next = floorFacilities[floor].facilities;
    floorFacilities[floor].facilities = newFacility;
    printf("Facility '%s' added successfully to floor %d.\n", name, floor);
}


void deleteFacilityFromFloor(int floor, const char* name) {
    if (floor < 0 || floor >= FLOORS) {
        printf("Invalid floor number!\n");
        return;
    }

    Facility* current = floorFacilities[floor].facilities;
    Facility* prev = NULL;

    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            if (prev == NULL) {
                floorFacilities[floor].facilities = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            printf("Facility '%s' deleted from floor %d.\n", name, floor);
            return;
        }
        prev = current;
        current = current->next;
    }

    printf("Facility '%s' not found on floor %d.\n", name, floor);
}


void updateFacilityDescription(int floor, const char* name, const char* newDescription) {
    if (floor < 0 || floor >= FLOORS) {
        printf("Invalid floor number!\n");
        return;
    }

    Facility* current = floorFacilities[floor].facilities;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            strcpy(current->description, newDescription);
            printf("Description for '%s' on floor %d updated.\n", name, floor);
            return;
        }
        current = current->next;
    }

    printf("Facility '%s' not found on floor %d.\n", name, floor);
}

// Menu to manage hotel facilities
void facilityMenu() {
    char *options[] = {
        "Add Facility",
        "View Facilities",
        "Delete Facility",
        "Update Facility",
        "Back to Main Menu"
    };
    int selected = 0;
    int choice;
    char name[50], description[100];

    while (1) {
        printf("\033[2J\033[H"); // Clear screen
        printf("=== Facility Management ===\n");

        for (int i = 0; i < 5; i++) {
            if (i == selected)
                printf(" 👉 \033[1;32m%s\033[0m\n", options[i]);
            else
                printf("    %s\n", options[i]);
        }

        int key = getch();
        if (key == 0 || key == 224) key = getch(); // For arrow keys

        switch (key) {
            case UP:
                selected = (selected - 1 + 5) % 5;
                break;
            case DOWN:
                selected = (selected + 1) % 5;
                break;
            case ENTER:
                printf("\033[2J\033[H");
                switch (selected) {
                    case 0: {
                        int floor;
                        printf("Enter floor number (0-%d): ", FLOORS - 1);
                        scanf("%d", &floor); getchar();
                        if (floor < 0 || floor >= FLOORS) {
                            printf("Invalid floor number!\n");
                            break;
                        }
                        printf("Enter facility name: ");
                        fgets(name, sizeof(name), stdin);
                        name[strcspn(name, "\n")] = '\0';

                        printf("Enter description: ");
                        fgets(description, sizeof(description), stdin);
                        description[strcspn(description, "\n")] = '\0';

                        addFacilityToFloor(floor, name, description);
                        break;
                    }
                    case 1:
                        viewFacilities();
                        break;
                    case 2: {
                        int floor;
                        printf("Enter floor number (0-%d): ", FLOORS - 1);
                        scanf("%d", &floor); getchar();

                        printf("Enter facility name to delete: ");
                        fgets(name, sizeof(name), stdin);
                        name[strcspn(name, "\n")] = '\0';

                        deleteFacilityFromFloor(floor, name);
                        break;
                    }
                    case 3: {
                        int floor;
                        printf("Enter floor number (0-%d): ", FLOORS - 1);
                        scanf("%d", &floor); getchar();

                        printf("Enter facility name to update: ");
                        fgets(name, sizeof(name), stdin);
                        name[strcspn(name, "\n")] = '\0';

                        printf("Enter new description: ");
                        fgets(description, sizeof(description), stdin);
                        description[strcspn(description, "\n")] = '\0';

                        updateFacilityDescription(floor, name, description);
                        break;
                    }
                    case 4:
                        return; // Exit the menu
                }
                printf("Press any key to return to menu...\n");
                getch();
                break;
        }
    }
}




                //THE MAIN
               //PROOOOOOGRAM :)

int main() {

    initFloorFacilities();
    // declaration of data
    int choice ,choiceRM , choiceGM , choiceSG,choiceBM ;
    addFacilityToFloor(0,"Restaurant", "Serves meals all day"); // add some facility
    addFacilityToFloor(0,"Spa", "Relaxation and wellness treatments");
    addFacilityToFloor(0,"WC", "");
    addFacilityToFloor(2,"Gym", "Fitness equipment and 24/7 access");
                printf("\033[2J\033[H"); //to clear the terminal
    
//loop the mai menu
do{

mainMenu();  

while(scanf("%d", &choice)!=1){

    printf ("Invalid input! Please enter a valid number.\n");
    while (getchar() != '\n'); // check if the input is a number
} 

printf("\033[2J\033[H"); //to clear the terminal

  switch (choice) 
    {
        case 1:
        do {
            manageRoomsMenu();
            scanf("%d", &choiceRM);
            printf("\033[2J\033[H"); //to clear the terminal
            switch (choiceRM) {

            case 1:
                addroom();
                
            break;

            case 2:
                viewrooms();

            break;

            case 3:
                updateRoom();

            break;

            case 4:
                deleteRoom();

            break;

            case 5:
                printf("Back to Main Menu...\n");

            break;

            default:
                 printf("Invalid choice! Try again.\n");
            }
        } while (choiceRM != 5);

    break;    
    case 2:
    do {
      manageGuestMenu();
      scanf("%d", &choiceGM);
      printf("\033[2J\033[H");
      switch (choiceGM) {
       case 1:
       addguest();
       break;
       case 2:
       viewguests();
       break;
       case 3:
       updateGuest();
       break;
       case 4:
       deleteGuest();
       break;
       default:
       printf("Invalid choice! Try again.\n");
       break;
      }
} while (choiceGM != 5);
    break;
    case 3:
    do
      {
      bookingMenu();
      scanf("%d",&choiceBM );
      printf("\033[2J\033[H");
      switch (choiceBM)
      {
      case 1:
        bookroom();
        break;
      case 2:
        cancelbooking();
        break;
      case 3:
        checkInGuest();
        break;
      case 4:
        checkOutGuest();
        break;
      case 5:
          displayCheckoutHistory();
        break;
      case 6:
        undoLastCheckout();
        break;
    
      default:
      printf("Invalid choice! Try again.\n");
        break;
      }
      } while (choiceBM!=7);
      
     break;
    case 4:
    searchMenu();
     break;
    case 5:
       displayHotelStructureTree();
     break;
    case 6:
      viewSchedule();
      printf("press 0 to back ");
      int a ;
      do {
        scanf("%d",&a);
        if (a!=0){printf("Please enter a valid number ");}   

      }while(a!=0);
    printf("\033[2J\033[H");
     break;
     case 7:
       saveData();
      break;
     case 8:
       loadData();
      break;
      case 9:
    facilityMenu();
    break;
    case 10:
    printf("Goodbye!\n");
   break;
    default:
    printf("Invalid choice! Try again.\n");
} 
} while (choice != 10); 

return 0;
}
// END :)