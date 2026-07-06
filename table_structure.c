#include<stdlib.h>
#include<stdio.h>
#include"command_handler.h"
#include"input.h"


const uint32_t ID_SIZE = size_of_attribute(Row,id);
 const uint32_t USERNAME_SIZE = size_of_attribute(Row,username);
 const uint32_t EMAIL_SIZE = size_of_attribute(Row,email);
 const uint32_t ROW_SIZE = ID_SIZE+USERNAME_SIZE+EMAIL_SIZE;


 const uint32_t ID_OFFSET = 0;
 const uint32_t USERNAME_OFFSET = ID_OFFSET+ID_SIZE;
 const uint32_t EMAIL_OFFSET = USERNAME_OFFSET+USERNAME_SIZE;
 const uint32_t ROW_OFFSET = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;

 const uint32_t PAGE_SIZE = 4096;
 const uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE ;
 const uint32_t TABLE_MAX_ROWS = TABLE_MAX_PAGES * ROWS_PER_PAGE;



Table* db_open(const char* filename){
    Pager* pager=pager_open(filename);
    uint32_t  num_rows = pager->file_length/ROW_SIZE;

    Table* table=(Table*)malloc(sizeof(Table));
    // table->num_rows=0;
    // for(int i=0;i<TABLE_MAX_PAGES;i++){
    //     table->pager[i]=NULL;
    // }
    table->pager=pager;
    table->num_rows=num_rows;

    return table;
}

Pager* pager_open(const char* filename){
    int fd = open(filename,O_RDWR | O_CREAT,S_IWUSR | S_IRUSR);
    if(fd==-1){
        printf("Unable to open the file\n");
        exit(EXIT_FAILURE);
    }
    off_t file_length=lseek(fd,0,SEEK_END);
    Pager* pager=malloc(sizeof(Pager));
    pager->file_descriptor=fd;
    pager->file_length=file_length;
    for(uint32_t i=0;i<TABLE_MAX_PAGES;i++){
        pager->pages[i]=NULL;
    }
    return pager;
}


void* get_page(Pager* pager ,uint32_t page_num){
    if(page_num>TABLE_MAX_PAGES){
        printf("Tried to fetch page number out of bounds. %d > %d\n",page_num,TABLE_MAX_PAGES);
        exit(EXIT_FAILURE);
    }
    if(pager->pages[page_num]==NULL){
        void* page = malloc(PAGE_SIZE);
        uint32_t num_pages = pager->file_length / PAGE_SIZE;
        if(pager->file_length%PAGE_SIZE){
            num_pages+=1;
        }
        if(page_num<=num_pages){
            lseek(pager->file_descriptor,page_num*PAGE_SIZE,SEEK_SET);
            ssize_t bytes_read=read(pager->file_descriptor,page,PAGE_SIZE);
            if(bytes_read==-1){
                printf("Error reading file");
                exit(EXIT_FAILURE);
            }
        }
        pager->pages[page_num]=page;
    }
    return pager->pages[page_num];
}

void db_close(Table* table){
    Pager* pager=table->pager;
    uint32_t num_full_pages=table->num_rows/ROWS_PER_PAGE;
    for(uint32_t i=0;i<num_full_pages;i++){
        if(pager->pages[i]==NULL){
            continue;
        }
        pager_flush(pager,i,PAGE_SIZE);
        free(pager->pages[i]);
        pager->pages[i] = NULL;
    }

    uint32_t nums_additional_rows = table->num_rows % ROWS_PER_PAGE ;
    if(nums_additional_rows > 0){
        uint32_t page_num = num_full_pages;
        if(pager->pages[page_num]!=NULL){
            pager_flush(pager,page_num,nums_additional_rows*ROW_SIZE);
            free(pager->pages[page_num]);
            pager->pages[page_num] = NULL;

        }
    }


    int result = close(pager->file_descriptor);
    if(result==-1){
        printf("Error closing the db file.\n");
        exit(EXIT_FAILURE);
    }

    for(uint32_t i=0; i < TABLE_MAX_PAGES;i++){
            void* page = pager->pages[i];
            if(page){
                free(page);
                pager->pages[i]=NULL;
            }
    }


    free(pager);
    free(table);
}

void pager_flush(Pager* pager,uint32_t page_num,uint32_t size){
    if(pager->pages[page_num]==NULL){
        printf("Tried to flush NUll Page");
        exit(EXIT_FAILURE);
    }
    off_t offset=lseek(pager->file_descriptor,page_num*PAGE_SIZE,SEEK_SET);
    if(offset==-1){
        printf("Error Seeking: %d\n",errno);
        exit(EXIT_FAILURE);
    }

    ssize_t bytes_written = write(pager->file_descriptor,pager->pages[page_num],size);
    if(bytes_written == -1){
        printf("Error Writing: %d\n");
        exit(EXIT_FAILURE);
    }
}

// void free_table(Table* table){

//     for(int i=0;i<TABLE_MAX_PAGES;i++){
//         free(table->pager[i]);
//     }
//     free(table);
// }

void* row_slot(uint32_t row_num,Table* table){

    uint32_t page_num = row_num / ROWS_PER_PAGE;
    // void* page=table->pages[page_num];
    // if(page==NULL){
    //     page=table->pages[page_num]=malloc(PAGE_SIZE);
    // }

    void* page = get_page(table->pager,page_num);
    uint32_t row_offset = row_num % ROWS_PER_PAGE;
    uint32_t byte_offset = row_offset*ROW_SIZE;
   
    return page+byte_offset;
}


void print_row(Row* row){
    printf("%d , %s , %s\n",row->id,row->username,row->email);
}

void serialize_row(Row* source,void* destination){
    memcpy(destination+ID_OFFSET,&(source->id),ID_SIZE);
    memcpy(destination+USERNAME_OFFSET,&(source->username),USERNAME_SIZE);
    memcpy(destination+EMAIL_OFFSET,&(source->email),EMAIL_SIZE);
}

void deserialize_row(void* source,Row* destination){

    memcpy(&(destination->id),source+ID_OFFSET,ID_SIZE);
    memcpy(&(destination->username),source+USERNAME_OFFSET,USERNAME_SIZE);
    memcpy(&(destination->email),source+EMAIL_OFFSET,EMAIL_SIZE);
}


ExecuteResult execute_insert(Statement* statement,Table* table){

    if(table->num_rows >= TABLE_MAX_ROWS){
        return EXECUTE_TABLE_FULL;
    }

    Row* row_to_insert = &(statement->row_to_insert);
    serialize_row(row_to_insert,row_slot(table->num_rows,table));
    table->num_rows+=1;

    return EXECUTE_SUCCESS; 
}


ExecuteResult execute_select(Statement* statement,Table* table){
    Row row;
    uint32_t i;
    for(i=0;i<table->num_rows;i++){
        deserialize_row(row_slot(i,table),&row);
        print_row(&row);
    }
    return EXECUTE_SUCCESS;
}




 