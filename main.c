#include "lab5.h"

int main() {
    Airport* db = load_from_file("flights.txt");
    int choice;
    while (1) {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            printf("err\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();
        switch (choice) {
            case 1: // вывод рейсов
                print_flights(db);
                break;
                
            case 2: { // добавление нового
                char flight_number[10], plane_name[50];
                char departure_time[6], arrival_time[6];
                
                printf("enter flight num (example: QF025): ");
                fgets(flight_number, sizeof(flight_number), stdin);
                flight_number[strcspn(flight_number, "\n")] = '\0';
                
                printf("enter plane name: ");
                fgets(plane_name, sizeof(plane_name), stdin);
                plane_name[strcspn(plane_name, "\n")] = '\0';

                printf("enter departure time (hh:mm): ");
                fgets(departure_time, sizeof(departure_time), stdin);
                departure_time[strcspn(departure_time, "\n")] = '\0';

                printf("enter arrival time (hh:mm): ");
                fgets(arrival_time, sizeof(arrival_time), stdin);
                arrival_time[strcspn(arrival_time, "\n")] = '\0';

                add_flight(db, flight_number, plane_name, departure_time, arrival_time);
                break;
            }  
            case 3: { // поиск рейса по номеру
                char flight_number[10];
                printf("enter flight number: ");
                fgets(flight_number, sizeof(flight_number), stdin);
                flight_number[strcspn(flight_number, "\n")] = '\0';
                Flight* flight = find_flight(db, flight_number);
                print_details(flight);
                break;
            }
            case 4: { // удаление рейса
                int id;
                printf("enter id for removing: ");
                scanf("%d", &id);
                clear_input_buffer();
                delete_flight(db, id);
                break;
            }
            case 5: { // редактирование рейса
                int id;
                char flight_number[10] = "", plane_name[50] = "";
                char departure_time[6] = "", arrival_time[6] = "";
                printf("enter id for editing: ");
                scanf("%d", &id);
                clear_input_buffer();
                Flight* current = db->head;
                while (current != NULL && current->id != id) {
                    current = current->next;
                }
                if (!current) {
                    printf("flight with id: %d not found\n", id);
                    break;
                }
                print_details(current);
                printf("new flight number: ");

                fgets(flight_number, sizeof(flight_number), stdin);
                flight_number[strcspn(flight_number, "\n")] = '\0';

                printf("new plane number: ");
                fgets(plane_name, sizeof(plane_name), stdin);
                plane_name[strcspn(plane_name, "\n")] = '\0';
                
                printf("new departure time (hh:mm): ");
                fgets(departure_time, sizeof(departure_time), stdin);
                departure_time[strcspn(departure_time, "\n")] = '\0';
                
                printf("new arrival time (hh:mm): ");
                fgets(arrival_time, sizeof(arrival_time), stdin);
                arrival_time[strcspn(arrival_time, "\n")] = '\0';
                
                edit_flight(db, id, flight_number, plane_name, departure_time, arrival_time);
                break;
            }
                
            case 6: // сохранение в файл
                save_to_file(db, "flights.txt");
                break;
                
            case 7: // выход
                save_to_file(db, "flights.txt");
                free_database(db);
                return 0;
                
            default:
                printf("incorrect number\n");
        }
    }
    return 0;
}
