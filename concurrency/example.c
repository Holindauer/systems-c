/// Assuming none of the system calls fail, when the thread 
/// p1 prints  what balance value p1 will print? 
///
/// The answer is 200 because the parent thread will wait for
/// each spawned thread to exit before spawning the next, so 
/// there is no opportunity for race condition.

volatile int balance = 0;

void *mythread(void *arg) {
    for (int i = 0; i < 200; i++)
        balance++;

    printf("Balance is %d\n", balance);
    return NULL;
}

void main(int argc, char *argv[]) {
    pthread_t p1, p2, p3;
    pthread_create(&p1, NULL, mythread, "A");
    pthread_join(p1, NULL);
    pthread_create(&p2, NULL, mythread, "B");
    pthread_join(p2, NULL);
    pthread_create(&p3, NULL, mythread, "C");
    pthread_join(p3, NULL);

    printf("Final Balance is %d\n", balance);
}
