![Alt text](https://upload.wikimedia.org/wikipedia/commons/thumb/a/a5/Flag_of_the_United_Kingdom_%281-2%29.svg/255px-Flag_of_the_United_Kingdom_%281-2%29.svg.png)

# Parallel Inner Product Calculation using C POSIX Threads

For the requested Assignment, click the link:
[Assignment](Assignment/)

For the Source Code with detailed comments, click the link:
[Code](Code/)

## Course Information
- **Course**: [Operating Systems II](https://ice.uniwa.gr/education/undergraduate/courses/operating-systems-ii/)
- **Semester**: 4
- **Program Study**: [UNIWA](https://www.uniwa.gr/)
- **Department**: [Informatics and Computer Engineering](https://ice.uniwa.gr/)
- **Lab Instructor**: [Psarras Nikolaos](https://ice.uniwa.gr/emd_person/20879/)
- **Academic Season**: 2021-2022

## Student Information
- **Name**: Athanasiou Vasileios Evangelos
- **Student ID**: 19390005
- **Status**: Undergraduate

## Assignment Title
**Title**: Parallel Inner Product Calculation using C Threads

## Assignment Overview

This project involves creating a C program that computes the inner product of two vectors, A and B, using multiple threads. The inner product is defined as:

\[ A1 * B1 + A2 * B2 + ... + An * Bn \]

where \( Ai \) and \( Bi \) are the elements of vectors A and B, respectively.

## Objectives

- Implement a parallelized calculation of the inner product using `p` threads.
- Each thread will calculate a partial sum corresponding to \( n/p \) terms.
- Update a shared variable (`total_sum`) with the results from each thread.
- Ensure the critical section is properly protected to prevent race conditions.
- Measure execution times for different numbers of threads (1, 2, 4, and 8).
- Handle user input for vector sizes and elements, either through command line or file input.

## Key Features

- **Dynamic Thread Creation**: The number of threads is determined by user input, allowing for flexibility.
- **Local Sum Calculation**: Each thread calculates its local sum independently before updating the global sum.
- **Mutex Protection**: Critical sections are protected using mutexes to ensure thread safety.
- **Random Initialization**: For larger vector sizes, vectors can be initialized with random numbers to test performance.

## Program Structure

1. **Input Handling**: Read `n` (size of vectors) and `p` (number of threads) from the user.
2. **Dynamic Memory Allocation**: Allocate memory for vectors A and B.
3. **Thread Creation**: Create threads that will each calculate a part of the inner product.
4. **Synchronization**: Use a mutex to protect the shared variable `total_sum`.
5. **Output**: Print the final inner product result.

## Requirements

- **Operating System**: Linux-based or any Unix-like OS.
- **Compiler**: GCC (GNU Compiler Collection).
- **Libraries**: Use of `pthread` for thread management.

## Installation and Usage

### 1. Clone the Repository
Download the repository to your local machine:
```
git clone https://github.com/Operating-Systems-2-aka-Uniwa/Threads.git
```
### 2. Compile the source code
Compile with gcc compiler the source codes and the flag -pthread to activate POSIX threads:
```
gcc -o pthread pthread.c -pthread
```
### 3. Run the code
Execute the executable file:
```
./pthread
```

![Alt text](https://upload.wikimedia.org/wikipedia/commons/thumb/5/5c/Flag_of_Greece.svg/255px-Flag_of_Greece.svg.png)

# Υπολογισμός Εσωτερικού Γινομένου σε Παράλληλη Εκτέλεση με χρήση C POSIX Νημάτων

Για την ζητούμενη Άσκηση, κάντε κλικ στον σύνδεσμο:  
[Άσκηση](Assignment/)

Για τον Κώδικα με λεπτομερή σχολιασμό, κάντε κλικ στον σύνδεσμο:  
[Κώδικας](Code/)

## Πληροφορίες Μαθήματος
- **Μάθημα**: [Λειτουργικά Συστήματα II](https://ice.uniwa.gr/education/undergraduate/courses/operating-systems-ii/)
- **Εξάμηνο**: 4
- **Πρόγραμμα Σπουδών**: [UNIWA](https://www.uniwa.gr/)
- **Τμήμα**: [Πληροφορικής και Μηχανικής Υπολογιστών](https://ice.uniwa.gr/)
- **Εκπαιδευτής Εργαστηρίου**: [Ψαράς Νικόλαος](https://ice.uniwa.gr/emd_person/20879/)
- **Ακαδημαϊκή Χρονιά**: 2021-2022

## Πληροφορίες Φοιτητή
- **Όνομα**: Αθανασίου Βασίλειος Ευάγγελος
- **Αριθμός Μητρώου**: 19390005
- **Κατάσταση**: Προπτυχιακός Φοιτητής

## Τίτλος Άσκησης
**Τίτλος**: Υπολογισμός Εσωτερικού Γινομένου σε Παράλληλη Εκτέλεση με χρήση C POSIX Νημάτων

## Επισκόπηση Άσκησης

Αυτό το έργο περιλαμβάνει τη δημιουργία ενός προγράμματος C που υπολογίζει το εσωτερικό γινόμενο δύο διανυσμάτων, A και B, χρησιμοποιώντας πολλαπλές θερμές. Το εσωτερικό γινόμενο ορίζεται ως:

\[ A1 * B1 + A2 * B2 + ... + An * Bn \]

όπου \( Ai \) και \( Bi \) είναι τα στοιχεία των διανυσμάτων A και B, αντίστοιχα.

## Στόχοι

- Υλοποίηση παραλληλοποιημένου υπολογισμού του εσωτερικού γινομένου χρησιμοποιώντας `p` θερμές.
- Κάθε θερμή θα υπολογίζει ένα μερικό άθροισμα που αντιστοιχεί σε \( n/p \) όρους.
- Ενημέρωση μιας κοινής μεταβλητής (`total_sum`) με τα αποτελέσματα από κάθε θερμή.
- Διασφάλιση ότι η κρίσιμη περιοχή προστατεύεται σωστά για να αποτραπούν οι συνθήκες ταυτόχρονης πρόσβασης.
- Μέτρηση χρόνου εκτέλεσης για διάφορους αριθμούς θερμών (1, 2, 4 και 8).
- Διαχείριση εισόδου από τον χρήστη για τα μεγέθη και τα στοιχεία των διανυσμάτων, είτε μέσω γραμμής εντολών είτε μέσω αρχείου.

## Κύρια Χαρακτηριστικά

- **Δυναμική Δημιουργία Θερμών**: Ο αριθμός των θερμών καθορίζεται από την είσοδο του χρήστη, προσφέροντας ευελιξία.
- **Υπολογισμός Τοπικού Αθροίσματος**: Κάθε θερμή υπολογίζει το τοπικό της άθροισμα ανεξάρτητα πριν ενημερώσει το παγκόσμιο άθροισμα.
- **Προστασία με Mutex**: Οι κρίσιμες περιοχές προστατεύονται με τη χρήση mutex για να διασφαλιστεί η ασφάλεια των θερμών.
- **Τυχαία Αρχικοποίηση**: Για μεγαλύτερα μεγέθη διανυσμάτων, τα διανύσματα μπορούν να αρχικοποιηθούν με τυχαίους αριθμούς για να ελεγχθεί η απόδοση.

## Δομή Προγράμματος

1. **Διαχείριση Εισόδου**: Ανάγνωση του `n` (μεγέθους των διανυσμάτων) και του `p` (αριθμού θερμών) από τον χρήστη.
2. **Δυναμική Δέσμευση Μνήμης**: Δέσμευση μνήμης για τα διανύσματα A και B.
3. **Δημιουργία Θερμών**: Δημιουργία θερμών που θα υπολογίσουν ένα μέρος του εσωτερικού γινομένου.
4. **Συγχρονισμός**: Χρήση mutex για την προστασία της κοινής μεταβλητής `total_sum`.
5. **Έξοδος**: Εκτύπωση του τελικού αποτελέσματος του εσωτερικού γινομένου.

## Απαιτήσεις

- **Λειτουργικό Σύστημα**: Βασισμένο σε Linux ή οποιοδήποτε Unix-like OS.
- **Μεταγλωττιστής**: GCC (GNU Compiler Collection).
- **Βιβλιοθήκες**: Χρήση `pthread` για τη διαχείριση θερμών.

## Εγκατάσταση και Χρήση

### 1. Κλωνοποίηση του Αποθετηρίου
Κατεβάστε το αποθετήριο στο τοπικό σας σύστημα:  
```
git clone https://github.com/Operating-Systems-2-aka-Uniwa/Threads.git
```
### 2. Μεταγλώττιση του κώδικα
Μεταγλωττίστε τον πηγαίο κώδικα με τον μεταγλωττιστή gcc και την επιλογή -pthread για να ενεργοποιήσετε τα POSIX νήματα:
```
gcc -o pthread pthread.c -pthread
```
### 3. Εκτέλεση του κώδικα
Εκτελέστε το εκτελέσιμο αρχείο:  
```
./pthread
```
