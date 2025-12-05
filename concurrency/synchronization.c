/// Question: what are all possible terminal output sequences
///
/// The thread t is created and begins running the function square. 
/// After the call to pthread_create(), the main thread prints ++i
/// and waits for the main thread to finish.
/// 
/// There is a subtle rrace condition here. i is shared across both 
/// threads. Since both the main thread and the spawned thread are 
/// mutating the same data, they may read/write prior to each others
/// write back depending on how the os schedules these.
///
/// The first options are purely sequention. 
/// 1: square executes first printing 4, then 3 is printed from the main thread
/// 2: 3 is printed first, to which 9 is printed in square because of the mutation
/// 3: It could aslo be that the increment and the print in square() happen before 
//     the main thread can print, causing 9 then 3
///   
///
/// In the race condition situation, we could have 3 printed from the main thread, 
/// but the square already simultaneously printed 4.
///
/// So the possible ouputs given a non deterministic scheduler are 34, 43, 39, 93


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