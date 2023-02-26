#pragma once

#include <iostream>
#include "master.h"
#include "slave.h"
#include "Club.h"
#include "Player.h"
#include "initDB.h"
#include "clubMethods.h"
#include "playerMethods.h"
#include "index.h"
#include "offset.h"

using namespace std;

void unsupported_operation() {
    cout << "This operation is not supported." << endl;
}

void display_options() {
    printf("Choose option:\n"
           "0 - Quit\n"
           "1 - Insert Club\n"
           "2 - Get Club\n"
           "3 - Update Club\n"
           "4 - Delete Club\n"
           "5 - Count of Clubs\n"
           "6 - Insert Player\n"
           "7 - Get Player\n"
           "8 - Update Player\n"
           "9 - Delete Player\n"
           "10 - Count of Players\n"
           //"11 - Info about all Players in database\n"
           //"12 - Info about all Clubs in database\n"
    );

}


int main() {
    init_db();
    struct offset o {};
    o.id = -1;
    if (get_offset_club() == -2)
        write_offset_club(&o);
    if (get_offset_player() == -2)
        write_offset_player(&o);
    int command_id;

    do {
        display_options();
        cin >> command_id;
        switch (command_id) {
            case 0:
                return 0;
            case 1:
                insert_club();
                break;
            case 2:
                get_club();
                break;
            case 3:
                update_club();
                break;
            case 4:
                delete_club();
                break;
            case 5:
                calculate_club();
                break;
            case 6:
                insert_player();
                break;
            case 7:
                get_player();
                break;
            case 8:
                update_player();
                break;
            case 9:
                delete_player();
                break;
            case 10:
                calculate_player();
                break;
            /*case 11:
                ut_player();
                break;
            case 12:
                ut_club();
                break;*/
            default:
                unsupported_operation();
                break;
        }
        cout << endl << endl;
    } while(1);
}


