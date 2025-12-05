/// Suppose the file foo.txt contains letters, and bar.txt contains numbers.
/// The dup2(oldfd, newfd) function copies (per-process) descriptor table
/// entry oldfd to entry newfd.
///
/// In the parent process, the foo.txt file is opened twice, returning file 
/// descriptors fda and fdb. The bar.txt file is opened, returning file 
/// descriptor fdc.
///
/// The conditional calls fork. The child process enters the conditional branch.
/// It calls dup2(fdb, fdc) which makes fdb now refer to the bar.txt instead of 
/// foo.txt. It then reads one byte from fda, fdb, and fdc. Which, after the dup2
/// call is 1 letter, and 2 number. The branch scope is dropped, then dup2 copies
/// the descriptor of fdb into fda. At this point, all file descriptors refer to 
/// bar.txt. Which causes 3 numbers to be read. fdb is closed, and reopened as 
/// bar.txt, then 3 numbers are read again. The child process in total reads 1 
/// letter and 8 numbers.
///
/// The parent process skips the conditional, waits for the forked process to exit,
/// then dups fdb into fda, resuling in fda, fdb, fdc refering to foo, foo, bar.
/// This reads two letters, one number. fdb is closed and reopened as bar.txt. 
/// Which results in 1 letter, then 2 numbers. Totalling 3 letters, 3 numbers.
int main () {
    int fda, fdb, fdc, pid;
    char c;

    // open for reading only
    fda = open("foo.txt", O_RDONLY, 0);
    fdb = open("foo.txt", O_RDONLY, 0);
    fdc = open("bar.txt", O_RDONLY, 0);

    if ((pid = fork ()) == 0) {
        dup2(fdb, fdc) ;
        read(fda, &c, 1) ;
        read(fdb, &c, 1) ;
        read(fdc, &c, 1) ;
    }
    if ( pid )
        wait (0) ;
    
    dup2(fda, fdb) ;
    read(fda, &c, 1) ;
    read(fdb, &c, 1) ;
    read(fdc, &c, 1) ;
    close (fdb) ;

    fdb = open("bar.txt", O_RDONLY, 0) ;

    read(fda, &c , 1);
    read(fdb, &c , 1);
    read(fdc, &c , 1);

    exit(0);
}