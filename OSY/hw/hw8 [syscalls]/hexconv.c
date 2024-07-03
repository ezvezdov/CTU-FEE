#define STDIN_FILENO 0
#define STDOUT_FILENO 1

#define SYSCALL_EXIT 1
#define SYSCALL_READ 3
#define SYSCALL_WRITE 4

#define END_OF_STRING_CHAR '\0'
#define HEX_BASE 16

// Write system call
int write(int fd, void *buf, int count){
    // Put the system call sys_write() number 4, in the EAX register.
    // Put the file descriptor in the EBX register.
    // Put the pointer to the output buffer in the ECX register.
    // Put the buffer size, i.e., the number of bytes to write, in the EDX register.
    // The system call returns the actual number of bytes written in the EAX register, in case of error, the error
    //     code is in the EAX register.
    int result = -1;
    
    asm volatile (
        "int $0x80;\n\t"
        : "=a" (result)
        : "a" (SYSCALL_WRITE),"b" (fd), "c" (buf),"d" (count)
        : "memory", "cc"
    );
    return result;
}

// Read system call
int read(int fd, void *buf, int count){
    // Put the system call sys_read() number 3, in the EAX register.
    // Put the file descriptor in the EBX register.
    // Put the pointer to the input buffer in the ECX register.
    // Put the buffer size, i.e., the number of bytes to read, in the EDX register.

    int result = -1;

    asm volatile (
        "int $0x80;\n\t"
        : "=a" (result)
        : "a" (SYSCALL_READ),"b" (fd), "c" (buf),"d" (count)
        : "memory", "cc"
    );
    return result;
}


// Exit system call
// void exit(int status)__attribute__ ((noreturn));
void exit(int status){
    // Put the system call sys_exit() number 1, in the EAX register.
    // Put the exit status in the EBX register.
    asm volatile (
        "mov %0, %%eax;\n\t"
        "mov %1, %%ebx;\n\t"
        "int $0x80;\n\t"
        : 
        : "r" (SYSCALL_EXIT),"r" (status)
        : "memory"
    );
    return;
}

// Get length of string
int strlen(char *buf){
    // Go through the buffer untill buf[i] != '\0'

    int length = 0;
    while (buf[length] != END_OF_STRING_CHAR){
        length++;
    }

    return length;
}
    
void sprintf(char *buf,char *format, unsigned int number){
    
    // Print to buffer number with format
    int buf_index = 0;

    char hex_reversed[20];
    int hex_length = 0;
    unsigned int number_part = 1;
    int format_length = strlen(format);
    for (int i = 0; i < format_length; i++)
    {
        // Check is current char special or not
        if(format[i] != '%' || i + 1 >= format_length || (i + 1 < format_length && format[i+1] != 'x') ){
            buf[buf_index] = format[i];
            buf_index++;
            continue;
        }
        
        // Converting to hex
        do{
            number_part = number % HEX_BASE;
            number -= number_part;
            number /= HEX_BASE;

            hex_reversed[hex_length] = number_part < 10 ? '0' + number_part : 'a' + (number_part - 10);
            
            hex_length++;
        }while(number != 0);

        for(int j = 0; j < hex_length; j++){
            buf[buf_index] = hex_reversed[hex_length - j - 1];
            buf_index++;
        }

        // skip 'x' symbol after '%'
        i++;
    }
    buf[buf_index] = '\0';     
}


int isnum(char ch)
{
    return ch >= '0' && ch <= '9';
}

int isspc(char ch)
{
    return ch == ' ' || ch == '\n';
}

static void print(unsigned num)
{
    char buf[20];
    sprintf(buf, "0x%x\n", num);
    int ret = write(STDOUT_FILENO, buf, strlen(buf));
    if (ret == -1)
        exit(1);
}



int main()
{
    
    char buf[20];
    unsigned num = 0;
    int i;
    int num_digits = 0;
    unsigned chars_to_process = 0;
    
    for (/* no init */; /* no end condition */; i++, chars_to_process--) {
        if (chars_to_process == 0) {
            int ret = read(STDIN_FILENO, buf, sizeof(buf));
            
            if (ret < 0)
                exit(1);
            i = 0;
            chars_to_process = ret;
        }
        if (
            num_digits > 0
            && (chars_to_process == 0 /* EOF */ || !isnum(buf[i]))
        ) {
            
            print(num);
            num_digits = 0;
            num = 0;
        }
        if (
            chars_to_process == 0 /* EOF */
            || (!isspc(buf[i]) && !isnum(buf[i]))
        )
            exit(0);

        if (isnum(buf[i])) {
            num = num * 10 + buf[i] - '0';
            num_digits++;
        }
    }
}

__attribute__((force_align_arg_pointer))
void _start(void)
{
    int main_ret = main();
    exit(main_ret);
    
}
