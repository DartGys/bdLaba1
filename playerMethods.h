#ifndef PLAYERMETHODS_H_INCLUDED
#define PLAYERMETHODS_H_INCLUDED

#include "slave.h"
#include "Player.h"


#endif // PLAYERMETHODS_H_INCLUDED

void get_player() {
    struct Player p;
    int id;
    cout << "Id:" << endl;
    cin >> id;
    p.Id = id;
    if(get_player(&p, id) == 0) {
        cout << "Player id: " << p.Id << endl << " Club id: " << p.clubId << endl << "Player Name: " << p.PlayerName <<
         endl << "Player number: " << p.PlayerNumber << endl <<"Player position: " << p.PlayerPosition  << endl;
    }
    else {
        cout << "Error occurred during the get operation" << endl;
    }
}

int insert_player(){
    struct Player p;

    cout << "Club Id: ";
    cin >> p.clubId;
    cout << "Player name: ";
    cin >> p.PlayerName;
    cout << "Player number: ";
    cin >> p.PlayerNumber;
    cout << "Player position: ";
    cin >> p.PlayerPosition;
    int result_code = insert_player(&p);
    if(result_code == 1) {
        cout << "Error occurred during the insert operation" << endl;
        return 1;
    }
    if(result_code == 2) {
        cout << "Error: Club with this id is not exist" << endl;
        return 1;
    }
    if (result_code == 0) {
        cout << "Player created with id: " << p.Id <<  endl;
    }
}

int update_player()
{
    struct Player p;

    cout << "Player Id: ";
    cin >> p.Id;
    cout << "Club Id: ";
    cin >> p.clubId;
    cout << "Player Name: ";
    cin >> p.PlayerName;
    cout << "Player number: ";
    cin >> p.PlayerNumber;
    cout << "Player position: ";
    cin >> p.PlayerPosition;
    int result_code = upd_player(&p);
    if(result_code == 1) {
        cout << "Error occurred during the update operation" << endl;
        return 1;
    }
    if(result_code == 2) {
        cout << "Error: Master with this id is not exsist" << endl;
        return 1;
    }
    if (result_code == 0) {
        cout << "Player was updated.";
    }
}

void delete_player(){
    struct Player p;
    int id;
    cout << "Player Id:" << endl;
    cin >> id;
    p.Id = id;

    if(del_player(id) == 0){
        cout << "Player deleted." << endl;
    } else{
        cout << "Error" << endl;
    }
}

void calculate_player()
{
    int slaves = calc_player();
    if(slaves == -1){
        cout << "Error occurred." << endl;
    } else{
        cout << "Amount of player: " << slaves << endl;
    }
}

void ut_player(){
    ut_pl();
}
