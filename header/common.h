#ifndef __MY_COMMON_THINGS__HH
#define __MY_COMMON_THINGS__HH

#include "../header/string.h"

#ifdef __cplusplus
extern "C" {
#endif

    string __DELIMITER__;
    string * _STOP_WORDS_;

    void hm (void);
    void hl (void);
    void hr (void);
    void print_bye (void);
    void print_logo (void);
    void clear_screen (void);
    void print_processing (void);
    void print_algorithm_table (void);
    void print_error (string message);
    void print_success (string message);
    

    void **** tokenize (string *** documents, unsigned int * n_docs, unsigned int * document_size);

    _Bool init_delimiter (void);
    _Bool term_delimiter (void);
    _Bool init_stop_words (void);
    _Bool term_stop_words (void);
    _Bool is_hyphen (string * _word);
    _Bool padding (unsigned int * px);
	_Bool is_stop_word (string * _word);
    _Bool change_delimiter (string delimiter);
    _Bool is_boolean_keyword (string * keyword);
    _Bool lower_input (string * document_token_query);
    _Bool get_input (string ** documents, unsigned int * document_size);
    _Bool drop_stop_words (string ** tokens, unsigned int * token_size);
    _Bool sort_term_token_2 (string ** tokens, unsigned int * n_tokens);
    _Bool analyse_query_stop_words (string ** query, unsigned int * n_tokens);
    _Bool print_token_empty_line (unsigned int * largest_token, _Bool is_indexed);
    _Bool print_token_dashed_line (unsigned int * largest_token, _Bool is_indexed);
    _Bool remove_repeated_term_token_2 (string ** tokens, unsigned int * n_tokens);
    _Bool solve_hyphen_problems (string *** tokens, unsigned int * n_docs, unsigned int * token_size);
	_Bool lower_all (string *** documents_query, unsigned int * n_docs, unsigned int * document_size);
    _Bool sort_term_token_1 (string *** doc_tokens, unsigned int * n_docs, unsigned int * n_tokens_per_doc);
    _Bool degrade_list_1 (string *** new_list, string *** old_list, unsigned int * index, unsigned int * meta);
    _Bool get_input_from_file (string file_name, string ** documents_query, unsigned int * document_query_size);
    _Bool remove_repeated_term_token_1 (string *** doc_tokens, unsigned int * n_docs, unsigned int * n_tokens_per_doc);
    _Bool print_doc_tokens (string *** tokens, unsigned int * n_docs, unsigned int * n_tokens_per_doc, unsigned int * largest_token);
    _Bool print_tokens (string ** tokens, unsigned int * n_tokens, unsigned int * largest_token, unsigned int * index, _Bool inc_header);
    
    char which_is_bigger (string * one, string * two);

    unsigned int n_digits (unsigned int number);
    unsigned int largest_token_term (void * tokens_dct, unsigned int * n_docs, unsigned int * n_tokens_per_doc);
    unsigned int get_input_from_files (string * file_names, string *** documents_query, unsigned int n_files, unsigned int ** document_query_size);

    unsigned int * perform_or_operation (unsigned int *** queries, char algo_type);
    unsigned int * perform_and_operation (unsigned int *** queries, unsigned char k, char algo_type);
    unsigned int * perform_not_operation (unsigned int ** query, unsigned int n_docs, char algo_type);

    string check_input (FILE * stream, size_t starter_size);

    string * degrade_list_2 (string *** new_list, unsigned int * meta);

#ifdef __cplusplus
}
#endif

#endif