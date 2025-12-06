/// Question: what are all possible terminal output sequences
///
/// The thread t is created and begins running the function square. 
/// After the call to pthread_create(), the main thread prints ++i
/// and waits for the spawned thread to finish.
/// 
/// There is a subtle race condition here. Note that i is on the stack, 
/// not the heap, therefore both threads get their own stack, the spawned
/// thread getting a copy of the address of i. If i was on the heap, it 
/// would be a more straightforward race condition involving mutation of 
/// the same address by two threads. However, what happens in this example
/// is that because the address is subject to mutation in the main loop 
/// prior to being copied into the child threads stack, it may mutate 
/// before it is copied in, or not, resulting in either two or 3 being 
/// passed into the thread handler function.
///
/// There are 4 options
/// 1: square executes first printing 4, then 3 is printed from the main thread
/// 2: 3 is printed first, to which 9 is printed in square because of the mutation
/// 3: It could aslo be that the increment and the print in square() happen before 
///    the main thread can print, causing 9 then 3
/// 4: the order of option #3 is reversed, causing 3 then 9
///
/// So the possible ouputs given a non deterministic scheduler are 34, 43, 39, 93
///
/// A lock would prevent this

void * square(void * x) {
    int * y = x;
    printf("%d", (* y) * (* y)); 
    return NULL;
}

int main () {
    pthread_t t;
    int i = 2;

    pthread_create(&t, NULL, square, &i); 
    
    printf("%d", ++i);

    pthread_join(t, NULL);

    return 0;
}