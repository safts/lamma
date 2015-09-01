
/*
 * The following getCPUTime() function works for most OSes.
 * On OSes that need it, link with librt to get POSIX timers (e.g. AIX, BSD, Cygwin, HP-UX, Linux, and Solaris, but not OSX).
 * Otherwise, the default libraries are sufficient.
 *
 * double startTime, endTime;
 * startTime = getCPUTime();
 * endTime = getCPUTime();
 * fprintf( stderr, "CPU time used = %lf\n", (endTime - startTime) );
 */

double getCPUTime();