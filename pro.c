// pro.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pro.h"

// ---------- HELPER FUNCTIONS ----------
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// ---------- BST FUNCTIONS ----------
Room* createRoom(int no, int cap) {
    Room* newRoom = (Room*)malloc(sizeof(Room));
    newRoom->roomNo = no;
    newRoom->capacity = cap;
    newRoom->available = 1;
    newRoom->left = newRoom->right = NULL;
    return newRoom;
}

Room* insertRoom(Room* root, int no, int cap) {
    if (root == NULL) return createRoom(no, cap);
    if (no < root->roomNo)
        root->left = insertRoom(root->left, no, cap);
    else if (no > root->roomNo)
        root->right = insertRoom(root->right, no, cap);
    return root;
}

void inorder(Room* root) {
    if (root == NULL) return;
    inorder(root->left);
    printf("Room %d | Capacity: %d | %s\n", root->roomNo, root->capacity,
           root->available ? "Available" : "Booked/Out of Service");
    inorder(root->right); 
}

Room* searchRoom(Room* root, int no) {
    if (root == NULL || root->roomNo == no) return root;
    if (no < root->roomNo)
        return searchRoom(root->left, no);
    return searchRoom(root->right, no);
}


// ---------- LINKED LIST (BOOKINGS) ----------
Booking* head = NULL;

void addBooking(char name[], int roomNo, int start, int end) {
    Booking* newB = (Booking*)malloc(sizeof(Booking));
    strcpy(newB->name, name);
    newB->roomNo = roomNo;
    newB->startTime = start;
    newB->endTime = end;
    newB->next = head;
    head = newB;
}

void displayBookings() {
    Booking* temp = head;
    if (!temp) {
        printf("No bookings yet!\n");
        return;
    }
    printf("\nCurrent Bookings:\n");
    while (temp) {
        printf("%s -> Room %d | Time: %d - %d\n", temp->name, temp->roomNo,
               temp->startTime, temp->endTime);
        temp = temp->next;
    }
}

// Returns roomNo if found, 0 otherwise
int removeFromBookings(char *name) {
    Booking *temp = head, *prev = NULL;
    while (temp && strcmp(temp->name, name) != 0) {
        prev = temp;
        temp = temp->next;
    }
    if (!temp) return 0; 

    int roomNo = temp->roomNo; 

    if (prev)
        prev->next = temp->next;
    else
        head = temp->next;
    
    free(temp);
    return roomNo; // Return the room number
}

// ---------- QUEUE (WAITING LIST) ----------
QueueNode *front = NULL, *rear = NULL;

void enqueue(char name[], int roomNo) {
    QueueNode* temp = (QueueNode*)malloc(sizeof(QueueNode));
    strcpy(temp->name, name);
    temp->roomNo = roomNo;
    temp->next = NULL;
    if (rear == NULL)
        front = rear = temp;
    else {
        rear->next = temp;
        rear = temp;
    }
}

void dequeue() {
    if (front == NULL) {
        printf("Waiting list empty\n");
        return;
    }
    QueueNode* temp = front;
    printf("Removed from waiting list: %s\n", front->name);
    front = front->next;
    if (front == NULL)
        rear = NULL;
    free(temp);
}

void showQueue() {
    QueueNode* temp = front;
    if (!temp) {
        printf("No one in waiting list\n");
        return;
    }
    printf("\nWaiting List:\n");
    while (temp) {
        printf("%s (Waiting for Room %d) -> ", temp->name, temp->roomNo);
        temp = temp->next;
    }
    printf("NULL\n");
}

int removeFromWaitingList(char *name) {
    QueueNode *temp = front, *prev = NULL;
    while (temp && strcmp(temp->name, name) != 0) {
        prev = temp;
        temp = temp->next;
    }
    if (!temp) return 0;
    if (prev)
        prev->next = temp->next;
    else
        front = temp->next;
    if (temp == rear)
        rear = prev;
    free(temp);
    return 1;
}

// ---------- CANCEL BOOKING (GENERAL) ----------
void cancelBooking(Room* root, char *name) {
    int roomToFree = removeFromBookings(name);

    if (roomToFree != 0) { 
        printf("Booking cancelled successfully.\n");
        
        Room* r = searchRoom(root, roomToFree);
        if (r) {
            // Note: This logic isn't perfect, as it just sets it
            // available even if other bookings exist.
            // But it's consistent with the project's design.
            r->available = 1; 
            printf("Room %d is now marked as 'Available'.\n", roomToFree);
        }
    } else if (removeFromWaitingList(name)) { 
        printf("Removed from waiting list.\n");
    
    } else { 
        printf("Booking not found!\n");
    }
}

// ---------- CONFLICT DETECTION ----------
int hasConflict(int roomNo, int start, int end) {
    Booking* temp = head;
    while (temp) {
        if (temp->roomNo == roomNo) {
            if ((start < temp->endTime) && (end > temp->startTime))
                return 1; // Overlap
        }
        temp = temp->next;
    }
    return 0; // No conflicts
}

// ---------- MIN HEAP ----------
void initHeap(MinHeap* heap) { heap->size = 0; }

void insertHeap(MinHeap* heap, Room* r) {
    if (heap->size >= MAX_ROOMS) return;
    int i = heap->size++;
    while (i > 0 && r->capacity < heap->arr[(i - 1) / 2]->capacity) {
        heap->arr[i] = heap->arr[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    heap->arr[i] = r;
}

Room* extractMin(MinHeap* heap) {
    if (heap->size == 0) return NULL;
    Room* min = heap->arr[0];
    Room* last = heap->arr[--heap->size];
    int i = 0, child;
    while (2 * i + 1 < heap->size) {
        child = 2 * i + 1;
        if (child + 1 < heap->size && heap->arr[child + 1]->capacity < heap->arr[child]->capacity)
            child++;
        if (last->capacity <= heap->arr[child]->capacity)
            break;
        heap->arr[i] = heap->arr[child];
        i = child;
    }
    heap->arr[i] = last;
    return min;
}

// Finds the first available room that fits
Room* autoAllocate(MinHeap* heap, int neededCap) {
    for (int i = 0; i < heap->size; i++) {
        if (heap->arr[i]->available && heap->arr[i]->capacity >= neededCap)
            return heap->arr[i];
    }
    return NULL;
}

// Finds the smallest room that fits, even if unavailable
Room* findRoomToWaitFor(MinHeap* heap, int neededCap) {
    Room* bestFit = NULL;
    for (int i = 0; i < heap->size; i++) {
        if (heap->arr[i]->capacity >= neededCap) {
            if (bestFit == NULL || heap->arr[i]->capacity < bestFit->capacity) {
                bestFit = heap->arr[i];
            }
        }
    }
    return bestFit; // Returns NULL if no room is large enough
}


// ---------- BOOKING LOGIC (FINAL) ----------
void bookRoom(Room* root, int no, char user[], int start, int end) {
    
    // 1. Find the room first
    Room* r = searchRoom(root, no);
    if (r == NULL) {
        printf("Room not found!\n");
        return;
    }

    // 2. Check if the room is "in service" (the 'available' flag)
    if (r->available == 0) {
        printf("Room %d is marked as fully booked/out of service! Adding %s to waiting list.\n", no, user);
        enqueue(user, no);
        return;
    }

    // 3. Check for a TIME conflict
    if (hasConflict(no, start, end)) {
        // If the time conflicts, add to waiting list
        printf("Time conflict detected for Room %d! Adding %s to waiting list.\n", no, user);
        enqueue(user, no); 
    } else {
        // No conflict AND the room is in service? Book it.
        // We DO NOT set r->available = 0
        addBooking(user, no, start, end);
        printf("%s successfully booked Room %d!\n", user, no);
    }
}