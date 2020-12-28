#ifndef __HEADER__MPS__
#define __HEADER__MPS__

#include "../header/common.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct _mps_ * mps;
    typedef struct _tfidf_ * tfidf;

    _Bool terminate_mps (mps * _mps, unsigned int * mps_size);
    _Bool terminate_tfidf (tfidf * _tfidf, unsigned int * mps_size);
    _Bool print_mps (mps * _mps, unsigned int * mps_size, unsigned int * n_docs, unsigned int * largest_token);
    _Bool print_tfidf (tfidf * _tfidf, mps * _mps, unsigned int * mps_size, unsigned int *  n_docs, unsigned int * largest_token);

    mps build_mps (string *** doc_tokens, unsigned int * n_docs, unsigned int * n_tokens_per_doc, _Bool model_type, unsigned char algo_type);

    unsigned int get_mps_size (string *** doc_tokens, unsigned int * n_docs, unsigned int * n_tokens_per_doc);

    unsigned int * get_posting_of (mps * _mps, unsigned int * mps_size, unsigned int * n_docs, string term);
    unsigned int * get_positioning_of (mps * _mps, unsigned int * mps_size, unsigned int docID, string term);
    unsigned int * process_query (string * query, mps * _mps, unsigned int * mps_size, tfidf * _tfidf, string *** doc_tokens, unsigned int * n_docs, unsigned int * n_tokens_per_doc);

    unsigned int *** build_inverted_index (string *** doc_tokens, unsigned int * n_docs, unsigned int * n_tokens_per_doc);
    unsigned int *** build_incidence_matrix (string *** doc_tokens, unsigned int * n_docs, unsigned int * n_tokens_per_doc);

    unsigned int **** build_positional_index (string *** doc_tokens, unsigned int * n_docs, unsigned int * n_tokens_per_doc);

    double query_ec_length (unsigned int * mps_size, tfidf * _tfidf);

    double * doc_ec_length (string *** doc_tokens, unsigned int * n_docs, unsigned int * n_tokens_per_doc, mps * _mps, unsigned int * mps_size, tfidf * _tfidf);

    tfidf build_tfidf (string *** doc_tokens, unsigned int * n_docs, unsigned int * n_tokens_per_doc);


#ifdef __cplusplus
}
#endif

#endif