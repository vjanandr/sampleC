// http://dev-console.blogspot.in/2012/01/some-knowledge-about-vdso.html

#include 
#include 
#include 
#include 

static void *getsys(char **envp) 
{
   Elf64_auxv_t *auxv;
   
   /* walk past all env pointers */
   while (*envp++ != NULL);

   /* and find ELF auxiliary vectors (if this was an ELF binary) */
   auxv = (Elf64_auxv_t *) envp;
   
   for ( ; auxv->a_type != AT_NULL; auxv++)
     if (auxv->a_type == AT_SYSINFO_EHDR)
       return (void *)auxv->a_un.a_val;
   
   fprintf(stderr, "no AT_SYSINFO_EHDR auxv entry found\n");
   exit(1);
}

int main(int argc, char *argv[], char **envp)
{
   unsigned char buffer[4096];
   void *p;
   
   p=getsys(envp);
   fprintf(stderr, "AT_SYSINFO_EHDR at %p\n",p);
   memcpy(buffer, p, 4096);
   write(1, buffer, 4096);
}
