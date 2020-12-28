#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "../header/mps.h"
#include "../header/common.h"

struct _mps_ {
    _Bool type; //Specify which model i am using [Boolean Model = 0 - (Free) Phrase Model = 1].
    string term; //The term.
    unsigned int df; //The frequency of the doc.
    unsigned int tf; //The frequency of the term.
    unsigned char algo; //Specify Which Alogrithm i am going to use [1:IncidenceMatrix - 2:InvertedIndex - 3: PositionalIndex - 4: BiwordIndex - 5:ProximityIndex].
    unsigned int * posting_list; //The posting of the term in [InvertedIndex - PostionalIndex - BiWord - 5:ProximityIndex]. Incase of IncidenceMatrix it will be used to hold ones and zeros. The size of the array is the number of docs.
    unsigned int ** positioning_list; //Hold the positions of the term in different documents [PositionalIndex - ProximityIndex].
};

struct _tfidf_ {
    double ** tf; //Holds the 1 + log10 tf.
    double ** idf; //Holds log10 N/df.
    double ** tfidf; //Holds the addition of both tf and idf.
};

/*
    STEPS:
        1-Get INPUT either from file(s) or keyboard. -Done
        2-Tokenize the input + degrade token from token list per line to token list per document. -Done
        3-Solve hypen problem. -Done
        4-Get the max length term in the all documents. -Done
        5-Print tokens for each document. -Done
        6-Sort The terms in each document. The sorting done by name -Done
        7-Print sorted terms in each document. -Done
        8-generate frequency + generate the posting_list or/and positional_list + reomve repeated terms + build the mps. -Done
        9-print dictionary and posting_list or/and positional_list.
        10-Degrade the terms from terms list per documents to terms list. -Done
        11-Sort list. The sorting done by name. -Done
        12-Print the sorting list. -Done
        13-Get the query from the keyboard. 
        14-Process query + build execution list.
        15-Perform operations. -Semi Done (Positional_index only the remains).
        16-Print the query result.
        17-Desing a look for the print and for errors.

    DONE STEPS:
        1, 2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 15: 12 for 17 is done.
*/


/*
INPUT: A pointer to an unsigned int which hold the length of the largest token
OUTPUT:Non
This function is used to print an empty line with pipes without a new line |            |          |
*/
void print_mps_empty_line_without_new_line (unsigned int * largest_token) {
    printf ("\t| ");
    for (unsigned int counter = 0; counter < (2 + (*largest_token) + 2); printf (" "), ++counter);
    printf (" | ");
    for (unsigned char counter = 0; counter < 18; printf (" "), ++counter);
    printf (" |");
}


/*
INPUT: A pointer to an unsigned int which hold the lenght of the largest token.
OUTPUT:Non
INFO: This function is used print dashed line without new line |------------------|------------------|
*/
void print_mps_dashed_line_without_new_line (unsigned int * largest_token) {
    printf ("\t| ");
    for (unsigned int counter = 0; counter < (2 + (*largest_token) + 2); printf ("-"), ++counter);
    printf (" | ");
    for (unsigned char counter = 0; counter < 18; printf ("-"), ++counter);
    printf (" |");
}


/*
INPUT: A pointer to an unsigned int which hold the length of the largest token.
OUTPUT:Non
INFO: This function is used to print an empty line with space |                |                 |\n
*/
void print_mps_empty_line (unsigned int * largest_token) {
    printf ("\t| ");
    for (unsigned int counter = 0; counter < (2 + (*largest_token) + 2); printf (" "), ++counter);
    printf (" | ");
    for (unsigned char counter = 0; counter < 18; printf (" "), ++counter);
    printf (" |\n");
}


/*
INPUT: A pointer to an unsigned int which hold the lenght of the largest token.
OUTPUT:Non
INFO: This function is used print dashed line with new line |------------------|------------------|\n
*/
void print_mps_dashed_line (unsigned int * largest_token) {
    printf ("\t| ");
    for (unsigned int counter = 0; counter < (2 + (*largest_token) + 2); printf ("-"), ++counter);
    printf (" | ");
    for (unsigned char counter = 0; counter < 18; printf ("-"), ++counter);
    printf (" |\n");
}


/*
INPUT: A pointer to unsigned int which hold the length of the largest token, a pointer to an unsigned which hold the number of documents and a pointer to unsigned int which hold the largest size value of all of the documents document
OUTPUT: Non
INFO: This function is used to print empty line of incidence matrix.
*/
void print_mps_incidence_empty_line (unsigned int * largest_token, unsigned int * n_docs, unsigned int * largest_per_doc) {
    if (!largest_per_doc) {
        printf ("\t| ");
        for (unsigned int counter = 0; counter < ((*largest_token) + 2); printf (" "), ++counter);
        printf (" |");
        for (unsigned int counter = 0; counter < (*n_docs); ++counter) {
            for (unsigned int inside = 0; inside < (n_digits ((*n_docs)) + 2 + 2); printf (" "), ++inside);
            printf ("|");
        }
        printf ("\n");
    } else {
        printf ("\t| ");
        for (unsigned int counter = 0; counter < ((*largest_token) + 2); printf (" "), ++counter);
        printf (" |");

        for (unsigned int counter = 0; counter < (*n_docs); ++counter) {
            for (unsigned int inside = 0; inside < ((*largest_per_doc) + 2 + 2); printf (" "), ++inside);
            printf ("|");
        }
        printf ("\n");
    }
}


/*
INPUT: A pointer to unsigned int which hold the length of the largest token, a pointer to an unsigned which hold the number of documents and a pointer to unsigned int which hold the largest size value of all of the documents document
OUTPUT: Non
INFO: This function is used to print dashed line of incidence matrix.
*/
void print_mps_incidence_dashed_line (unsigned int * largest_token, unsigned int * n_docs, unsigned int * largest_per_doc) {
    if (!largest_per_doc) {
        printf ("\t| ");
        for (unsigned int counter = 0; counter < ((*largest_token) + 2); printf ("-"), ++counter);
        printf (" |");
        for (unsigned int counter = 0; counter < (*n_docs); ++counter) {
            for (unsigned int inside = 0; inside < (n_digits ((*n_docs)) + 2 + 2); printf ("-"), ++inside);
            printf ("|");
        }
        printf ("\n");
    } else {
        printf ("\t| ");
        for (unsigned int counter = 0; counter < ((*largest_token) + 2); printf ("-"), ++counter);
        printf (" |");

        for (unsigned int counter = 0; counter < (*n_docs); ++counter) {
            for (unsigned int inside = 0; inside < ((*largest_per_doc) + 2 + 2); printf ("-"), ++inside);
            printf ("|");
        }
        printf ("\n");
    }
}


/*
INPUT: Apointer to an array of mps, a pointer to unsigned int which hold the size of the array and a pointer to an unsigned int which hold the length of the largest term
OUTPUT: Non.
INFO: This function is used to print the inverted index in a tabular format.
*/
void print_inverted_index (mps * _mps, unsigned int * mps_size, unsigned int * largest_token) {
    if (_mps != NULL && mps_size != NULL) {
        if ((*_mps) != NULL && (*mps_size) != 0) {
            unsigned int * temp = NULL;

            if ((*largest_token) < 5) {
                do {temp = (unsigned int *) malloc (sizeof (unsigned int));} while (temp == NULL);
                (*temp) = (*largest_token);
                (*largest_token) = 5;
            }

            unsigned int diff = 0;

            printf ("\t ");
            for (unsigned int counter = 0; counter < (5 + ((*largest_token) + 2) + 18 + 2); printf ("_"), ++counter);
            printf ("\n");

            print_mps_empty_line (largest_token);

            printf ("\t|  ");
            diff = ((*largest_token) + 2) - 5;
            diff /= 2;
            padding (&diff);
            SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 5);
            printf ("Terms");
            SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
            if ((((*largest_token) + 2) - 5) % 2 != 0)
                diff++;
            padding (&diff);
            printf ("  | ");
            diff = 5;
            padding (&diff);
            SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 6);
            printf ("Doc Freq");
            SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
            padding (&diff);
            printf (" |\n");

            print_mps_dashed_line (largest_token);
            print_mps_empty_line (largest_token);

            for (unsigned int counter = 0; counter < (*mps_size); ++counter) {
                diff = ((*largest_token) + 2 - str_len (&(*_mps) [counter].term));
                printf ("\t|  ");
                diff /= 2;
                padding (&diff);
                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 11);
                printf ("%s", (*_mps) [counter].term);
                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
                if (((*largest_token) + 2 - str_len (&(*_mps) [counter].term)) % 2 != 0)
                    ++diff;
                padding (&diff);
                printf ("  | ");
                diff = 18 - n_digits ((*_mps) [counter].df);
                diff /= 2;
                padding (&diff);
                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 9);
                printf ("%u", (*_mps) [counter].df);
                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
                if ((18 - n_digits ((*_mps) [counter].df)) % 2 != 0)
                    diff++;
                padding (&diff);
                printf (" |       ");
                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 2);
                unsigned int total_arr_len = ((*_mps) [counter].df * 3);
                for (unsigned int inside = 0; inside < (*_mps) [counter].df; total_arr_len += n_digits ((*_mps) [counter].posting_list [inside]), ++inside);
                for (unsigned int inside = 0; inside < total_arr_len; printf ("-"), ++inside);
                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
                printf ("\n");
                print_mps_dashed_line_without_new_line (largest_token);
                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 4);
                printf (" --->");
                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
                printf (" |");
                for (unsigned int inside = 0; inside < (*_mps) [counter].df; ++inside) {
                    printf (" ");
                    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 9);
                    printf ("%u", (*_mps) [counter].posting_list [inside]);
                    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
                    if ((inside + 1) == (*_mps) [counter].df)
                        printf ("  |");
                    else
                        printf (" |");
                }

                printf ("\n");
                print_mps_empty_line_without_new_line (largest_token);
                printf ("       ");
                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 2);
                for (unsigned int inside = 0; inside < total_arr_len; printf ("-"), ++inside);
                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
                printf ("\n");

            }

            printf ("\t|");
            for (unsigned int counter = 0; counter < (4 + ((*largest_token) + 2)); printf ("_"), ++counter);
            printf ("|");
            for (unsigned char counter = 0; counter < 20; printf ("_"), ++counter);
            printf ("|\n\n");

            if (temp != NULL) {
                (*largest_token) = (*temp);
                free (temp);
                temp = NULL;
            }
        }
    }
}


/*
INPUT: Apointer to an array of mps, a pointer to unsigned int which hold the size of the array and a pointer to an unsigned int which hold the length of the largest term
OUTPUT:Non
INFO: This function is used to print positional index in a tabular format.
*/
void print_positional_index (mps * _mps, unsigned int * mps_size, unsigned int * largest_token) {
    if (_mps != NULL && mps_size != NULL) {
        if ((*_mps) != NULL && (*mps_size) != 0) {
            unsigned int * temp = NULL;

            if ((*largest_token) < 5) {
                do {temp = (unsigned int *) malloc (sizeof (unsigned int));} while (temp == NULL);
                (*temp) = (*largest_token);
                (*largest_token) = 5;
            }

            unsigned int diff = 0;

            printf ("\t ");
            for (unsigned int counter = 0; counter < (5 + ((*largest_token) + 2) + 18 + 2); printf ("_"), ++counter);
            printf ("\n");

            print_mps_empty_line (largest_token);

            printf ("\t|  ");
            diff = ((*largest_token) + 2) - 5;
            diff /= 2;
            padding (&diff);
            SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 5);
            printf ("Terms");
            SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
            if ((((*largest_token) + 2) - 5) % 2 != 0)
                diff++;
            padding (&diff);
            printf ("  | ");
            diff = 5;
            padding (&diff);
            SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 6);
            printf ("Doc Freq");
            SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
            padding (&diff);
            printf (" |\n");

            print_mps_dashed_line (largest_token);
            print_mps_empty_line (largest_token);

            for (unsigned int counter = 0; counter < (*mps_size); ++counter) {
                diff = ((*largest_token) + 2 - str_len (&(*_mps) [counter].term));
                printf ("\t|  ");
                diff /= 2;
                padding (&diff);
                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 11);
                printf ("%s", (*_mps) [counter].term);
                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
                if (((*largest_token) + 2 - str_len (&(*_mps) [counter].term)) % 2 != 0)
                    ++diff;
                padding (&diff);
                printf ("  | ");
                diff = 18 - n_digits ((*_mps) [counter].df);
                diff /= 2;
                padding (&diff);
                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 9);
                printf ("%u", (*_mps) [counter].df);
                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
                if ((18 - n_digits ((*_mps) [counter].df)) % 2 != 0)
                    diff++;
                padding (&diff);
                printf (" |       ");
                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 2);
                unsigned int total_arr_len = ((*_mps) [counter].df * 5);
                for (unsigned int inside = 0; inside < (*_mps) [counter].df; total_arr_len += n_digits ((*_mps) [counter].posting_list [inside]), ++inside);
                for (unsigned int inside = 0; inside < (*_mps) [counter].df; total_arr_len += ((*_mps) [counter].positioning_list [inside][0] * 2), ++inside);
                for (unsigned int inside = 0; inside < (*_mps) [counter].df; ++inside)
                    for (unsigned int _1 = 1; _1 <= (*_mps) [counter].positioning_list [inside][0]; total_arr_len += n_digits ((*_mps) [counter].positioning_list [inside][_1]), ++_1);
                for (unsigned int inside = 0; inside < total_arr_len; printf ("-"), ++inside);
                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
                printf ("\n");
                print_mps_dashed_line_without_new_line (largest_token);
                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 4);
                printf (" --->");
                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
                printf (" |");
                for (unsigned int inside = 0; inside < (*_mps) [counter].df; ++inside) {
                    printf (" ");
                    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 9);
                    printf ("%u", (*_mps) [counter].posting_list [inside]);
                    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
                    printf (": ");
                    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 14);
                    for (unsigned int _1 = 1; _1 <= (*_mps) [counter].positioning_list [inside][0]; printf ("%u, ", (*_mps) [counter].positioning_list [inside][_1]), ++_1);
                    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
                    if ((inside + 1) == (*_mps) [counter].df)
                        printf ("  |");
                    else
                        printf (" |");
                }

                printf ("\n");
                print_mps_empty_line_without_new_line (largest_token);
                printf ("       ");
                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 2);
                for (unsigned int inside = 0; inside < total_arr_len; printf ("-"), ++inside);
                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
                printf ("\n");

            }

            printf ("\t|");
            for (unsigned int counter = 0; counter < (4 + ((*largest_token) + 2)); printf ("_"), ++counter);
            printf ("|");
            for (unsigned char counter = 0; counter < 20; printf ("_"), ++counter);
            printf ("|\n\n");

            if (temp != NULL) {
                (*largest_token) = (*temp);
                free (temp);
                temp = NULL;
            }
        }

    }
}


/*
INPUT: Apointer to an array of mps, a pointer to unsigned int which hold the size of the array, 2d array which hold the tfidf, apointer to an unsigned int which hold the number of documents
     and a pointer to an unsigned int which hold the length of the largest term
OUTPUT: Non.
INFO: This function is print incidence matrix and tf, idf and tfidf in the same way.
*/
void print_incidence_matrix (mps * _mps, unsigned int * mps_size, double ** _tfidf, unsigned int * n_docs, unsigned int * largest_token) {
    if (_mps != NULL && mps_size != NULL && n_docs != NULL && largest_token != NULL) {
        if ((*_mps) != NULL && (*mps_size) != 0 && (*n_docs) != 0 && (*largest_token) != 0) {
            if (_tfidf) {
                unsigned int max_float = 0;

                for (unsigned int counter = 0; counter < (*mps_size); ++counter)
                    for (unsigned int inside = 0; inside < ((*n_docs) - 1); ++inside) {
                        if (n_digits ((unsigned int) _tfidf [counter][inside]) > n_digits ((unsigned int) _tfidf [counter][(inside + 1)]))
                            max_float = n_digits ((unsigned int) _tfidf [counter][inside]);
                        else
                            max_float = n_digits ((unsigned int) _tfidf [counter][inside + 1]);
                    }

                max_float += 3;


                if ((n_digits ((*n_docs)) + 1) > max_float)
                    max_float = n_digits ((*n_docs)) + 1;
                
                printf ("\t ");
                for (unsigned int counter = 0; counter < ((*largest_token) + 2 + 2 + 1 + ((*n_docs) * (max_float + 2 + 2 + 1)) - 1); printf ("_"), ++counter);
                printf ("\n");

                print_mps_incidence_empty_line (largest_token, n_docs, &max_float);
                
                printf ("\t|");
                for (unsigned int counter = 0; counter < ((*largest_token) + 2 + 2); printf (" "), ++counter);
                printf ("|");

                unsigned int diff = 0;
                

                for (unsigned int counter = 0; counter < (*n_docs); ++counter) {
                    printf (" ");
                    diff = (max_float + 2) - (1 + n_digits ((counter + 1)));
                    diff /= 2;
                    padding (&diff);
                    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 4);
                    printf ("D%u", (counter + 1));
                    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
                    if (((max_float + 2) - (1 + n_digits ((counter + 1)))) % 2 != 0)
                        ++diff;
                    padding (&diff);
                    printf (" |");
                }
                printf ("\n");

                print_mps_incidence_dashed_line (largest_token, n_docs, &max_float);
                print_mps_incidence_empty_line (largest_token, n_docs, &max_float);

                for (unsigned int counter = 0; counter < (*mps_size); ++counter) {
                    printf ("\t| ");
                    diff = ((*largest_token) + 2) - str_len (&(*_mps) [counter].term);
                    diff /= 2;
                    padding (&diff);
                    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 2);
                    printf ("%s", (*_mps) [counter].term);
                    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
                    if ((((*largest_token) + 2) - str_len (&(*_mps) [counter].term)) % 2 != 0)
                        ++diff;
                    padding (&diff);
                    printf (" |");
                    for (unsigned int inside = 0; inside < (*n_docs); ++inside) {
                        printf (" ");
                        diff = (max_float + 2) - (3 + n_digits ((unsigned int) _tfidf [counter][inside]));
                        diff /= 2;
                        padding (&diff);
                        SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 9);
                        printf ("%.2lf", _tfidf [counter][inside]);
                        SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
                        if (((max_float + 2) - (3 + n_digits ((unsigned int) _tfidf [counter][inside]))) % 2 != 0)
                            ++diff;
                        padding (&diff);
                        printf (" |");
                    }
                    printf ("\n");

                    print_mps_incidence_dashed_line (largest_token, n_docs, &max_float);
                    print_mps_incidence_empty_line (largest_token, n_docs, &max_float);

                }

                printf ("\t|");
                for (unsigned int counter = 0; counter < ((*largest_token) + 2 + 2); printf ("_"), ++counter);
                printf ("|");
                for (unsigned int counter = 0; counter < (*n_docs); ++counter) {
                    for (unsigned int inside = 0; inside < (max_float + 2 + 2); printf ("_"), ++inside);
                    printf ("|");
                }
                printf ("\n");
            } else {
                printf ("\t ");
                for (unsigned int counter = 0; counter < ((*largest_token) + 2 + 2 + 1 + ((*n_docs) * (n_digits ((*n_docs)) + 2 + 2 + 1)) - 1); printf ("_"), ++counter);
                printf ("\n");
                
                print_mps_incidence_empty_line (largest_token, n_docs, NULL);

                unsigned int diff = 0;
                unsigned int max_n_d_doc = n_digits ((*n_docs));

                printf ("\t| ");
                for (unsigned int counter = 0; counter < ((*largest_token) + 2); printf (" "), ++counter);
                printf (" |");

                for (unsigned int counter = 0; counter < (*n_docs); ++ counter) {
                    printf ("  ");
                    diff = max_n_d_doc - n_digits ((counter + 1));
                    diff /= 2;
                    padding (&diff);
                    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 4);
                    printf ("D%u", (counter + 1));
                    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
                    if ((max_n_d_doc - n_digits ((counter + 1))) % 2 != 0)
                        ++diff;
                    padding (&diff);
                    printf (" |");
                }

                printf ("\n");

                print_mps_incidence_dashed_line (largest_token, n_docs, NULL);
                print_mps_incidence_empty_line (largest_token, n_docs, NULL);

                for (unsigned int counter = 0; counter < (*mps_size); ++counter) {
                    printf ("\t| ");
                    diff = (*largest_token) + 2 - str_len (&(*_mps) [counter].term);
                    diff /= 2;
                    padding (&diff);
                    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 2);
                    printf ("%s", (*_mps) [counter].term);
                    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
                    if (((*largest_token) + 2 - str_len (&(*_mps) [counter].term)) % 2 != 0)
                        diff++;
                    padding (&diff);
                    printf (" |");

                    for (unsigned int inside = 0; inside < (*n_docs); ++inside) {
                        printf ("  ");
                        diff = 1 + max_n_d_doc - 1;
                        diff /= 2;
                        padding (&diff);
                        SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 9);
                        printf ("%u", (*_mps) [counter].posting_list [inside]);
                        SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
                        if ((1 + max_n_d_doc - 1) % 2 != 0)
                            diff++;
                        padding (&diff);
                        printf (" |");
                    }
                    printf ("\n");

                    print_mps_incidence_dashed_line (largest_token, n_docs, NULL);
                    print_mps_incidence_empty_line (largest_token, n_docs, NULL);
                }

                printf ("\t|");
                for (unsigned int counter = 0; counter < ((*largest_token) + 2 + 2); printf ("_"), ++counter);
                printf ("|");
                for (unsigned int counter = 0; counter < (*n_docs); ++counter) {
                    for (unsigned int inside = 0; inside < (n_digits ((*n_docs)) + 2 + 2); printf ("_"), ++inside);
                    printf ("|");
                }
                printf ("\n");
            }
        }
    }
}


/*
INPUT: An array which holds the query result to be sorted, the query terms, the number of query terms, the mps of the document, the mps size of the document, the tfidf of the document, the documents, the number of documents and the number of tokens in each document
OUTPUT: Return 1 if the sorting is done, otherwise return 0.
INFO: This function is used to sort the query result by checking the similarity between the query and each document. THe sorting is done in a descending order
*/
_Bool sort_query_result (unsigned int * _return_, string ** query_terms, unsigned int * query_n_terms, mps * _mps, unsigned int * mps_size, tfidf * _tfidf, string *** doc_tokens, unsigned int * n_docs, unsigned int * n_tokens_per_doc) {
    if (_return_ != NULL && query_terms != NULL && query_n_terms != NULL && _mps != NULL && mps_size != NULL && _tfidf != NULL && doc_tokens != NULL && n_docs != NULL && n_tokens_per_doc != NULL) { //Checks if the passed arguments are correct.
        if ((*_return_) != 0 && (*query_terms) != NULL && (*query_n_terms) != 0 && (*_mps) != NULL && (*mps_size) != 0 && (*_tfidf) != NULL && (*doc_tokens) != NULL && (n_docs) != NULL && (*n_tokens_per_doc) != 0) { //Checks if the passed are pointing and holding the correct data
            unsigned int n = 1; //Represent that  a query is in a form of one document

            //Remove the repeated tokens in the query.
            remove_repeated_term_token_2 (query_terms, query_n_terms);
            //Sort the tersm in the query
            sort_term_token_2 (query_terms, query_n_terms);

            //Gets the query tfidf
            tfidf query_tfidf = build_tfidf (&query_terms, &n, query_n_terms);

            //Gets another tfidf for the document
            tfidf doc_temp_tfidf = build_tfidf (doc_tokens, n_docs, n_tokens_per_doc);

            double query_length = query_ec_length (query_n_terms, &query_tfidf); //Get the ec_length for the query
            double * doc_length = doc_ec_length (doc_tokens, n_docs, n_tokens_per_doc, _mps, mps_size, _tfidf); //Get the ec_length for the document

            for (unsigned int counter = 0; counter < (*query_n_terms); ++counter) { //Foreach query_distinict term
                query_tfidf -> tf [counter][0] /= query_length; //Normalize the term
                printf ("The query normalized for the #["); //Print it
                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 1);
                printf ("%u", (counter + 1));
                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
                printf ("] term is: ");
                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 2);
                printf ("%.2lf.\n", query_tfidf -> tf [counter][0]);
                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
            }
            printf ("\n");

            //Normalize all documents term frequency
            for (unsigned int counter = 0; counter < (*mps_size); ++counter)
                for (unsigned int inside = 0; inside < (*n_docs); ++inside)
                    doc_temp_tfidf -> tf [counter][inside] /= doc_length [inside];

            //The score which the sorting will be passed upon
            double * score = NULL;
            //By default each index holds the value zero
            do {score = (double *) calloc (_return_ [0], sizeof (double));} while (score == NULL);

            for (unsigned int counter = 0; counter < _return_ [0]; ++counter) { //Foreach document in the query result
                unsigned int * meta = NULL; //Create meta about that document
                do {meta = (unsigned int *) malloc (sizeof (unsigned int) * 3);} while (meta == NULL);

                meta [0] = n_tokens_per_doc [(_return_ [(counter + 1)] - 1)];
                meta [1] = 1;
                meta [2] = n_tokens_per_doc [(_return_ [(counter + 1)] - 1)];

                //Make a top level pointer for the doc
                string ** temp = &(*doc_tokens) [(_return_ [(counter + 1)] - 1)];
                string * terms = degrade_list_2 (&temp, meta); //Make a copy of that document

                temp = NULL;

                remove_repeated_term_token_2 (&terms, &meta [0]); //Remove repeated terms
                sort_term_token_2 (&terms, &meta [0]); //Sort terms

                for (unsigned int inside = 0; inside < (*query_n_terms); ++inside) { //For each term in that query
                    for (unsigned int i = 0; i < meta [0]; ++i) //See if it exist in the document.
                        if (compare_string (&(*query_terms) [inside], terms [i])) { //Check if they are same
                            for (unsigned int _1 = 0; _1 < (*mps_size); ++_1) //Loop through the mps
                                if (compare_string (&(*_mps) [_1].term, terms [i])) { //See if the term of that document equal that mps term.
                                    score [counter] += (query_tfidf -> tf [inside][0] * doc_temp_tfidf -> tf [_1][(_return_ [(counter + 1)] - 1)]); //If it does. Multiply the query normalized for that term and the with the normalized of that term in the document and sum it in the score of that document.
                                    break;
                                }
                            
                            break;
                        }
                }

                for (unsigned int inside = 0; inside < meta [0]; free (terms [inside]), terms [inside] = NULL, ++inside);
                free (terms);
                terms = NULL;

                free (meta);
                meta = NULL;
            }


            //Print the score for each document in the return result from the query.
            for (unsigned int counter = 0; counter < _return_ [0]; ++counter) {
                printf ("The similarity between the docID #[");
                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 9);
                printf ("%u", _return_ [(counter + 1)]);
                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
                printf ("] and the query is: ");
                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 2);
                printf ("%.2lf", score [counter]);
                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
                printf (".\n");
            }

            //Sorting the return:
            for (unsigned int counter = 0; counter < _return_ [0]; ++counter)
                for (unsigned int inside = 0; inside < (_return_ [0] - 1); ++inside)
                    if (score [inside] < score [(inside + 1)]) {
                        double temp_score = score [inside];
                        score [inside] = score [(inside + 1)];
                        score [(inside + 1)] = temp_score;

                        unsigned int temp = _return_ [(inside + 1)];
                        _return_ [(inside + 1)] = _return_ [(inside + 2)];
                        _return_ [(inside + 2)] = temp;
                    }

            printf ("\n\n");

            //Free the doc_ec_length
            free (doc_length);
            doc_length = NULL;

            //Free the score
            free (score);
            score = NULL;

            //Terminate the temp tfidf of the document
            terminate_tfidf (&doc_temp_tfidf, mps_size);
            //Terminate the temp tfidf of the query
            terminate_tfidf (&query_tfidf, query_n_terms);

            return 1;
        } else
            return 0;
    } else
        return 0;
}

/*
INPUT: A pointer to an array of mps and a pointer to an unsigned int which hold the size of the mps.
OUTPUT: Return 1 if the termination is successful, otherwise return 0.
INFO: This function is used to terminate/de-allocate the mps from the memory.
*/
_Bool terminate_mps (mps * _mps, unsigned int * mps_size) {
    if (_mps != NULL && mps_size != NULL) { //Checks if the passed arguments are passed correctly.
        if ((_mps) != NULL && (*mps_size) != 0) { //Checks if the passed arguments holds and points at the right data

            //For each term
            for (unsigned int counter = 0; counter < (*mps_size); free ((*_mps) [counter].term), (*_mps) [counter].term = NULL, free ((*_mps) [counter].posting_list), (*_mps) [counter].posting_list = NULL, ++counter)
                if ((*_mps) [counter].type == 1 && (*_mps) [counter].algo == 3) { //Check if we are using phrase model and positional_index algo
                    for (unsigned int inside = 0; inside < (*_mps) [counter].df; free ((*_mps) [counter].positioning_list [inside]), (*_mps) [counter].positioning_list [inside] = NULL, ++inside);
                    free ((*_mps) [counter].positioning_list);
                    (*_mps) [counter].positioning_list = NULL;
                }

            free ((*_mps));
            (*_mps) = NULL;

            return 1;
        } else
            return 0;
    } else
        return 0;
}


/*
INPUT: A pointer to a tfidf.
OUTPUT: Return 1 if the termination is a success, otherwise return 0
INFO: This function is used to terminate/de-allocate the tfidf
*/
_Bool terminate_tfidf (tfidf * _tfidf, unsigned int * mps_size) {
    if (_tfidf != NULL && mps_size != NULL) { //Checks if the passed argument is passed correctly
        if ((*_tfidf) != NULL && (*mps_size) != 0) { //Checks if the passed argument holds and points at the right data
            for (unsigned int counter = 0; counter < (*mps_size); free ((*_tfidf) -> tf [counter]), (*_tfidf) -> tf [counter] = NULL, free ((*_tfidf) -> idf [counter]), (*_tfidf) -> idf [counter] = NULL, free ((*_tfidf) -> tfidf [counter]), (*_tfidf) -> tfidf [counter] = NULL, ++counter);
            free ((*_tfidf) -> tf);
            (*_tfidf) -> tf = NULL;

            free ((*_tfidf) -> idf);
            (*_tfidf) -> idf = NULL;

            free ((*_tfidf) -> tfidf);
            (*_tfidf) -> tfidf = NULL;

            free ((*_tfidf));
            (*_tfidf) = NULL;

            return 1;
        } else
            return 0;
    } else
        return 0;
}


/*
INPUT: A pointer to an array of mps and a pointer to an unsigned int which hold the size of that array.
OUTPUT: OUTPUT Return 1 if the printing is done successfult, otherwise return 0
INFO: This function is used to print the mps.
*/
_Bool print_mps (mps * _mps, unsigned int * mps_size, unsigned int * n_docs, unsigned int * largest_token) {
    if (_mps != NULL && mps_size != NULL && n_docs != NULL) { //Checks if the passed arguments are passed correctly
        if ((*_mps) != NULL && (*mps_size) != 0 && (*n_docs) != 0) { //Checks if the passed arguments are holding and pointing the correct data
            if ((*_mps) [0].algo == 1) //Checks if we are using incidence matrix algo
                print_incidence_matrix (_mps, mps_size, NULL, n_docs, largest_token);
            else if ((*_mps) [0].algo == 2) //Checks if we are using inverted index algo
                print_inverted_index (_mps, mps_size, largest_token);
            else if ((*_mps) [0].algo == 3) //Checks if we are using positional index alg
                print_positional_index (_mps, mps_size, largest_token);
            else
                return 0;

            return 1;
        } else
            return 0;
    } else
        return 0;
}


/*
INPUT: A pointer to an array of tfidf and a pointer to an unsigned int which hold the size of that array
OUTPUT: Return 1 if the printing is successful, otherwise return 0
INFO: This function is used to print tfidf
*/
_Bool print_tfidf (tfidf * _tfidf, mps *_mps, unsigned int * mps_size, unsigned int * n_docs, unsigned int * largest_token) {
    if (_tfidf != NULL && _mps != NULL && mps_size != NULL) { //Checks if the passed arguments are passed correctly
        if ((*_tfidf) != NULL && (*_mps) != NULL && (*mps_size) != 0) { //Checks if the passed arguments are pointing and holding the correct data
            printf ("\n\nPrinting TF:\n");
            print_incidence_matrix (_mps, mps_size, (*_tfidf) -> tf, n_docs, largest_token); //Print the tf in incidence matrix format.
            printf ("\n\nPrinting IDF:\n");
            print_incidence_matrix (_mps, mps_size, (*_tfidf) -> idf, n_docs, largest_token); //Print the idf in incidence matrix format.
            printf ("\n\nPrinting TF-IDF:\n");
            print_incidence_matrix (_mps, mps_size, (*_tfidf) -> tfidf, n_docs, largest_token); //Print the tfidf in incidence matrix format.
            printf ("\n\n");
            return 1;
        } else
            return 0;
    } else
        return 0;
}


/*
INPUT: A pointer to 3d array of strings which are the documents, a pointer to an unsigned int which hold the numbe of documents, an array of unsigned int which hold the number of tokens in each document.
    a _Bool type which indicate either i use boolean model or phrase (free) model and an unsigned char which indicates what type of algorithm i will use with the model type.

OUTPUT: Return an array of mps, otherwie return NULL.
INFO: This function is used to build the mps (multiple purpose struct). Which hold the dictionary, posting and positioning list
*/
mps build_mps (string *** doc_tokens, unsigned int * n_docs, unsigned int * n_tokens_per_doc, _Bool model_type, unsigned char algo_type) {
    if (doc_tokens != NULL && n_docs != NULL && n_tokens_per_doc != NULL && algo_type != 0) { //Checks if the passed arguments are passed correctly.
        if ((*doc_tokens) != NULL && (*n_docs) != 0 && (*n_tokens_per_doc) != 0) { //Checks if the passed arguments points and holds the correct data.
            if (model_type) { //Checks if the model is phrase (free) model
                if (algo_type == 3) { //Checks if the used passed or wants to used positional_index model
                    unsigned int mps_size = get_mps_size (doc_tokens, n_docs, n_tokens_per_doc); //Get the total number of distinict terms in all of the documents
                    
                    if (mps_size != 0) { //Checks if the size is not 0
                        unsigned int * meta = NULL; //Create the meta data about the documents
                        do {meta = (unsigned int *) malloc (sizeof (unsigned int) * ((*n_docs) + 2));} while (meta == NULL);

                        meta [0] = 0; //Hold the total number of tokens in all of the documents.
                        meta [1] = (*n_docs); //Hold the number of documents
                        //For each document store its number of tokens in the meta [(2 + counter)] and add that amount to the meta [0]
                        for (unsigned int counter = 0; counter < (*n_docs); meta [0] += n_tokens_per_doc [counter], meta [(2 + counter)] = n_tokens_per_doc [counter], ++counter);

                        //Transform the doc_tokens from 3d to 2d.
                        string * terms = degrade_list_2 (doc_tokens, meta);

                        //Checks if the transformation is done correctly.
                        if (terms != NULL) {
                            if (remove_repeated_term_token_2 (&terms, &meta [0])) { //Remove the repeated tokens in that list
                                //De-allocate the meta data from the memory
                                free (meta);
                                meta = NULL;

                                //Sort the terms
                                if (sort_term_token_2 (&terms, &mps_size)) {
                                    //Build the positional_index model.
                                    unsigned int **** positional_index = build_positional_index (doc_tokens, n_docs, n_tokens_per_doc);

                                    //Checks if the model has been built successfuly
                                    if (positional_index != NULL) {
                                        //Create the return result the dictionary which hold the data and data about the data.
                                        mps dictionary = NULL;
                                        do {dictionary = (mps) malloc (sizeof (struct _mps_) * mps_size);} while (dictionary == NULL);

                                        //For each terms.
                                        for (unsigned int counter = 0; counter < mps_size; ++counter) {
                                            dictionary [counter].term = give_me_str_copy (&terms [counter]); //Put that term in the current iteration of the dictionary
                                            free (terms [counter]); //Free that old term
                                            terms [counter] = NULL;

                                            dictionary [counter].type = 1; //Notify that the dictionary is using phrase (free) model
                                            dictionary [counter].algo = 3; //Notify that the dictionary is useing positional_index model

                                            dictionary [counter].tf = positional_index [0][1][0][counter]; //Store the value of term frequency of that term
                                            dictionary [counter].df = positional_index [0][0][0][counter]; //Store the value of docu frequency of that term

                                            dictionary [counter].posting_list = positional_index [0][2][counter]; //Store the posting list of that term.
                                            positional_index [0][2][counter] = NULL;

                                            dictionary [counter].positioning_list = positional_index [1][counter]; //Store the positioning list of that term
                                            positional_index [1][counter] = NULL;
                                        }

                                        //Deallocate the positional_index model and the terms
                                        free (positional_index [0][0][0]);
                                        positional_index [0][0][0] = NULL;
                                        free (positional_index [0][0]);
                                        positional_index [0][0] = NULL;

                                        free (positional_index [0][1][0]);
                                        positional_index [0][1][0] = NULL;
                                        free (positional_index [0][1]);
                                        positional_index [0][1] = NULL;

                                        free (positional_index [0][2]);
                                        positional_index [0][2] = NULL;

                                        free (positional_index [0]);
                                        positional_index [0] = NULL;

                                        free (positional_index [1]);
                                        positional_index [1] = NULL;

                                        free (positional_index);
                                        positional_index = NULL;

                                        free (terms);
                                        terms = NULL;

                                        return dictionary;
                                    } else {
                                        for (unsigned int counter = 0; counter < mps_size; free (terms [counter]), terms [counter] = NULL, ++counter);
                                        free (terms);
                                        terms = NULL;

                                        return NULL;
                                    }
                                } else {
                                    for (unsigned int counter = 0; counter < mps_size; free (terms [counter]), terms [counter] = NULL, ++counter);
                                    free (terms);
                                    terms = NULL;

                                    return NULL;
                                }
                            } else {
                                for(unsigned int counter = 0; counter < meta [0]; free (terms [counter]), terms [counter] = NULL, ++counter);
                                free (terms);
                                terms = NULL;

                                free (meta);
                                meta = NULL;

                                return NULL;
                            }
                        } else {
                            free (meta);
                            meta = NULL;

                            return NULL;
                        }
                    } else
                        return NULL;
                } else
                    return NULL;
            } else { //Means the user is using the Boolean model
                if (algo_type == 1 || algo_type == 2) { //Checks if the algrothim the user chose was either incidence_matrix or inverted index.
                    unsigned int mps_size = get_mps_size (doc_tokens, n_docs, n_tokens_per_doc); //Get the mps size. Which is the total number of distinict tokens in all of the documents.

                    if (mps_size != 0) { //Checks if the size does not equal to zero
                        unsigned int * meta = NULL; //Create the meta which hold information about the docuemnts
                        do {meta = (unsigned int *) malloc (sizeof (unsigned int) * ((*n_docs) + 2));} while (meta == NULL);

                        meta [0] = 0;
                        meta [1] = (*n_docs);
                        for (unsigned int counter = 0; counter < (*n_docs); meta [0] += n_tokens_per_doc [counter], meta [(2 + counter)] = n_tokens_per_doc [counter], ++counter);

                        string * terms = degrade_list_2 (doc_tokens, meta); //Hold all of the distinict terms in all of the documents.

                        //Checks if the degrading is done correctly.
                        if (terms != NULL) {
                            if (remove_repeated_term_token_2 (&terms, &meta [0])) { //Remove all repeated token.
                                //Deallocate the meta from the memory
                                free (meta);
                                meta = NULL;

                                if (sort_term_token_2 (&terms, &mps_size)) { //Sort the terms.

                                    unsigned int *** data = NULL; //Build either incidence_matrix or inverted_index.
                                    if (algo_type == 1)
                                        data = build_incidence_matrix (doc_tokens, n_docs, n_tokens_per_doc); //Build incidence_matrix
                                    else
                                        data = build_inverted_index (doc_tokens, n_docs, n_tokens_per_doc); //Build inverted_index

                                    if (data != NULL) { //Checks if building the model was successful.
                                        
                                        mps dictionary = NULL;
                                        do {dictionary = (mps) malloc (sizeof (struct _mps_) * mps_size);} while (dictionary == NULL);

                                        for (unsigned int counter = 0; counter < mps_size; ++counter) {
                                            dictionary [counter].term = give_me_str_copy (&terms [counter]);
                                            free (terms [counter]);
                                            terms [counter] = NULL;

                                            dictionary [counter].type = 0;

                                            if (algo_type == 1)
                                                dictionary [counter].algo = 1;
                                            else
                                                dictionary [counter].algo = 2;

                                            dictionary [counter].tf = data [1][0][counter];
                                            dictionary [counter].df = data [0][0][counter];

                                            dictionary [counter].posting_list = data [2][counter];
                                            data [2][counter] = NULL;

                                            dictionary [counter].positioning_list = NULL;
                                        }

                                        free (data [0][0]);
                                        data [0][0] = NULL;
                                        free (data [0]);
                                        data [0] = NULL;

                                        free (data [1][0]);
                                        data [1][0] = NULL;
                                        free (data [1]);
                                        data [1] = NULL;

                                        free (data [2]);
                                        data [2] = NULL;

                                        free (data);
                                        data = NULL;

                                        free (terms);
                                        terms = NULL;

                                        return dictionary;
                                    } else {
                                        for (unsigned int counter = 0; counter < mps_size; free (terms [counter]), terms [counter] = NULL, ++counter);
                                        free (terms);
                                        terms = NULL;

                                        return NULL;
                                    }
                                } else {
                                    for (unsigned int counter = 0; counter < mps_size; free (terms [counter]), terms [counter] = NULL, ++counter);
                                    free (terms);
                                    terms = NULL;

                                    return NULL;
                                }
                            } else {
                                for (unsigned int counter = 0; counter < meta [0]; free (terms [counter]), terms [counter] = NULL, ++counter);
                                free (terms);
                                terms = NULL;
                                free (meta);
                                meta = NULL;

                                return NULL;
                            }
                        } else {
                            free (meta);
                            meta = NULL;
                            return NULL;
                        }
                    } else
                        return NULL;
                } else
                    return NULL;
            }

            return NULL;
        } else
            return NULL;
    } else
        return NULL;
}


/*
INPUT: A pointer to a 3d array, a pointer to an unsigned int which hold the number of documents and an array of unsigned int which hold the number of tokens in each doc.
OUTPUT: Return a non zero value if everything is done correctly, otherwise return 0.
INFO: This function is used to calculate the mps_size to be able to build mps.
*/
unsigned int get_mps_size (string *** doc_tokens, unsigned int * n_docs, unsigned int * n_tokens_per_doc) {
    if (doc_tokens != NULL && n_docs != NULL && n_tokens_per_doc != NULL) { //Checks if the passed arguments are passed correctly.
        if ((*doc_tokens) != NULL && (*n_docs) != 0 && (*n_tokens_per_doc) != 0) { //Checks if the passed arguments points and holds the correct data.
            unsigned int mps_size = 0; //The mps size.
            unsigned int * meta = NULL; //Holds a meta about the doc_tokens

            do {meta = (unsigned int *) malloc (sizeof (unsigned int) * (2 + (*n_docs)));} while (meta == NULL); //Allocate space for the meta.

            meta [1] = (*n_docs); //Set the meta [1] to the value of n_docs which is the number of documents
            for (unsigned int counter = 0; counter < (*n_docs); mps_size += n_tokens_per_doc [counter], meta [(2 + counter)] = n_tokens_per_doc [counter], ++counter); //Get the total number of tokens in all of the documents and assign the number of tokens to the meta array
            meta [0] = mps_size; //Let the meta [0] have the value of the mps_size which is the total number of tokens in these documents.

            string * degraded_list = degrade_list_2 (doc_tokens, meta); //Degrade the 3d list to 2d list.

            if (degraded_list != NULL) { //Check if the used does not equal to NULL.
                if (remove_repeated_term_token_2 (&degraded_list, &meta [0])) { //Checks if the removal of duplicats terms/tokens is done correctly.
                    for (unsigned int counter = 0; counter < meta [0]; free (degraded_list [counter]), degraded_list [counter] = NULL, ++counter); //Free tokens/terms from the list
                    free (degraded_list); //Deallocate the list
                    degraded_list = NULL;

                    mps_size = meta [0]; //let the size used_length to equal the meta [0].

                    //Deallocate the meta.
                    free (meta);
                    meta = NULL;

                    return mps_size;
                } else {
                    for (unsigned int counter = 0; counter < meta [0]; free (degraded_list [counter]), degraded_list [counter] = NULL, ++counter);
                    free (degraded_list);
                    degraded_list = NULL;
                    free (meta);
                    meta = NULL;

                    return 0;
                }
            } else {
                free (meta);
                meta = NULL;
                return 0;
            }
        } else
            return 0;
    } else
        return 0;
}


/*
INPUT: A pointer to an array of mps, a pointer to an unsigned int which hold the legnth of the array and string which hold the string we are looking for
OUTPUT: Return the posting list of a term, otherwise return NULL.
INFO: This function is used to get the posting list of a specific term
NOTE: Call this function even if you are using incidence matrix as the term vector is stored in the posting list.
*/
unsigned int * get_posting_of (mps * _mps, unsigned int * mps_size, unsigned int * n_docs, string term) {
    if (_mps != NULL && mps_size != NULL && term != NULL && n_docs != NULL) { //Checking if the passed arguments are passed correctly
        if ((*_mps) != NULL && (*mps_size) != 0 && (*n_docs) != 0) { //Checks if the passed arguments points and holds the correct data
            unsigned int term_size = str_len (&term); //Get the size of wanted term

            if (term_size != 0) { //Check if the term is not empty
                _Bool found = 0;
                unsigned int index = 0;

                for (unsigned int counter = 0; counter < (*mps_size); ++counter) //For each term
                    if (compare_string (&term, (*_mps) [counter].term)) { //Check if the passed term equals the term in that iteration
                        found = 1;
                        index = counter;
                        break;
                    }
                
                if (found) {
                    unsigned int * _return_ = NULL;

                    if ((*_mps) [0].algo == 1) { //Checks if we are using Incidence matrix
                        do {_return_ = (unsigned int *) malloc (sizeof (unsigned int) * ((*n_docs) + 1));} while (_return_ == NULL); //Allocate space by the number of documents
                        _return_ [0] = (*n_docs); //Let the first index hold the value of n_docs
                        for (unsigned int counter = 1; counter <= (*n_docs); _return_ [counter] = (*_mps) [index].posting_list [(counter - 1)], ++counter); //Loop through the posting list and continue assign the value to the return value.
                    } else if ((*_mps) [0].algo == 2 || (*_mps) [0].algo == 3) { //Checks if we are using inverted index or positionial index
                        do {_return_ = (unsigned int *) malloc (sizeof (unsigned int) * ((*_mps) [index].df + 1));} while (_return_ == NULL); //Allocate space by the document frequency.
                        _return_ [0] = (*_mps) [index].df; //Let the first index holds the document frequency of that term
                        for (unsigned int counter = 1; counter <= (*_mps) [index].df; _return_ [counter] = (*_mps) [index].posting_list [(counter - 1)], ++counter); //Loop through the posting list and assign the value to the return value.
                    } else
                        return NULL;
                    

                    return _return_;
                } else
                    return NULL; //Means nothing is found.
            } else
                return NULL;
        } else
            return NULL;
    } else
        return NULL;
}


/*
INPUT: A pointer to an array of mps, a pointer to an unsigned int which hold the size of mps, an unsigned int which specify which document positioning list to use and a string which hold the string
OUTPUT: Return the positioning list of a specif document of a term, otherwise NULL.
INFO: This function is used to get the positioning list of a specific document of a specific term.
*/
unsigned int * get_positioning_of (mps * _mps, unsigned int * mps_size, unsigned int docID, string term) {
    if (_mps != NULL && mps_size != NULL && term != NULL) { //Checks if the passed arguments are passed correctly.
        if ((*_mps) != NULL && (*mps_size) != 0) { //Checks if the passed arguments are holding and pointing at the correct data.
            unsigned int term_size = str_len (&term); //Get the size of the passed term

            if (term_size != 0) { //Make sure that the term passed is not empty
                _Bool found = 0; //Used to check if we found the the wanted term
                unsigned int index = 0; //Holds the index the term that we will get its positioning list
                unsigned int pos_index = 0; //Holds the index that contains the positional index of that document of that term

                for (unsigned int counter = 0; counter < (*mps_size); ++counter) //For each term
                    if (compare_string (&term, (*_mps) [counter].term)) { //Check if the passed term equal the term in the mps
                        for (unsigned int inside = 0; inside < (*_mps) [counter].df; ++inside)
                            if (docID == (*_mps) [counter].posting_list [inside]) {//Checks if the docID is between the document frequency
                                found = 1;
                                index = counter;
                                pos_index = inside;
                                break;
                            }

                        if (found)
                            break;
                    }
                
                if (found) {
                    unsigned int * _return_ = NULL; //The return Value

                    //Allocate space for the return
                    do {_return_ = (unsigned int *) malloc (sizeof (unsigned int) * ((*_mps) [index].positioning_list [pos_index][0] + 1));} while (_return_ == NULL);
                    _return_ [0] = (*_mps) [index].positioning_list [pos_index][0]; //Let the first index of the return has the size of the poistioning list of that document

                    //Create a copy of the same positioning list for that document and return it.
                    for (unsigned int counter = 1; counter <= (*_mps) [index].positioning_list [pos_index][0]; _return_ [counter] = (*_mps) [index].positioning_list [pos_index][counter], ++counter);
                    return _return_;
                } else
                    return NULL; //Means nothing is found.
            } else
                return NULL;
        } else
            return NULL;
    } else
        return NULL;
}


/*
INPUT: 

OUTPUT: An array of unsigned int which hold the returned documents and its size as the first index, otherwise return NULL
INFO: This function is used to process the query and return the matched documents.
*/
unsigned int * process_query (string * query, mps * _mps, unsigned int * mps_size, tfidf * _tfidf, string *** doc_tokens, unsigned int * n_docs, unsigned int * n_tokens_per_doc) {
    if (query != NULL && _mps != NULL && mps_size != NULL && _tfidf != NULL && n_docs != NULL && doc_tokens != NULL && n_tokens_per_doc != NULL) { //Checks if the passed arguments are passed correctly
        if ((*query) != NULL && (*_mps) != NULL && (*mps_size) != 0 && (*_tfidf) != NULL && (*n_docs) != 0 && (*doc_tokens) != NULL && (*n_tokens_per_doc) != 0) { //Checks if the passed arguments are holding an pointing at the correct data
            unsigned int query_size = str_len (query); //Get the size of the query

            if (query_size != 0) { //Make sure that the user did not send an empty string.
                unsigned int * _return_ = NULL; //The output of the function

                lower_input (query); //Turn all uppercase characters into lowercase characters.

                if ((*_mps) [0].type == 1) { //Checks we using phrase (free) model
                    void *** content = NULL; //Hold the result from str_word_split and str_char_split

                    if (str_len (&__DELIMITER__) > 1) //Checks if the delimiter holds more than one character
                        content = str_word_split (query, __DELIMITER__);
                    else //Means the delimiter only holds one character
                        content = str_char_split (query, __DELIMITER__ [0]);

                    if (content != NULL) { //Checks if the output does't not equal to NULL.
                        //Assign the return to its respective type.
                        string * terms = ((string *) content [1]);
                        unsigned int * terms_size = ((unsigned int *) content [0][0]);

                        //Free the output from str_word_split and str_char_split
                        content [1] = NULL;
                        content [0][0] = NULL;
                        free (content);
                        content = NULL;

                        //Free the index of the split. As the split function create a backup size of the list in the first index
                        free (terms [0]);
                        terms [0] = NULL;

                        //Shit the content of the list one index up. So the at the end of the list will be null and reallocate the list to remove it
                        for (unsigned int counter = 0; counter <= ((*terms_size) - 1); terms [counter] = terms [(counter + 1)], terms [(counter + 1)] = NULL, ++counter);

                        void * reallocation = NULL;

                        //Reallocate the list size from terms_size + 1 to terms_size
                        do {reallocation = realloc (terms, sizeof (string) * (*terms_size));} while (reallocation == NULL);

                        //checks if the reallocation happens in different memory block
                        if (reallocation != terms)
                            terms = (string *) reallocation; //Let terms var hold the address of the new reallocation
                        
                        reallocation = NULL;

                        if (analyse_query_stop_words (&terms, terms_size)) //Checks if we can remove stop words from the query.
                            drop_stop_words (&terms, terms_size); //Remove stop words from the query

                        if ((*terms_size) == 1) { //Checks if after removal of stop word the length is 1
                            _return_ = get_posting_of (_mps, mps_size, n_docs, terms [0]);

                            //Free the list
                            free (terms [0]);
                            terms [0] = NULL;
                            free (terms);
                            terms = NULL;

                            free (terms_size);
                            terms_size = NULL;
                        } else { //Checks if the size of the list is two
                            unsigned int ** t1_t2 = NULL; //Which hold the posting_list for t1 and t2. t stands for term
                            do {t1_t2 = (unsigned int **) malloc (sizeof (unsigned int *) * 2);} while (t1_t2 == NULL); //Allocate space for it.

                            unsigned int ** p1_p2 = NULL; //Which holds the positions list for t1 and t2 depending on the and operation

                            unsigned int * indexing = NULL; //Holds the docIDs and its respective index of p1_p2 positional_index for sorting
                            
                            for (unsigned int counter = 0; counter <= ((*terms_size) - 2); ++counter) { //For each term. We subtract 2 because we always start by two, then one by one

                                if (counter == 0) //If it is firt time get the posting of that term. And we only do it once because the result of anding of posting list is stored in that index
                                    t1_t2 [0] = get_posting_of (_mps, mps_size, n_docs, terms [counter]);

                                t1_t2 [1] = get_posting_of (_mps, mps_size, n_docs, terms [(counter + 1)]); //Get the posting list for the next term


                                void * output = NULL; //Hold the output of called function to checks there result

                                if (t1_t2 [0] != NULL && t1_t2 [1] != NULL) { //Checks if accuring the posting list is done succesfully
                                    output = perform_and_operation (&t1_t2, 0, 2); //Perform anding operation on document IDs

                                    if (output != NULL) { //Checks if there is an intersection between the two posting list
                                        //Free the old result
                                        free (t1_t2 [0]);
                                        t1_t2 [0] = NULL;

                                        t1_t2 [0] = (unsigned int *) output; //Let the first index of t1_t2 holds the address of the result
                                        output = NULL; //set output to NULL

                                        if (counter == 0) { //Checks if it is the first time
                                            //Allocate space for the p1_p2
                                            do {p1_p2 = (unsigned int **) malloc (sizeof (unsigned int *) * (t1_t2 [0][0] * 2));} while (p1_p2 == NULL);

                                            //Get the positioning_list of that term from that document. The term of counter
                                            for (unsigned int inside = 0; inside < t1_t2 [0][0]; inside++)
                                                p1_p2 [inside] = get_positioning_of (_mps, mps_size, t1_t2 [0][(inside + 1)], terms [counter]);
                                            
                                            //Get the positioning_list of that term from that document. The term of (counter + 1)
                                            for (unsigned int inside = 0; inside < t1_t2 [0][0]; inside++)
                                                p1_p2 [(inside + t1_t2 [0][0])] = get_positioning_of (_mps, mps_size, t1_t2 [0][(inside + 1)], terms [(counter + 1)]);

                                            //Allocate space for the indexing
                                            do {indexing = (unsigned int *) malloc (sizeof (unsigned int) * (t1_t2 [0][0] + 1));} while (indexing == NULL);
                                            
                                            //Make a copy of t1_t2 [0] and hold the indices of the p1_p2 till the t1_t2 [0][0]
                                            indexing [0] = t1_t2 [0][0];
                                            for (unsigned int inside = 0; inside < t1_t2 [0][0]; indexing [(inside + 1)] = t1_t2 [0][(inside + 1)], ++inside);

                                        } else { //Means it is not the first time
                                            unsigned int i = 1; //Iterator for the new t1_t2 [0].
                                            unsigned int x = 1; //Iterator for the indexing [0].
                                            unsigned int y = 0; //Iterator for p1_p2.

                                            while (i <= t1_t2 [0][0] && x <= indexing [0]) { //Loop until the one of the list reaches its end.
                                                if (t1_t2 [0][i] == indexing [x]) {
                                                    p1_p2 [y++] = p1_p2 [(x - 1)];
                                                    i++;
                                                }

                                                x++;
                                            }

                                            //Allocate the indexing
                                            do {output = realloc (indexing, sizeof (unsigned int) * (t1_t2 [0][0] + 1));} while (output == NULL);
                                            if (output != indexing) //Check if the address is not the same
                                                indexing = (unsigned int *) output;

                                            output = NULL;

                                            //Let the first index hold the size
                                            indexing [0] = t1_t2 [0][0];
                                            //Contain the copy of the t1_t2
                                            for (unsigned int inside = 1; inside < t1_t2 [0][0]; indexing [inside] = t1_t2 [0][inside], ++inside);

                                            //Realloc to the new size
                                            do {output = realloc (p1_p2, sizeof (unsigned int *) * (t1_t2 [0][0] * 2));} while (output == NULL);
                                            if (output != p1_p2)
                                                p1_p2 = (unsigned int **) output;
                                            
                                            output = NULL;

                                            //Get the positioning_list for the second half of the p1_p2
                                            for (unsigned int inside = t1_t2 [0][0]; inside < (t1_t2 [0][0] * 2); ++inside) {
                                                if (p1_p2 [inside] != NULL) {
                                                    free (p1_p2 [inside]);
                                                    p1_p2 [inside] = NULL;
                                                }

                                                p1_p2 [inside] = get_positioning_of (_mps, mps_size, t1_t2 [0][((inside + 1) - t1_t2 [0][0])], terms [(counter + 1)]);
                                            }
                                        }

                                        //Which will be passed to the perfrom anding operation. To perform anding on positions
                                        unsigned int ** processing = NULL;
                                        do {processing = (unsigned int **) malloc (sizeof (unsigned int *) * 2);} while (processing == NULL);

                                        unsigned int real_size = t1_t2 [0][0]; //Hold the or number of documents for this intersection

                                        for (unsigned inside = 0; inside < t1_t2 [0][0]; ++inside) { //Loop till the length of the t1_t2
                                            processing [0] = p1_p2 [inside];
                                            processing [1] = p1_p2 [(t1_t2 [0][0] + inside)];
                                            
                                            output = perform_and_operation (&processing, 1, 3); //Perform anding on positions where k is 1.

                                            free (p1_p2 [inside]); //The list in the inside
                                            p1_p2 [inside] = NULL;

                                            if (output != NULL) { //If the operation is done correctly
                                                p1_p2 [inside] = (unsigned int *) output; //Take the addres and assign it to the p1_p2 of inside
                                                output = NULL;
                                            } else //Means there is no itersection
                                                --real_size; //Decrease the size by one
                                        }

                                        //Free processing
                                        processing [0] = NULL;
                                        processing [1] = NULL;
                                        free (processing);
                                        processing = NULL;

                                        //See if the current documents holds intersection and the size is not 0
                                        if (real_size != t1_t2 [0][0] && real_size != 0) {
                                            unsigned int update = 0;

                                            for (unsigned int inside = 0; inside < t1_t2 [0][0]; ++inside) //Loop until we reach the size of the t1_t2
                                                if (p1_p2 [inside] == NULL) { //If the p1_p2 of that document is NULL, which means no itersection has happend
                                                    t1_t2 [0][(inside + 1)] = 0; //Set that document id to 0, which means not with us
                                                    update++;
                                                }

                                            //Let all nulls be at the end of the list
                                            for (unsigned int inside = 0; inside < ((t1_t2 [0][0] * 2) - 1); ++inside)
                                                if (p1_p2 [inside] == NULL) {
                                                    p1_p2 [inside] = p1_p2 [(inside + 1)];
                                                    p1_p2 [(inside + 1)] = NULL;
                                                }

                                            //Reallocate the p1_p2 list
                                            output = NULL;
                                            do {output = realloc (p1_p2, sizeof (unsigned int *) * ((t1_t2 [0][0] * 2) - update));} while (output == NULL);
                                            if (output != p1_p2)
                                                p1_p2 = (unsigned int **) output;
                                            
                                            output = NULL;

                                            //Create the new list of documentIDs
                                            unsigned int * new_posting_list = NULL;
                                            do {new_posting_list = (unsigned int *) malloc (sizeof (unsigned int) * (real_size + 1));} while (new_posting_list == NULL);

                                            new_posting_list [0] = real_size;

                                            real_size = 0;
                                            for (unsigned int inside = 0; inside < t1_t2 [0][0]; ++inside)
                                                if (t1_t2 [0][(inside + 1)] != 0) {
                                                    new_posting_list [(real_size + 1)] = t1_t2 [0][(inside + 1)];
                                                    ++real_size;
                                                }

                                            //Free the old list
                                            free (t1_t2 [0]);
                                            t1_t2 [0] = NULL;

                                            t1_t2 [0] = new_posting_list;
                                            new_posting_list = NULL;

                                            //Realloc the indexing
                                            do {output = realloc (indexing, sizeof (unsigned int) * (t1_t2 [0][0] + 1));} while (output == NULL);
                                            if (output != indexing)
                                                indexing = (unsigned int *) output;

                                            output = NULL;

                                            //Make a copy of the t1_t2 [0].
                                            indexing [0] = t1_t2 [0][0];
                                            for (unsigned int inside = 0; inside < t1_t2 [0][0]; indexing [(inside + 1)] = t1_t2 [0][(inside + 1)], ++inside);
                                        } else if (real_size == 0) { //Means all documents does not intersect
                                            //Free all
                                            for (unsigned int inside = 0; inside < (t1_t2 [0][0] * 2); ++inside)
                                                if (p1_p2 [inside] != NULL) {
                                                    free (p1_p2 [inside]);
                                                    p1_p2 [inside] = NULL;
                                                }

                                            free (p1_p2);
                                            p1_p2 = NULL;

                                            free (t1_t2 [0]);
                                            t1_t2 [0] = NULL;
                                            free (t1_t2 [1]);
                                            t1_t2 [1] = NULL;
                                            free (t1_t2);
                                            t1_t2 = NULL;

                                            free (indexing);
                                            indexing = NULL;

                                            for (unsigned int counter = 0; counter < (*terms_size); free (terms [counter]), terms [counter] = NULL, ++counter);
                                            free (terms);
                                            terms = NULL;
                                            free (terms_size);
                                            terms_size = NULL;

                                            break;
                                        }
                                    } else { //Means there is no intersection between the two posting list.
                                        if (p1_p2 != NULL) {
                                            for (unsigned int inside = 0; inside < (t1_t2 [0][0] * 2); ++inside)
                                                if (p1_p2 [inside] != NULL) {
                                                    free (p1_p2 [inside]);
                                                    p1_p2 [inside] = NULL;
                                                }

                                            free (p1_p2);
                                            p1_p2 = NULL;
                                        }

                                        free (t1_t2 [0]);
                                        t1_t2 [0] = NULL;

                                        free (t1_t2 [1]);
                                        t1_t2 [1] = NULL;

                                        free (t1_t2);
                                        t1_t2 = NULL;

                                        if (indexing != NULL) {
                                            free (indexing);
                                            indexing = NULL;
                                        }
                                        
                                        for (unsigned int counter = 0; counter < (*terms_size); free (terms [counter]), terms [counter] = NULL, ++counter);
                                        free (terms);
                                        terms = NULL;
                                        free (terms_size);
                                        terms_size = NULL;

                                        break;
                                    }

                                    free (t1_t2 [1]);
                                    t1_t2 [1] = NULL;
                                } else { //Means on the terms of the query does not exist.
                                    if (p1_p2 != NULL) {
                                        for (unsigned int inside = 0; inside < (t1_t2 [0][0] * 2); ++inside)
                                            if (p1_p2 [inside] != NULL) {
                                                free (p1_p2 [inside]);
                                                p1_p2 [inside] = NULL;
                                            }

                                        free (p1_p2);
                                        p1_p2 = NULL;
                                    }

                                    if (t1_t2 [0] != NULL) { //Checks if the first is not null to free it
                                        free (t1_t2 [0]);
                                        t1_t2 [0] = NULL;
                                    }

                                    if (t1_t2 [1] != NULL) { //Checks if the second is not null to free it
                                        free (t1_t2 [1]);
                                        t1_t2 [1] = NULL;
                                    }

                                    //Free the list
                                    free (t1_t2);
                                    t1_t2 = NULL;

                                    if (indexing != NULL) {
                                        free (indexing);
                                        indexing = NULL;
                                    }

                                    for (unsigned int counter = 0; counter < (*terms_size); free (terms [counter]), terms [counter] = NULL, ++counter);
                                    free (terms);
                                    terms = NULL;
                                    free (terms_size);
                                    terms_size = NULL;

                                    break;
                                }
                            }

                            if (t1_t2 != NULL) { //Checks after we finishing looping that the t1_t2 is not freed or NULL.
                                _return_ = t1_t2 [0]; //Lest the _return_ contains that address of the first array of t1_t2

                                //Free p1_p2
                                for (unsigned int counter = 0; counter < (t1_t2 [0][0] * 2); ++counter)
                                    if (p1_p2 [counter] != NULL) {
                                        free (p1_p2 [counter]);
                                        p1_p2 [counter] = NULL;
                                    }

                                free (p1_p2);
                                p1_p2 = NULL;

                                t1_t2 [0] = NULL;
                                //Free t1_t2
                                free (t1_t2);
                                t1_t2 = NULL;

                                //free indexing if it is not already freed.
                                if (indexing != NULL) {
                                    free (indexing);
                                    indexing = NULL;
                                }

                                if (!sort_query_result (_return_, &terms, terms_size, _mps, mps_size, _tfidf, doc_tokens, n_docs, n_tokens_per_doc)) {
                                    free (_return_);
                                    _return_ = NULL;
                                }

                                for (unsigned int counter = 0; counter < (*terms_size); free (terms [counter]), terms [counter] = NULL, ++counter);
                                free (terms);
                                terms = NULL;
                                free (terms_size);
                                terms_size = NULL;
                            }
                        }
                    } else//Means the query contains only one word
                        _return_ = get_posting_of (_mps, mps_size, n_docs, (*query)); //Get the posting list of that term
                } else { //Means we using boolean model.
                
                }

                return _return_; //Return Array which holds the documentIDs that contains the query. In case of phrase (free) model the documentIDs will be sorted on the similarity.
            } else
                return NULL;
        } else
            return NULL;
    } else
        return NULL;
}


/*
INPUT: A pointer to a 3d array of string, a pointer to an unsigned int which hold the number of documents and an array which holds the number of tokens in each document.
OUTPUT: Return a 3d array of unsigned int, otherwise NULL.
INFO: This function is used to build the inverted index of the documentTOkens
NOTE: The return consist of 3 indices:
[0][0][0] = docFrequency
[1][0][0] = terFrequency
[2] = posting list.
*/
unsigned int *** build_inverted_index (string *** doc_tokens, unsigned int * n_docs, unsigned int * n_tokens_per_doc) {
    if (doc_tokens != NULL && n_docs != NULL && n_tokens_per_doc != NULL) { //Checks if the passed arguments are passed correctly
        if ((*doc_tokens) != NULL && (*n_docs) != 0 && (*n_tokens_per_doc) != 0) { //Checks if the passed arguments holds and points at the correct data.
            unsigned int mps_size = get_mps_size (doc_tokens, n_docs, n_tokens_per_doc); //Get the total number of distinict terms

            if (mps_size != 0) { //Checks if the result does not equal zero
                unsigned int * meta = NULL; //Allocate place for meta, which hold information about doc_tokens
                do {meta = (unsigned int *) malloc (sizeof (unsigned int) * ((*n_docs) + 2));} while (meta == NULL);
                
                meta [0] = 0; //Set the total number of tokens to zero
                meta [1] = (*n_docs); //Set the number of document to the meta [1]
                //For each document, add the number of tokens in that document to the meta [0] and set the meta [2 + counter] to hold the number of tokens in that document.
                for (unsigned int counter = 0; counter < (*n_docs); meta [0] += n_tokens_per_doc [counter], meta [(2 + counter)] = n_tokens_per_doc [counter], ++counter);

                //Degrade the 3d list to 2d
                string * terms = degrade_list_2 (doc_tokens, meta);

                //Checks if the degrading is successful
                if (terms != NULL) {
                    if (remove_repeated_term_token_2 (&terms, &meta [0])) { //Remove the repeated tokens
                        //de-allocate meta from the memory
                        free (meta);
                        meta = NULL;

                        //Sort the terms alphabitacly
                        if (sort_term_token_2 (&terms, &mps_size)) {

                            //Allocate place for the return result.
                            unsigned int *** _return_ = NULL;
                            do {_return_ = (unsigned int ***) malloc (sizeof (unsigned int **) * 3);} while (_return_ == NULL);

                            //An array which holds the document frequency for each term
                            do {_return_ [0] = (unsigned int **) malloc (sizeof (unsigned int *));} while (_return_ [0] == NULL);
                            do {_return_ [0][0] = (unsigned int *) calloc (mps_size, sizeof (unsigned int));} while (_return_ [0][0] == NULL);

                            //An array which holds the term frequency for each term
                            do {_return_ [1] = (unsigned int **) malloc (sizeof (unsigned int *));} while (_return_ [1] == NULL);
                            do {_return_ [1][0] = (unsigned int *) calloc (mps_size, sizeof (unsigned int));} while (_return_ [1][0] == NULL);

                            //An array which holds the posting list for each term.
                            do {_return_ [2] = (unsigned int **) malloc (sizeof (unsigned int *) * mps_size);} while (_return_ [2] == NULL);
                            for (unsigned int counter = 0; counter < mps_size; ++counter)
                                do {_return_ [2][counter] = (unsigned int *) calloc ((*n_docs), sizeof (unsigned int));} while (_return_ [2][counter] == NULL);

                            unsigned int outter_mps = 0; //Used for an iterator to the _return_ [0], _return_ [1] and _return_ [2]

                            for (unsigned int counter = 0; counter < mps_size; ++counter) { //For each term
                                for (unsigned int docID = 0; docID < (*n_docs); ++docID) { //Loop through each document
                                    _Bool does_exist = 0; //Used to check whether the term exists in that document
                                    
                                    for (unsigned int tokenID = 0; tokenID < n_tokens_per_doc [docID]; ++tokenID) { //Check if the term exist in that document
                                        if (compare_string (&terms [counter], (*doc_tokens) [docID][tokenID])) { //Copmare the term to the tokens exists in that document
                                            does_exist = 1; //Se the does_exist to 1
                                            ++_return_ [1][0][outter_mps]; //Increment the term_frequency by one
                                        }
                                    }

                                    if (does_exist) { //If the term did exist in that document
                                        ++_return_ [0][0][outter_mps]; //Increment the document frequency by one
                                        _return_ [2][outter_mps][docID] = (docID + 1); //Let Assign the id of the document to the posting list
                                    }
                                        
                                }

                                ++outter_mps;
                            }

                            unsigned int ** posting_list = NULL; //Create the modified posting list.

                            //Allocate space for it
                            do {posting_list = (unsigned int **) malloc (sizeof (unsigned int *) * mps_size);} while (posting_list == NULL);

                            //For each term allocat to it a precise length of the posting length
                            for (unsigned int counter = 0; counter < mps_size; ++counter)
                                do {posting_list [counter] = (unsigned int *) malloc (sizeof (unsigned int) * _return_ [0][0][counter]);} while (posting_list == NULL);
                            
                            //For each terms
                            for (unsigned int counter = 0; counter < mps_size; ++counter) {
                                unsigned int i = 0; //Which holds the iterator for the new posting list
                                
                                for (unsigned int inside = 0; inside < (*n_docs); ++inside) //Check the old posting list value
                                    if (_return_ [2][counter][inside] != 0) //If the value does not equal to zero
                                        posting_list [counter][i++] = _return_ [2][counter][inside]; //Add this value to the new posting list of that term and increment i by one
                                
                                free (_return_ [2][counter]); //Free the old posting list
                                _return_ [2][counter] = NULL;
                            }

                            //Free the array of posting list
                            free (_return_ [2]);
                            _return_ [2] = NULL;

                            //Assign the new posting list.
                            _return_ [2] = posting_list;
                            posting_list = NULL;

                            for (unsigned int counter = 0; counter < mps_size; free (terms [counter]), terms [counter] = NULL, ++counter);
                            free (terms);
                            terms = NULL;

                            return _return_;
                        } else {
                            for (unsigned int counter = 0; counter < mps_size; free (terms [counter]), terms [counter] = NULL, ++counter);
                            free (terms);
                            terms = NULL;
                            return NULL;
                        }
                    } else {
                        for (unsigned int counter = 0; counter < meta [0]; free (terms [counter]), terms [counter] = NULL, ++counter);
                        free (terms);
                        terms = NULL;
                        free (meta);
                        meta = NULL;
                        return NULL;
                    }
                } else {
                    free (meta);
                    meta = NULL;
                    return NULL;
                }
            } else //Means the user sent an empty list
                return NULL;
        } else
            return NULL;
    } else
        return NULL;
}


/*
INPUT: A pointer to a 3d array of strings, a pointer to an unsigned int which hold the number of documents and an array of an unsigned int which holds the number of tokens in each doc
OUTPUT: return a 3d array of size 3, otherwise return NULL
INFO: This function is used to build the incidenc_matrix of the doc_tokens.
Note: The return will hold.
    [0][0] = doc frequency.
    [1][0] = ter frequency.
    [2] = incidence vector for each term.
*/
unsigned int *** build_incidence_matrix (string *** doc_tokens, unsigned int * n_docs, unsigned int * n_tokens_per_doc) {
    if (doc_tokens != NULL && n_docs != NULL && n_tokens_per_doc != NULL) { //Checks if the passed arguments are passed correctly.
        if ((*doc_tokens) != NULL && (*n_docs) != 0 && (*n_tokens_per_doc) != 0) { //Checks if the passed arguments holds and points to the correct data
            unsigned int mps_size = get_mps_size (doc_tokens, n_docs, n_tokens_per_doc); //Get the mps_size. Which means the total number of distincts terms in all of the documents

            if (mps_size != 0) { //Checks if the mps_size does not equal to zero.

                //Allocate space for the meta.
                unsigned int * meta = NULL;
                do {meta = (unsigned int *) malloc (sizeof (unsigned int) * ((*n_docs) + 2));} while (meta == NULL);
                meta [1] = (*n_docs);
                meta [0] = 0;
                for (unsigned int counter = 0; counter < (*n_docs); meta [0] += n_tokens_per_doc [counter], meta [(2 + counter)] = n_tokens_per_doc [counter], ++counter);

                string * terms = degrade_list_2 (doc_tokens, meta); //Holds the distinicts tokens.

                if (terms != NULL) { //Checks if the degrading is done correctly.
                    if (remove_repeated_term_token_2 (&terms, &meta [0])) {

                        free (meta);
                        meta = NULL;

                        if (sort_term_token_2 (&terms, &mps_size)) {
                            unsigned int *** _return_ = NULL; //The hold the return result.
                            do {_return_ = (unsigned int ***) malloc (sizeof (unsigned int **) * 3);} while (_return_ == NULL); //Allocate space for the return result.

                            //Allocate space for the first index of the return result.
                            do {_return_ [0] = (unsigned int **) malloc (sizeof (unsigned int *));} while (_return_ [0] == NULL);
                            do {_return_ [0][0] = (unsigned int *) calloc (mps_size, sizeof (unsigned int));} while (_return_ [0][0] == NULL);

                            //Allocate space for the second index of the return result.
                            do {_return_ [1] = (unsigned int **) malloc (sizeof (unsigned int *));} while (_return_ [0] == NULL);
                            do {_return_ [1][0] = (unsigned int *) calloc (mps_size, sizeof (unsigned int));} while (_return_ [1][0] == NULL);

                            //Allocate space for the incidence vector.
                            do {_return_ [2] = (unsigned int **) malloc (sizeof (unsigned int *) * mps_size);} while (_return_ [2] == NULL);
                            for (unsigned int counter = 0; counter < mps_size; ++counter)
                                do {_return_ [2][counter] = (unsigned int *) calloc ((*n_docs), sizeof (unsigned int));} while (_return_ [2][counter] == NULL);

                            unsigned int outter_mps = 0; //Helps in _return_ assignment.

                            //For each distinict term.
                            for (unsigned int counter = 0; counter < mps_size; ++counter) { //For each distinict term
                                for (unsigned int docID = 0; docID < (*n_docs); ++docID) { //Loop through each document
                                    _Bool does_exist = 0; //Used to check if the term exist in that document. in order to set one in the term vector.

                                    //Compare tokens with term if match increment tf and df and assign to the term vector the docID to 1.
                                    for (unsigned int tokenID = 0; tokenID < n_tokens_per_doc [docID]; ++tokenID) {
                                        if (compare_string (&terms [counter], (*doc_tokens) [docID][tokenID])) {
                                            does_exist = 1;
                                            ++_return_ [1][0][outter_mps];
                                            _return_ [2][outter_mps][docID] = 1;
                                        }
                                    }

                                    if (does_exist)
                                        ++_return_ [0][0][outter_mps];
                                }

                                ++outter_mps;
                            }

                            for (unsigned int counter = 0; counter < mps_size; free (terms [counter]), terms [counter] = NULL, ++counter);
                            free (terms);
                            terms = NULL;

                            return _return_;
                        } else {
                            //Deallocate the terms var from memory
                            for (unsigned int counter = 0; counter < meta [0]; free (terms [counter]), terms [counter] = NULL, ++counter);
                            free (terms);
                            terms = NULL;
                            free (meta);
                            meta = NULL;
                            return NULL;
                        }
                    } else {
                        //Deallocate the terms var from memory
                        for (unsigned int counter = 0; counter < meta [0]; free (terms [counter]), terms [counter] = NULL, ++counter);
                        free (terms);
                        terms = NULL;
                        free (meta);
                        meta = NULL;
                        return NULL;
                    }
                } else {
                    //Deallocate the meta from the memory.
                    free (meta);
                    meta = NULL;

                    return NULL;
                }
            } else
                return NULL;
        } else
            return NULL;
    } else
        return NULL;
}


/*
INPUT: A pointer to a 3d array of strings, a pointer to an unsigned int which hold the number of documents and an array of an unsigned int which holds the number of tokens in each doc
OUTPUT: Return a 4d array of unsigned int, otherwise NULL.
INFO: This function is used to build the positional index of the documentTOkens
NOTE: The return consist of 2 indices:
[0] = holds the inverted_index data
[1] = positiong list
*/
unsigned int **** build_positional_index (string *** doc_tokens, unsigned int * n_docs, unsigned int * n_tokens_per_doc) {
    if (doc_tokens != NULL && n_docs != NULL && n_tokens_per_doc != NULL) { //Checks if the passed arguments are passed correctly.
        if ((*doc_tokens) != NULL && (*n_docs) != 0 && (*n_tokens_per_doc) != 0) { //Checks if the passed arguments holds and points at the correct data.
            unsigned int mps_size = get_mps_size (doc_tokens, n_docs, n_tokens_per_doc); //Get the mps_size which is the total number of the distinict tokens in all of the documents

            if (mps_size != 0) { //Checks if the result is not zero
                unsigned int * meta = NULL; //Allocate space for the meta, which hold information about the doc_tokens
                do {meta = (unsigned int *) malloc (sizeof (unsigned int) * ((*n_docs) + 2));} while (meta == NULL);

                meta [0] = 0; //This index will holds the total number of tokens in all of the doucments.
                meta [1] = (*n_docs); //This index whill hold the number of doucments

                //In each document, increment its number of documents to the first index and set the 2 + current document to the number of the tokens in that document.
                for (unsigned int counter = 0; counter < (*n_docs); meta [0] += n_tokens_per_doc [counter], meta [(2 + counter)] = n_tokens_per_doc [counter], ++counter);

                string * terms = degrade_list_2 (doc_tokens, meta); //Degrage from 3d to 2d
                if (terms != NULL) { //Checks if the return is correctly
                    if (remove_repeated_term_token_2 (&terms, &meta [0])) { //Remove all the duplicates in the 2d list.
                        //Free the meta
                        free (meta);
                        meta = NULL;

                        //Sort the list in alphabitical order.
                        if (sort_term_token_2 (&terms, &mps_size)) {
                            unsigned int *** inverted_index = build_inverted_index (doc_tokens, n_docs, n_tokens_per_doc); //Build the inverted_index 

                            if (inverted_index != NULL) { //Checks if the inverted_index is created successfuly.
                                unsigned int **** positioning_list = NULL; //Allocate space for the posting list.
                                do {positioning_list = (unsigned int ****) malloc (sizeof (unsigned int ***) * 2);} while (positioning_list == NULL);

                                //Let the first index of the positional_index holds the data from the inverted index
                                positioning_list [0] = inverted_index;
                                inverted_index = NULL;

                                //The second index hold the positional list for each term in each documents depending on the document frequency.
                                do {positioning_list [1] = (unsigned int ***) malloc (sizeof (unsigned int **) * mps_size);} while (positioning_list [1] == NULL);

                                //For each term
                                for (unsigned int counter = 0; counter < mps_size; ++counter) {
                                    //Allocate space depending on the document frequency, which means in how many documents does that term appeard in
                                    do {positioning_list [1][counter] = (unsigned int **) malloc (sizeof (unsigned int *) * positioning_list [0][0][0][counter]);} while (positioning_list [1][counter] == NULL);

                                    //And for each one of these document allocate space to hold the postion of that term. The size should be the maximum number of tokens in that document in addition to one which hold the size of the positional list of that document
                                    for (unsigned int inside = 0; inside < positioning_list [0][0][0][counter]; ++inside)
                                        do {positioning_list [1][counter][inside] = (unsigned int *) calloc ((n_tokens_per_doc [(positioning_list [0][2][counter][inside] - 1)] + 1), sizeof (unsigned int));} while (positioning_list [1][counter][inside] == NULL);
                                }

                                //For each term
                                for (unsigned int counter = 0; counter < mps_size; ++counter) {
                                    for (unsigned int documents = 0; documents < positioning_list [0][0][0][counter]; ++documents) { //Go through its document frequency
                                        unsigned int real_size = 1; //The iterator for the positional list of that term in that document

                                        //Loop throguh all of the tokens in that doucment
                                        for (unsigned int tokens = 1; tokens <= n_tokens_per_doc [(positioning_list [0][2][counter][documents] - 1)]; ++tokens) {
                                            if (compare_string (&terms [counter], (*doc_tokens) [(positioning_list [0][2][counter][documents] - 1)][(tokens - 1)])) { //If the token match the term
                                                positioning_list [1][counter][documents][real_size++] = tokens; //Save the position of that token in the positional list
                                            }
                                        }

                                        positioning_list [1][counter][documents][0] = (real_size - 1); //Save the number of positions of that term in that document

                                        //Reallocate that positional list to the right size from the real_size var
                                        void * reallocation = NULL;
                                        do {reallocation = realloc (positioning_list [1][counter][documents], sizeof (unsigned int) * real_size);} while (reallocation == NULL);

                                        if (reallocation != positioning_list [1][counter][documents])
                                            positioning_list [1][counter][documents] = (unsigned int *) reallocation;
                                        
                                        reallocation = NULL;
                                    }
                                }

                                //Free the terms.
                                for (unsigned int counter = 0; counter < mps_size; free (terms [counter]), terms [counter] = NULL, ++counter);
                                free (terms);
                                terms = NULL;

                                //Return the positional index.
                                return positioning_list;
                            } else {
                                for (unsigned int counter = 0; counter < mps_size; free (terms [counter]), terms [counter] = NULL, ++counter);
                                free (terms);
                                terms = NULL;
                                return NULL;
                            }
                        } else {
                            for (unsigned int counter = 0; counter < mps_size; free (terms [counter]), terms [counter] = NULL, ++counter);
                            free (terms);
                            terms = NULL;
                            return NULL;
                        }
                    } else {
                        for (unsigned int counter = 0; counter < meta [0]; free (terms [counter]), terms [counter] = NULL, ++counter);
                        free (terms);
                        terms = NULL;
                        free (meta);
                        meta = NULL;
                        return NULL;
                    }
                } else {
                    free (meta);
                    meta = NULL;
                    return NULL;
                }
            } else
                return NULL;
        } else
            return NULL;
    } else
        return NULL;
}


/*
INPUT: A pointer to an unsigned int which holds the number of distinict tokens in the query and a pointer to tfidf
OUTPUT: Returns non negative value which holds ec-length of the query, otherwise return -1.5.
INFO: This function calculate the ec-length of the query.
*/
double query_ec_length (unsigned int * mps_size, tfidf * _tfidf) {
    if (mps_size != NULL && _tfidf != NULL) { //Check that the passed arguments are passed correctly
        if ((*mps_size) != 0 && (*_tfidf) != NULL) { //Checks that the passed arguments points and holds the correct data
            double length = 0.0; //The return resutl which is ec_length of the query

            for (unsigned int counter = 0; counter < (*mps_size); ++counter) //For each term frequency of the query
                length += pow ((*_tfidf) -> tf [counter][0], 2); //Get is power and sum it to the length

            length = sqrt (length); //Get the square root of the length

            return length; //Return it.
        } else
            return -1.5;
    } else
        return -1.5;
}


/*
INPUT: A pointer to 3d array which is list of documents, the number of documents, an array which holds the number of tokens in each document, a pointer to mps
    a pointer to the mps size and a pointer to tfidf.

OUTPUT: Return an array of size n_docs, otherwise returns NULL.
INFO: This calculate the ec-length for the whole documents
*/
double * doc_ec_length (string *** doc_tokens, unsigned int * n_docs, unsigned int * n_tokens_per_doc, mps * _mps, unsigned int * mps_size, tfidf * _tfidf) {
    if (doc_tokens != NULL && n_docs != NULL && n_tokens_per_doc != NULL && _mps != NULL && mps_size != NULL && _tfidf != NULL) { //Checks if the passed arguments are passed correctly.
        if ((*doc_tokens) != NULL && (*n_docs) != 0 && (*n_tokens_per_doc) != 0 && (*_mps) != NULL && (*mps_size) != 0 && (*_tfidf) != NULL) { //Checks if the passed arguments holds and pointing at the right data
            double * length = NULL; //The returned array
            //Allocate it
            do {length = (double *) calloc ((*n_docs), sizeof (double));} while (length == NULL);

            for (unsigned int counter = 0; counter < (*n_docs); ++counter) { //For each document
                unsigned int * meta = NULL; //Create a meta which holds information about the document like how many tokens in it
                do {meta = (unsigned int *) malloc (sizeof (unsigned int) * 3);} while (meta == NULL);

                meta [0] = n_tokens_per_doc [counter];
                meta [1] = 1;
                meta [2] = n_tokens_per_doc [counter];

                string ** temp = &(*doc_tokens) [counter];

                string * terms = degrade_list_2 (&temp, meta); //Make a copy of that document tokens
                temp = NULL;

                remove_repeated_term_token_2 (&terms, &meta[0]); //Remove the repeated terms/tokens
                sort_term_token_2 (&terms, &meta [0]); //Sort it

                unsigned int index = 0; //Holds the index of the mps
                _Bool found = 0; //CHecks if the term is found or not
                for (unsigned int inside = 0; inside < meta [0]; ++inside) { //For each distinict term in that document
                    for (unsigned int i = 0; i < (*mps_size); ++i) //Loop through the mps
                        if (compare_string (&terms [inside], (*_mps) [i].term)) { //Compare the distinict terms with the terms in the mps. IF the same
                            index = i; //Hold the mps index of that term
                            found = 1; //Set found to 1
                            break; //Break from the loop
                        }

                    if (found) { //Check if we found the term ferquency of that term of that document
                        length [counter] += pow ((*_tfidf) -> tf [index][counter], 2); //Get its power and sum it to the length of that document
                        found = 0; //Set found back to 0
                    } //else means the length of that counter is 0
                }

                //Free the copy
                for (unsigned int inside = 0; inside < meta [0]; free (terms [inside]), terms [inside] = NULL, ++inside);
                free (terms);
                terms = NULL;
                //Free meta
                free (meta);
                meta = NULL;

                //Get the square root of that length
                length [counter] = sqrt (length [counter]);
            }

            return length; //Return the arry
        } else
            return NULL;
    } else
        return NULL;
}


/*
INPUT: A pointer to a 3d array of strings, a pointer to an unsigned int which hold the number of documents and an array of unsigned int which holds the number of tokens for each document.
OUTPUT: Returns a pointer to tfidf, otherwie NULL
INFO: This function is used to build the tfidf
*/
tfidf build_tfidf (string *** doc_tokens, unsigned int * n_docs, unsigned int * n_tokens_per_doc) {
    if (doc_tokens != NULL && n_docs != NULL && n_tokens_per_doc != NULL) { //checks if the passed arguments are passed correctly.
        if ((*doc_tokens) != NULL && (*n_docs) != 0 && (*n_tokens_per_doc) != 0) { //Checks if the passed arguments are holding and pointing at the right data.
            unsigned int mps_size = get_mps_size (doc_tokens, n_docs, n_tokens_per_doc); //Get mps size, which means the total number of distinict tokens in all of the documents

            if (mps_size != 0) { //Checks if the size is not zero
                unsigned int * meta = NULL; //Meta data about the whole documents
                do {meta = (unsigned int *) malloc (sizeof (unsigned int) * ((*n_docs) + 2));} while (meta == NULL);

                meta [0] = 0;
                meta [1] = (*n_docs);
                for (unsigned int counter = 0; counter < (*n_docs); meta [0] += n_tokens_per_doc [counter], meta [(2 + counter)] = n_tokens_per_doc [counter], ++counter);

                string * terms = degrade_list_2 (doc_tokens, meta); //Transform the 3d to 2d

                if (terms != NULL) { //Checks if the transformation is done successfully.
                    if (remove_repeated_term_token_2 (&terms, &meta [0])) { //Remove any duplicates
                        //Deallocate meta from the memory.
                        free (meta);
                        meta = NULL;

                        if (sort_term_token_2 (&terms, &mps_size)) { //Sort tokens alphabiticly
                            tfidf _tfidf = NULL; //The return.
                            do {_tfidf = (tfidf) malloc (sizeof (struct _tfidf_));} while (_tfidf == NULL);

                            _tfidf -> tf = NULL;
                            _tfidf -> idf = NULL;
                            _tfidf -> tfidf = NULL;

                            do {_tfidf -> tf = (double **) malloc (sizeof (double *) * mps_size);} while (_tfidf -> tf == NULL);
                            do {_tfidf -> idf = (double **) malloc (sizeof (double *) * mps_size);} while (_tfidf -> idf == NULL);
                            do {_tfidf -> tfidf = (double **) malloc (sizeof (double *) * mps_size);} while (_tfidf -> tfidf == NULL);

                            for (unsigned int counter = 0; counter < mps_size; ++counter) {
                                do {_tfidf -> tf [counter] = (double *) calloc ((*n_docs), sizeof (double));} while (_tfidf -> tf [counter] == NULL);
                                do {_tfidf -> idf [counter] = (double *) calloc ((*n_docs), sizeof (double));} while (_tfidf -> idf [counter] == NULL);
                                do {_tfidf -> tfidf [counter] = (double *) calloc ((*n_docs), sizeof (double));} while (_tfidf -> tfidf [counter] == NULL);
                            }
   
                            for (unsigned int counter = 0; counter < mps_size; ++counter) {
                                for (unsigned int docID = 0; docID < (*n_docs); ++docID) {
                                    _Bool does_exist = 0;
                                    
                                    for (unsigned int tokens = 0; tokens < n_tokens_per_doc [docID]; ++tokens) {
                                        if (compare_string (&terms [counter], (*doc_tokens) [docID][tokens])) {
                                            does_exist = 1;
                                            _tfidf -> tf [counter][docID] += (double) 1.0;
                                        }
                                    }
                                    
                                    if (does_exist)
                                        _tfidf -> idf [counter][docID] += (double) 1.0;
                                    
                                    if (_tfidf -> tf [counter][docID] != 0)
                                        _tfidf -> tf [counter][docID] = (double) (1 + log10 (_tfidf -> tf [counter][docID]));
                                    
                                    if (_tfidf -> idf [counter][docID] != 0) {
                                        _tfidf -> idf [counter][docID] = (double) ((*n_docs) / _tfidf -> idf [counter][docID]);
                                        _tfidf -> idf [counter][docID] = (double) log10 (_tfidf -> idf [counter][docID]);
                                    }

                                    _tfidf -> tfidf [counter][docID] = _tfidf -> tf [counter][docID] * _tfidf -> idf [counter][docID];
                                }
                            }

                            for (unsigned int counter = 0; counter < mps_size; free (terms [counter]), terms [counter] = NULL, ++counter);
                            free (terms);
                            terms = NULL;

                            return _tfidf;
                        } else {
                            for (unsigned int counter = 0; counter < mps_size; free (terms [counter]), terms [counter] = NULL, ++counter);
                            free (terms);
                            terms = NULL;

                            return NULL;
                        }
                    } else {
                        for (unsigned int counter = 0; counter < meta [0]; free (terms [counter]), terms [counter] = NULL, ++counter);
                        free (terms);
                        terms = NULL;

                        free (meta);
                        meta = NULL;

                        return NULL;
                    }
                } else {
                    free (meta);
                    meta = NULL;
                    
                    return NULL;
                }
            } else
                return NULL;
        } else
            return NULL;
    } else
        return NULL;
}