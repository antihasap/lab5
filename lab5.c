#include "lab5.h"

Airport* create_database() {
    Airport* db = (Airport*)malloc(sizeof(Airport));
    if (!db) {
        printf("err\n");
        return NULL;
    }
    db->head = NULL;
    db->count = 0;
    db->next_id = 1;
    return db;
}

void free_database(Airport* db) {
    Flight* curr = db->head;
    while (curr != NULL) {
        Flight* temp = curr;
        curr = curr->next;
        free(temp);
    }
    free(db);
}

void save_to_file(Airport* db, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("err");
        return;
    }
    
    Flight* curr = db->head;
    while (curr != NULL) {
        fprintf(file, "%d|%s|%s|%s|%s\n", curr->id, curr->flight_number,
                curr->plane_name,curr->departure_time, curr->arrival_time);
        curr = curr->next;
    }
    
    fclose(file);
    printf("saved to %s\n", filename);
}

Airport* load_from_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("file '%s' not found\n", filename);
        return create_database();
    }
    
    Airport* db = create_database();
    char line[256];
    int max_id = 0;
    
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';
        char* elem;
        char* elems[5];
        int i = 0;

        elem = strtok(line, "|");
        while (elem != NULL && i < 5) {
            elems[i++] = elem;
            elem = strtok(NULL, "|");
        }
        if (i == 5) {
            Flight* new_flight = (Flight*)malloc(sizeof(Flight));
            new_flight->id = atoi(elems[0]);
            strncpy(new_flight->flight_number, elems[1], sizeof(new_flight->flight_number) - 1);
            strncpy(new_flight->plane_name, elems[2], sizeof(new_flight->plane_name) - 1);
            strncpy(new_flight->departure_time, elems[3], sizeof(new_flight->departure_time) - 1);
            strncpy(new_flight->arrival_time, elems[4], sizeof(new_flight->arrival_time) - 1);
            new_flight->next = NULL;

            if (db->head == NULL) {
                db->head = new_flight;
            } else {
                Flight* curr = db->head;
                while (curr->next != NULL) {
                    curr = curr->next;
                }
                curr->next = new_flight;
            }
            
            db->count++;
            if (new_flight->id > max_id) {
                max_id = new_flight->id;
            }
        }
    }

    db->next_id = max_id + 1;
    fclose(file);
    printf("loaded %d flight(s) from %s\n", db->count, filename);
    return db;
}

void add_flight(Airport* db, const char* flight_number, const char* plane_name,
                const char* departure_time, const char* arrival_time) {
    Flight* new_flight = (Flight*)malloc(sizeof(Flight));
    if (!new_flight) {
        printf("err\n");
        return;
    }

    new_flight->id = db->next_id++;
    strncpy(new_flight->flight_number, flight_number, sizeof(new_flight->flight_number) - 1);
    strncpy(new_flight->plane_name, plane_name, sizeof(new_flight->plane_name) - 1);
    strncpy(new_flight->departure_time, departure_time, sizeof(new_flight->departure_time) - 1);
    strncpy(new_flight->arrival_time, arrival_time, sizeof(new_flight->arrival_time) - 1);
    new_flight->next = NULL;
    
    if (db->head == NULL) {
        db->head = new_flight;
    } else {
        Flight* curr = db->head;
        while (curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = new_flight;
    }
    
    db->count++;
    printf("flight '%s' added with id: %d\n", flight_number, new_flight->id);
}

void print_flights(Airport* db) {
    if (db->count == 0) {
        printf("empty\n");
        return;
    }
    
    Flight* curr = db->head;
    while (curr != NULL) {
        printf("%d │ %s │ %s │ %s │ %s\n", curr->id, curr->flight_number, curr->plane_name, curr->departure_time, curr->arrival_time);
        curr = curr->next;
    }
}

Flight* find_flight_by_number(Airport* db, const char* flight_number) {
    Flight* curr = db->head;
    while (curr != NULL) {
        if (strcmp(curr->flight_number, flight_number) == 0) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

void find_flights_by_plane(Airport* db, const char* plane_name) {
    Flight* curr = db->head;
    int found = 0;
    printf("\nflights with plane '%s':", plane_name);
    while (curr != NULL) {
        if (strstr(curr->plane_name, plane_name) != NULL) {
            printf("%d | %s | %s | %s\n",
                   curr->id, curr->flight_number,
                   curr->departure_time, curr->arrival_time);
            found = 1;
        }
        curr = curr->next;
    }
    if (!found) {
        printf("no flights found with plane: %s\n", plane_name);
    }
}

void find_flights_by_departure_time(Airport* db, const char* departure_time) {
    Flight* curr = db->head;
    int found = 0;
    printf("\nflights departing at: %s:\n", departure_time);
    while (curr != NULL) {
        if (strcmp(curr->departure_time, departure_time) == 0) {
            printf("%d | %s | %s | %s\n",
                   curr->id, curr->flight_number,
                   curr->plane_name, curr->arrival_time);
            found = 1;
        }
        curr = curr->next;
    }
    if (!found) {
        printf("no flights found departing at: %s\n", departure_time);
    }
}

void find_flights_by_arrival_time(Airport* db, const char* arrival_time) {
    Flight* curr = db->head;
    int found = 0;
    printf("\nflights arriving at: %s\n", arrival_time);
    while (curr != NULL) {
        if (strcmp(curr->arrival_time, arrival_time) == 0) {
            printf("%d | %s | %s | %s\n",
                   curr->id, curr->flight_number,
                   curr->plane_name, curr->departure_time);
            found = 1;
        }
        curr = curr->next;
    }
    if (!found) {
        printf("no flights found arriving at: %s\n", arrival_time);
    }
}

void delete_flight(Airport* db, int id) {
    if (db->head == NULL) {
        printf("empty\n");
        return;
    }
    
    Flight* curr = db->head;
    Flight* prev = NULL;
    
    if (curr != NULL && curr->id == id) {
        db->head = curr->next;
        free(curr);
        db->count--;
        return;
    }
    
    while (curr != NULL && curr->id != id) {
        prev = curr;
        curr = curr->next;
    }
    
    if (curr == NULL) {
        printf("flight with id: %d not found\n", id);
        return;
    }
    
    prev->next = curr->next;
    free(curr);
    db->count--;
}

int validate_time_format(const char* time) { // проверка формата времени
    if (strlen(time) != 5) {
        return 0;
    }
    if (time[2] != ':') {
        return 0;
    }
    for (int i = 0; i < 5; i++) {
        if (i != 2) {
            if (time[i] < '0' || time[i] > '9') {
                return 0;
            }
        }
    }
    int hour = (time[0] - '0') * 10 + (time[1] - '0');
    int minute = (time[3] - '0') * 10 + (time[4] - '0');
    if (hour < 0 || hour > 23) {
        return 0;
    }
    if (minute < 0 || minute > 59) {
        return 0;
    }
    return 1;
}

void edit_flight(Airport* db, int id, char* flight_number, 
                char* plane_name, char* departure_time,
                char* arrival_time) {
    Flight* curr = db->head;
    while (curr != NULL && curr->id != id) {
        curr = curr->next;
    }
    
    if (curr == NULL) {
        printf("flight with id: %d not found\n", id);
        return;
    }

    if (strlen(flight_number) > 0) {
        strncpy(curr->flight_number, flight_number, sizeof(curr->flight_number) - 1);
    }
    if (strlen(plane_name) > 0) {
        strncpy(curr->plane_name, plane_name, sizeof(curr->plane_name) - 1);
    }
    if (strlen(departure_time) > 0) {
        strncpy(curr->departure_time, departure_time, sizeof(curr->departure_time) - 1);
    }
    if (strlen(arrival_time) > 0) {
        strncpy(curr->arrival_time, arrival_time, sizeof(curr->arrival_time) - 1);
    }
}

void print_details(Flight* flight) {
    if (flight == NULL) {
        printf("flight not found\n");
        return;
    }

    printf("id: %d\n", flight->id);
    printf("flight number: %s\n", flight->flight_number);
    printf("plane: %s\n", flight->plane_name);
    printf("departure time: %s\n", flight->departure_time);
    printf("arrival time: %s\n", flight->arrival_time);
}

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void print_menu() {
    printf("1. show all flights\n");
    printf("2. add a new flight\n");
    printf("3. find flights\n");
    printf("4. remove flight\n");
    printf("5. edit flight\n");
    printf("6. save all\n");
    printf("7. exit\n");
}

void print_search_menu() {
    printf("\n=== SEARCH FLIGHTS ===\n");
    printf("1. Search by flight number\n");
    printf("2. Search by plane name\n");
    printf("3. Search by departure time\n");
    printf("4. Search by arrival time\n");
    printf("5. Back to main menu\n");
    printf("Choose search option: ");
}
