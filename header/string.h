#ifndef __MY__C__STRING__
#define __MY__C__STRING__

#ifdef __cplusplus
extern "C" {
#endif
	typedef char * string;

	//If the value of the user's string can be represented in a struct.
	typedef struct {
		int **** sint;
		string *** strn;
		double **** dble;
		short int **** sshi;
		long long int **** slli;
		unsigned int **** _uint;
		unsigned short int **** ushi;
		unsigned long long int **** ulli;
	} BASIC_STRUCT;

	//If the valie of the user's string can be represented in a union.
	typedef union {
		int **** sint;
		string *** strn;
		double **** dble;
		short int **** sshi;
		long long int **** slli;
		unsigned int **** _uint;
		unsigned short int **** ushi;
		unsigned long long int **** ulli;
	} BASIC_UNION;

	//This are only used in allocate_union_struct_var function
	#define __BASIC__UNION__(var) (*((BASIC_UNION **) var))
	#define __BASIC__STRUCT__(var) (*((BASIC_STRUCT **) var))

	//Functions that returns void *** value
	void *** str_char_split (string * _string, char spliter);
	void *** str_word_split (string * _string, string spliter);


	//Functions that return void ** value
	void ** to_union_struct (string * _string);


	//Function that return _Bool value or _Bool value.
	_Bool is_Htab (char c);
	_Bool is_Vtab (char c);
	_Bool is_space (char c);
	_Bool is_char_alpha (char c);
	_Bool is_char_lower (char c);
	_Bool is_char_upper (char c);
	_Bool is_char_number (char c);
	_Bool is_char_alpnum (char c);
	_Bool is_char_special (char c);
	_Bool title (string * _string);
	_Bool is_var (string * _string);
	_Bool is_scar (string * _string);
	_Bool is_ucar (string * _string);
	_Bool is_sint (string * _string);
	_Bool is_uint (string * _string);
	_Bool is_sshi (string * _string);
	_Bool is_ushi (string * _string);
	_Bool is_slli (string * _string);
	_Bool is_ulli (string * _string);
	_Bool is_class (string * _string);
	_Bool is_type (string * datatype);
	_Bool is_alpha (string * _string);
	_Bool is_char_white_space (char c);
	_Bool is_constraint (string * var);
	_Bool is_astrik (string * _string);
	_Bool is_double (string * _string);
	_Bool is_alpnum (string * _string);
	_Bool is_number (string * _string);
	_Bool is_keyword (string * _string);
	_Bool is_special (string * _string);
	_Bool is_abstract (string * _string);
	_Bool term_string (string * _string);
	_Bool is_modifier (string * _string);
	_Bool is_interface (string * _string);
	_Bool is_white_space (string * _string);
	_Bool reverse_string (string * _string);
	_Bool is_it_function (string * _string);
	_Bool is_var_definition (string * _string);
	_Bool is_var_declaration (string * _string);
	_Bool is_it_ptr_function (string * _string);
	_Bool check_experssion (string * experssion);
	_Bool check_func_parameters (string * parameters);
	_Bool string_override (string * main, string new);
	_Bool init_string (string * _string, string data);
	_Bool combine (string * main, string * secondary);
	_Bool display_analysis (unsigned int *** analysis);
	_Bool remove_at (string * _string, unsigned int c);
	_Bool contains (string * _string, string * checker);
	_Bool does_it_end_with (string * _string, string end);
	_Bool compare_string (string * main, string secondary);
	_Bool does_it_begin_with (string * _string, string begin);
	_Bool does_value_match_type (string * value, string * type);
	_Bool remove_repeated_char (string * _string, char spliter);
	_Bool remove_repeated_word (string * _string, string spliter);
	_Bool append (string * _string, _Bool type, string * content);
	_Bool insert_at (string * _string, unsigned int index, char c);
	_Bool is_str_in (string ** list, unsigned int * len, string str);
	_Bool term_split (string ** split_content, unsigned int ** size);
	_Bool insert_word_at (string * _string, unsigned int index, string word);
	_Bool bind_at (string * _string, unsigned int index, string * to_be_bind);
	_Bool to_upper (string * _string, unsigned int size, unsigned int * indices);
	_Bool to_lower (string * _string, unsigned int size, unsigned int * indices);
	_Bool remove_at_these (string * _string, unsigned int chars_size, unsigned int * cs);
	_Bool insert_words_at_these (string * _string, unsigned int * indices, string ** words);
	_Bool remove_word_at (string * _string, unsigned int begin_index, unsigned int end_index);
	_Bool n_append (string * _string, _Bool type, unsigned int amount_to_append, string * content);
	_Bool insert_at_these (string * _string, unsigned int size, unsigned int * indices, string cs);
	_Bool append_these (string * _string, unsigned int how_many, _Bool * type, string ** to_append);
	_Bool contains_the_following (string * _string, unsigned int number_of_things, string ** checkers);
	_Bool n_bind_at (string * _string, unsigned int index, unsigned int amount_to_bind, string * to_be_bind);
	_Bool remove_words_at_these (string * _string, unsigned int size, unsigned int * begin, unsigned int * end);
	_Bool n_compare_string (string * main, string secondary, unsigned int main_start_from, unsigned int secondary_start_from, unsigned int amount);

	//Function that return char value.
	char char_to_upper (char c);
	char char_to_lower (char c);
	char to_scar (string * _string);
	char get_type_code (string * type);
	char is_union_struct (string * _string);
	char char_at (string * _string, unsigned int index);

	//Function that return unsigned char value
	unsigned char to_ucar (string * _string);

	//Function that return string or string * value.
	string give_me_str_copy (string * _string);
	string single_char_split (string * _string, char spliter);
	string chars_at (string * _string, unsigned int * indices);
	string single_word_split (string * _string, string spliter);
	string substring (string * _string, unsigned int begin, unsigned int end);
	string * substrings (string * _string, unsigned int n_subs, unsigned int * begins, unsigned int * ends);

	//Function that return unsingend int or arrays of unsigned int.
	unsigned int str_len (string * _string);
	unsigned int to_uint (string * _string);
	unsigned int split_len (string * first);
	unsigned int n_alphas (string * _string);
	unsigned int n_Lcases (string * _string);
	unsigned int n_Ucases (string * _string);
	unsigned int n_spaces (string * _string);
	unsigned int n_Vspaces (string * _string);
	unsigned int n_Hspaces (string * _string);
	unsigned int n_numbers (string * _string);
	unsigned int n_specials (string * _string);
	unsigned int hash_string (string * _string);
	unsigned int n_white_spaces (string * _string);
	unsigned int index_of (string * _string, char c);
	unsigned int last_index_of (string * _string, char _char);

	//Functions of type unsigned int *.
	unsigned int * indices_of_alphas (string * _string);
	unsigned int * indices_of_Lcases (string * _string);
	unsigned int * indices_of_Ucases (string * _string);
	unsigned int * indices_of_spaces (string * _string);
	unsigned int * indices_of_numbers (string * _string);
	unsigned int * indices_of_Vspaces (string * _string);
	unsigned int * indices_of_Hspaces (string * _string);
	unsigned int * indices_of_specials (string * _string);
	unsigned int * indices_of (string * _string, char _char);
	unsigned int * index_of_word (string * _string, string word);
	unsigned int * indices_of_word (string * _string, string word);

	//Function that return unsigned int **
	unsigned int ** occurences (string * _string, string chars_words);
	unsigned int ** indices_of_n_strings (string * _string, string cs);
	unsigned int ** indices_of_words (string * _string, string words);

	//Functions of type unsigned int ***.
	unsigned int *** string_analysis (string * _string);

	//Functions that return int value.
	int to_sint (string * _string);

	//Functions that return unsigned short value.
	unsigned short to_ushi (string * _string);

	//Functions that return short value
	short to_sshi (string * _string);

	//Functions that return unsigned long long int.
	unsigned long long int to_ulli (string *_string);
	
	//Functions that return long long int.
	long long int to_slli (string * _string);

	//Functions that return double.
	double to_double (string * _string);

	//Functions that return basic union.
	BASIC_UNION * to_union (string * _string);

	//Functions that return basic struct.
	BASIC_STRUCT * to_struct (string * _string);

	
	//Functions the user does not need to use, but if he/ she understands, then it is okay.
	void generate_backup_split_size (string ** split_list, unsigned int size_of_split_list);
	_Bool allocate_union_struct_var (void ** union_struct, _Bool type, string * experssion);
	unsigned int hash_char (char c);
	unsigned int power_of_ten (unsigned int size);
	unsigned int ** sizes_for_union_struct_var_alloc (string * experssion);
#ifdef __cplusplus
}
#endif

#endif