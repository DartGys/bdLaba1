#ifndef CLUBMETHODS_H_INCLUDED
#define CLUBMETHODS_H_INCLUDED

#include "slave.h"
#include "master.h"
#include "Club.h"

#endif // CLUBMETHODS_H_INCLUDED

void get_club() {
    struct Club c;
    int id;
    cout << "Club Id:" << endl;
    cin >> id;
    c.id = id;
    if(get_club(&c, id) == 0) {
        cout << " Id: " << c.id << endl << "Club Name: " << c.ClubName << endl << "Club coach: " <<c.ClubCoachName <<
        endl << "Club origin: " << c.ClubOrigin << endl;
    }
    else {
        cout << "Error occurred during the get operation" << endl;
    }
}

void insert_club(){
    struct Club c;

    cout << "Club name: ";
    cin >> c.ClubName;
    cout << "Club coach: ";
    cin >> c.ClubCoachName;
    cout << "Club origin: ";
    cin >> c.ClubOrigin;

    if (insert_club(&c) == 0) {
        cout << "Club was created with id: " << c.id << endl;
    }
    else {
        cout << "Error occurred during the insert operation" << endl;
    }
}

void update_club()
{
    struct Club c;

    cout << "Id: ";
    cin >> c.id;
    cout << "Club name: ";
    cin >> c.ClubName;
    cout << "Club coach: ";
    cin >> c.ClubCoachName;
    cout << "Club origin: ";
    cin >> c.ClubOrigin;
    if(update_club(&c) == 0) {
        cout << "Club was updated" << endl;
    }
    else {
        cout << "Error occurred during the update operation" << endl;
    }
}

void delete_club() {
    int id;
    cout << "Id: ";
    cin >> id;

    if(del_club(id) == 0){
        cout << "Club was deleted." << endl;
    }
    else {
        cout << "Error" << endl;
    }
}

int calculate_club()
{
    int result = calc_club();
    if(calc_club() == -1){
        cout << "There is no club or error occurred." << endl;
        return 1;
    }
    cout << "Number of Clubs: " << result << endl;
    return 0;
}

void ut_club(){
    ut_cl();
}
