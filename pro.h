
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
    int roomNo;
    struct QueueNode* next;
} QueueNode;

// ---------- HEAP ----------
#define MAX_ROOMS 200
typedef struct {
    Room* arr[MAX_ROOMS];
    int size;
} MinHeap;

// ---------- FUNCTION DECLARATIONS ----------

// BST
Room* createRoom(int no, int cap);
Room* insertRoom(Room* root, int no, int cap);
void inorder(Room* root);
Room* searchRoom(Room* root, int no);
Room* deleteRoom(Room* root, int roomNo);

// Linked List (Bookings)
void addBooking(char name[], int roomNo, int start, int end);
void displayBookings();
int removeFromBookings(char *name);
void removeRoomRelatedBookings(int roomNo);

// Queue (Waiting List)
void enqueue(char name[], int roomNo);
void dequeue();
void showQueue();
int removeFromWaitingList(char *name);
void removeWaitingListForRoom(int roomNo);

// Booking Operations
void bookRoom(Room* root, int no, char user[], int start, int end);
void cancelBooking(Room* root, char *name);

// Conflict Detection
int hasConflict(int roomNo, int start, int end);

// Heap
void initHeap(MinHeap* heap);
void insertHeap(MinHeap* heap, Room* r);
Room* extractMin(MinHeap* heap);
Room* autoAllocate(MinHeap* heap, int neededCap);
Room* findRoomToWaitFor(MinHeap* heap, int neededCap);
void rebuildHeap(MinHeap* heap, Room* root);

// Helper
void clearInputBuffer();

// NEW: Auto-generate rooms 101–200
void autoGenerateRooms(Room **root, MinHeap *heap);

#endif
