#pragma once

#include "cstdlib"
#include "cstdio"
#include "master.h"

#define SLAVE_DATA "S.fl"
#define SLAVE_IND "S.ind"
#define SLAVE_OFFSET "S.ofs"
#define SLAVE_SIZE sizeof(struct Player)
#define INDEX_SIZE sizeof(struct index)

#include "Club.h"
#include "Player.h"

int write_offset_player(offset* o);
int get_offset_player();
int get_player(Player* player, int id);
int del_player(int id);
int del_club(int id);
int del_all_player_clubid(int ClubId);
int upd_player(Player* player);
int insert_player(Player* player);
int calc_player();
int ut_s();
int save_s(Player* s, index i);
index get_next_index_player();
index get_index_player(int id);
int is_index_player(int id);
int insert_index_player(index i);
int replace_index_player(index i);
int try_del_player_clubid(index i, int id);

int write_offset_player(offset* o) {
    FILE* database = fopen(SLAVE_OFFSET, "w+");
    // w+ Открывает пустой файл для чтения и записи. Если файл существует, его содержимое удаляется.

    fseek(database, 0, SEEK_SET);
    fwrite(o, OFFSET, 1, database);
    fclose(database);

    return 0;
}

int get_offset_player() {
    struct offset o{};
    o.id = -2;
    FILE* database = fopen(SLAVE_OFFSET, "rb+");
    // r 	Открывает для чтения. Если файл не существует или его не удается найти, вызов завершается ошибкой
    // b 	Открытие в двоичном (непереложенном) режиме; Переводы, включающие символы возврата каретки и перевода строки, подавляются.
    fseek(database, 0, SEEK_SET);
    fread(&o, OFFSET, 1, database);
    fclose(database);

    return o.id;
}


int get_player(Player* s, int id) {
    index i = get_index_player(id);
    if (i.id == -1)
        return 1;

    FILE* database = fopen(SLAVE_DATA, "rb+");

    fseek(database, i.id * i.record_size, SEEK_SET);
    fread(s, SLAVE_SIZE, 1, database);
    fclose(database);

    if (i.exists == 1)
        return 1;

    return 0;
}

int save_player(Player* player, index i) {
    FILE* database = fopen(SLAVE_DATA, "rb+");

    if (database == nullptr)
        return 1;

    // Insert a data about a user in the .fl file

    player->Id = i.id;
    fseek(database, i.id * SLAVE_SIZE, SEEK_SET);
    fwrite(player, SLAVE_SIZE, 1, database);
    fclose(database);

    // Create an indexer and store it inn the .ind file
    insert_index_player(i);
}

int insert_player(Player* player) {
    if(is_index_club(player->clubId) == 1) return 2;
    index i = get_next_index_player();
    if (get_index_club(player->clubId).exists == 1)
        return 2;

    return save_player(player, i);
}

int upd_player(Player* player) {
    if(player->Id > get_offset_player() || is_index_player(player->Id) == 1)
        return 1;
    index check = get_index_player(player->Id);
    if (check.exists == 1)
        return 1;

    struct index i{};
    struct Club club{};
    if(get_club(&club, player->clubId) == 1) return 2;
    if (get_index_club(player->clubId).exists == 1)
        return 2;
    i.id = player->Id;
    return save_player(player, i);
}


// TODO: Implement simple slave delete based on slave_id
int del_player(int id)
{
    if(id > get_offset_player() || is_index_player(id) == 1)
        return 1;

    index i = get_index_player(id);
    if (i.id == -1 || i.exists == 1)
        return 1;

    i.exists = 1;
    if (replace_index_player(i) == 1)
        return 1;

    return 0;
}

int del_club(int id) {
    if (is_index_club(id) == 1)
        return 1;

    index i = get_index_club(id);
    if (i.id == -1 || i.exists == 1)
        return 1;

    i.exists = 1;
    if (replace_index_club(i) == 1)
        return 1;

    return del_all_player_clubid(id);
}


int del_all_player_clubid(int clubId)
{
    if (is_index_club(clubId) == 1)
        return 1;
    int last_index = get_offset_player();
    for (int j = last_index; j >= 0; j--)
    {
        index i = get_index_player(j);
        if (i.exists == 1)
            continue;
        try_del_player_clubid(i, clubId);
    }
    return 0;
}

int try_del_player_clubid(index i, int id)
{
    struct Player player{};
    if (get_player(&player, i.id) == 1)
        return 1;
    if (player.clubId == id)
        del_player(i.id);
    return 0;
}

int insert_index_player(index i) {
    FILE* index_collection = fopen(SLAVE_IND, "a+b");
    if (index_collection == nullptr)
        return 1;

    fseek(index_collection, i.id * INDEX_SIZE, SEEK_SET);
    fwrite(&i, INDEX_SIZE, 1, index_collection);
    fclose(index_collection);
}

int replace_index_player(index i) {
    FILE* index_collection = fopen(SLAVE_IND, "rb+");
    if (index_collection == nullptr)
        return 1;

    fseek(index_collection, i.id * INDEX_SIZE, SEEK_SET);
    fwrite(&i, INDEX_SIZE, 1, index_collection);
    fclose(index_collection);
}

index get_next_index_player() {
    struct index i{};
    i.id = get_offset_player() + 1;
    i.record_size = SLAVE_SIZE;
    i.exists = 0;

    struct offset o{};
    o.id = i.id;
    write_offset_player(&o);

    return i;
};


index get_index_player(int id) {
    struct index i{};
    if (is_index_player(id) == 1)
    {
        i.id = -1;
        return i;
    }

    FILE* index_collection = fopen(SLAVE_IND, "rb");
    fseek(index_collection, id * INDEX_SIZE, SEEK_SET);
    fread(&i, INDEX_SIZE, 1, index_collection);
    fclose(index_collection);
}

int is_index_player(int id) {
    int latest = get_offset_player();
    if (id > latest)
        return 1;
    FILE* index_collection = fopen(SLAVE_IND, "rb");
    if (index_collection == nullptr)
        return 1;

    fseek(index_collection, 0, SEEK_END);
    int index_size = ftell(index_collection);

    if (index_size == 0 || id * INDEX_SIZE > index_size)
        return 1;

    fclose(index_collection);
    return 0;
}

int calc_player() {
    FILE* index_collection = fopen(SLAVE_IND, "rb");
    if (index_collection == nullptr)
        return -1;

    int counter = 0;

    int amount = get_offset_player();
    for (int i = 0; i <= amount; i++)
    {
        struct index ind{};
        fseek(index_collection, 0 * INDEX_SIZE, SEEK_SET);
        fread(&ind, INDEX_SIZE, 1, index_collection);
        fclose(index_collection);
        if (ind.exists == 0)
            counter++;
    }

    return counter;
}

void ut_pl() {
    int records_amount = get_offset_player();
    cout << "Indexes and records (players), which were added to the database" << endl;
    FILE* index_collection = fopen(SLAVE_IND, "rb+");
    for (int i = 0; i <= records_amount; i++)
    {
        struct index ind{};
        fseek(index_collection, i * INDEX_SIZE, SEEK_SET);
        fread(&ind, INDEX_SIZE, 1, index_collection);
        cout << ind.id << "; " << ind.record_size << "; " << ind.exists << endl;
    }
    fclose(index_collection);
    for (int i = 0; i <= records_amount; i++)
    {
        struct Player p{};
        get_player(&p, i);
        cout << p.Id << "; " << p.clubId << "; " << p.PlayerName << "; " << p.PlayerNumber << "; " << p.PlayerPosition << endl;
    }
}
