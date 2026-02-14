/*
Lab OS 2 / Exercise 2 / Question 1 / 2021-22
Name: Athanasiou Vasileios Evangelos
Student ID: 19390005
Compilation method: gcc -o pthread pthread.c -pthread
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/types.h>

/***************************************************************************** The "vector" structure ***********************************************************************************************/
typedef struct                             
{
    int *A;                                                                                    // Declaration of a pointer A of type "int" that will point to the integer "vector" A.
    int *B;                                                                                    // Declaration of a pointer B of type "int" that will point to the integer "vector" B.
    int N;                                                                                     // Declaration of the size of vectors A and B.
    int i;                                                                                     // Declaration of an integer counter used as an identifier for each "thread" that will lock the "mutex".
    int num_threads;                                                                           // Declaration of the number of "threads" that will be created for the calculation of the inner product A * B (a1b1 + a2b2 + …… + anbn).
} vector;
/***********************************************************************************************************************************************************************************************/

/****************************************************************************** Declaration of shared resources ****************************************************************************************/
int first_elements;                                                                            // Declaration of an integer global-shared variable, which will be updated each time a "thread" uses it to identify the terms of the inner product of the "vectors" it is responsible for, to calculate their sum.
int last_element;                                                                              // Declaration of an integer global-shared variable, which will be updated each time a "thread" uses it to define from which term of the inner product of "vectors" A and B the next "thread" that locks the "mutex" should start calculating its corresponding sum.
int total_sum;                                                                                 // Declaration of the integer global-shared variable, which will be updated each time a "thread" uses it to add the sum of some terms of the inner product A * B (a1b1 + a2b2 + …… + anbn) that it calculated.
/***********************************************************************************************************************************************************************************************/

/****************************************************************************** Declaration and static initialization of "mutex" *******************************************************************/ 
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;                                               // Initialization of a global variable of type structure "pthread_mutex_t" with the constant PTHREAD_MUTEX_INITIALIZER, which is included from the "POSIX Threads" libraries (lines 11-12), meaning the "mutex" (lines 182-192) that is used to protect the shared resources "first_elements, last_element, total_sum" (lines 26-28) in the critical section of the "threads" from being used simultaneously by two or more "threads".
/***********************************************************************************************************************************************************************************************/

/****************************************************************************** Declaration of the critical section of the "threads" *********************************************************************/
void *inner_product (void *);                                                                  // Declaration of the "inner_product" function (critical section), which will be called by the "threads" that will be created and is implemented in lines 167-201.
/***********************************************************************************************************************************************************************************************/

int main (int argc, char *argv[])
{
    pthread_t *pthreads;                                                                       // Declaration of a pointer of type structure "pthread_t" (included from the "POSIX Threads" libraries (lines 11-12)) that will point to the "pthreads" array, which will contain objects of type structure "pthread_t", meaning the "threads" that will be created.
    vector v;                                                                                  // Declaration of an object of type "vector", to be passed as a parameter with its attributes to the "inner_product" function that the "threads" will call (line 121).
    int p;                                                                                     // Declaration of an integer variable for storing the number of "threads" that will be created. 
    int u, m, k, l, w;                                                                         // Declaration of integer counters for executing loops (lines 92-96, 102-103, 108-112, 114-118, 127-128 respectively).
    
    srand (time (NULL));                                                                       // Call to the "srand ()" function from the "time.h" library to set the reference point (the current time) for generating pseudo-random integer numbers.
    
    printf ("Input the number of threads : ");
    scanf ("%d", &p);                                                                          // Reads from the "stdin" channel and the address of the variable "p", the number of "threads" that will be created.
    v.num_threads = p;                                                                         // Assigning the number of "threads" to be created to the "num_threads" attribute of the "vector" object.
    pthreads = (pthread_t *) malloc (p * sizeof (pthread_t));                                  // Dynamic memory allocation using the "pthreads" pointer to create the array that will contain objects of type "pthread_t", meaning the "threads" that will be created.
    if (!pthreads)                                                                             /* Checking if the "pthreads" pointer points to "NULL". */
    {
        perror ("Error in allocating heap memory");                                            // Prints an error message to the "stderr" channel for the failed memory allocation in line 51.
        exit (1);                                                                              // The process terminates with an "exit value" of 1. 
    }
    printf ("\n");
    do                                                                                         /* Loop for entering the size of "vectors" A and B. */
    {
        system ("clear");
        printf ("Number of threads is %d.\n", v.num_threads);                                  // Prints the number of "threads" to the "stdout" channel.
        printf ("------------------------------------------------------------------------------------------------------------------------------------\n");
        printf ("(!) Size of vectors must be an integer multiple of the number of threads (Size %% Num_threads == 0).\n");
        printf ("(!) If size of vectors is greater than or equal to 50, then each vector's element will be filled with random integer numbers from 0 to 10.\n");
        printf ("------------------------------------------------------------------------------------------------------------------------------------\n");
        printf ("Input size of vectors : ");
        scanf ("%d", &v.N);                                                                    // Reads from the "stdin" channel and the address of the variable "v.N" (attribute of the "vector" object), the size of the vectors A and B.
    }
    while (v.N % p != 0);                                                                      /* Checks if the size of vectors A and B is an integer multiple of the number of "threads" that will be created. */
        
/****************************************************************************** Initialization of shared resources **********************************************************************************/    
    first_elements = 0;                                                                        // Initialization of the global-shared variable with the value 0, for the position of the first elements that the first "thread" that locks the "mutex" (lines 182-192) will use for calculating its corresponding sum.
    last_element = v.N / v.num_threads;                                                        // Initialization of the global-shared variable with the value "size of vectors / number of threads that will be created", for the position of the last elements that the first "thread" that locks the "mutex" (lines 182-192) will use to calculate its corresponding sum.
    total_sum = 0;                                                                             // Initialization of the global-shared variable with the value 0, for the inner product of vectors A and B.
/***********************************************************************************************************************************************************************************************/
    
    v.A = (int *) malloc (v.N * sizeof (int));                                                 // Dynamic memory allocation using the "v.A" pointer (attribute of the "vector" object) to create the "vector" A, which will contain "v.N" (attribute of the "vector" object) integer numbers.
    if (!v.A)                                                                                  /* Checking if the "v.A" pointer (attribute of the "vector" object) points to "NULL". */
    {
        perror ("Error in allocating heap memory");                                            // Prints an error message to the "stderr" channel for the failed memory allocation in line 77.
        exit (1);                                                                              // The process terminates with an "exit value" of 1.
    }
    v.B = (int *) malloc (v.N * sizeof (int));                                                 // Dynamic memory allocation using the "v.B" pointer (attribute of the "vector" object) to create the "vector" B, which will contain "v.N" (attribute of the "vector" object) integer numbers.
    if (!v.B)                                                                                  /* Checking if the "v.B" pointer (attribute of the "vector" object) points to "NULL". */
    {
        perror ("Error in allocating heap memory");                                            // Prints an error message to the "stderr" channel for the failed memory allocation in line 83.
        exit (1);                                                                              // The process terminates with an "exit value" of 1.
    }
    printf ("\n");
    if (v.N >= 50)                                                                             /* Checking if the size of the vectors is greater than or equal to 50. */
    {
        for (u = 0; u < v.N; u++)                                                              /* Loop for assigning integer numbers to the vectors. */
        {
            v.A[u] = rand () % 11;                                                             // Assigning a pseudo-random number from the range [0,10] to each element of vector A using the "rand ()" function from the "stdlib.h" library.
            v.B[u] = rand () % 11;                                                             // Assigning a pseudo-random number from the range [0,10] to each element of vector B using the "rand ()" function from the "stdlib.h" library.
        }
        system ("clear");
        printf ("Size of each vector is %d.\n", v.N);                                          // Prints the size of the vectors to the "stdout" channel.
        printf ("Number of threads is %d.\n", v.num_threads);                                  // Prints the number of "threads" that will be created to the "stdout" channel.
		printf ("Each thread will add %d / %d = %d terms ", v.N, v.num_threads, last_element); // Prints the size of the "vectors", the number of "threads" that will be created, and the number of terms that each "thread" will calculate in the sum of the inner product.
		printf ("of the inner product A * B (a1b1 + a2b2 + …… + anbn).\n\n");
		for (m = 0; m < v.N; m++)                                                              /* Loop for printing the elements of the "vectors". */
			printf ("A[%-5d] : %-5d  B[%d] : %d\n", m + 1, v.A[m], m + 1, v.B[m]);             // Prints the contents of vectors A and B to the "stdout" channel.
		printf ("\n");
	}
	else                                                                                       /* The size of the "vectors" is not greater than or equal to 50. */
	{
		for (k = 0; k < v.N; k++)                                                              /* Loop for inputting integer numbers into vector A. */
		{
			printf ("Input element %d of vector A : ", k + 1);                                 // Prints the position of vector A, where a value will be assigned, to the "stdout" channel.
			scanf ("%d", &v.A[k]);                                                             // Reads from the "stdin" channel and the address of the variable "v.A[k]" (attribute of the "vector" structure), the element of position "k" of vector A, and this process repeats until "k >= v.N".
		}
		printf ("\n");
		for (l = 0; l < v.N; l++)                                                              /* Loop for inputting integer numbers into vector B. */
		{
			printf ("Input element %d of vector B : ", l + 1);                                 // Prints the position of vector B, where a value will be assigned, to the "stdout" channel.
			scanf ("%d", &v.B[l]);                                                             // Reads from the "stdin" channel and the address of the variable "v.B[l]" (attribute of the "vector" structure), the element of position "l" of vector B, and this process repeats until "l >= v.N".
		}
		printf ("\n");
	}

	system ("clear");
	printf ("Size of each vector is %d.\n", v.N);                                              // Prints the size of the "vectors" to the "stdout" channel.
	printf ("Number of threads is %d.\n", v.num_threads);                                      // Prints the number of "threads" that will be created to the "stdout" channel.
	printf ("Each thread will add %d / %d = %d terms ", v.N, v.num_threads, last_element);     // Prints the size of the "vectors", the number of "threads" that will be created, and the number of terms that each "thread" will calculate in the sum of the inner product.
	printf ("of the inner product A * B (a1b1 + a2b2 + …… + anbn).\n\n");
	for (w = 0; w < v.N; w++)                                                                  /* Loop for printing the elements of the "vectors". */
		printf ("A[%d] : %-5d  B[%d] : %d\n", w + 1, v.A[w], w + 1, v.B[w]);                   // Prints the contents of vectors A and B to the "stdout" channel.
	printf ("\n");

	for (v.i = 0; v.i < v.num_threads; v.i++)                                                  /* Loop for creating the "threads". */
	{
		if (pthread_create (&pthreads[v.i], NULL, &inner_product, (void *) &v) < 0)            /* Call to the "pthread_create ()" function included from the "POSIX Threads" libraries (lines 11-12), for creating "v.num_threads" (attribute of the "vector" structure) "threads", which will call the "inner_product" function (critical section, lines 167-201) with parameter the object "v" of type "vector" structure and check for return error value. */
		{
			perror ("pthread_create () failed to execute");                                    // Prints an error message to the "stderr" channel for the failed execution of "pthread_create ()".
			exit (1);                                                                          // The process terminates with an "exit value" of 1.
		}
	}

	for (v.i = 0; v.i < p; v.i++)                                                              /* Loop for synchronizing between the "threads". */
	{
		if (pthread_join (pthreads[v.i], NULL) < 0)                                            /* Call to the "pthread_join ()" function included from the "POSIX Threads" libraries (lines 11-12), for synchronizing the "threads", so they can be freed from memory and check for return error value. */
		{
			perror ("pthread_join () failed to execute");                                      // Prints an error message to the "stderr" channel for the failed execution of "pthread_join ()".
			exit (1);                                                                          // The process terminates with an "exit value" of 1.
		}
	}

	if (pthread_mutex_destroy (&mut) < 0)                                                      /* Call to the "pthread_mutex_destroy ()" function included from the "POSIX Threads" libraries (lines 11-12), for destroying the "mutex", so it can be freed from memory (lines 182-192) and which was created in line 32 to protect the shared resources from simultaneous use by two or more "threads" and check for return error value. */
	{
		perror ("pthread_mutex_destroy () failed to execute");                                 // Prints an error message to the "stderr" channel for the failed execution of "pthread_mutex_destroy ()".
		exit (1);                                                                              // The process terminates with an "exit value" of 1.
	}

	printf ("\n");
	printf ("Inner product of A * B (a1b1 + a2b2 + …… + anbn) : %d\n\n", total_sum);           // Prints the content of the global-shared variable "total_sum" to the "stdout" channel, i.e., the inner product of vectors A and B calculated by the "threads".

	free (pthreads);                                                                           // Frees the memory used by the "pthreads" pointer for the creation of the array that contained objects of type structure "pthread_t", i.e., the "threads" that were created.
	free (v.A);                                                                                // Frees the memory used by the "v.A" pointer (attribute of the "vector" structure) for the creation of vector A, which contained "v.N" (attribute of the "vector" structure) integer numbers.
	free (v.B);                                                                                // Frees the memory used by the "v.B" pointer (attribute of the "vector" structure) for the creation of vector B, which contained "v.N" (attribute of the "vector" structure) integer numbers.

	return 0;
}

/****************************************************************************** Κρίσιμο τμήμα **************************************************************************************************/
void *inner_product (void *parameter)                                                          
{
    vector *pV = (vector *) parameter;                                                         // Δήλωση ενός δείκτη τύπου δομής "vector" που δείχνει στην παράμετρο της συνάρτησης, όπου μετατρέπεται από "void *" σε "vector *". Ένας δείκτης που δείχνει στο αντικείμενο που περάστηκε ως παράμετρο κατά την δημιουργία των "threads" (γραμμή 133).
    int j;                                                                                     // Δήλωση ενός ακέραιου μετρητή για την εκτέλεση βρόχου (γραμμές 174-200).
    int local_sum;                                                                             // Δήλωση μιας ακέραιας τοπικής μεταβλητής, όπου εκχωρείται το άθροισμα των όρων που υπολόγισε το κάθε "thread".
    pV->i++;                                                                                   // Αύξηση του ακέραιου μετρητή για το αναγνωριστικό του κάθε "thread" που θα κλειδώνει το "mutex" (γραμμές 182-192), κατά 1. 
    printf ("I am thread #%d.\n", pV->i);                                                      // Τυπώνεται στο κανάλι "stdout" ο ακέραιος μετρητής για το αναγνωριστικό του κάθε "thread" που θα κλειδώνει το "mutex" κατά 1.
    local_sum = 0;                                                                             // Αρχικοποίηση της ακέραιας τοπικής μεταβλητής, όπου εκχωρείται το άθροισμα των όρων που υπολόγισε το κάθε "thread", με την τιμή 0.
    for (j = 0; j < pV->N; j++)                                                                /* Βρόχος για την σάρωση των δύο "διανυσμάτων". */
    {  
        if (pthread_mutex_lock (&mut) < 0)                                                     /* Κλήση της συνάρτησης "pthread_mutex_lock ()" που εισάγεται από τις βιβλιοθήκες των "POSIX Threads" (γραμμές 11-12), για το κλείδωμα του "mutex" (γραμμές 182-192) που δημιουργήθηκε για την προστασία των κοινών πόρων στο κρίσιμο τμήμα από ταυτόχρονη χρήση τους από δύο ή περισσότερα "threads" και έλεγχος για επιστροφή τιμής σφάλματος. */
        {
            perror ("pthread_mutex_lock () failed to execute");                                // Τυπώνεται στο κανάλι "stderr" ένα μήνυμα λάθους για την αποτυχημένη εκτέλεση της "pthread_mutex_lock ()".
            exit (1);                                                                          // Η διεργασία τερματίζεται με "exit status" την τιμή 1.
        }
/******************************************************************************* Mutex *********************************************************************************************************/
        printf ("Thread #%d is locking the mutex.\n", pV->i);                                  // Τυπώνεται στο κανάλι "stdout" ο ακέραιος μετρητής για το αναγνωριστικό του "thread" που κλείδωσε το "mutex".
        printf ("Global variable \"total_sum\" is being updated...\n");
        while (first_elements < last_element)                                                  /* Βρόχος για το άθροισμα των αντίστοιχων όρων που αναλογεί στο "thread" που κλείδωσε το "mutex". */
        {
            local_sum = local_sum + pV->A[first_elements] * pV->B[first_elements];             // Το άθροισμα των όρων που αναλογεί στο "thread" που κλείδωσε το "mutex".
            first_elements++;                                                                  // Ανανεώνεται και ο αριθμός θέσης των πρώτων στοιχείων που θα χρησιμοποιήσει το "thread" στο άθροισμα του μέχρι το στοιχείο με αριθμό θέσης "last_element". 
        }
        total_sum = total_sum + local_sum;                                                     // Εκχώρηση του αθροίσματος που υπολόγισε το "thread" που κλείδωσε το "mutex" στην καθολική-κοινή μεταβλητή "total_sum" για το εσωτερικό γινόμενο των "διανυσμάτων".
        printf ("total_sum : %d\n", total_sum);                                                // Τυπώνεται στο κανάλι "stdout" το περιεχόμενο της "total_sum", δηλαδή, το "thread" ενημερώνει το νέο περιεχόμενο της κοινής μεταβλητής. 
        last_element = last_element + (pV->N / pV->num_threads);                               // Ανανεώνεται και ο αριθμός θέσης των τελευταίων στοιχείων που θα χρησιμοποιήσει το επόμενο "thread", που θα κλειδώσει το "mutex", στο άθροισμα του.
        printf ("Thread #%d is unlocking the mutex.\n\n", pV->i);                              // Τυπώνεται στο κανάλι "stdout" ο ακέραιος μετρητής για το αναγνωριστικό του "thread" που ξεκλειδώνει το "mutex".
/***********************************************************************************************************************************************************************************************/
        if (pthread_mutex_unlock (&mut) < 0)                                                   /* Κλήση της συνάρτησης "pthread_mutex_unlock ()" που εισάγεται από τις βιβλιοθήκες των "POSIX Threads" (γραμμές 11-12), για το ξεκλείδωμα του "mutex" (γραμμές 182-192) που δημιουργήθηκε για την προστασία των κοινών πόρων στο κρίσιμο τμήμα από ταυτόχρονη χρήση τους από δύο ή περισσότερα "threads" και έλεγχος για επιστροφή τιμής σφάλματος. */
        {
            perror ("pthread_mutex_unlock () failed to execute");                              // Τυπώνεται στο κανάλι "stderr" ένα μήνυμα λάθους για την αποτυχημένη εκτέλεση της "pthread_mutex_unlock ()".
            exit (1);                                                                          // Η διεργασία τερματίζεται με "exit status" την τιμή 1.
        }
        pthread_exit (NULL);                                                                   // Το "thread" τερματίζει την λειτουργία του.
    }
}
/***********************************************************************************************************************************************************************************************/

/*  ΕΡΩΤΗΣΕΙΣ 
  
    #1 Υπάρχει κάποιο «κρίσιμο τμήμα» στον κώδικά σας που απαιτεί προστασία ? Αν ναι, γράψτε 
    επιπλέον τον κώδικα για την προστασία του κρίσιμου αυτού τμήματος.
    
    Απάντηση: Το "κρίσιμο τμήμα" που απαιτεί προστασία είναι το τμήμα του κώδικα που τα "threads"
    χρησιμοποιούν τους κοινούς πόρους, που είναι οι καθολικές-κοινές μεταβλητές "first_elements, 
    last_element, total_sum" (δήλωση των μεταβλητών στις γραμμές 26-28, αρχικοποίηση των μεταβλητών
    στις γραμμές 72-74). Σαφώς, για να αποφευχθεί η ταυτόχρονη χρήση των κοινών πόρων από δύο ή 
    περισσότερα "threads", που οδηγεί σε λογικά σφάλματα για τα περιεχόμενα αυτών των μεταβλητών, 
    δημιουργούμε στατικά ένα "mutex" στη γραμμή 31, που θα προστατεύει αυτό το "κρίσιμο τμήμα".
    Με την βοήθεια των συναρτήσεων "pthread_mutex_lock ()" (γραμμή 176) και "pthread_mutex_unlock ()"
    (γραμμή 194) που εισάγονται από τις βιβλιοθήκές των "POSIX Threads" στις γραμμές 11-12, επιτυγχάνεται
    αυτή η προστασία, καθώς με την κλήση της πρώτης (lock) από κάποιο "thread", δίνεται η πρόσβαση 
    στο "mutex", στο "thread" που την κάλεσε αφήνοντας τα άλλα "threads" σε αναμονή μέχρι να γίνει 
    η κλήση της δεύτερης (unlock). Συνεπώς, το κρίσιμο τμήμα που υλοποιείται στις γραμμές 182-192 
    και συνεπώς, την χρήση των κοινών πόρων, δηλαδή, των καθολικών μεταβλητών "first_elements, 
    last_element, total_sum", την κάνει το "thread" που κλείδωσε το "mutex" στη γραμμή 175
    και αφήνεται ελεύθερο για χρήση από άλλο "thread" με το ξεκλείδωμα του στη γραμμή 194./****************************************************************************** Critical Section **************************************************************************************************/
void *inner_product (void *parameter)                                                          
{
    vector *pV = (vector *) parameter;                                                         // Declaration of a pointer of type structure "vector" that points to the function parameter, where it is cast from "void *" to "vector *". A pointer that points to the object passed as a parameter when creating the "threads" (line 133).

    int j;                                                                                     // Declaration of an integer counter for executing a loop (lines 174-200).
    int local_sum;                                                                             // Declaration of an integer local variable, where the sum of the terms calculated by each "thread" is assigned.
    pV->i++;                                                                                   // Increment the integer counter for the identifier of each "thread" that will lock the "mutex" (lines 182-192) by 1. 
    printf ("I am thread #%d.\n", pV->i);                                                      // Prints the integer counter for the identifier of each "thread" that will lock the "mutex" by 1 to the "stdout" channel.
    local_sum = 0;                                                                             // Initialization of the integer local variable, where the sum of the terms calculated by each "thread" is assigned the value 0.
    for (j = 0; j < pV->N; j++)                                                                /* Loop for scanning the two "vectors". */
    {  
        if (pthread_mutex_lock (&mut) < 0)                                                     /* Call to the function "pthread_mutex_lock ()" included from the "POSIX Threads" libraries (lines 11-12), to lock the "mutex" (lines 182-192) created to protect the shared resources in the critical section from simultaneous use by two or more "threads" and check for return error value. */
        {
            perror ("pthread_mutex_lock () failed to execute");                                // Prints an error message to the "stderr" channel for the failed execution of "pthread_mutex_lock ()".
            exit (1);                                                                          // The process terminates with an "exit status" of 1.
        }
/******************************************************************************* Mutex *********************************************************************************************************/
        printf ("Thread #%d is locking the mutex.\n", pV->i);                                  // Prints the integer counter for the identifier of the "thread" that locked the "mutex" to the "stdout" channel.
        printf ("Global variable \"total_sum\" is being updated...\n");
        while (first_elements < last_element)                                                  /* Loop for summing the corresponding terms that correspond to the "thread" that locked the "mutex". */
        {
            local_sum = local_sum + pV->A[first_elements] * pV->B[first_elements];             // The sum of the terms corresponding to the "thread" that locked the "mutex".
            first_elements++;                                                                  // Updates the position number of the first elements that the "thread" will use in the sum until the element with position number "last_element". 
        }
        total_sum = total_sum + local_sum;                                                     // Assigns the sum calculated by the "thread" that locked the "mutex" to the global-shared variable "total_sum" for the inner product of the "vectors".
        printf ("total_sum : %d\n", total_sum);                                                // Prints the content of "total_sum", i.e., the "thread" updates the new content of the shared variable to the "stdout" channel. 
        last_element = last_element + (pV->N / pV->num_threads);                               // Updates the position number of the last elements that the next "thread" will use, which will lock the "mutex", in the sum.
        printf ("Thread #%d is unlocking the mutex.\n\n", pV->i);                              // Prints the integer counter for the identifier of the "thread" that unlocks the "mutex" to the "stdout" channel.
/***********************************************************************************************************************************************************************************************/
        if (pthread_mutex_unlock (&mut) < 0)                                                   /* Call to the function "pthread_mutex_unlock ()" included from the "POSIX Threads" libraries (lines 11-12), to unlock the "mutex" (lines 182-192) created to protect the shared resources in the critical section from simultaneous use by two or more "threads" and check for return error value. */
        {
            perror ("pthread_mutex_unlock () failed to execute");                              // Prints an error message to the "stderr" channel for the failed execution of "pthread_mutex_unlock ()".
            exit (1);                                                                          // The process terminates with an "exit status" of 1.
        }
        pthread_exit (NULL);                                                                   // The "thread" terminates its operation.
    }
}
/***********************************************************************************************************************************************************************************************/

/*  QUESTIONS 
  
    #1 Is there any "critical section" in your code that requires protection? If yes, additionally write 
    the code for protecting this critical section.
    
    Answer: The "critical section" that requires protection is the part of the code where the "threads"
    use the shared resources, which are the global-shared variables "first_elements, 
    last_element, total_sum" (declaration of the variables on lines 26-28, initialization of the variables
    on lines 72-74). Clearly, to prevent simultaneous access to the shared resources by two or 
    more "threads", which leads to logical errors in the contents of these variables, we statically create a "mutex" on line 31, which will protect this "critical section".
    With the help of the functions "pthread_mutex_lock ()" (line 176) and "pthread_mutex_unlock ()"
    (line 194) included from the "POSIX Threads" libraries on lines 11-12, this protection is achieved,
    as by calling the first (lock) from a "thread", access to the "mutex" is given to the "thread" that called it, leaving other "threads" waiting until the second (unlock) is called. Thus, the critical section implemented in lines 182-192 
    and therefore, the use of shared resources, i.e., the global variables "first_elements, 
    last_element, total_sum", is handled by the "thread" that locked the "mutex" on line 175
    and is released for use by another "thread" when it unlocks it on line 194. 

    
|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||     

    #2 Try running your program for large and very large 'n' (using input vectors initialized with random numbers) and measure the execution times for one and more than one thread (e.g., 1, 2, 4, and 8 threads).
    
    Answer:   Vectors' size      Threads        Execution Time  
                   100,000            1               3.00 sec.
                   100,000            2               2.51 sec.
                   100,000            4               2.40 sec.
                   100,000            8               2.38 sec.
                 1,000,000            1              22.00 sec.
                 1,000,000            2              21.18 sec.
                 1,000,000            4              21.00 sec.
                 1,000,000            8              21.00 sec.
                10,000,000            1             211.00 sec.
                10,000,000            2             208.00 sec.
                10,000,000            4             208.00 sec.
                10,000,000            8             208.00 sec.
                
    SAMPLE RUNS
    
    #1 ./pthread
    Input the number of threads : 5
    
    ........................................system ("clear") (Clear screen)........................................................
    
    Number of threads is 5.
    ------------------------------------------------------------------------------------------------------------------------------------
    (!) Size of vectors must be an integer multiple of the number of threads (Size % Num_threads == 0).
    (!) If the size of vectors is greater than or equal to 50, then each vector's place will be filled with random integer numbers from 0 to 10.
    ------------------------------------------------------------------------------------------------------------------------------------
    Input size of vectors : 8                               // The size of the vectors is not an integer multiple of the number of "threads" (8 % 5 == 3 != 0). 
    
    ........................................system ("clear") (Clear screen)........................................................
    
    Number of threads is 5.
    ------------------------------------------------------------------------------------------------------------------------------------
    (!) Size of vectors must be an integer multiple of the number of threads (Size % Num_threads == 0).
    (!) If the size of vectors is greater than or equal to 50, then each vector's place will be filled with random integer numbers from 0 to 10.
    ------------------------------------------------------------------------------------------------------------------------------------
    Input size of vectors : 10                              // The size of the vectors is an integer multiple of the number of "threads" (10 % 5 == 0 == 0).

    Input element 1 of vector A : 2
    Input element 2 of vector A : 45 
    Input element 3 of vector A : 12
    Input element 4 of vector A : 178
    Input element 5 of vector A : 42
    Input element 6 of vector A : 13
    Input element 7 of vector A : 43
    Input element 8 of vector A : 21
    Input element 9 of vector A : 56
    Input element 10 of vector A : 3 

    Input element 1 of vector B : 45
    Input element 2 of vector B : 6
    Input element 3 of vector B : 12
    Input element 4 of vector B : 0
    Input element 5 of vector B : 33
    Input element 6 of vector B : 34
    Input element 7 of vector B : 56
    Input element 8 of vector B : 43
    Input element 9 of vector B : 23
    Input element 10 of vector B : 12
    
    ........................................system ("clear") (Clear screen)........................................................
    
    Size of each vector is 10.
    Number of threads is 5.
    Each thread will add 10 / 5 = 2 terms of the inner product A * B (a1b1 + a2b2 + …… + anbn).
    
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    A[1] : 2      B[1] : 45                                  // Thread #1
    A[2] : 45     B[2] : 6
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------   
    A[3] : 12     B[3] : 12                                  // Thread #2
    A[4] : 178    B[4] : 0
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    A[5] : 42     B[5] : 33                                  // Thread #3
    A[6] : 13     B[6] : 34
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    A[7] : 43     B[7] : 56                                  // Thread #4
    A[8] : 21     B[8] : 43
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    A[9] : 56     B[9] : 23                                  // Thread #5 
    A[10] : 3      B[10] : 12
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    I am thread #1.                                          
    Thread #1 is locking the mutex.                          // local_sum1 = A[1] * B[1] + A[2] * B[2] = 2 * 45 + 45 * 6 = 360 
    Global variable "total_sum" is being updated...
    total_sum : 360                                          // total_sum = 0 + local_sum1 = 0 + 360 = 360
    Thread #1 is unlocking the mutex.
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    I am thread #2.                                           
    Thread #2 is locking the mutex.                          // local_sum2 = A[3] * B[3] + A[4] * B[4] = 12 * 12 + 178 * 0 = 144 
    Global variable "total_sum" is being updated...
    total_sum : 504                                          // total_sum = local_sum1 + local_sum2 = 360 + 144 = 504
    Thread #2 is unlocking the mutex.
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    I am thread #3.                                          
    Thread #3 is locking the mutex.                          // local_sum3 = A[5] * B[5] + A[6] * B[6] = 42 * 33 + 13 * 34 = 1,828
    Global variable "total_sum" is being updated...
    total_sum : 2332                                         // total_sum = (local_sum1 + local_sum2) + local_sum3 = (360 + 144) + 1,828 = 2,332
    Thread #3 is unlocking the mutex.
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    I am thread #4.                                         
    Thread #4 is locking the mutex.                          // local_sum4 = A[7] * B[7] + A[8] * B[8] = 43 * 56 + 21 * 43 = 3,311
    Global variable "total_sum" is being updated...
    total_sum : 5643                                         // total_sum = (local_sum1 + local_sum2 + local_sum3) + local_sum4 = (360 + 144 + 1,828) + 3,311 = 5,643
    Thread #4 is unlocking the mutex.
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    I am thread #5.                                          
    Thread #5 is locking the mutex.                          // local_sum5 = A[9] * B[9] + A[10] * B[10] = 56 * 23 + 3 * 12 = 1,324
    Global variable "total_sum" is being updated...
    total_sum : 6967                                         // total_sum = (local_sum1 + local_sum2 + local_sum3 + local_sum4) + local_sum5 = (360 + 144 + 1,828 + 3,311) + 1,324 = 6,967
    Thread #5 is unlocking the mutex.
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    Inner product of A * B (a1b1 + a2b2 + …… + anbn) : 6967  // total_sum = local_sum1 + local_sum2 + local_sum3 + local_sum4 + local_sum5 = 360 + 144 + 1,828 + 3,331 + 1,324 = 6,967
    
|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||   
    
    #2 ./LS2-19390005-Δ4-Β-2-1
    Input the number of threads : 10
    
    ........................................system ("clear") (Clear screen)........................................................
    
    Number of threads is 10.
    ------------------------------------------------------------------------------------------------------------------------------------
    (!) Size of vectors must be an integer multiple of the number of threads (Size % Num_threads == 0).
    (!) If the size of vectors is greater than or equal to 50, then each vector's place will be filled with random integer numbers from 0 to 10.
    ------------------------------------------------------------------------------------------------------------------------------------
    Input size of vectors : 50                               // The size of the vectors is an integer multiple of the number of "threads" (50 % 10 == 0 == 0). 
   
    ........................................system ("clear") (Clear screen)........................................................
    
    Size of each vector is 50.
    Number of threads is 10.
    Each thread will add 50 / 10 = 5 terms of the inner product A * B (a1b1 + a2b2 + …… + anbn).
    
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    A[1] : 6      B[1] : 5                                   // Thread #1
    A[2] : 2      B[2] : 2
    A[3] : 2      B[3] : 1
    A[4] : 7      B[4] : 1
    A[5] : 9      B[5] : 3
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    A[6] : 6      B[6] : 0                                   // Thread #2
    A[7] : 6      B[7] : 10
    A[8] : 6      B[8] : 1
    A[9] : 0      B[9] : 7
    A[10] : 3      B[10] : 5
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    A[11] : 5      B[11] : 7                                 // Thread #3
    A[12] : 8      B[12] : 1
    A[13] : 10     B[13] : 4
    A[14] : 10     B[14] : 4
    A[15] : 6      B[15] : 9
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    A[16] : 9      B[16] : 1                                 // Thread #4
    A[17] : 3      B[17] : 1
    A[18] : 1      B[18] : 3
    A[19] : 0      B[19] : 9
    A[20] : 4      B[20] : 9
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    A[21] : 1      B[21] : 8                                 // Thread #5
    A[22] : 8      B[22] : 5
    A[23] : 8      B[23] : 3
    A[24] : 4      B[24] : 6
    A[25] : 8      B[25] : 7
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    A[26] : 0      B[26] : 0                                 // Thread #6
    A[27] : 1      B[27] : 9
    A[28] : 10     B[28] : 0
    A[29] : 2      B[29] : 10
    A[30] : 4      B[30] : 8
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    A[31] : 8      B[31] : 1                                 // Thread #7
    A[32] : 7      B[32] : 10
    A[33] : 0      B[33] : 9
    A[34] : 0      B[34] : 9
    A[35] : 7      B[35] : 5
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    A[36] : 6      B[36] : 6                                 // Thread #8
    A[37] : 0      B[37] : 3
    A[38] : 0      B[38] : 6
    A[39] : 4      B[39] : 4
    A[40] : 1      B[40] : 10
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    A[41] : 9      B[41] : 0                                 // Thread #9
    A[42] : 10     B[42] : 8
    A[43] : 9      B[43] : 10
    A[44] : 9      B[44] : 9
    A[45] : 9      B[45] : 0
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    A[46] : 4      B[46] : 4                                 // Thread #10
    A[47] : 10     B[47] : 9
    A[48] : 1      B[48] : 10
    A[49] : 7      B[49] : 2
    A[50] : 7      B[50] : 1
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    I am thread #1.                                         
    Thread #1 is locking the mutex.                          // local_sum1 = A[1] * B[1] + A[2] * B[2] + A[3] * B[3] + A[4] * B[4] + A[5] * B[5] = 6 * 5 + 2 * 2 + 2 * 1 + 7 * 1 + 9 * 3 = 70 
    Global variable "total_sum" is being updated...
    total_sum : 70                                           // total_sum = 0 + local_sum1 = 0 + 70 = 70
    Thread #1 is unlocking the mutex.
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    I am thread #2.
    Thread #2 is locking the mutex.                          // local_sum2 = A[6] * B[6] + A[7] * B[7] + A[8] * B[8] + A[9] * B[9] + A[10] * B[10] = 6 * 0 + 6 * 10 + 6 * 1 + 0 * 7 + 3 * 5 = 81 
    Global variable "total_sum" is being updated...
    total_sum : 151                                          // total_sum = local_sum1 + local_sum2 = 70 + 81 = 151
    Thread #2 is unlocking the mutex.
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    I am thread #3.
    Thread #3 is locking the mutex.                          // local_sum3 = A[11] * B[11] + A[12] * B[12] + A[13] * B[13] + A[14] * B[14] + A[15] * B[15] = 5 * 7 + 8 * 1 + 10 * 4 + 10 * 4 + 6 * 9 = 177
    Global variable "total_sum" is being updated...
    total_sum : 328                                          // total_sum = (local_sum1 + local_sum2) + local_sum3 = 151 + 177 = 328
    Thread #3 is unlocking the mutex.
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    I am thread #4.
    Thread #4 is locking the mutex.                          // local_sum4 = A[16] * B[16] + A[17] * B[17] + A[18] * B[18] + A[19] * B[19] + A[20] * B[20] = 9 * 1 + 3 * 1 + 1 * 3 + 0 * 9 + 4 * 9 = 51
    Global variable "total_sum" is being updated...
    total_sum : 379                                          // total_sum = (local_sum1 + local_sum2 + local_sum3) + local_sum4 = 328 + 51 = 379
    Thread #4 is unlocking the mutex.
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    I am thread #5.
    Thread #5 is locking the mutex.                          // local_sum5 = A[21] * B[21] + A[22] * B[22] + A[23] * B[23] + A[24] * B[24] + A[25] * B[25] = 1 * 8 + 8 * 5 + 8 * 3 + 4 * 6 + 8 * 7 = 152
    Global variable "total_sum" is being updated...
    total_sum : 531                                          // total_sum = (local_sum1 + local_sum2 + local_sum3 + local_sum4) + local_sum5 = 379 + 152 = 531
    Thread #5 is unlocking the mutex.
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    I am thread #6.
    Thread #6 is locking the mutex.                          // local_sum6 = A[26] * B[26] + A[27] * B[27] + A[28] * B[28] + A[29] * B[29] + A[30] * B[30] = 0 * 0 + 1 * 9 + 10 * 0 + 2 * 10 + 4 * 8 = 61
    Global variable "total_sum" is being updated...
    total_sum : 592                                          // total_sum = (local_sum1 + local_sum2 + local_sum3 + local_sum4 + local_sum5) + local_sum6 = 531 + 61 = 592
    Thread #6 is unlocking the mutex.
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    I am thread #7.
    Thread #7 is locking the mutex.                          // local_sum7 = A[31] * B[31] + A[32] * B[32] + A[33] * B[33] + A[34] * B[34] + A[35] * B[35] = 8 * 1 + 7 * 10 + 0 * 9 + 0 * 9 + 7 * 5 = 113
    Global variable "total_sum" is being updated...
    total_sum : 705                                          // total_sum = (local_sum1 + local_sum2 + local_sum3 + local_sum4 + local_sum5 + local_sum6) + local_sum7 = 592 + 113 = 705
    Thread #7 is unlocking the mutex.
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    I am thread #8.
    Thread #8 is locking the mutex.                          // local_sum8 = A[36] * B[36] + A[37] * B[37] + A[38] * B[38] + A[39] * B[39] + A[40] * B[40] = 6 * 6 + 0 * 3 + 0 * 6 + 4 * 4 + 1 * 10 = 62
    Global variable "total_sum" is being updated...
    total_sum : 767                                          // total_sum = (local_sum1 + local_sum2 + local_sum3 + local_sum4 + local_sum5 + local_sum6 + local_sum7) + local_sum8 = 705 + 62 = 767
    Thread #8 is unlocking the mutex.
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    I am thread #9.
    Thread #9 is locking the mutex.                          // local_sum9 = A[41] * B[41] + A[42] * B[42] + A[43] * B[43] + A[44] * B[44] + A[45] * B[45] = 9 * 0 + 10 * 8 + 9 * 10 + 9 * 9 + 9 * 0 = 251
    Global variable "total_sum" is being updated...
    total_sum : 1018                                         // total_sum = (local_sum1 + local_sum2 + local_sum3 + local_sum4 + local_sum5 + local_sum6 + local_sum7 + local_sum8) + local_sum9 = 767 + 251 = 1,018
    Thread #9 is unlocking the mutex.
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    I am thread #10.
    Thread #10 is locking the mutex.                         // local_sum10 = A[46] * B[46] + A[47] * B[47] + A[48] * B[48] + A[49] * B[49] + A[50] * B[50] = 4 * 4 + 10 * 9 + 1 * 10 + 7 * 2 + 7 * 1 = 137
    Global variable "total_sum" is being updated...
    total_sum : 1155                                         // total_sum = (local_sum1 + local_sum2 + local_sum3 + local_sum4 + local_sum5 + local_sum6 + local_sum7 + local_sum8 + local_sum9) + local_sum10 = 1,018 + 137 = 1,155 
    Thread #10 is unlocking the mutex.
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Inner product of A * B (a1b1 + a2b2 + …… + anbn) : 1155      // total_sum = (local_sum1 + local_sum2 + local_sum3 + local_sum4 + local_sum5 + local_sum6 + local_sum7 + local_sum8 + local_sum9 + local_sum10 = 70 + 81 + 177 + 51 + 152 + 61 + 113 + 62 + 251 + 137 = 1,155
*/
    
            
