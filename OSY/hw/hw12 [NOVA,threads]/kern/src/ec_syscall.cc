#include "ec.h"
#include "ptab.h"
#include "bits.h"

#define NBRK_MAX 0xBFFFF000
#define DEBUG 0

typedef enum {
    sys_print      = 1,
    sys_sum        = 2,
    sys_break      = 3,
    sys_thr_create = 4,
    sys_thr_yield  = 5,
} Syscall_numbers;


void Ec::syscall_handler (uint8 a)
{
    // Get access to registers stored during entering the system - see
    // entry_sysenter in entry.S
    Sys_regs * r = current->sys_regs();
    Syscall_numbers number = static_cast<Syscall_numbers> (a);

    switch (number) {
        case sys_print: {
            // Tisk řetězce na sériovou linku
            char *data = reinterpret_cast<char*>(r->esi);
            unsigned len = r->edi;
            for (unsigned i = 0; i < len; i++)
                printf("%c", data[i]);
            break;
        }
        case sys_sum: {
            // Naprosto nepotřebné systémové volání na sečtení dvou čísel
            int first_number = r->esi;
            int second_number = r->edi;
            r->eax = first_number + second_number;
            break;
        }
        case sys_break: {
            // Break syscall 
            // https://www.youtube.com/watch?v=vEXRpiI4Dhk
            // https://osy.pages.fel.cvut.cz/docs/cviceni/lab10/pdf/osy-nova-intro.pdf
            mword address = reinterpret_cast<mword> (r->esi);
            if (DEBUG) printf ("\nSTART nbrk syscall %d with address 0x%lx\n", number,static_cast<mword> (address));
            
            if(address == (mword)NULL || address == Ec::break_current){
                r->eax = Ec::break_current;
                break;
            }
            if(address >= NBRK_MAX || address < Ec::break_min){
                r->eax = (mword)NULL;
                break;
            }
            
            mword virt = align_up(Ec::break_current,PAGE_SIZE);
            mword phys;
            mword attr = Ptab::PRESENT | Ptab::USER | Ptab::RW;

            bool allocation_error = false;

            if(address > virt){
                // alloc new memory
                if (DEBUG) printf("Allocating new memory\n");

                if(address > virt + PAGE_SIZE && Ec::break_current < virt + PAGE_SIZE && virt != Ec::break_current){
                    int *start = reinterpret_cast<int*> (Ec::break_current & (0xfffffffffffffffe));
                    int *end = reinterpret_cast<int*> (min(virt,address));
                    if (DEBUG) printf("ZERO %p - %p\n",start,end);
                    
                    int *mem = start;
                    for(int i = 0; mem + i < end; i++){
                        mem[i] = 0;
                    }
                }
                
                while(address > virt){
                    if (DEBUG) printf("Alloc page 0x%lx - 0x%lx\n",virt, virt + PAGE_SIZE);
                    void *page = Kalloc::allocator.alloc_page (1, Kalloc::FILL_0);
                    if(!page){
                        if (DEBUG) printf("ERROR: ALLOCATING PAGE!\n");
                        allocation_error = true;
                        virt-=PAGE_SIZE;
                        break;
                    }

                    phys = Kalloc::virt2phys(page);

                    if(!Ptab::insert_mapping(virt, phys, attr)){
                        if (DEBUG) printf("ERROR: INSERT MAPPING!\n");
                        if (DEBUG) printf("FREE page 0x%lx - 0x%lx\n",virt, virt + PAGE_SIZE);
                        Kalloc::allocator.free_page(page);
                        allocation_error = true;
                        virt-=PAGE_SIZE;
                        break;
                    }


                    virt += PAGE_SIZE;
                }
                if(allocation_error){
                //Free all allocated memory
                    while (virt >= Ec::break_current){
                        if (DEBUG) printf("FREE MAP 0x%lx - 0x%lx\n",(virt),(virt+PAGE_SIZE));

                        phys = Ptab::get_mapping(virt);
                        void *page = Kalloc::phys2virt(phys  & ~PAGE_MASK);

                        Kalloc::allocator.free_page(page);

                        Ptab::insert_mapping(virt,0,0);
                        
                        virt-=PAGE_SIZE;    
                    };
                    r->eax = (mword)NULL;
                    break;
                }
            }
            else{
                if (DEBUG) printf("Deallocating memory\n");
                virt -= PAGE_SIZE;
                while (virt >= address)
                {
                    if (DEBUG) printf("FREE MAP 0x%lx - 0x%lx\n",virt,virt+PAGE_SIZE);
                    phys = Ptab::get_mapping(virt) ;
                    void *page = Kalloc::phys2virt(phys  & ~PAGE_MASK) ;
                    
                    Kalloc::allocator.free_page(page);
                    Ptab::insert_mapping(virt,0,0);
                    virt-=PAGE_SIZE;
                }

                int *start = reinterpret_cast<int*> (Ec::break_current & (0xfffffffffffffffe));//(align_dn(Ec::break_current,1));//(Ec::break_current & (0xffffffffffffff00));
                int *end = reinterpret_cast<int*> (address);
                int *mem = start;

                for(int i = 0; mem + i < end; i++){
                    if (DEBUG && i == 0) printf("ZERO %p - %p\n",start,end);
                    mem[i] = 0;
                }
            }

        

            
            r->eax = Ec::break_current;
            Ec::break_current = address;

            if (DEBUG) printf("EXIT nbrk syscall\n");
            break;
        }
        case sys_thr_create: {
            mword start_routine = reinterpret_cast<mword> (r->esi);
            mword stack_top = reinterpret_cast<mword> (r->edi);

            // Create new thread
            Ec *new_thread = new Ec();
            if(!new_thread){
                if(DEBUG) printf("ERROR: new thread wasn't created!\n");
                r->eax = 1;
                break;
            }

            // Push new thread into linked list
            new_thread->next = Ec::current->next;
            Ec::current->next = new_thread;
            
            // Setup start_routine and stack_top
            new_thread->regs.edx = start_routine;
            new_thread->regs.ecx = stack_top;

            r->eax = 0;
            break;
        }
        case sys_thr_yield: {
            // Set next thread as current
            Ec::current = Ec::current->next;

            // Do routine
            Ec::current->make_current();
            break;
        }
        default:
            printf ("unknown syscall %d\n", number);
            break;
    };

    ret_user_sysexit();
}