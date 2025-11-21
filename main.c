
// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pro.h"

int main() {
    Room* root = NULL;
    MinHeap heap;
    initHeap(&heap);
    int valid = 0;

    // AUTO CREATE 100 ROOMS (101 → 200)
    autoGenerateRooms(&root, &heap);

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
        printf("8. Delete Room\n");
        printf("9. Exit\n");
        printf("Enter your choice: ");

        while (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("Invalid input. Enter number: ");
        }
        clearInputBuffer();

        switch (choice) {

        case 1:
            printf("Enter Room No and Capacity (e.g., 101 50): ");

            while (1) {
                if (scanf("%d %d", &no, &cap) == 2 && no >= 0 && cap >= 0) {
                    break; // valid input
                }

                printf("Invalid input. Please enter two NON-NEGATIVE numbers: ");
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

            valid = 0;
            printf("Enter your name: ");
            scanf("%s", name);
            inorder(root);
            clearInputBuffer();

            printf("Enter Room No: ");
            scanf("%d", &no);
            clearInputBuffer();

            do{printf("Start Time (1->24): ");
                if (scanf("%d", &start) != 1 || start < 1 || start > 24) {
                printf("Invalid. Enter a number from 1 to 24.\n");
                clearInputBuffer();
            } else {
                valid = 1;
                }
            } while (!valid);

            valid = 0;
            do {
            printf("End Time (1->24): ");
            if (scanf("%d", &end) != 1 || end < 1 || end > 24) {
            printf("Invalid. Enter a number from 1 to 24.\n");
            clearInputBuffer();
            } else {
            valid = 1;
            }
            } while (!valid);

            bookRoom(root, no, name, start, end);
            break;

        case 4:
            printf("Enter your name: ");
            scanf("%s", name);
            clearInputBuffer();

            printf("Required capacity: ");
            scanf("%d", &need);

            Room *best = autoAllocate(&heap, need);
            if (!best) {
                printf("No free room. Added to waiting list.\n");
                break;
            }

            printf("Assigned Room %d (cap %d)\n", best->roomNo, best->capacity);
            printf("Start & End Time: ");
            scanf("%d %d", &start, &end);

            bookRoom(root, best->roomNo, name, start, end);
            break;

        case 5:
            displayBookings();
            break;

        case 6:
            printf("Enter name to cancel: ");
            scanf("%s", name);
            clearInputBuffer();
            cancelBooking(root, name);
            break;

        case 7:
            showQueue();
            break;

        case 8:
            printf("Enter Room No to delete: ");
            scanf("%d", &no);
            clearInputBuffer();

            removeRoomRelatedBookings(no);
            removeWaitingListForRoom(no);

            root = deleteRoom(root, no);
            rebuildHeap(&heap, root);

            printf("Room %d deleted (if existed).\n", no);
            break;

        case 9:
            printf("Exiting...\n");
            exit(0);

        default:
            printf("Invalid choice\n");
        }
    }

    return 0;
}
