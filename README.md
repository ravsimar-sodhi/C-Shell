Shell


* Shell prompt is displayed as <username@hostname:PWD>. PWD is displayed relative to HOME directory, which is set as the directory in which the shell is initially started.

USAGE: make
       ./shell

* Built-in Commands include
** pwd -> returns present working directory
** cd [path] -> changes current working directory to the specified directory, if it exists. Otherwise, and error is shown
** echo -> Displays a string on the console.
** ls [OPTIONS] [PATH] -> displays information about the directory specified in PATH, if it exists, or about the PWD
    *** -l --> displays detailed information
    *** -a --> displays hidden files
** pinfo [PID] -> displays process information about the shell's process, or PID if specified
** nightswatch -n [seconds] [command]-> Periodically executes one of the two commands("dirty","interrupts") every n seconds. Press 'q' to exit.
** setenv var [value] -> Sets value of environment variable "var" to [value]. If variable does not exist, it is created and initialised.
** unsetenv var -> destroys the environment variable var
** jobs -> prints a list of all currently running jobs
** kjob <jobNumber> <signalNumber> -> takes the job id of a running job and sends a signal value to that process
** fg <jobNumber> -> brings a running or a stopped background job with given job number to foreground.
** bg <jobNumber> -> changes a stopped background job to a running background job.
** overkill -> kills all background process at once.
** quit -> exits the shell
