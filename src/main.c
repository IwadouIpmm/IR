#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "../header/mps.h"
#include "../header/common.h"
#include "../header/string.h"


void print_query_result (unsigned int * query_result);

_Bool get_input_from (char * option);
_Bool inverted_index (char * option);
_Bool incidence_matrix (char * option);
_Bool positional_index (char * option);


int main (int argc, char ** args) { // Main function
    _Bool correct_execution = 0; //Checks if the exection of a algorithm is done correctly or not. To corrspend with an appropiate return result

    _STOP_WORDS_ = NULL;
    __DELIMITER__ = NULL;

    if (init_stop_words ()) {
        if (init_delimiter ()) {
            while (1) { //Loop until the user choose to quit the app.
                change_delimiter (" "); //If the delimiter is not white space, then change to white space
                print_logo (); //Print The logo of the App.
                print_algorithm_table (); //Print the table of content of the App.
                
                char option = 0; //A varible which will hold the option that the user choosed.
                
                while (1) { //Loop until the user enters the write choice.
                    printf ("\t\t\t\t\t\t\t\t\tYour Entery: ");
                    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 4);
                    scanf ("%c", &option);
                    fflush (stdin); //Reomve anything from the keyboard buffer.
                    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
                    printf ("\n");

                    if (option >= 49 && option <= 52)
                        break;
                    else {
                        print_error ("The only acceptable values are from 1 to 4.");
                        system ("pause");
                        print_logo ();
                        print_algorithm_table ();
                    }
                }

                if (option == 52) //Check if the choice equals to option EXIT, then quit the app.
                    break;
                
                printf ("\n\n");
                if (option == 49) {
                    if (incidence_matrix (&option)) //Execute Incidence_Matrix Algorithm
                        correct_execution = 1; //If executed_successfuly, set correct_execution varaible to 1.
                    else
                        correct_execution = 0; //Otherwise set the correct_execution variable to 0.
                } else if (option == 50) {
                    if (inverted_index (&option)) //Execute Inverted_index Algorithm
                        correct_execution = 1; //If the execution is a success, set correct_execution varaible to 1.
                    else
                        correct_execution = 0; //Otherwise set the correct_exection varaible to 0.
                } else {
                    if (positional_index (&option)) //Execute Positional_index Algorithm.
                        correct_execution = 1; //If the execution is a success, set the correct_exection variable to 1.
                    else
                        correct_execution = 0; //Otherwise set the correct_execution variable to 0.
                }
            }

            if (correct_execution) { //Depending on the value of the variable return the appropiate value to the OS representing in which stat the program exited on.
                print_bye ();
                system ("pause");
                clear_screen ();
                print_success ("Program execution was successful.");
                system ("pause");
                return EXIT_SUCCESS;
            } else {
                print_error ("The execution was incorrect.");
                system ("pause");
                return EXIT_FAILURE;
            }
        } else {
            print_error ("There is a problem in initializing the delimieter.");
            system ("pause");
            return EXIT_FAILURE;
        }
    } else {
        print_error ("There is a problem in initializing the stop words list.");
        system ("pause");
        return EXIT_FAILURE;
    }
}


/*
INPUT: An array of integers holds the documents.
OUTPUT: Non
INFO: This function will print the query result in an array format
*/
void print_query_result (unsigned int * query_result) {
    unsigned int length = (query_result [0] * 3);
    for (unsigned int counter = 0; counter < query_result [0]; length += n_digits (query_result [(counter + 1)]), ++counter);
    for (unsigned char counter = 0; counter < 49; printf (" "), ++counter);
    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 14);
    for (unsigned int counter = 0; counter < length; printf ("-"), ++counter);
    printf ("\n");
    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);

    printf ("The query was found in the following documents: ");
    

    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 14);
    printf ("|");
    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);

    for (unsigned int counter = 0; counter < query_result [0]; ++counter) {
        printf (" ");
        SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 9);
        printf ("%u", query_result [(counter + 1)]);
        SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
        printf (" ");
        SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 14);
        if ((counter + 1) == query_result [0])
            printf (" |");
        else
            printf ("|");
        SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
    }

    printf ("\n");
    for (unsigned char counter = 0; counter < 49; printf (" "), ++counter);
    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 14);
    for (unsigned int counter = 0; counter < length; printf ("-"), ++counter);
    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
    printf ("\n\n");
}


/*
INPUT: A pointer to a char type.
OUTPUT: 1 if the user wants read data from file, otherwise return 0. Which means the user wants gets data from keyboard.
INFO: This function is used to check from which source the user, wants to gets his data.
*/
_Bool get_input_from (char * option) {
    printf ("Do You Wan To Get The Document Data From File?\n\tPress (");
    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 3);
    printf ("Y");
    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
    printf (", ");
    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 3);
    printf ("y");
    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
    printf (") To Read From File Or Any Character To Enter Data From Keyboard.\n\tYour Entery: ");
    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 4);
    scanf ("%c", option);
    fflush (stdin);

    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
    
    if ((*option) == 'Y' || (*option) == 'y')
        return 1;
    else
        return 0;

}

/*
INPUT: A pointer to a char type.
OUTPUT: Return 1 if the algorithm executed successfully, otherwise return 0.
INFO: This function is used to execute the inverted_index algorthim.
*/
_Bool inverted_index (char * option) {
    return 1;
}


/*
INPUT: A pointer to a char type.
OUTPUT: Return 1 if the algorithm executed successfully, otherwise return 0.
INFO: This function is used to execute the incidence_matrix algorithm.
*/
_Bool incidence_matrix (char * option) {
    return 1;
}


/*
INPUT: A pointer to a char type.
OUTPUT: Return 1 if the algorithm executed successfully, otherwise return 0.
INFO: This function is used to execute the Positional_index algorithm.
*/
_Bool positional_index (char * option) {
    _Bool from_which_input = get_input_from (option); //Get from which stream does the user like to get the data. 1 from files and 0 from keyboard.
    
    printf ("\n\n");

    unsigned int n_docs = 0; //Holds the number of documents
    string ** documents = NULL; //Holds the documents
    unsigned int largest_length = 0; //Holds the largest token term in all of the documents
    unsigned int * n_tokens_per_doc = NULL; //Holds the number of lines in each document at the beginning and then holds the number of tokens in each document

    while (1) { //Change the delimiter.
        printf ("Do you want to change the delimiter?\nPress (");
        SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 2);
        printf ("Y");
        SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
        printf (",");
        SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 2);
        printf ("y");
        SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
        printf (") to change the delimiter, or anything to exist.\n\tYour Entery: ");
        SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 9);
        scanf ("%c", option);
        SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
        fflush (stdin);

        if ((*option) == 'Y' || (*option) == 'y') {
            printf ("\n\nEnter a delimiter: ");
            SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 9);
            string input = check_input (stdin, 10);
            SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);

            if (input != NULL) { //Check if the string was not empty
                change_delimiter (input);
                print_success ("The delimiter has changed.\n");
                system ("pause");
                clear_screen ();
                free (input);
                input = NULL;
                break;
            } else { //The string was empty
                print_error ("The delimiter can not be empty.");
                system ("pause");
                clear_screen ();
            }
        } else
            break;
    }

    
    if (from_which_input) { //Getting the input from files
        n_docs = 10; //The default number of file is 10.
        
        string * files = NULL; //The array which holds the 
        do {files = (string *) malloc (sizeof (string) * n_docs);} while (files == NULL);
        files [0] = "txt/1.txt";
        files [1] = "txt/2.txt";
        files [2] = "txt/3.txt";
        files [3] = "txt/4.txt";
        files [4] = "txt/5.txt";
        files [5] = "txt/6.txt";
        files [6] = "txt/7.txt";
        files [7] = "txt/8.txt";
        files [8] = "txt/9.txt";
        files [9] = "txt/10.txt";

        
        n_docs = get_input_from_files (files, &documents, n_docs, &n_tokens_per_doc); //Read files
        print_processing ();

        //Free files
        free (files);
        files = NULL;

        if (n_docs != 0) { //Checks if the reading was succesful
            print_success ("Files has been read successfully.");
            system ("pause");
            clear_screen ();
        } else { //Reading wasn't successful.
            print_error ("All the files are incorrect. Please enter vaild files to process.");
            system ("pause");
            clear_screen ();
            return 0;
        }
    } else { //Getting the input from keyboard.
        printf ("\n\n");
        SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);

        while (1) {
            printf ("Enter the number of documents you want to scan: ");
            SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 9);
            string input = check_input (stdin, 10);
            SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);

            if (input != NULL) {
                if (is_uint (&input)) {
                    n_docs = to_uint (&input);
                    break;
                } else {
                    print_error ("The input must be a positive number of maximum 10 digits.");
                    system ("pause");
                    clear_screen ();
                }

                free (input);
                input = NULL;
            } else {
                print_error ("The number can not be empty.");
                system ("pause");
                clear_screen ();
            }
        }

        do {documents = (string **) malloc (sizeof (string *) * n_docs);} while (documents == NULL);
        do {n_tokens_per_doc = (unsigned int *) malloc (sizeof (unsigned int) * n_docs);} while (n_tokens_per_doc == NULL);

        for (unsigned int counter = 0; counter < n_docs; ++counter) {
            do {documents [counter] = (string *) malloc (sizeof (string));} while (documents [counter] == NULL);
            n_tokens_per_doc [counter] = 1;
        }

        for (unsigned int counter = 0; counter < n_docs; ++counter) {
            while (1) {
                printf ("Enter Data For DocNo #[");
                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 2);
                printf ("%u", (counter + 1));
                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
                printf ("]: ");
                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 9);
                string input = check_input (stdin, 10);
                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);

                if (input != NULL) {
                    documents [counter][0] = give_me_str_copy (&input);
                    free (input);
                    input = NULL;
                    break;
                } else {
                    print_error ("Document Can't Not Be Empty.");
                    system ("pause");
                    clear_screen ();
                }
            }

            printf ("\n\n");
        }
    }

    printf ("Case falding the documents:\n\n"); //Lower case all of the documents
    Sleep (10);
    print_processing ();

    if (lower_all (&documents, &n_docs, n_tokens_per_doc)) { //case falding all of the documents
        print_success ("All documents has been gone through case folding process.");

        for (unsigned int counter = 0; counter < n_docs; ++counter) {
            printf ("Data From File #[");
            SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 4);
            printf ("%u", (counter + 1));
            SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
            printf ("]:\n");

            for (unsigned int inside = 0; inside < n_tokens_per_doc [counter]; ++inside) {
                printf ("\tLine #[");
                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 2);
                printf ("%u", (inside + 1));
                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
                printf ("] is: ");
                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 9);
                printf ("%s\n\n", documents [counter][inside]);
                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
            }

            printf ("\n");
        }

        system ("pause");
        clear_screen ();

        printf ("Starting the tokenizing process:\n\n");
        void **** content = tokenize (&documents, &n_docs, n_tokens_per_doc);
        print_processing ();

        if (content != NULL) {
            print_success ("Document Data has been tokenized.");
            system ("pause");
            clear_screen ();

            //Free the old document data.
            for (unsigned int counter = 0; counter < n_docs; free (documents [counter]), documents [counter] = NULL, ++counter)
                for (unsigned int inside = 0; inside < n_tokens_per_doc [counter]; free (documents [counter][inside]), documents [counter][inside] = NULL, ++inside);
                    
            free (documents);
            documents = NULL;

            //Free the old n_tokens_per_doc;
            free (n_tokens_per_doc);
            n_tokens_per_doc = NULL;

            //Get the new data
            documents = ((string **) content [0]);
            n_tokens_per_doc = ((unsigned int *) content [1][0][0]);

            content [0] = NULL;
            content [1][0][0] = NULL;
            free (content [1][0]);
            content [1][0] = NULL;
            free (content [1]);
            content [1] = NULL;
            free (content);
            content = NULL;

            print_processing ();

            if (solve_hyphen_problems (&documents, &n_docs, n_tokens_per_doc)) { //Check if it is done
                print_success ("All hyphen problems have been solved.");
                system ("pause");
                clear_screen ();

                printf ("Sorting documents:\n\n");
                Sleep (10);
                print_processing ();

                if (sort_term_token_1 (&documents, &n_docs, n_tokens_per_doc)) {
                    largest_length = largest_token_term ((void *) documents, &n_docs, n_tokens_per_doc);

                    print_success ("Sorting the content of each document is a success.");
                    
                    printf ("Data in all of the documents after all of these processes is:\n\n");
                    print_doc_tokens (&documents, &n_docs, n_tokens_per_doc, &largest_length);
                    printf ("\n\n");
                    system ("pause");
                    clear_screen ();

                    printf ("Creating the mps:\n\n");
                
                    unsigned int mps_size = get_mps_size (&documents, &n_docs, n_tokens_per_doc);
                
                    print_processing ();
                    mps _mps = build_mps (&documents, &n_docs, n_tokens_per_doc, 1, 3);

                    if (_mps != NULL) {
                        print_success ("MPS was created successfully.");
                        printf ("MPS as positional index:\n\n");
                        print_mps (&_mps, &mps_size, &n_docs, &largest_length);
                        printf ("\n\n");
                        system ("pause");
                        clear_screen ();

                        printf ("Creating TFIDF:\n\n");
                        Sleep (10);

                        print_processing ();
                        tfidf _tfidf = build_tfidf (&documents, &n_docs, n_tokens_per_doc);

                        if (_tfidf != NULL) {
                            print_success ("TFIDF has been created.");
                            printf ("TF, IDF & TFIDF are:\n\n");
                            print_tfidf (&_tfidf, &_mps, &mps_size, &n_docs, &largest_length);
                            printf ("\n\n");
                            system ("pause");
                            clear_screen ();

                            while (1) {
                                printf ("Enter A Query: ");
                                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 9);
                                string query = check_input (stdin, 10);
                                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);

                                if (query != NULL) {
                                    print_processing ();
                                    unsigned int * query_result = process_query (&query, &_mps, &mps_size, &_tfidf, &documents, &n_docs, n_tokens_per_doc);
                                    
                                    if (query_result != NULL) {
                                        print_query_result (query_result);
                                        system ("pause");

                                        free (query_result);
                                        query_result = NULL;

                                        print_success ("Query Processing Was A Success.");
                                        system ("pause");
                                        clear_screen ();
                                    } else {
                                        print_error ("No documents Were Founds.");
                                        system ("pause");
                                        clear_screen ();
                                    }

                                    free (query);
                                    query = NULL;
                                } else {
                                    print_error ("Query can not be empty.");
                                    system ("pause");
                                    clear_screen ();
                                }

                                printf ("Do you want to try again?\nPress (");
                                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 2);
                                printf ("Y");
                                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
                                printf (",");
                                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 2);
                                printf ("y");
                                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
                                printf (") to try again or anything to exit.\n\tYour Entery: ");
                                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 9);
                                scanf ("%c", option);
                                fflush (stdin);
                                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);

                                if ((*option) != 'Y' && (*option) != 'y')
                                    break;
                                else
                                    clear_screen ();
                            }

                            terminate_mps (&_mps, &mps_size);
                            terminate_tfidf (&_tfidf, &mps_size);
                        } else {
                            print_error ("Unable to create TFIDF.");
                            system ("pause");
                            clear_screen ();

                            terminate_mps (&_mps, &mps_size);

                            for (unsigned int counter = 0; counter < n_docs; free (documents [counter]), documents [counter] = NULL, ++counter)
                                for (unsigned int inside = 0; inside < n_tokens_per_doc [counter]; free (documents [counter][inside]), documents [counter][inside] = NULL, ++inside);

                            free (documents);
                            documents = NULL;
                            free (n_tokens_per_doc);
                            n_tokens_per_doc = NULL;
                            
                            return 0;
                        }
                    } else {
                        print_error ("Unable to build the MPS.");
                        system ("pause");
                        clear_screen ();

                        for (unsigned int counter = 0; counter < n_docs; free (documents [counter]), documents [counter] = NULL, ++counter)
                            for (unsigned int inside = 0; inside < n_tokens_per_doc [counter]; free (documents [counter][inside]), documents [counter][inside] = NULL, ++inside);

                        free (documents);
                        documents = NULL;
                        free (n_tokens_per_doc);
                        n_tokens_per_doc = NULL;
                        
                        return 0;    
                    }
                } else {
                    print_error ("Unable to sort the documents");
                    system ("pause");
                    clear_screen ();

                    for (unsigned int counter = 0; counter < n_docs; free (documents [counter]), documents [counter] = NULL, ++counter)
                        for (unsigned int inside = 0; inside < n_tokens_per_doc [counter]; free (documents [counter][inside]), documents [counter][inside] = NULL, ++inside);

                    free (documents);
                    documents = NULL;
                    free (n_tokens_per_doc);
                    n_tokens_per_doc = NULL;
                    
                    return 0;
                }
            } else { //Function did not execute correctly.
                print_error ("Unable to solve hyphen problems.");
                system ("pause");
                clear_screen ();

                for (unsigned int counter = 0; counter < n_docs; free (documents [counter]), documents [counter] = NULL, ++counter)
                    for (unsigned int inside = 0; inside < n_tokens_per_doc [counter]; free (documents [counter][inside]), documents [counter][inside] = NULL, ++inside);

                free (documents);
                documents = NULL;
                free (n_tokens_per_doc);
                n_tokens_per_doc = NULL;

                return 0;
            }
        } else {
            print_error ("Unable to tokenize the documents.");
            system ("pause");
            clear_screen ();

            //Free documents
            for (unsigned int counter = 0; counter < n_docs; free (documents [counter]), documents [counter] = NULL, ++counter)
                for (unsigned int inside = 0; inside < n_tokens_per_doc [counter]; free (documents [counter][inside]), documents [counter][inside] = NULL, ++inside);
                    
            free (documents);
            documents = NULL;

            //Free n_tokens_per_doc;
            free (n_tokens_per_doc);
            n_tokens_per_doc = NULL;

            return 0;
        }
    } else {
        print_error ("Unable to case folde the documents.");
        system ("pause");
        clear_screen ();

        //Free documents
        for (unsigned int counter = 0; counter < n_docs; free (documents [counter]), documents [counter] = NULL, ++counter)
            for (unsigned int inside = 0; inside < n_tokens_per_doc [counter]; free (documents [counter][inside]), documents [counter][inside] = NULL, ++inside);
                
        free (documents);
        documents = NULL;

        //Free n_tokens_per_doc;
        free (n_tokens_per_doc);
        n_tokens_per_doc = NULL;

        return 0;
    }

    for (unsigned int counter = 0; counter < n_docs; free (documents [counter]), documents [counter] = NULL, ++counter)
        for (unsigned int inside = 0; inside < n_tokens_per_doc [counter]; free (documents [counter][inside]), documents [counter][inside] = NULL, ++inside);

    free (documents);
    documents = NULL;
    free (n_tokens_per_doc);
    n_tokens_per_doc = NULL;

    return 1;
}