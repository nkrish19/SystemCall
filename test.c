#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <linux/fcntl.h>
#include <linux/fs.h>
#include <linux/fcntl.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <stdio.h>

int main() {

    int p;
    char *f;
    
    printf("Enter PID: \n");
    scanf("%d", &p);
    printf("Enter filename: \n");
    scanf("%s", f);

    long int stat = syscall(545, p, f);

    if (stat==0) {
        printf("Succesfull execution\n");
    }
    else {
        printf("Error\n");
        printf("errno %li\n", stat);
    }

    return 0;
}