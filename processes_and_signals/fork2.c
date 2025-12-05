
/// say i is odd
/// 
/// say n=7 
/// The parent process loop has 4 iterations, i={0, 2, 4, 6}
/// 
/// Each time you fork in the parent, you double the processes
/// 
/// if n=-1 you dont fork, 1 process
///
/// if n=1, you have 1 fork, 2 procesess
///
/// if n=3, you now have an additional iter in the parent loop, 
/// which will fork once into two processes that both result in
/// two processes, totaling 4 
///
/// if n=5, 8 
///
/// if n=7, 16
///
/// This pattern continues, forming the patter that for odd integers
/// the number of processes spawned is 2^((n+1)/2)) 
void foo ( int n )
{
    int i ;
    for ( i = n -1; i >=0; i -=2)
    {
        fork () ;
    }
    printf (" hello \ n ") ;
    exit (0) ;
}