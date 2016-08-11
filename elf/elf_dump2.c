#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <libelf.h>
#include <gelf.h>
#include <unistd.h>
#include <sys/auxv.h>


void
main(int argc, char **argv)
{
    Elf         *elf;
    Elf_Scn     *scn = NULL;
    GElf_Shdr   shdr;
    Elf_Data    *data;
    GElf_Ehdr *ehdr;
    size_t shstrndx;
    int         fd, ii, count;
    char *name;

    ehdr = (void *) getauxval(AT_SYSINFO_EHDR);

    if (elf_getshdrstrndx(elf, &shstrndx) != 0) {
        printf("Error : elf_getshdrstrndx\n");
        goto out_close;
    }

    while ((scn = elf_nextscn(elf, scn)) != NULL) {
        gelf_getshdr(scn, &shdr);
        name = elf_strptr(ehdr, ehdr->e_shstrndx, shdr.sh_name);
        printf("name = %s\n, addr = 0x%x\n", name, (unsigned)shdr.sh_addr);
    }
out_close:
    elf_end(elf);
}

