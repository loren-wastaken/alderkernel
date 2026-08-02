#include "paging.h"

#define PAGE_SIZE          4096
#define ENTRIES_PER_TABLE  1024
#define IDENTITY_MAP_MB    128   // MUST match pmm.c's MAX_MANAGED_MEMORY
#define TABLES_NEEDED      (IDENTITY_MAP_MB / 4) // each table covers 4MB

#define PAGE_PRESENT 0x1
#define PAGE_RW      0x2

static unsigned int page_directory[ENTRIES_PER_TABLE] __attribute__((aligned(4096)));
static unsigned int page_tables[TABLES_NEEDED][ENTRIES_PER_TABLE] __attribute__((aligned(4096)));

static void enable_paging(unsigned int page_directory_phys)
{
    asm volatile("mov %0, %%cr3" :: "r"(page_directory_phys) : "memory");

    unsigned int cr0;
    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000; // set the PG bit
    asm volatile("mov %0, %%cr0" :: "r"(cr0) : "memory");
}

void paging_init(void)
{
    for (unsigned int table = 0; table < TABLES_NEEDED; table++) {
        for (unsigned int entry = 0; entry < ENTRIES_PER_TABLE; entry++) {
            unsigned int phys = (table * ENTRIES_PER_TABLE + entry) * PAGE_SIZE;
            page_tables[table][entry] = phys | PAGE_PRESENT | PAGE_RW;
        }
        page_directory[table] = ((unsigned int)&page_tables[table][0]) | PAGE_PRESENT | PAGE_RW;
    }

    for (unsigned int table = TABLES_NEEDED; table < ENTRIES_PER_TABLE; table++) {
        page_directory[table] = 0; // not present - touching this = page fault (vector 14)
    }

    enable_paging((unsigned int)&page_directory[0]);
}