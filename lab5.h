#ifndef LAB5_H
#define LAB5_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

typedef struct Flight {
    int id;
    char flight_number[10];
    char plane_name[50];
    char departure_time[6];
    char arrival_time[6];
    struct Flight* next;
} Flight;

typedef struct {
    Flight* head;
    int count;
    int next_id;
} Airport;

Airport* create_database();
void free_database(Airport* db);

void save_to_file(Airport* db, const char* filename);
Airport* load_from_file(const char* filename);

void add_flight(Airport* db, const char* flight_number, const char* plane_name,
                const char* departure_time, const char* arrival_time);
void print_flights(Airport* db);
Flight* find_flight(Airport* db, const char* flight_number);
void delete_flight(Airport* db, int id);
void edit_flight(Airport* db, int id, char* flight_number, 
                char* plane_name, char* departure_time,
                char* arrival_time);
                
void print_details(Flight* flight);
void clear_input_buffer();
void print_menu();

#endif