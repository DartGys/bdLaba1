#pragma once

#include "cstdlib"
#include "cstdio"
#include "index.h"
#include <cstring>
#include "Club.h"
#include "offset.h"

#define MASTER_DATA "M.fl"
#define MASTER_IND "M.ind"
#define MASTER_OFFSET "M.ofs"
#define MASTER_SIZE sizeof(struct Club)
#define INDEX_SIZE sizeof(struct index)

#include "Club.h"
#include "Player.h"

using namespace std;


int write_offset_club(offset* o);
int get_club(Club* club, int id);
int update_club(Club* club);
int insert_club(Club* club);
int calc_club();
int ut_m();
int save_club(Club* m, index i);
index get_index_club(int id);
index get_next_index_club();
int insert_index_club(index i);
int replace_index_club(index i);
int write_offset_club(offset* o) {
    FILE* database = fopen(MASTER_OFFSET, "w+");

    fseek(database, 0, SEEK_SET);
    fwrite(o, OFFSET, 1, database);
    fclose(database);

    return 0;
}

int get_offset_club() {
    struct offset o{};
    o.id = -2;
    FILE* database = fopen(MASTER_OFFSET, "rb+");

    fseek(database, 0, SEEK_SET);
    fread(&o, OFFSET, 1, database);
    fclose(database);

    return o.id;
}

int get_club(Club* club, int id) {
    index i = get_index_club(id);
    if (i.id == -1)
        return 1;

    FILE* database = fopen(MASTER_DATA, "rb+");

    fseek(database, i.id * i.record_size, SEEK_SET);
    fread(club, MASTER_SIZE, 1, database);
    fclose(database);

    if (i.exists == 1)
        return 1;

    return 0;
}

int is_index_club(int id) {
    int latest = get_offset_club();
    if (id > latest)
        return 1;
    FILE* index_collection = fopen(MASTER_IND, "rb");
    if (index_collection == nullptr)
        return 1;

    fseek(index_collection, 0, SEEK_END);
    long index_size = ftell(index_collection);

    if (index_size == 0 || id * INDEX_SIZE > index_size)
        return 1;

    fclose(index_collection);
    return 0;
}

index get_index_club(int id) {
    struct index i{};
    if (is_index_club(id) == 1)
    {
        i.id = -1;
        return i;
    }

    FILE* index_collection = fopen(MASTER_IND, "rb");
    fseek(index_collection, id * INDEX_SIZE, SEEK_SET);
    fread(&i, INDEX_SIZE, 1, index_collection);
    fclose(index_collection);
}

int insert_club(Club* club) {
    index i = get_next_index_club();
    return save_club(club, i);
}

int update_club(Club* club) {
    struct index i{};
    if (club->id > get_offset_club() || is_index_club(i.id) == 1)
        return 1;
    i = get_index_club(club->id);
    if (i.exists == 1)
        return 1;
    return save_club(club, i);
}

index get_next_index_club() {
    struct index i{};
    i.id = get_offset_club() + 1;
    i.record_size = MASTER_SIZE;
    i.exists = 0;

    struct offset o{};
    o.id = i.id;
    write_offset_club(&o);

    return i;
}

int save_club(Club* club, index i) {
    FILE* database = fopen(MASTER_DATA, "rb+");

    if (database == nullptr)
        return 1;

    // Insert a data about a user in the .fl file

    club->id = i.id;
    fseek(database, i.id * MASTER_SIZE, SEEK_SET);
    fwrite(club, MASTER_SIZE, 1, database);
    fclose(database);

    // Create an indexer and store it inn the .ind file
    insert_index_club(i);
}

int insert_index_club(index i) {
    FILE* index_collection = fopen(MASTER_IND, "a+b");
    // Открывается для чтения и добавления. Операция добавления включает удаления маркера EOF перед записью новых данных в файл.
    //Маркер EOF не восстанавливается после завершения записи. Создает файл, если он не существует.
    if (index_collection == nullptr)
        return 1;

    fseek(index_collection, i.id * INDEX_SIZE, SEEK_SET);
    fwrite(&i, INDEX_SIZE, 1, index_collection);
    fclose(index_collection);
}

int replace_index_club(index i) {
    FILE* index_collection = fopen(MASTER_IND, "rb+");
    if (index_collection == nullptr)
        return 1;

    fseek(index_collection, i.id * INDEX_SIZE, SEEK_SET);
    fwrite(&i, INDEX_SIZE, 1, index_collection);
    fclose(index_collection);
}

int calc_club() {
    FILE* index_collection = fopen(MASTER_IND, "rb");
    if (index_collection == nullptr)
        return -1;

    int counter = 0;

    int amount = get_offset_club();
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

void ut_cl() {
    int records_amount = get_offset_club();
    cout << "Indexes and records (clubs), which were added to the database:" << endl;
    FILE* index_collection = fopen(MASTER_IND, "rb+");
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
        struct Club c{};
        get_club(&c, i);
        cout << c.id << "; " << c.ClubName << "; " << c.ClubCoachName << "; " << c.ClubOrigin << endl;
    }
}
