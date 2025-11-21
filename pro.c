
// pro.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pro.h"

// ---------- HELPER ----------
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// ---------- BST ----------
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
    if (!root) return;
    inorder(root->left);
    printf("Room %d | Capacity: %d \n",
           root->roomNo, root->capacity);
    inorder(root->right);
}

Room* searchRoom(Room* root, int no) {
    if (!root || root->roomNo == no) return root;
    if (no < root->roomNo)
        return searchRoom(root->left, no);
    return searchRoom(root->right, no);
}

// ---------- DELETE ROOM ----------
Room* deleteRoom(Room* root, int roomNo) {
    if (!root) return NULL;

    if (roomNo < root->roomNo)
        root->left = deleteRoom(root->left, roomNo);
    else if (roomNo > root->roomNo)
        root->right = deleteRoom(root->right, roomNo);
    else {
        // Leaf
        if (!root->left && !root->right) {
            free(root);
            return NULL;
        }
        // One child
        else if (!root->left) {
            Room* t = root->right;
            free(root);
            return t;
        }
        else if (!root->right) {
            Room* t = root->left;
            free(root);
            return t;
        }
        // Two children → inorder successor
        else {
            Room* succ = root->right;
            while (succ->left)
                succ = succ->left;

            root->roomNo = succ->roomNo;
            root->capacity = succ->capacity;
            root->available = succ->available;

            root->right = deleteRoom(root->right, succ->roomNo);
        }
    }
    return root;
}

// ---------- BOOKINGS ----------
Booking* head = NULL;

void addBooking(char name[], int roomNo, int start, int end) {
    Booking* b = malloc(sizeof(Booking));
    strcpy(b->name, name);
    b->roomNo = roomNo;
    b->startTime = start;
    b->endTime = end;
    b->next = head;
    head = b;
}

void displayBookings() {
    if (!head) {
        printf("No bookings yet!\n");
        return;
    }
    Booking *t = head;
    printf("\nCurrent Bookings:\n");
    while (t) {
        printf("%s -> Room %d | Time %d->%d\n",
               t->name, t->roomNo, t->startTime, t->endTime);
        t = t->next;
    }
}

int removeFromBookings(char *name) {
    Booking *t = head, *p = NULL;
    while (t && strcmp(t->name, name) != 0) {
        p = t;
        t = t->next;
    }
    if (!t) return 0;

    int rn = t->roomNo;

    if (p) p->next = t->next;
    else head = t->next;

    free(t);
    return rn;
}

// Remove all bookings for a room (delete room)
void removeRoomRelatedBookings(int roomNo) {
    Booking *t = head, *p = NULL;
    while (t) {
        if (t->roomNo == roomNo) {
            Booking *del = t;
            if (p) p->next = t->next;
            else head = t->next;
            t = t->next;
            free(del);
        } else {
            p = t;
            t = t->next;
        }
    }
}

// ---------- WAITING LIST ----------
QueueNode *front = NULL, *rear = NULL;

void enqueue(char name[], int roomNo) {
    QueueNode *n = malloc(sizeof(QueueNode));
    strcpy(n->name, name);
    n->roomNo = roomNo;
    n->next = NULL;

    if (!rear)
        front = rear = n;
    else {
        rear->next = n;
        rear = n;
    }
}

void showQueue() {
    QueueNode *t = front;
    if (!t) {
        printf("No one in waiting list\n");
        return;
    }
    printf("\nWaiting List:\n");
    while (t) {
        printf("%s (Room %d) -> ", t->name, t->roomNo);
        t = t->next;
    }
    printf("NULL\n");
}

void removeWaitingListForRoom(int roomNo) {
    QueueNode *t = front, *p = NULL;

    while (t) {
        if (t->roomNo == roomNo) {
            QueueNode *del = t;
            if (p) p->next = t->next;
            else front = t->next;

            t = t->next;
            if (del == rear) rear = p;
            free(del);
        } else {
            p = t;
            t = t->next;
        }
    }
}

// ---------- CANCEL ----------
void cancelBooking(Room* root, char *name) {
    int r = removeFromBookings(name);
    if (r) {
        Room* rm = searchRoom(root, r);
        if (rm) rm->available = 1;
        printf("Booking cancelled.\n");
    }
    else if (removeFromWaitingList(name))
        printf("Removed from waiting list.\n");
    else
        printf("Name not found.\n");
}

// ---------- CONFLICT ----------
int hasConflict(int roomNo, int s, int e) {
    Booking* t = head;
    while (t) {
        if (t->roomNo == roomNo &&
           (s < t->endTime && e > t->startTime))
            return 1;
        t = t->next;
    }
    return 0;
}

// ---------- HEAP ----------
void initHeap(MinHeap* heap) { heap->size = 0; }

void insertHeap(MinHeap* heap, Room* r) {
    int i = heap->size++;
    while (i > 0 && r->capacity < heap->arr[(i - 1) / 2]->capacity) {
        heap->arr[i] = heap->arr[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    heap->arr[i] = r;
}

Room* autoAllocate(MinHeap* heap, int need) {
    for (int i = 0; i < heap->size; i++)
        if (heap->arr[i]->capacity >= need && heap->arr[i]->available)
            return heap->arr[i];
    return NULL;
}

Room* findRoomToWaitFor(MinHeap* heap, int need) {
    Room *best = NULL;
    for (int i = 0; i < heap->size; i++)
        if (heap->arr[i]->capacity >= need)
            if (!best || heap->arr[i]->capacity < best->capacity)
                best = heap->arr[i];
    return best;
}

void rebuildHeap(MinHeap* heap, Room* root) {
    heap->size = 0;
    if (!root) return;

    if (root) insertHeap(heap, root);
    if (root->left) rebuildHeap(heap, root->left);
    if (root->right) rebuildHeap(heap, root->right);
}

// ---------- BOOKING ----------
void bookRoom(Room* root, int no, char user[], int s, int e) {
    Room* r = searchRoom(root, no);
    if (!r) {
        printf("Room not found!\n");
        return;
    }

    if (!r->available) {
        printf("Room busy. Added to waiting list.\n");
        enqueue(user, no);
        return;
    }

    if (hasConflict(no, s, e)) {
        printf("Time conflict. Added to waiting list.\n");
        enqueue(user, no);
    } else {
        addBooking(user, no, s, e);
        printf("%s booked Room %d!\n", user, no);
    }
}

// ---------- AUTO GENERATE 101–200 ----------
void autoGenerateRooms(Room **root, MinHeap *heap) {
    for (int i = 101; i <= 200; i++) {
        int cap = 10 + (i % 40); // 10–49
        *root = insertRoom(*root, i, cap);
        insertHeap(heap, searchRoom(*root, i));
    }
    printf("\n100 Rooms Added Automatically (101->200)\n");
}

int removeFromWaitingList(char *name) {
    QueueNode *temp = front, *prev = NULL;

    while (temp && strcmp(temp->name, name) != 0) {
        prev = temp;
        temp = temp->next;
    }

    if (!temp) return 0;   // Not found

    if (prev)
        prev->next = temp->next;
    else
        front = temp->next;

    if (temp == rear)
        rear = prev;

    free(temp);
    return 1;   // Removed successfully
}
