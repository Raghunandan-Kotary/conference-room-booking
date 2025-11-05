// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pro.h"

int main() {
    Room* root = NULL;
    MinHeap heap;
    initHeap(&heap);
    int choice, no, cap, start, end, need;
    char name[50];

    while (1) {
        printf("\n=== Conference Room Booking System ===\n");
        printf("1. Add Room\n");
        printf("2. Display Rooms\n");
        printf("3. Book Room (Manual)\n");
        printf("4. Auto Allocate Room\n");
        printf("5. Show Bookings\n");
        printf("6. Cancel Booking\n"); 
        printf("7. Show Waiting List\n"); 
        printf("8. Exit\n"); 
        printf("Enter your choice: ");

         
        while (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number: ");
            clearInputBuffer();
        }
        clearInputBuffer();  

        switch (choice) {
            case 1:
                printf("Enter Room No and Capacity (e.g., 101 50): ");
                 
                while (scanf("%d %d", &no, &cap) != 2) {
                    printf("Invalid input. Please enter two numbers: ");
                    clearInputBuffer();
                }
                clearInputBuffer();  

                root = insertRoom(root, no, cap);
                insertHeap(&heap, searchRoom(root, no));
                break;

            case 2:
                inorder(root);
                break;

            case 3:
                printf("Enter your name: ");
                scanf("%s", name); 
                clearInputBuffer(); 

                printf("Enter Room No: ");
                 
                while (scanf("%d", &no) != 1) {
                    printf("Invalid input. Please enter a number: ");
                    clearInputBuffer();
                }
                clearInputBuffer();  

                printf("Enter Start Time (24-hr format, e.g., 9 for 9am): ");
                 
                while (scanf("%d", &start) != 1) {
                    printf("Invalid input. Please enter a number: ");
                    clearInputBuffer();
                }
                clearInputBuffer(); 

                printf("Enter End Time (24-hr format, e.g., 17 for 5pm): ");
                 
                while (scanf("%d", &end) != 1) {
                    printf("Invalid input. Please enter a number: ");
                    clearInputBuffer();
                }
                clearInputBuffer();  

                bookRoom(root, no, name, start, end);
                break;

            case 4:
                printf("Enter your name: ");
                scanf("%s", name);
                clearInputBuffer(); 

                printf("Enter required capacity: ");
                 
                while (scanf("%d", &need) != 1) {
                    printf("Invalid input. Please enter a number: ");
                    clearInputBuffer();
                }
                clearInputBuffer();  

                {
                    Room* best = autoAllocate(&heap, need);
                    if (best) {
                        printf("Auto-assigned Room %d (Capacity %d)\n", best->roomNo, best->capacity);
                        printf("Enter Start Time (24-hr format, e.g., 9 for 9am): ");
                         
                        while (scanf("%d", &start) != 1) {
                            printf("Invalid input. Please enter a number: ");
                            clearInputBuffer();
                        }
                        clearInputBuffer();  

                        printf("Enter End Time (24-hr format, e.g., 17 for 5pm): ");
                         
                        while (scanf("%d", &end) != 1) {
                            printf("Invalid input. Please enter a number: ");
                            clearInputBuffer();
                        }
                        clearInputBuffer();  
                        
                        bookRoom(root, best->roomNo, name, start, end);
                    } else {
                        printf("No suitable room is currently *available*.\n");
                        printf("Checking for a room to add you to the waiting list...\n");
                        Room* roomToWait = findRoomToWaitFor(&heap, need);
                        
                        if (roomToWait) {
                            printf("Found Room %d (Capacity %d). Adding %s to its waiting list.\n", 
                                   roomToWait->roomNo, roomToWait->capacity, name);
                            enqueue(name, roomToWait->roomNo);
                        } else {
                            printf("Sorry, no room in the system is large enough for %d people.\n", need);
                        }
                    }
                }
                break;

            case 5:
                displayBookings();
                break;

            case 6:
                printf("Enter name to cancel booking: ");
                scanf("%s", name);
                clearInputBuffer();  
                cancelBooking(root, name);
                break;

            case 7: 
                showQueue();
                break;

            case 8: 
                printf("Exiting...\n");
                exit(0);

            default:
                printf("Invalid choice!\n");
        }
    }
    return 0;
}