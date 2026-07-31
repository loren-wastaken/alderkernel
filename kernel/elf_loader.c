/* Minimal ELF loader for ELF32 modules placed by GRUB
 * This loader copies PT_LOAD segments to their p_vaddr and
 * jumps to the ELF entry point. It assumes the ELF is
 * already linked to absolute addresses (no relocations).
 */

#include <stdint.h>
#include "headers/print.h"

#define EI_NIDENT 16

typedef uint32_t Elf32_Addr;
typedef uint32_t Elf32_Off;
typedef uint16_t Elf32_Half;
typedef uint32_t Elf32_Word;

typedef struct {
    unsigned char e_ident[EI_NIDENT];
    Elf32_Half e_type;
    Elf32_Half e_machine;
    Elf32_Word e_version;
    Elf32_Addr e_entry;
    Elf32_Off  e_phoff;
    Elf32_Off  e_shoff;
    Elf32_Word e_flags;
    Elf32_Half e_ehsize;
    Elf32_Half e_phentsize;
    Elf32_Half e_phnum;
    Elf32_Half e_shentsize;
    Elf32_Half e_shnum;
    Elf32_Half e_shstrndx;
} Elf32_Ehdr;

typedef struct {
    Elf32_Word p_type;
    Elf32_Off  p_offset;
    Elf32_Addr p_vaddr;
    Elf32_Addr p_paddr;
    Elf32_Word p_filesz;
    Elf32_Word p_memsz;
    Elf32_Word p_flags;
    Elf32_Word p_align;
} Elf32_Phdr;

#define PT_LOAD 1

/* simple memcpy/memset implementations suitable for freestanding */
void *memcpy(void *dst, const void *src, unsigned int n) {
    unsigned char *d = (unsigned char*)dst;
    const unsigned char *s = (const unsigned char*)src;
    for (unsigned int i = 0; i < n; i++) d[i] = s[i];
    return dst;
}

void *memset(void *dst, int c, unsigned int n) {
    unsigned char *d = (unsigned char*)dst;
    unsigned char cc = (unsigned char)c;
    for (unsigned int i = 0; i < n; i++) d[i] = cc;
    return dst;
}

int elf_load_from_module(uint32_t mod_start)
{
    Elf32_Ehdr *ehdr = (Elf32_Ehdr *)mod_start;
    if (!(ehdr->e_ident[0] == 0x7F && ehdr->e_ident[1] == 'E' && ehdr->e_ident[2] == 'L' && ehdr->e_ident[3] == 'F')) {
        print_text("elf: bad magic\n");
        return -1;
    }

    if (ehdr->e_phoff == 0 || ehdr->e_phnum == 0) {
        print_text("elf: no program headers\n");
        return -1;
    }

    Elf32_Phdr *ph = (Elf32_Phdr *)(mod_start + ehdr->e_phoff);
    for (int i = 0; i < ehdr->e_phnum; i++) {
        if (ph[i].p_type != PT_LOAD) continue;
        void *dest = (void *) (ph[i].p_vaddr);
        void *src  = (void *) (mod_start + ph[i].p_offset);
        // copy file contents into destination vaddr
        memcpy(dest, src, ph[i].p_filesz);
        // zero the rest
        if (ph[i].p_memsz > ph[i].p_filesz) {
            memset((char*)dest + ph[i].p_filesz, 0, ph[i].p_memsz - ph[i].p_filesz);
        }
    }

    void (*entry)(void) = (void(*)(void)) (ehdr->e_entry);
    print_text("elf: jumping to entry\n");
    entry();
    print_text("elf: entry returned\n");
    return 0;
}
