#ifndef INITDB_H_INCLUDED
#define INITDB_H_INCLUDED

void init_db() {
    FILE *fp;
    if ((fp = fopen(MASTER_DATA, "r")) == NULL)
    {
        fclose(fp);
        fp = fopen(MASTER_DATA, "wb+");
        fwrite(0, 0, 1, fp);
        fclose(fp);
    }
    if ((fp = fopen(MASTER_IND, "r")) == NULL)
    {
        fclose(fp);
        fwrite(0, 0, 1, fp);
        fp = fopen(MASTER_IND, "wb+");
        fclose(fp);
    }
    if ((fp = fopen(SLAVE_DATA, "r")) == NULL)
    {
        fclose(fp);
        fp = fopen(SLAVE_DATA, "wb+");
        fwrite(0, 0, 1, fp);
        fclose(fp);
    }
    if ((fp = fopen(SLAVE_IND, "r")) == NULL)
    {
        fclose(fp);
        fwrite(0, 0, 1, fp);
        fp = fopen(SLAVE_IND, "wb+");
        fclose(fp);
    }
}


#endif // INITDB_H_INCLUDED
