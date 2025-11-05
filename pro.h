// pro.h
#ifndef BOOKING_H
#define BOOKING_H

// ---------- STRUCTURES ----------
typedef struct Room {
    int roomNo;
    int capacity;
    int available;
    struct Room *left, *right;
} Room;

typedef struct Booking {
    char name[50];
    int roomNo;
    int startTime, endTime; 
    struct Booking* next;
} Booking;

typedef struct QueueNode {
    char name[50];
    int roomNo; // Stores which room they are waiting for
    struct QueueNode* next;
} QueueNode;

// ---------- HEAP ----------
#define MAX_ROOMS 50
typedef struct {
    Room* arr[MAX_ROOMS];
    int size;
} MinHeap;

// ---------- FUNCTION DECLARATIONS ----------

// BST (Rooms)
Room* createRoom(int no, int cap);
Room* insertRoom(Room* root, int no, int cap);
void inorder(Room* root);
Room* searchRoom(Room* root, int no);

// Linked List (Bookings)
void addBooking(char name[], int roomNo, int start, int end);
void displayBookings();
int removeFromBookings(char *name); 

// Queue (Waiting List)
void enqueue(char name[], int roomNo); // Takes roomNo
void dequeue();
void showQueue();
int removeFromWaitingList(char *name);

// Booking Operations
void bookRoom(Room* root, int no, char user[], int start, int end);
void cancelBooking(Room* root, char *name);

// Conflict Detection
int hasConflict(int roomNo, int start, int end);

// Min Heap Functions
void initHeap(MinHeap* heap);
void insertHeap(MinHeap* heap, Room* r);
Room* extractMin(MinHeap* heap);
Room* autoAllocate(MinHeap* heap, int neededCap);
Room* findRoomToWaitFor(MinHeap* heap, int neededCap);

// Helper Functions
void clearInputBuffer(); 

#endif