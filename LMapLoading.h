#ifndef LMAPLOADING_H_INCLUDED
#define LMAPLOADING_H_INCLUDED
#include <IL/il.h>
#include <IL/ilu.h>
#include "LTexture.h"
#include <filesystem>
#include <SDL.h>
#include <SDL_events.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
using namespace std;

//Terrain variables
#define terrain_variable_coord_x 1
#define terrain_variable_coord_y 2
#define terrain_variable_coord_z 3
#define terrain_variable_chunk_coord_x 4
#define terrain_variable_chunk_coord_y 5
#define terrain_variable_chunk_coord_z 6

/*
    Credit to https://dev.to/gtanyware/dynamic-arrays-in-c-24oj Graham Trott
    This is a doubly linked list element able to store any kind of data.
    The content is immutable within this class.
*/
class LinkedListElement {

    private:

       const void* content;
       LinkedListElement* previous = nullptr;
       LinkedListElement* next = nullptr;

    public:

       // Get the content of this element
       void* get() {
           return (void*)content;
       }
       // Get the previous element pointer
       LinkedListElement* getPrevious() {
           return previous;
       }
       // Set the previous element pointer
       void setPrevious(LinkedListElement* data) {
           previous = data;
       }
       // Get the next element pointer
       LinkedListElement* getNext() {
           return next;
       }
       // Set the next element pointer
       void setNext(LinkedListElement* data) {
           next = data;
       }
        // Constructor
        LinkedListElement(const void* data) {
            content = data;
        }
        // Destructor
        ~LinkedListElement() {}
};

class LinkedList {

    private:
        int size = 0;                          // the number of items
        LinkedListElement* head = nullptr;
        LinkedListElement* tail = nullptr;

    public:
        // Get the size (the number of elements in the list)
        int getSize() {
            return size;
        };
        void add(const void* data) {
            LinkedListElement* element = new LinkedListElement(data);
            if (size == 0) {
                head = element;
                tail = head;
            } else {
                tail->setNext(element);
                element->setPrevious(tail);
                tail = element;
            }
            ++size;
        }
        void* get(int index) {
            if (index < size) {
                LinkedListElement* element = head;
                while (index > 0) {
                    element = element->getNext();
                    --index;
                }
                return element->get();
            }
            return nullptr;
        }
        // Clear the list. Remove everything except the element data.
        void clear() {
            LinkedListElement* walker = head;
            if (walker != nullptr) {
                while (walker->getNext() != nullptr) {
                    LinkedListElement* next = walker->getNext();
                    delete walker;
                    walker = next;
                }
                delete walker;
            }
            size = 0;
        }
        // Default constructor
        LinkedList() {}
        // Destructor
        ~LinkedList() {
            clear();
         }
};

/*
    ObjectArray is a memory-efficient class for managing arrays of arbitrary objects.
*/
class ObjectArray {

    private:
        int size = 0;                         // the number of items
        void** array = (void**)malloc(1);     // the array of items
        LinkedList* list;                     // a list to hold new data items

    public:
        // int grid_comparison_array[1000];
        // Get the size (the number of elements in the array)
        int getSize() {
            return this->size + list->getSize();
        };
        int chunk_size_eval_increment, chunk_enum, chunk_scope = 0;
        int peripheral_offset_x, peripheral_offset_y = 0;
        int chunk_breadth, chunk_width = 30;
        // Get a specified item.
        // If the index is greater than the array size, return the item from the list.
        void* get(int n) {
            if (n < size) {
                return array[n];
            }
            else if (n < size + list->getSize()) {
                return (void*)list->get(n - size);
            }
            return nullptr;
        };
        // Add an item. This goes into the linked list.
        void add(const void* item) {
            list->add(item);
        }
        // Flatten this item by creating a single array to hold all the data.
        void flatten() {
            void** oldArray = array;
            int oldSize = size;
            // Create a new array big enough for the old array and the list
            int total = oldSize + list->getSize();
            if (total > 0) {
                array = (void**)malloc(sizeof(ObjectArray*) * (total));
                // Copy the old array to the new
                size = 0;
                while (size < oldSize) {
                    array[size] = oldArray[size];
                    size++;
                }
                free(oldArray);
                // Copy the list to the new array
                int n = 0;
                while (n < list->getSize()) {
                    array[size++] = list->get(n++);
                }
                list->clear();
            }
        }
        // Default constructor
        ObjectArray() {
            list = new LinkedList();
        }
        // Destructor
        ~ObjectArray() {
            free(array);
            delete list;
         }
};

class Terrain {
  private:
    const char* name;
  public:
    char* getName() { return (char*)this->name; }
    Terrain(const char* name) { this->name = name; }
    int variables[100];
};

typedef struct chunk_storage{
  int chunk_size;
  LinkedList terrain_descriptions;
  int chunk_flags[];
} chunk_storage;

chunk_storage *load_chunk_from_file(const char *name_of_file);

ObjectArray* describe_chunk(const char *name_of_file, ObjectArray *chunk);

void describe_peripheral_chunk(const char *name_of_file, ObjectArray *chunk);

// , chunk_storage chunk_to_store

#endif // LMAPLOADING_H_INCLUDED
