#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "../header/string.h"
#include "../header/macro_errors.h"


//Functions that returns void *** value
/*
INPUT: A poitnter of string and char which will be used to split the string. The string must include data.
	The spliter my include [space, tab, (lower & upper) case characters, numbers and special character] that
	a char type can hold.

OUTPUT: Return null if the INPUT constraint is not met or some problem is met otherwise return void ***.
	to access the unsigned int in index 0. var1 = ((unsigned int *) return_value [0][0]), then NULL return_value [0][0] = NULL.
	to access the list of strings in index 1. var2 = ((string *) return_value [1]), the NULL return_val [1] = NULL.
	then free the return_value. free (return_value) and the null it return_value = NULL.

INFO: This function is used to split the string into set of strings if a certain character is met. Like function split in java
	Or string tokenizer in c.

Example:
	Input: _string is = "My Name Is Alex.", spliter = ' '.
	output:
		---------
		| 0 | 1 |
		--------- Where A is the address of either a list of strings or an address of unsigned integer.
		| 0 | A |
		--------
		| A |
		-----

		The visual of the return with data.

		_________
		| 0 | 1 |
		|___|___|________________
		| 0 | 0 | 1 | 2 | 3 | 4 |
		|___|___|___|___|___|___|
		| 4 | 4 | M | N | I | A |
		|___|\0 | y | a | s | l |
		    |___|\0 | m |\0 | e | 
		        |___| e |___| x |
		            |\0 |   |\0 |
		            |___|   |___|
*/
void *** str_char_split (string * _string, char spliter) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			if (is_char_white_space (spliter) || is_char_alpha (spliter) || is_char_number (spliter) || is_char_special (spliter)) {
				if (str_len (_string) > 0) {
					remove_repeated_char (_string, spliter);
					if (str_len (_string) > 0) {
						unsigned int * indices_of_spliter = indices_of (_string, spliter);

						if (indices_of_spliter != NULL) {
							if (indices_of_spliter [0] == 1 && indices_of_spliter [1] == 0) {
								//If the string is like that " A" and the spliter is space.
								free (indices_of_spliter);
								indices_of_spliter = NULL;

								//Allocate the list of strings that will be returned.
								string * split = NULL;
								do {split = (string *) malloc (sizeof (string) * 2);} while (split == NULL);
								do {split [0] = (string) malloc (sizeof (char) * 2);} while (split [0] == NULL);

								//The equivalent of generate_backup_split_size
								split [0][0] = 1;
								split [0][1] = '\0';
								split [1] = give_me_str_copy (_string);

								//Remove the first space in the string.
								remove_at (_string, (unsigned int) 0);

								//Allocate the size of list of strings.
								unsigned int * split_size = NULL;
								do {split_size = (unsigned int *) malloc (sizeof (unsigned int));} while (split_size == NULL);
								(*split_size) = 1;

								//Allocate the return value.
								void *** return_value = NULL;
								do {return_value = (void ***) malloc (sizeof (void **) * 2);} while (return_value == NULL);
								do {return_value [0] = (void **) malloc (sizeof (void *));} while (return_value [0] == NULL);
								return_value [0][0] = (void *) split_size;
								return_value [1] = (void **) split;

								return return_value;
							} else {
								if (indices_of_spliter [indices_of_spliter [0]] == (str_len (_string) - 1)) //Check if the string end with spliter character. If it dose reduce the size by 1.
									--indices_of_spliter [0];

								//Allocate the split_size.
								unsigned int * split_size = NULL;
								do {split_size = (unsigned int *) malloc (sizeof (unsigned int));} while (split_size == NULL);
								(*split_size) = (indices_of_spliter [0] + 1);

								//Allocate the list of strings.
								string * split = NULL;
								do {split = (string *) malloc (sizeof (string) * ((*split_size) + 1));} while (split == NULL);

								//But a backup size if the user accidentally freed the unsigned int address which holds the size of the split.
								generate_backup_split_size (&split, (*split_size));

								//My Name Is-> 1 2. 
								for (unsigned int counter = 0; counter < (*split_size); counter++) {
									if (counter == 0) //The first iteration of the loop
										split [(1 + counter)] = substring (_string, 0, (indices_of_spliter [(counter + 1)] - 1));
									
									else {
										if ((counter + 1) == (*split_size)) //The last iteration of the loop.
											split [(1 + counter)] = substring (_string, (indices_of_spliter [counter] + 1), (str_len (_string) - 1));
									
										else
											split [(1 + counter)] = substring (_string, (indices_of_spliter [counter] + 1), (indices_of_spliter [(1 + counter)] - 1));
									}
								}

								//Free indices_of_spliter
								free (indices_of_spliter);
								indices_of_spliter = NULL;

								//Allocate the return_value.
								void *** return_value = NULL;
								do {return_value = (void ***) malloc (sizeof (void **) * 2);} while (return_value == NULL);
								do {return_value [0] = (void **) malloc (sizeof (void *));} while (return_value [0] == NULL);
								return_value [0][0] = (void *) split_size;
								return_value [1] = (void **) split;

								return return_value;
							}
						} else {
							free (indices_of_spliter);
							indices_of_spliter = NULL;
							return NULL;
						}
					} else
						return NULL;
				} else
					return NULL;
			} else
				return NULL;
		} else
			return NULL;
	} else
		return NULL;
}


/*
INPUT: A pointer to a string which hold the main string and a string which hold the spliter Must be at least two characters.
OUTPUT: Return Like the above function, Otherwise Null.
INFO: This function is used to split the string into set of string depends on a word, which must be at least two characters long.
*/
void *** str_word_split (string * _string, string spliter) {
	if (_string != NULL && spliter != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size > 0 && str_len (&spliter) >= 2 && str_len (&spliter) < _string_size) {
				remove_repeated_word (_string, spliter);

				_string_size = str_len (_string);
				if (_string_size > 0) {
				
					unsigned int * indices = indices_of_word (_string, spliter);
						
					if (indices != NULL) {
						if (indices [0] == 2 && indices [1] == 0) {
							_string_size = indices [2];
							free (indices);
							indices = NULL;

							unsigned int * split_size = NULL;
							do {split_size = (unsigned int *) malloc (sizeof (unsigned int));} while (split_size == NULL);
							(*split_size) = 1;

							string * split = NULL;
							do {split = (string *) malloc (sizeof (string) * 2);} while (split == NULL);
							do {split [0] = (string) malloc (sizeof (char) * 2);} while (split [0] == NULL);

							split [0][0] = 1;
							split [0][1] = '\0';

							split [1] = substring (_string, (_string_size + 1), (str_len (_string) - 1));

							void *** content = NULL;
							do {content = (void ***) malloc (sizeof (void **) * 2);} while (content == NULL);
							do {content [0] = (void **) malloc (sizeof (void *));} while (content [0] == NULL);

							content [0][0] = (void *) split_size;
							content [1] = (void **) split;

							return content;
						} else {
							if (indices [indices [0]] == (_string_size - 1))
								indices [0] -= 2;
							
							unsigned int * split_size = NULL;
							do {split_size = (unsigned int *) malloc (sizeof (unsigned int));} while (split_size == NULL);
							(*split_size) = (indices [0] / 2) + 1;
							
							string * split = NULL;
							do {split = (string *) malloc (sizeof (string) * ((*split_size) + 1));} while (split == NULL);

							generate_backup_split_size (&split, (*split_size));

							_string_size = 0;

							//v  w  x  y  z.
							//[6-1-2-4-5-7-8-10-11]
							unsigned int inc = 2;
							for (; _string_size < (*split_size); ++_string_size) {
								if (_string_size == 0) {
									split [(_string_size + 1)] = substring (_string, 0, (indices [1] - 1));
								} else {
									if ((_string_size + 1) == (*split_size)) {
										split [(_string_size + 1)] = substring (_string, (indices [indices [0]] + 1), (str_len (_string) - 1));
									} else {
										split [(_string_size + 1)] = substring (_string, (indices [inc] + 1), (indices [(inc + 1)] - 1));
										inc += 2;
									}
								}
							}

							void *** content = NULL;
							do {content = (void ***) malloc (sizeof (void **) * 2);} while (content == NULL);
							do {content [0] = (void **) malloc (sizeof (void *));} while (content [0] == NULL);

							content [0][0] = (void *) split_size;
							content [1] = (void **) split;

							free (indices);
							indices = NULL;

							return content;
						}
					} else
						return NULL;
				} else
					return NULL;
			} else
				return NULL;
		} else
			return NULL;
	} else
		return NULL;
}


//Functions that return void ** type
/*
INPUT: A pointer to a string which hold the main string.
OUTPUT: Return a pointer to a void *, otherwise return NULL. The return will consisit of two indices the first index convert it to type char.
	if its value is 2, then convert second index to struct, otherwise convert the second index to union.

INFO: This function is used to convert the string into a basic union/ struct.
	Example:
		union: sshi 2^2*[5-6], dble 0, strn 2^1*2


The avaialbe types in a basic union/ struct are: sshi - ushi - sint - uint - slli - ulli - dble - strn.
NOTE: Do not repeat a type as it will be ignored as it is already initialized. So for each type decide what you want.
	
	Also remember the variables name is the same as the type you have written in the string in order to access them in the union/ struct, except of the following:
		uint = _uint
*/
void ** to_union_struct (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size != 0) {
				char * _union_struct = NULL;
				do {_union_struct = (char *) malloc (sizeof (char));} while (_union_struct == NULL);
				(*_union_struct) = is_union_struct (_string);

				if ((*_union_struct) != 0) {
					BASIC_UNION * _union = NULL;
					BASIC_STRUCT * _struct = NULL;

					if ((*_union_struct) == 1) {
						do {_union = (BASIC_UNION *) malloc (sizeof (BASIC_UNION));} while (_union == NULL);
						_union -> sint = NULL;
						_union -> strn = NULL;
						_union -> sshi = NULL;
						_union -> ushi = NULL;
						_union -> slli = NULL;
						_union -> ulli = NULL;
						_union -> dble = NULL;
						_union -> _uint = NULL;
					} else {
						do {_struct = (BASIC_STRUCT *) malloc (sizeof (BASIC_STRUCT));} while (_struct == NULL);
						_struct -> sshi = NULL;
						_struct -> ushi = NULL;
						_struct -> slli = NULL;
						_struct -> ulli = NULL;
						_struct -> dble = NULL;
						_struct -> strn = NULL;
						_struct -> sint = NULL;
						_struct -> _uint = NULL;
					}
					
					void *** content = str_char_split (_string, ':');

					if (content != NULL) {
						string * split_content = ((string *) content [1]);
						unsigned int * split_size = ((unsigned int *) content [0][0]);

						content [1] = NULL;
						content [0][0] = NULL;
						free (content);
						content = NULL;

						free (split_content [0]);
						split_content [0] = NULL;
						free (split_content [1]);
						split_content [1] = NULL;

						remove_at (&split_content [2], (str_len (&split_content [2]) - 1));

						content = str_char_split (&split_content [2], ',');

						if (content != NULL) {
							free (split_content [2]);
							split_content [2] = NULL;
							free (split_content);
							split_content = NULL;
							free (split_size);
							split_size = NULL;

							split_content = ((string *) content [1]);
							split_size = ((unsigned int *) content [0][0]);

							_string_size = 1;
							_Bool all_correct = 1;
							for (; _string_size <= (*split_size); _string_size++) {
								if ((*_union_struct) == 1) {
									if (!allocate_union_struct_var ((void **) &_union, 0, &split_content [_string_size])) {
										all_correct = 0;
										break;
									}
								} else {
									if (!allocate_union_struct_var ((void **) &_struct, 1, &split_content [_string_size])) {
										all_correct = 0;
										break;
									}
								}
							}

							_string_size = 0;
							for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
							free (split_content);
							split_content = NULL;
							free (split_size);
							split_size = NULL;


							if (all_correct) {
								void ** _return = NULL;
								do {_return = (void **) malloc (sizeof (void *) * 2);} while (_return == NULL);
								_return [0] = ((void *) _union_struct);

								if ((*_union_struct) == 1)
									_return [1] = ((void *) _union);
								else
									_return [1] = ((void *) _struct);

								return _return;
							} else
								return NULL;
						} else
							return NULL;
					} else
						return NULL;
				} else
					return NULL;
			} else
				return NULL;
		} else
			return NULL;
	} else
		return NULL;
}

//Easy Functions.
/*
INPUT: A character.
OUTPUT: 1 if the character is horizintal tab otherwise return 0.
INFO: This function is used to check if the character is horizintal tab.
*/
_Bool is_Htab (char c) {
	if (c == 9)
		return 1;
	else
		return 0;
}


/*
INPUT: A character.
OUTPUT: 1 if the character is vertical tab otherwise return 0.
INFO: This function is used to check if the character is vertical tab.
*/
_Bool is_Vtab (char c) {
	if (c == 11)
		return 1;
	else
		return 0;
}


/*
INPUT: A character.
OUTPUT: 1 if the character is space otherwise return 0.
INFO: This function is used to check if the character is space.
*/
_Bool is_space (char c) {
	if (c == 32)
		return 1;
	else
		return 0;
}


/*
INPUT: A character.
OUTPUT: 1 if the character is Lower or Upper case Character otherwise return 0.
INFO: This function is used to check if the character is Lower or Upper case Character.
*/
_Bool is_char_alpha (char c) {
	if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
		return 1;
	else
		return 0;
}


/*
INPUT: A character.
OUTPUT: 1 if the character is Lower Upper case Character otherwise return 0.
INFO: This function is used to check if the character is Lower Upper case Character.
*/
_Bool is_char_lower (char c) {
	if (is_char_alpha (c) && (c >= 97 && c <= 122))
		return 1;
	else
		return 0;
}


/*
INPUT: A character.
OUTPUT: 1 if the character is Upper case Character otherwise return 0.
INFO: This function is used to check if the character is Upper case Character.
*/
_Bool is_char_upper (char c) {
	if (is_char_alpha (c) && (c >= 65 && c <= 90))
		return 1;
	else
		return 0;
}


/*
INPUT: A character.
OUTPUT: 1 if the character is number otherwise return 0.
INFO: This function is used to check if the character is number.
*/
_Bool is_char_number (char c) {
	if (c >= 48 && c <= 57)
		return 1;
	else
		return 0;
}


/*
INPUT: A character.
OUTPUT: 1 if the character is special character otherwise return 0.
INFO: This function is used to check if the character is special character.
*/
_Bool is_char_special (char c) {
	if ((c >= 33 && c <= 47) || (c >= 58 && c <= 64) || (c >= 91 && c <= 96) || (c >= 123 && c <= 126))
		return 1;
	else
		return 0;
}


/*
INPUT: A character.
OUTPUT: Return 1 if the character can be either a number or alphabitcal number, otherwise return 0.
INFO: This function is used to check wehter the character is number or alpha.
*/
_Bool is_char_alpnum (char c) {
	if (is_char_alpha (c) || is_char_number (c))
		return 1;
	else
		return 0;
}


/*
INPUT: A character.
OUTPUT: 1 if the character is white space otherwise return 0.
INFO: This function is used to check if the character is whitespace.
*/
_Bool is_char_white_space (char c) {
	if (c == 9 || c == 11 || c == 32)
		return 1;
	else
		return 0;
}


/*
INPUT: A pointer of string. Must contain data.
OUTPUT: Return 1 if at least on character titled. Otherwise return 0.
INFO: This function is used to turn the first character into uppercase character.
*/
_Bool title (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			//Check if the size of the string is greater than Zero. Which means that the string not empty.
			if (_string_size > 0) {
				_Bool change = 0;
				_Bool at_least_one = 0;
				for (unsigned int counter = 0; counter < _string_size; ++counter) {
					if (counter == 0 && is_char_lower ((*_string) [counter])) {
						(*_string) [0] = ((*_string) [0] - 32);
						continue;
					}

					if (is_char_white_space ((*_string) [counter])) {
						change = 1;
						continue;
					}

					if (change) {
						change = 0;
						if (is_char_alpha ((*_string) [counter]) && is_char_lower ((*_string) [counter])) {
							at_least_one = 1;
							(*_string) [counter] = ((*_string) [counter] - 32);
						}
					}
				}

				return at_least_one;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to a string which hold the main string.
OUTPUT: Return 1 if the string can represent a variable, otherwise return 0.
INFO: This function is used to check if the string can be represent a variable.
*/
_Bool is_var (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size > 0) {
				_Bool var = 1;

				for (unsigned int counter = 0; counter < _string_size; ++counter) {
					if (counter == 0) {
						if (!is_char_alpha ((*_string) [counter]) && (*_string) [counter] != '_') {
							var = 0;
							break;
						}
					} else if (!is_char_number ((*_string) [counter]) && !(is_char_special ((*_string) [counter]) && (*_string) [counter] == '_') && !is_char_alpha ((*_string) [counter])) {
						var = 0;
						break;
					}
				}

				return var;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to a string which hold the main string.
OUTPUT: Return 1 if the string can be converted to signed char, otherwise return 0.
INFO: This function is used to check if the string can be converted to unsigned char. The maximum number of digits of signed char is 3.
	The maximum value if positive: 127.
	The maximum value of negative: -128.
*/
_Bool is_scar (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size > 0 && _string_size <= 4) {
				_Bool is_signed = 0;
				unsigned char counter = 0;

				if ((*_string) [0] == '-') {
					is_signed = 1;
					counter = 1;
				}

				_Bool is_number = 0;
				unsigned char total = 0;
				counter++;

				for (; counter < _string_size; ++counter) {
					if (is_char_number ((*_string) [counter])) {
						total += 9;
						is_number = 1;
					} else {
						is_number = 0;
						break;
					}
				}

				if (counter == 1) {
					if (is_char_number ((*_string) [0]))
						return 1;
					else
						return 0;
				} else if (_string_size == 2 && counter == 2 && is_signed) {
					if (is_char_number ((*_string) [1]))
						return 1;
					else
						return 0;
				} else if (is_number) {
					if (is_signed) {
						if (is_char_number ((*_string) [1])) {
							total += ((*_string) [1] - 48);
							
							if (is_number && total <= MAXD__SCAR) {
								if (_string_size == 4) {
									if ((*_string) [2] <= 50 && (*_string) [3] <= 56)
										return	 1;
									else
										return 0;
								} else
									return 1;
							} else
								return 0;
						} else
							return 0;
					} else {
						if (is_char_number ((*_string) [0])) {
							total += ((*_string) [0] - 48);
							if (is_number && total <= MAXD__SCAR) {
								if (_string_size == 3) {
									if ((*_string) [1] <= 50 && (*_string) [2] <= 55)
										return 1;
									else
										return 0;
								} else
									return 1;
							} else
								return 0;
						} else
							return 0;
					}
				} else
					return 0;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A ppinter to a string which hold the main string.
OUTPUT: Return 1 if the string can be converted to unsigned char, otherwise return 0.
INFO: This function is used to check if the string can be converted to unsigned char. The maximum number of digits is 3.
	The maximum value is: 255.
*/
_Bool is_ucar (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size != 0 && _string_size <= 3) {
				_Bool is_number = 0;
				unsigned char total = 0;
				unsigned char counter = 1;

				for (; counter < _string_size; ++counter) {
					if (is_char_number ((*_string) [counter])) {
						is_number = 1;
						total += 9;
					} else {
						is_number = 0;
						break;
					}
				}

				if (counter == 1) {
					if (is_char_number ((*_string) [0])) {
						return 1;
					} else
						return 0;
				} else if (is_number) {
					if (is_char_number ((*_string) [0])) {
						is_number = 1;
						total += ((*_string) [0] - 48);
					} else
						is_number = 0;

					if (is_number && total <= MAXD__UCAR) {
						if (_string_size == 3) {
							if ((*_string) [0] == 50) {
								if ((*_string) [1] <= 53 && (*_string) [2] <= 53)
									return 1;
								else
									return 0;
							} else
								return 1;
						} else
							return 1;
					} else
						return 0;
				} else
					return 0;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to string which hold the main string.
OUTPUT: Return 1 if the string cn be converted to signed integer, otherwise return 0.
INFO: This function is used to check if the string can be converted into signed int.
	The maximum number of digits is 10
	The maximum poistive value is: 2147483647
	The maximum negative value is: -2147483648
	
*/
_Bool is_sint (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size != 0 && _string_size <= 11) {
				_Bool is_signed = 0;
				unsigned char counter = 0;

				if ((*_string) [0] == '-') {
					is_signed = 1;
					counter = 1;
				}

				_Bool is_number = 0;
				unsigned char total = 0;
				++counter;

				for (; counter < _string_size; ++counter) {
					if (is_char_number ((*_string) [counter])) {
						is_number = 1;
						total += 9;
					} else {
						is_number = 0;
						break;
					}
				}

				if (counter == 1) {
					if (is_char_number ((*_string) [0]))
						return 1;
					else
						return 0;
				} else if (_string_size == 2 && counter == 2 && is_signed) {
					if (is_char_number ((*_string) [1]))
						return 1;
					else
						return 0;
				} else if (is_number) {
					if (is_signed) {
						if (is_char_number ((*_string) [1])) {
							is_number = 1;
							total += ((*_string) [1] - 48);
						} else
							is_number = 0;

						if (is_number && total <= MAXD__SINT) {
							if (_string_size == 11) {
								if ((*_string) [1] < 50)
									return 1;
								else {
									if ((*_string) [2] <= 49 && (*_string) [3] <= 52 && (*_string) [4] <= 55 && (*_string) [5] <= 52 && (*_string) [6] <= 56 && (*_string) [7] <= 51 && (*_string) [8] <= 54 && (*_string) [9] <= 52 && (*_string) [10] <= 56)
										return 1;
									else
										return 0;
								}
							} else
								return 1;
						} else
							return 0;
					} else {
						if (is_char_number ((*_string) [0])) {
							is_number = 1;
							total += ((*_string) [0] - 48);
						} else
							is_number = 0;

						if (is_number && total <= MAXD__SINT) {
							if (_string_size == 10) {
								if ((*_string) [0] < 50)
									return 1;
								else {
									if ((*_string) [1] <= 49 && (*_string) [2] <= 52 && (*_string) [3] <= 55 && (*_string) [4] <= 52 && (*_string) [5] <= 56 && (*_string) [6] <= 51 && (*_string) [7] <= 54 && (*_string) [8] <= 52 && (*_string) [9] <= 55)
										return 1;
									else
										return 0;
								}
							} else
								return 1;
						} else
							return 0;
					}
				} else
					return 0;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to a string which hold the main string.
OUTPUT: Return 1 if the number can be converted to unsigned int, otherwise return 0.
INFO: This function is used to check if the number can be converted to unsigned integer.
	The maximum number of digits is 10
	The maximum value is: 4294967295
	
*/
_Bool is_uint (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size > 0 && _string_size <= 10) {
				_Bool is_number = 0;
				unsigned char total = 0;
				unsigned char counter = 1;
				
				for (; counter < _string_size; ++counter) {
					if (is_char_number ((*_string) [counter])) {
						total += 9;
						is_number = 1;
					} else {
						is_number = 0;
						break;
					}
				}

				if (counter == 1) {
					if (is_char_number ((*_string) [0]))
						return 1;
					else
						return 0;
				} else if (is_number) {
					if (is_char_number ((*_string) [0])) {
						is_number = 1;
						total += ((*_string) [0] - 48);
					} else
						is_number = 0;
					
					if (is_number && total <= MAXD__UINT) {
						if (_string_size == 10) {
							if ((*_string) [0] < 52)
								return 1;
							else {
								if ((*_string) [1] <= 50 && (*_string) [2] <= 57 && (*_string) [3] <= 52 && (*_string) [4] <= 57 && (*_string) [5] <= 54 && (*_string) [6] <= 55 && (*_string) [7] <= 50 && (*_string) [8] <= 57 && (*_string) [9] <= 53)
									return 1;
								else
									return 0;
							}
						} else
							return 1;
					} else
						return 0;
				} else
					return 0;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to a string which hold the main string.
OUTPUT: Return 1 if the string is signed is short int, otherwise return 0.
INFO: This functinon is used to check if the string can be converted to signed short int.
	The maximum number of digits is 5
	The maximum positive value is: 32767
	The maximum negative value is: -32768
*/
_Bool is_sshi (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size != 0 && _string_size <= 6) {
				_Bool is_signed = 0;
				unsigned char counter = 0;

				if ((*_string) [0] == '-') {
					is_signed = 1;
					++counter;
				}

				_Bool is_number = 0;
				unsigned char total = 0;
				++counter;

				for (; counter < _string_size; ++counter) {
					if (is_char_number ((*_string) [counter])) {
						is_number = 1;
						total += 9;
					} else {
						is_number = 0;
						break;
					}
				}

				if (counter == 1) {
					if (is_char_number ((*_string) [0]))
						return 1;
					else
						return 0;
				} else if (_string_size == 2 && counter == 2 && is_signed) {
					if (is_char_number ((*_string) [1]))
						return 1;
					else
						return 0;
				} else if (is_number) {
					if (is_signed) {
						if (is_char_number ((*_string) [1])) {
							is_number = 1;
							total += ((*_string) [1] - 48);
						} else
							is_number = 0;

						if (is_number && total <= MAXD__SSHI) {
							if (_string_size == 6) {
								if ((*_string) [1] < 51)
									return 1;
								else {
									if ((*_string) [2] <= 50 && (*_string) [3] <= 55 && (*_string) [4] <= 54 && (*_string) [5] <= 56)
										return 1;
									else
										return 0;
								}
							} else
								return 1;
						} else
							return 0;
					} else {
						if (is_char_number ((*_string) [0])) {
							is_number = 1;
							total += ((*_string) [0] - 48);
						} else
							is_number = 0;

						if (is_number && total <= MAXD__SSHI) {
							//32767
							if (_string_size == 5) {
								if ((*_string) [0] < 51)
									return 1;
								else {
									if ((*_string) [1] <= 50 && (*_string) [2] <= 55 && (*_string) [3] <= 54 && (*_string) [4] <= 55)
										return 1;
									else
										return 0;
								}
							} else
								return 1;
						} else
							return 0;
					}
				} else
					return 0;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to string which hold the main string.
OUTPUT: Return 1 if the string can be coverted to a usigned integer, otherwise return 0.
INFO: This function is used to check if the string can be converted into unsigned short integer.
	The maximum number of digits is 5
	The maximum value is: 65535
*/
_Bool is_ushi (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size > 0 && _string_size <= 5) {
				_Bool is_number = 0;
				unsigned char total = 0;
				unsigned char counter = 1;

				for (; counter < _string_size; ++counter) {
					if (is_char_number ((*_string) [counter])) {
						total += 9;
						is_number = 1;
					} else {
						is_number = 0;
						break;
					}
				}
				
				if (counter == 1) {
					if (is_char_number ((*_string) [0]))
						return 1;
					else
						return 0;
				} else if (is_number) {
					if (is_char_number ((*_string) [0])) {
						is_number = 1;
						total += ((*_string) [0] - 48);
					} else
						is_number = 0;

					if (is_number && total <= MAXD__USHI) {
						if (_string_size == 5) {
							if ((*_string) [0] < 54)
								return 1;
							else {
								if ((*_string) [1] <= 53 && (*_string) [2] <= 53 && (*_string) [3] <= 51 && (*_string) [4] <= 53)
									return 1;
								else
									return 0;
							}
						} else
							return 1;
					} else
						return 0;
				} else
					return 0;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to string which hold the main string.
OUTPUT: Return 1 if the string can be converted into signed long long int, otherwise return 0.
INFO: This function is used to check if the string can be converted int signed long long int.
	The maximum number of digits is 19
	The maximum positive value is: 9223372036854775807
	The maximum negative value is: -9223372036854775808
*/
_Bool is_slli (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size != 0 && _string_size <= 20) {
				_Bool is_signed = 0;
				unsigned char counter = 0;

				if ((*_string) [0] == '-') {
					is_signed = 1;
					counter = 1;
				}

				_Bool is_number = 0;
				unsigned char total = 0;
				++counter;

				for (; counter < _string_size; ++counter) {
					if (is_char_number ((*_string) [counter])) {
						is_number = 1;
						total += 9;
					} else {
						is_number = 0;
						break;
					}
				}

				if (counter == 1) {
					if (is_char_number ((*_string) [0]))
						return 1;
					else
						return 0;
				} else if (counter == 2 && is_signed && _string_size == 2) {
					if (is_char_number ((*_string) [1]))
						return 1;
					else
						return 0;
				} else if (is_number) {
					if (is_signed) {
						if (is_char_number ((*_string) [1])) {
							is_number = 1;
							total += ((*_string) [1] - 48);
						} else
							is_number = 0;

						if (is_number && total <= MAXD__SLLI) {
							if (_string_size == 20) {
								if ((*_string) [1] < 57)
									return 1;
								else {
									is_number = 0;
									for (counter = 2; counter < 20; ++counter) {
										switch (counter) {
											case 2: if ((*_string) [2] <= 50) is_number = 1; break;
											case 3: if ((*_string) [3] <= 50) is_number = 1; else is_number = 0; break;
											case 4: if ((*_string) [4] <= 51) is_number = 1; else is_number = 0; break;
											case 5: if ((*_string) [5] <= 51) is_number = 1; else is_number = 0; break;
											case 6: if ((*_string) [6] <= 55) is_number = 1; else is_number = 0; break;
											case 7: if ((*_string) [7] <= 50) is_number = 1; else is_number = 0; break;
											case 8: if ((*_string) [8] == 48) is_number = 1; else is_number = 0; break;
											case 9: if ((*_string) [9] <= 51) is_number = 1; else is_number = 0; break;
											case 10: if ((*_string) [10] <= 54) is_number = 1; else is_number = 0; break;
											case 11: if ((*_string) [11] <= 56) is_number = 1; else is_number = 0; break;
											case 12: if ((*_string) [12] <= 53) is_number = 1; else is_number = 0; break;
											case 13: if ((*_string) [13] <= 52) is_number = 1; else is_number = 0; break;
											case 14: if ((*_string) [14] <= 55) is_number = 1; else is_number = 0; break;
											case 15: if ((*_string) [15] <= 55) is_number = 1; else is_number = 0; break;
											case 16: if ((*_string) [16] <= 53) is_number = 1; else is_number = 0; break;
											case 17: if ((*_string) [17] <= 56) is_number = 1; else is_number = 0; break;
											case 18: if ((*_string) [18] == 48) is_number = 1; else is_number = 0; break;
											case 19: if ((*_string) [19] <= 56) is_number = 1; else is_number = 0; break;
											default: is_number = 0; break;
										}

										if (!is_number)
											break;
									}

									if (is_number)
										return 1;
									else
										return 0;
								}
							} else
								return 1;
						} else
							return 0;
					} else {
						if (is_char_number ((*_string) [0])) {
							is_number = 1;
							total += ((*_string) [0] - 48);
						} else
							is_number = 0;

						if (is_number && total <= MAXD__SLLI) {
							if (_string_size == 19) {
								if ((*_string) [0] < 57)
									return 1;
								else {
									is_number = 0;
									for (counter = 1; counter < 19; ++counter) {
										switch (counter) {
											case 1: if ((*_string) [1] <= 50) is_number = 1; break;
											case 2: if ((*_string) [2] <= 50) is_number = 1; else is_number = 0; break;
											case 3: if ((*_string) [3] <= 51) is_number = 1; else is_number = 0; break;
											case 4: if ((*_string) [4] <= 51) is_number = 1; else is_number = 0; break;
											case 5: if ((*_string) [5] <= 55) is_number = 1; else is_number = 0; break;
											case 6: if ((*_string) [6] <= 50) is_number = 1; else is_number = 0; break;
											case 7: if ((*_string) [7] == 48) is_number = 1; else is_number = 0; break;
											case 8: if ((*_string) [8] <= 51) is_number = 1; else is_number = 0; break;
											case 9: if ((*_string) [9] <= 54) is_number = 1; else is_number = 0; break;
											case 10: if ((*_string) [10] <= 56) is_number = 1; else is_number = 0; break;
											case 11: if ((*_string) [11] <= 53) is_number = 1; else is_number = 0; break;
											case 12: if ((*_string) [12] <= 52) is_number = 1; else is_number = 0; break;
											case 13: if ((*_string) [13] <= 55) is_number = 1; else is_number = 0; break;
											case 14: if ((*_string) [14] <= 55) is_number = 1; else is_number = 0; break;
											case 15: if ((*_string) [15] <= 53) is_number = 1; else is_number = 0; break;
											case 16: if ((*_string) [16] <= 56) is_number = 1; else is_number = 0; break;
											case 17: if ((*_string) [17] == 48) is_number = 1; else is_number = 0; break;
											case 18: if ((*_string) [18] <= 55) is_number = 1; else is_number = 0; break;
											default: is_number = 0; break;
										}

										if (!is_number)
											break;
									}

									if (is_number)
										return 1;
									else
										return 0;
								}
							} else
								return 1;
						} else
							return 0;
					}
				} else
					return 0;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to string which hold the main string.
OUTPUT: Return 1 if the string can be converted to unsigned long long int, otherwise return 0.
INFO: This function is used to check is the string can be converted to unsigned long long int.
	The maximum number of digits is 20
	The maximum value is: 18446744073709551615
*/
_Bool is_ulli (string * _string) {
	if (_string != NULL) {
			if ((*_string) != NULL) {
					unsigned int _string_size = str_len (_string);

					if (_string_size > 0 && _string_size <= 20) {
						_Bool is_number = 0;
						unsigned char total = 0;
						unsigned char counter = 1;

						for (; counter < _string_size; ++counter) {
							if (is_char_number ((*_string) [counter])) {
								is_number = 1;
								total += 9;
							} else {
								is_number = 0;
								break;
							}
						}

						if (counter == 1) {
							if (is_char_number ((*_string) [0]))
								return 1;
							else
								return 0;
						} else if (is_number) {
							if (is_char_number ((*_string) [0])) {
								is_number = 1;
								total += ((*_string) [0] - 48);
							} else
								is_number = 0;
							
							if (is_number && total <= MAXD__ULLI) {
								if (_string_size == 20) {
									
									is_number = 0;
									for (counter = 1; counter < 20; ++counter) {
										switch (counter) {
											case 1: if ((*_string) [1] <= 56) is_number = 1; break;
											case 2: if ((*_string) [2] <= 52) is_number = 1; else is_number = 0; break;
											case 3: if ((*_string) [3] <= 52) is_number = 1; else is_number = 0; break;
											case 4: if ((*_string) [4] <= 54) is_number = 1; else is_number = 0; break;
											case 5: if ((*_string) [5] <= 55) is_number = 1; else is_number = 0; break;
											case 6: if ((*_string) [6] <= 52) is_number = 1; else is_number = 0; break;
											case 7: if ((*_string) [7] <= 52) is_number = 1; else is_number = 0; break;
											case 8: if ((*_string) [8] == 48) is_number = 1; else is_number = 0; break;
											case 9: if ((*_string) [9] <= 55) is_number = 1; else is_number = 0; break;
											case 10: if ((*_string) [10] <= 51) is_number = 1; else is_number = 0; break;
											case 11: if ((*_string) [11] <= 55) is_number = 1; else is_number = 0; break;
											case 12: if ((*_string) [12] == 48) is_number = 1; else is_number = 0; break;
											case 13: if ((*_string) [13] <= 57) is_number = 1; else is_number = 0; break;
											case 14: if ((*_string) [14] <= 53) is_number = 1; else is_number = 0; break;
											case 15: if ((*_string) [15] <= 53) is_number = 1; else is_number = 0; break;
											case 16: if ((*_string) [16] <= 49) is_number = 1; else is_number = 0; break;
											case 17: if ((*_string) [17] <= 54) is_number = 1; else is_number = 0; break;
											case 18: if ((*_string) [18] <= 49) is_number = 1; else is_number = 0; break;
											case 19: if ((*_string) [19] <= 53) is_number = 1; else is_number = 0; break;
											default: is_number = 0; break;
										}

										if (!is_number)
											break;
									}

									if (is_number)
										return 1;
									else
										return 0;
								} else
									return 1;
							} else
								return 0;
						} else
							return 0;
					} else
						return 0;
			} else
				return 0;
	} else
			return 0;
}


/*
INPUT: A pointer to string which hold the main string.
OUTPUT: Return 1 if the string can represent a class definition, otherwise return 0.
INFO: This function is used to check if the string represent a definition of a class.
	AccessModifier class ClassName [Extend/Implement] [SuperClass/Interface] {

	Mut be written in this way exactly. See other function to know how to everything is connected.
*/
_Bool is_class (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size != 0) {
				void *** content = str_char_split (_string, ' ');

				if (content != NULL) {
					unsigned int * split_size = ((unsigned int *) content [0][0]);
					string * split_content = ((string *) content [1]);

					content [0][0] = NULL;
					content [1] = NULL;
					free (content);
					content = NULL;

					if ((*split_size) == 4 || (*split_size) == 6) {
						if (is_modifier (&split_content [1])) {
							if (compare_string (&split_content [2], "class")) {
								if (is_var (&split_content [3])) {
									if ((*split_size) == 4) {
										if (str_len (&split_content [4]) == 1 && split_content [4][0] == '{') {
											_string_size = 0;
											for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
											free (split_content);
											split_content = NULL;
											free (split_size);
											split_size = NULL;

											return 1;
										} else {
											_string_size = 0;
											for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
											free (split_content);
											split_content = NULL;
											free (split_size);
											split_size = NULL;

											return 0;
										}
									} else {
										if (compare_string (&split_content [4], "Extend") || compare_string (&split_content [4], "Implement")) {
											if (is_var (&split_content [5])) {
												if (str_len (&split_content [6]) == 1 && split_content [6][0] == '{') {
													_string_size = 0;
													for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
													free (split_content);
													split_content = NULL;
													free (split_size);
													split_size = NULL;

													return 1;
												} else {
													_string_size = 0;
													for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
													free (split_content);
													split_content = NULL;
													free (split_size);
													split_size = NULL;

													return 0;
												}
											} else {
												_string_size = 0;
												for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
												free (split_content);
												split_content = NULL;
												free (split_size);
												split_size = NULL;

												return 0;
											}
										} else {
											_string_size = 0;
											for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
											free (split_content);
											split_content = NULL;
											free (split_size);
											split_size = NULL;

											return 0;
										}
									}
								} else {
									_string_size = 0;
									for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
									free (split_content);
									split_content = NULL;
									free (split_size);
									split_size = NULL;

									return 0;
								}
							} else {
								_string_size = 0;
								for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
								free (split_content);
								split_content = NULL;
								free (split_size);
								split_size = NULL;

								return 0;
							}
						} else {
							_string_size = 0;
							for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
							free (split_content);
							split_content = NULL;
							free (split_size);
							split_size = NULL;

							return 0;
						}
					} else {
						_string_size = 0;
						for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
						free (split_content);
						split_content = NULL;
						free (split_size);
						split_size = NULL;

						return 0;
					}
				} else
					return 0;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to string which hold the main string.
OUTPUT: Return 1 if the string is a datatype, otherwise return 0.
INFO: This function is used to check if the string can represent a type.
		The input to choose the type be like:
				bt
				bst
				int
				void
				sshi
				ushi
				sint
				uint
				strn
				dble
				slli
				ulli
				scar
				ucar
				char
				list
				bool
				tree
				hash
				_Bool
				stack
				queue
				union
				llist
				float
				short
				double
				lstack
				lqueue
				fllist
				string
				struct
				dllist
				cllist
				fcllist
				dcllist
				short int
				long long
				unsigned int
				unsigned char
				long long int
				unsigned short int
				unsigned long long int
				signed char
				signed int
				signed short
				signed short int
				signed long long
				signed long long int
*/
_Bool is_type (string * datatype) {
	if (datatype != NULL) {
		if ((*datatype) != NULL) {
			unsigned int _string_size = str_len (datatype);

			if (_string_size > 0) {
				_Bool it_is = 0;
				for (unsigned char counter = 0; counter < 49; ++counter) {
					switch (counter) {
						case 0: if (compare_string (datatype, "bt")) it_is = 1; break;
						case 1: if (compare_string (datatype, "bst")) it_is = 1; break;
						case 2: if (compare_string (datatype, "int")) it_is = 1; break;
						case 3: if (compare_string (datatype, "void")) it_is = 1; break;
						case 4: if (compare_string (datatype, "sshi")) it_is = 1; break;
						case 5: if (compare_string (datatype, "ushi")) it_is = 1; break;
						case 6: if (compare_string (datatype, "sint")) it_is = 1; break;
						case 7: if (compare_string (datatype, "uint")) it_is = 1; break;
						case 8: if (compare_string (datatype, "slli")) it_is = 1; break;
						case 9: if (compare_string (datatype, "ulli")) it_is = 1; break;
						case 10: if (compare_string (datatype, "bool")) it_is = 1; break;
						case 11: if (compare_string (datatype, "strn")) it_is = 1; break;
						case 12: if (compare_string (datatype, "dble")) it_is = 1; break;
						case 13: if (compare_string (datatype, "scar")) it_is = 1; break;
						case 14: if (compare_string (datatype, "ucar")) it_is = 1; break;
						case 15: if (compare_string (datatype, "char")) it_is = 1; break;
						case 16: if (compare_string (datatype, "list")) it_is = 1; break;
						case 17: if (compare_string (datatype, "tree")) it_is = 1; break;
						case 18: if (compare_string (datatype, "hash")) it_is = 1; break;
						case 19: if (compare_string (datatype, "_Bool")) it_is = 1; break;
						case 20: if (compare_string (datatype, "stack")) it_is = 1; break;
						case 21: if (compare_string (datatype, "queue")) it_is = 1; break;
						case 22: if (compare_string (datatype, "union")) it_is = 1; break;
						case 23: if (compare_string (datatype, "llist")) it_is = 1; break;
						case 24: if (compare_string (datatype, "float")) it_is = 1; break;
						case 25: if (compare_string (datatype, "short")) it_is = 1; break;
						case 26: if (compare_string (datatype, "double")) it_is = 1; break;
						case 27: if (compare_string (datatype, "lstack")) it_is = 1; break;
						case 28: if (compare_string (datatype, "lqueue")) it_is = 1; break;
						case 29: if (compare_string (datatype, "fllist")) it_is = 1; break;
						case 30: if (compare_string (datatype, "string")) it_is = 1; break;
						case 31: if (compare_string (datatype, "struct")) it_is = 1; break;
						case 32: if (compare_string (datatype, "dllist")) it_is = 1; break;
						case 33: if (compare_string (datatype, "cllist")) it_is = 1; break;
						case 34: if (compare_string (datatype, "fcllist")) it_is = 1; break;
						case 35: if (compare_string (datatype, "dcllist")) it_is = 1; break;
						case 36: if (compare_string (datatype, "short int")) it_is = 1; break;
						case 37: if (compare_string (datatype, "long long")) it_is = 1; break;
						case 38: if (compare_string (datatype, "unsigned int")) it_is = 1; break;
						case 39: if (compare_string (datatype, "unsigned char")) it_is = 1; break;
						case 40: if (compare_string (datatype, "long long int")) it_is = 1; break;
						case 41: if (compare_string (datatype, "unsigned short int")) it_is = 1; break;
						case 42: if (compare_string (datatype, "unsigned long long int")) it_is = 1; break;
						case 43: if (compare_string (datatype, "signed char")) it_is = 1; break;
						case 44: if (compare_string (datatype, "signed short")) it_is = 1; break;
						case 45: if (compare_string (datatype, "signed short int")) it_is = 1; break;
						case 46: if (compare_string (datatype, "signed int")) it_is = 1; break;
						case 47: if (compare_string (datatype, "signed long long")) it_is = 1; break;
						case 48: if (compare_string (datatype, "signed long long int")) it_is = 1; break;
						default: break;
					}

					if (it_is)
						break;
				}

				return it_is;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to string. Must contain data.
OUTPUT: Return 1 if the whole string is alphabical. Otherwise return 0.
INFO: This function is used to check if the whole string is alpha.
*/
_Bool is_alpha (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size > 0) {
				for (unsigned int counter = 0; counter < _string_size; ++counter)
					if (!is_char_alpha ((*_string) [counter]))
						return 0;

				return 1;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT:	A pointer to string which hold the main string.
OUTPUT: Return 1 if the string is constraint, otherwise return 0.
INFO: This function is used to check if the string is constriant.
	The constraint can be like that:
		datatype 0, the 0 means normal declaration of a variable.
		datatype 1^n, the 1 means create one dimension array of that variable and ^ means you specfice the size of the 1d array. And n means the size.
		datatype 2^n*n the 2 means create two dimension arrays of that variable and ^ means you specify the first n is the 2d size the * is the seperator between dimension sizes.
		and that goes on.
		
		To specify the size of a specific dimension only and only if your datatype is 2d dimension or more. The specification applies to the lower dimension only:
			For example if you have a 2 dimension array, then you apply the size of 2 dimension without specification and for 1 dimension arrays from the 2 dimension
			you specify the size of each 1d array in each index of 2d array.

			2^2*[5-3]: which means the 2d array has two indices. The first index has 1d array of size 5 and the second index has 1d array of size 3.

			Otherwise you only do this: 2^2*5: And this means the two indices of the 2d array will have 1d arrays of size 5.

	We only accept or check for the m^n*... part.
*/
_Bool is_constraint (string * con) {
	if (con != NULL) {
		if ((*con) != NULL) {
			unsigned int _string_size = str_len (con);

			if (_string_size > 0) {
				if ((*con) [0] == 48)
					return 1;

				void *** content = str_char_split (con, '^');
				
				if (content != NULL) {
					unsigned int * split_size = ((unsigned int *) content [0][0]);
					string * split_content = ((string *) content [1]);

					content [0][0] = NULL;
					content [1] = NULL;
					free (content);
					content = NULL;

					//2^2*5 or 2^2*[1-2]
					if ((*split_size) == 2) {
						free (split_content [0]);
						if (is_uint (&split_content [1])) {
							unsigned  int * dimension_size = NULL;
							do {dimension_size = (unsigned int *) malloc (sizeof (unsigned int));} while (dimension_size == NULL);
							(*dimension_size) = to_uint (&split_content [1]);

							if ((*dimension_size) == 1)
								if (is_uint (&split_content [2])) {
									free (split_content [1]);
									split_content [1] = NULL;
									free (split_content [2]);
									split_content [2] = NULL;
									free (dimension_size);
									dimension_size = NULL;
									free (split_size);
									split_size = NULL;
									
									return 1;
								}

							content = str_char_split (&split_content [2], '*');

							if (content != NULL) {
							
								free (split_content [2]);
								split_content [2] = NULL;
								free (split_content [1]);
								split_content [1] = NULL;
								free (split_content);
								split_content = NULL;
								free (split_size);
								split_size = NULL;

								split_size = ((unsigned int *) content [0][0]);
								split_content = ((string *) content [1]);

								content [0][0] = NULL;
								content [1] = NULL;
								free (content);
								content = NULL;

								//2*5 or 2*[1-2]
								if ((*split_size) == (*dimension_size)) {
									free (dimension_size);
									dimension_size = NULL;
									free (split_content [0]);
									split_content [0] = NULL;

									
									_Bool invalid = 0;
									unsigned int previous_dimension_size = 0;
									for (unsigned int counter = 1; counter <= (*split_size); ++counter) {
										if (split_content [counter][0] == '[') {
											_string_size = str_len (&split_content [counter]);

											if (split_content [counter][(_string_size - 1)] == ']') {
												//1-2
												if (remove_at_these (&split_content [counter], 2, (unsigned int []) {0, (_string_size - 1)})) {
													content = str_char_split (&split_content [counter], '-');
														
													if (content != NULL) {
														unsigned int * temp_size = ((unsigned int *) content [0][0]);
														string * temp = ((string *) content [1]);
														content [0][0] = NULL;
														content [1] = NULL;
														free (content);
														content = NULL;

														if ((*temp_size) == previous_dimension_size) {
															previous_dimension_size = 0;
															
															for (unsigned int iterator = 1; iterator <= (*temp_size); ++iterator) {
																if (!is_uint (&temp [iterator])) {
																	invalid = 1;
																	break;
																} else {
																	previous_dimension_size += to_uint (&temp [iterator]);
																}
															}

															for (unsigned int iterator = 0; iterator <= (*temp_size); free (temp [iterator]), temp [iterator] = NULL, ++iterator);
															free (temp);
															temp = NULL;
															free (temp_size);
															temp_size = NULL;
															
															if (invalid)
																break;
														} else {
															for (unsigned int iterator = 0; iterator <= (*temp_size); free (temp [iterator]), temp [iterator] = NULL, ++iterator);
															free (temp);
															temp = NULL;
															free (temp_size);
															temp_size = NULL;
															invalid = 1;
															break;
														}
													} else {
														for (unsigned int counter = 1; counter <= (*split_size); free (split_content [counter]), split_content [counter] = NULL, ++counter);
														free (split_content);
														split_content = NULL;
														free (split_size);
														split_size = NULL;
													}
												} else {
													invalid = 1;
													break;
												}
											} else {
												invalid = 1;
												break;
											}
										} else if (is_uint (&split_content [counter])) {
											previous_dimension_size = to_uint (&split_content [counter]);
										} else {
											invalid = 1;
											break;
										}
									}

									for (unsigned int counter = 1; counter <= (*split_size); free (split_content [counter]), split_content [counter] = NULL, ++counter);
									free (split_content);
									split_content = NULL;
									free (split_size);
									split_size = NULL;

									if (!invalid)
										return 1;
									else
										return 0;
								} else {
									free (dimension_size);
									dimension_size = NULL;
									for (unsigned int counter = 0; counter <= (*split_size); free (split_content [counter]), split_content [counter] = NULL, ++counter);
									free (split_content);
									split_content = NULL;
									free (split_size);
									split_size = NULL;

									return 0;
								}
							} else {
								free (dimension_size);
								dimension_size = NULL;
								for (unsigned int counter = 1; counter <= (*split_size); free (split_content [counter]), split_content [counter] = NULL, ++counter);
								free (split_content);
								split_content = NULL;
								free (split_size);
								split_size = NULL;

								return 0;
							}
						} else {
							for (unsigned int counter = 1; counter <= (*split_size); free (split_content [counter]), split_content [counter] = NULL, ++counter);
							free (split_content);
							split_content = NULL;
							free (split_size);
							split_size = NULL;

							return 0;
						}
					} else {
						free (split_size);
						split_size = NULL;
						free (split_content [0]);
						free (split_content [1]);
						free (split_content);
						split_content = NULL;

						return 0;
					}
				} else
					return 0;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to a string which hold the main string.
OUTPUT: Return 1 if the whole string contains astriks, otherwise return 0.
INFO: This function is used to check if the string only constains astrick characters.
*/
_Bool is_astrik (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size != 0) {
				for (unsigned int counter = 0; counter < _string_size; ++counter)
					if ((*_string) [counter] != '*')
						return 0;

				return 1;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to string which hold the main string.
OUTPUT: Retur 1 if the string can be converted into double, otherwise return 0.
INFO: This function is used to check if the input can be converted into double.
	We have 20 digits before precision points, 1 point which is precision point and 5 precsion digits and 1 - for the negative side.
*/
_Bool is_double (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size > 0 && _string_size <= 27) {
				_Bool ist_dot = 0;
				_Bool ist_num = 0;

				if ((*_string) [0] == '-')
					ist_dot = 1;

				unsigned char counter = 0;
				if (ist_dot) {
					counter = 1;
					ist_dot = 0;
				}

				for (; counter < _string_size; ++counter) {
					if (is_char_number ((*_string) [counter])) {
						ist_num = 1;
					} else if (!ist_dot) {
							if ((*_string) [counter] == '.')
								ist_dot = 1;
							else
								break;
					} else {
						ist_num = 0;
						break;
					}
				}

				if (ist_dot && ist_num)
					return 1;
				else
					return 0;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to string. Must contain data.
OUTPUT: Return 1 if the whole string contains alphas and numbers. Otherwise return 0.
INFO: This function is used to check if the whole string is alphanumerical string.
*/
_Bool is_alpnum (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size > 0) {
				for (unsigned int counter = 0; counter < _string_size; ++counter)
					if (!is_char_alpha ((*_string) [counter]) && !is_char_number ((*_string) [counter]))
						return 0;

				return 1;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to string. Must contain data.
OUTPUT: 1 if the whole string is number otherwise return 0.
INFO: This function is used to check if the whole string in a number.
*/
_Bool is_number (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size > 0) {
				for (unsigned int counter = 0; counter < _string_size; ++counter)
					if (!is_char_number ((*_string) [counter]))
						return 0;

				return 1;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to string which hold the main string.
OUTPUT: Return 1 if the string represent a keyword, otherwise return 0.
INFO: This function is used to check if the string can represent a keyword like:
	class, Extend, Implement, typedef, interface, abstract, return, this, case, default, switch, if, else, else if, #include, #define, #ifdef, #ifndef
	, #if, #else, #elif, #endif, #pragram, #undefine, for, while, do, const, volatile, restrict, inline, __attribute__, va_list, new and static.
*/
_Bool is_keyword (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size != 0) {
				if (compare_string (_string, "class")) {
					return 1;	
				} else if (compare_string (_string, "Extend")) {
					return 1;
				} else if (compare_string (_string, "Implement")) {
					return 1;
				} else if (compare_string (_string, "typedef")) {
					return 1;
				} else if (compare_string (_string, "interface")) {
					return 1;
				} else if (compare_string (_string, "abstract")) {
					return 1;
				} else if (compare_string (_string, "static")) {
					return 1;
				} else if (compare_string (_string, "return")) {
					return 1;
				} else if (compare_string (_string, "this")) {
					return 1;
				} else if (compare_string (_string, "case")) {
					return 1;
				} else if (compare_string (_string, "default")) {
					return 1;
				} else if (compare_string (_string, "switch")) {
					return 1;
				} else if (compare_string (_string, "if")) {
					return 1;
				} else if (compare_string (_string, "else")) {
					return 1;
				} else if (compare_string (_string, "else if")) {
					return 1;
				} else if (compare_string (_string, "#include")) {
					return 1;
				} else if (compare_string (_string, "#if")) {
					return 1;
				} else if (compare_string (_string, "#else")) {
					return 1;
				} else if (compare_string (_string, "#elif")) {
					return 1;
				} else if (compare_string (_string, "#endif")) {
					return 1;
				} else if (compare_string (_string, "#define")) {
					return 1;
				} else if (compare_string (_string, "#ifdef")) {
					return 1;
				} else if (compare_string (_string, "ifndef")) {
					return 1;
				} else if (compare_string (_string, "#undefine")) {
					return 1;
				} else if (compare_string (_string, "#pragram")) {
					return 1;
				} else if (compare_string (_string, "for")) {
					return 1;
				} else if (compare_string (_string, "while")) {
					return 1;
				} else if (compare_string (_string, "do")) {
					return 1;
				} else if (compare_string (_string, "const")) {
					return 1;
				} else if (compare_string (_string, "volatile")) {
					return 1;
				} else if (compare_string (_string, "restrict")) {
					return 1;
				} else if (compare_string (_string, "inline")) {
					return 1;
				} else if (compare_string (_string, "__attribute__")) {
					return 1;
				} else if (compare_string (_string, "va_list")) {
					return 1;
				} else if (compare_string (_string, "new")) {
					return 1;
				} else {
					return 0;
				}
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to string. Must contain data.
OUTPUT:	Return 1 if the whole string contains special characters (white spaces) included. Otherwise return 0.
INFO: This function is used to check if the whole string contains special characters.
	(White spaces) included.
*/
_Bool is_special (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size > 0) {
				for (unsigned int counter = 0; counter < _string_size; ++counter)
					if (!is_char_special ((*_string) [counter]) && !is_char_white_space ((*_string) [counter]))
						return 0;

				return 1;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to string which hold the main string.
OUTPUT: Return 1 if the string represent an abstract class, otherwise return 0.
INFO: This function is used to check if the string can represent an abstract class.
	AccessModifer abstract AbstractName [Extend/Implement] [SuperClass/interface] {
*/
_Bool is_abstract (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size != 0) {
				void *** content = str_char_split (_string, ' ');

				if (content != NULL) {
					unsigned int * split_size = ((unsigned int *) content [0][0]);
					string * split_content = ((string *) content [1]);

					content [0][0] = NULL;
					content [1] = NULL;
					free (content);
					content = NULL;

					if ((*split_size) == 4 || (*split_size) == 6) {
						if (is_modifier (&split_content [1])) {
							if (compare_string (&split_content [2], "abstract")) {
								if (is_var (&split_content [3])) {
									if ((*split_size) == 4) {
										if (str_len (&split_content [4]) == 1 && split_content [4][0] == '{') {
											_string_size = 0;
											for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
											free (split_content);
											split_content = NULL;
											free (split_size);
											split_size = NULL;

											return 1;
										} else {
											_string_size = 0;
											for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
											free (split_content);
											split_content = NULL;
											free (split_size);
											split_size = NULL;

											return 0;
										}
									} else {
										if (compare_string (&split_content [4], "Extend") || compare_string (&split_content [4], "Implement")) {
											if (is_var (&split_content [5])) {
												if (str_len (&split_content [6]) == 1 && split_content [6][0] == '{') {
													_string_size = 0;
													for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
													free (split_content);
													split_content = NULL;
													free (split_size);
													split_size = NULL;

													return 1;
												} else {
													_string_size = 0;
													for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
													free (split_content);
													split_content = NULL;
													free (split_size);
													split_size = NULL;

													return 0;
												}
											} else {
												_string_size = 0;
												for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
												free (split_content);
												split_content = NULL;
												free (split_size);
												split_size = NULL;

												return 0;
											}
										} else {
											_string_size = 0;
											for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
											free (split_content);
											split_content = NULL;
											free (split_size);
											split_size = NULL;

											return 0;
										}
									}
								} else {
									_string_size = 0;
									for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
									free (split_content);
									split_content = NULL;
									free (split_size);
									split_size = NULL;

									return 0;
								}
							} else {
								_string_size = 0;
								for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
								free (split_content);
								split_content = NULL;
								free (split_size);
								split_size = NULL;

								return 0;
							}
						} else {
							_string_size = 0;
							for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
							free (split_content);
							split_content = NULL;
							free (split_size);
							split_size = NULL;

							return 0;
						}
					} else {
						_string_size = 0;
						for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
						free (split_content);
						split_content = NULL;
						free (split_size);
						split_size = NULL;

						return 0;
					}
				} else
					return 0;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to string. Must be initialized.
OUTPUT: Return 1 if string is terminated otherwise return 0.
INFO: This function is used to terminate the string.
*/
_Bool term_string (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			free ((*_string));
			(*_string) = NULL;

			return 1;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to a string which hold the main string.
OUTPUT: Return 1 if the string represent a modifier, otherwise return 0.
INFO: This function is used to check if the string represent a modifer.
	Default, Public, Private, Protected.

	Default: Means the object can be accessed only and only if in the same directory.
	Public: Means the object can be accesed from any directory.
	Protected: Means The object can be accessed from the same directory or by a child from different directroy.
	Private: Only accessable in the current folder.
*/
_Bool is_modifier (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size != 0) {
				if (compare_string (_string, "Default")) {
					return 1;
				} else if (compare_string (_string, "Private")) {
					return 1;
				} else if (compare_string (_string, "Public")) {
					return 1;
				} else if (compare_string (_string, "Protected")) {
					return 1;
				} else {
					return 0;
				}
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to a string which hold the main string.
OUTPUT: Return 1 if the string represent an interface, otherwise return 0.
INFO: This function is used to check if the string represent an interface.
	AccessModifer interface InterfaceName [Extend] [InterfaceName,AnotherOne,...] {

***Important***: If you Extend more than one file, then between each name but a comma not space is allowed before or after only commas. Otherwise an error will be returned.
*/
_Bool is_interface (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size != 0) {
				void *** content = str_char_split (_string, ' ');
				
				if (content != NULL) {
					unsigned int * split_size = ((unsigned int *) content [0][0]);
					string * split_content = ((string *) content [1]);

					content [0][0] = NULL;
					content [1] = NULL;
					free (content);
					content = NULL;

					if (is_modifier (&split_content [1])) {
						if (compare_string (&split_content [2], "interface")) {
							if (is_var (&split_content [3])) {
								if ((*split_size) == 4) {
									if (str_len (&split_content [4]) == 1 && split_content [4][0] == '{') {
										_string_size = 0;
										for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
										free (split_content);
										split_content = NULL;
										free (split_size);
										split_size = NULL;

										return 1;
									} else {
										_string_size = 0;
										for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
										free (split_content);
										split_content = NULL;
										free (split_size);
										split_size = NULL;

										return 0;
									}
								} else {
									if (compare_string (&split_content [4], "Extend")) {
										if (is_var (&split_content [5])) {
											if (str_len (&split_content [6]) == 1 && split_content [6][0] == '{') {
												_string_size = 0;
												for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
												free (split_content);
												split_content = NULL;
												free (split_size);
												split_size = NULL;

												return 1;
											} else {
												_string_size = 0;
												for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
												free (split_content);
												split_content = NULL;
												free (split_size);
												split_size = NULL;

												return 0;
											}
										} else {
											content = str_char_split (&split_content [5], ',');
											if (content != NULL) {
												string * temp = ((string *) content [1]);
												unsigned int * temp_size = ((unsigned int *) content [0][0]);

												content [1] = NULL;
												content [0][0] = NULL;
												free (content);
												content = NULL;

												_Bool all_correct = 1;
												_string_size = 1;

												for (; _string_size <= (*temp_size); ++_string_size) {
													if (temp [_string_size][0] == ' ')
														remove_at (&temp [_string_size], 0);

													if (!is_var (&temp [_string_size])) {
														all_correct = 0;
														break;
													}
												}
												
												_string_size = 0;
												for (; _string_size <= (*temp_size); free (temp [_string_size]), temp [_string_size] = NULL, ++_string_size);
												free (temp);
												temp = NULL;
												free (temp_size);
												temp_size = NULL;

												if (all_correct) {
													if (str_len (&split_content [6]) == 1 && split_content [6][0] == '{') {
														_string_size = 0;
														for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
														free (split_content);
														split_content = NULL;
														free (split_size);
														split_size = NULL;

														return 1;
													} else {
														_string_size = 0;
														for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
														free (split_content);
														split_content = NULL;
														free (split_size);
														split_size = NULL;

														return 0;
													}
												} else {
													_string_size = 0;
													for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
													free (split_content);
													split_content = NULL;
													free (split_size);
													split_size = NULL;

													return 0;
												}
											} else {
												_string_size = 0;
												for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
												free (split_content);
												split_content = NULL;
												free (split_size);
												split_size = NULL;

												return 0;
											}
										}
									} else {
										_string_size = 0;
										for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
										free (split_content);
										split_content = NULL;
										free (split_size);
										split_size = NULL;

										return 0;
									}
								}
							} else {
								_string_size = 0;
								for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
								free (split_content);
								split_content = NULL;
								free (split_size);
								split_size = NULL;

								return 0;
							}
						} else {
							_string_size = 0;
							for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
							free (split_content);
							split_content = NULL;
							free (split_size);
							split_size = NULL;

							return 0;
						}
					} else {
						_string_size = 0;
						for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
						free (split_content);
						split_content = NULL;
						free (split_size);
						split_size = NULL;

						return 0;
					}
				} else
					return 0;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to string. Must contain data.
OUTPUT: Return 1 if the whole string contains white space only. Otherwise return 0.
INFO: This function is used to check if the whole string consist of whitespaces variations.
*/
_Bool is_white_space (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size > 0) {
				for (unsigned int counter = 0; counter < _string_size; ++counter)
					if (!is_char_white_space ((*_string) [counter]))
						return 0;

				return 1;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to string. Must contain data.
OUTPUT: If the string is reversed otherwise return 0.
INFO: This function is used to reverse the string. First become last and last become first.
Example:
	If the input is "ABCDE".
	The output will be "EDCBA"

	In the first iteration: "EBCDA".
	In the second iteration: "EDCBA".
	In the third iteration : "EDCBA".
*/
_Bool reverse_string (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size > 0) {
				for (unsigned int counter = 0; counter < (_string_size - 2); ++counter) { //We substract the size by 2. Because we change two characters at a time.
					char temp = (*_string) [counter];
					(*_string) [counter] = (*_string) [(_string_size - (counter + 1))];
					(*_string) [(_string_size - (counter + 1))] = temp;
				}

				return 1;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to a string which hold the main string.
OUTPUT: Return 1 if the string represent a function, otherwise return 0.
INFO: This function is used to check if the string represent a function.
	Example:
		datatype [*] var ([datatype [*] var], ...);
*/
_Bool is_it_function (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size =  str_len (_string);

			if (_string_size != 0) {
				string temp_1 = ";";
				string temp_2 = ");";

				if (does_it_end_with (_string, ");")) {
					remove_at_these (_string, 2, (unsigned int []) {(_string_size - 2), (_string_size - 1)});

					void *** content = str_char_split (_string, '(');

					if (content != NULL) {
						unsigned int * split_size = ((unsigned int *) content [0][0]);
						string * split_content = ((string *) content [1]);

						content [1] = NULL;
						content [0][0] = NULL;
						free (content);
						content = NULL;

						if ((*split_size) == 2 || (*split_size) == 1) {
							if ((*split_size) == 1) {
								append (_string, 0, &temp_2);
								_string_size = 0;
								for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
								free (split_content);
								split_content = NULL;
								free (split_size);
								split_size = NULL;

								return 1;
							}

							_string_size = str_len (&split_content [1]);
							//datatype [*] var;
							if (split_content [1][(_string_size - 1)] == ' ')
								split_content [1][(_string_size - 1)] = ';';
							else
								append (&split_content [1], 0, &temp_1);

							if (is_var_declaration (&split_content [1])) {
								_Bool all_correct = check_func_parameters (&split_content [2]);

								append (_string, 0, &temp_2);
								_string_size = 0;
								for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
								free (split_content);
								split_content = NULL;
								free (split_size);
								split_size = NULL;

								return all_correct;
							} else {
								append (_string, 0, &temp_2);
								_string_size = 0;
								for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
								free (split_content);
								split_content = NULL;
								free (split_size);
								split_size = NULL;

								return 0;
							}
						} else {
							append (_string, 0, &temp_2);
							_string_size = 0;
							for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
							free (split_content);
							split_content = NULL;
							free (split_size);
							split_size = NULL;

							return 0;
						}
					} else
						return 0;
				} else
					return 0;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to a function which hold the main string.
OUTPUT: Return 1 if the string represent variable definition, otherwise return 0.
INFO: This function is used to check if the string represent a variable definition.
	datatype [*] VarName=Data; 
	datatype [*] VarName= Data;
	datatype [*] VarName = Data;
*/
_Bool is_var_definition (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size != 0) {
				void *** content = str_char_split (_string, '=');

				if (content != NULL) {
					unsigned int * split_size = ((unsigned int *) content [0][0]);
					string * split_content = ((string *) content [1]);

					content [0][0] = NULL;
					content [1] = NULL;
					free (content);
					content = NULL;

					if ((*split_size) == 2) {
						if (split_content [2][(str_len (&split_content [2]) - 1)] == ';') {
							remove_at (&split_content [2], (str_len (&split_content [2]) - 1));

							_string_size = str_len (&split_content [1]);

							if (is_char_alpha (split_content [1][(_string_size - 1)]) || is_char_number (split_content [1][(_string_size - 1)]) || split_content [1][(_string_size - 1)] == '_') {
								string temp = ";";
								append (&split_content [1], 0, &temp);
							} else
								insert_at (&split_content [1], (_string_size - 1), ';');

							if (is_var_declaration (&split_content [1])) {
								string temp = single_char_split (&split_content [1], ' ');
								
								if (split_content [2][0] == ' ')
									remove_at (&split_content [2], 0);

								if (does_value_match_type (&split_content [2], &temp)) {
									free (temp);
									temp = NULL;

									_string_size = 0;
									for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
									free (split_content);
									split_content = NULL;
									free (split_size);
									split_size = NULL;

									return 1;
								} else {
									free (temp);
									temp = NULL;

									_string_size = 0;
									for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
									free (split_content);
									split_content = NULL;
									free (split_size);
									split_size = NULL;

									return 0;
								}
							} else {
								_string_size = 0;
								for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
								free (split_content);
								split_content = NULL;
								free (split_size);
								split_size = NULL;

								return 0;
							}
						} else {
							_string_size = 0;
							for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
							free (split_content);
							split_content = NULL;
							free (split_size);
							split_size = NULL;

							return 0;
						}
					} else {
						_string_size = 0;
						for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
						free (split_content);
						split_content = NULL;
						free (split_size);
						split_size = NULL;

						return 0;
					}
				} else
					return 0;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to a string which hold the main string.
OUTPUT: Return 1 if the string represent variable declaration, otherwise return 0.
INFO: This function is used to check if the string represent variable declaration.
	datatype [*] VarName;
*/
_Bool is_var_declaration (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size != 0) {
				if ((*_string) [(_string_size - 1)] == ';') {
					if (remove_at (_string, (_string_size - 1))) {
						void *** content = str_char_split (_string, ' ');

						if (content != NULL) {
							unsigned int * split_size = ((unsigned int *) content [0][0]);
							string * split_content = ((string *) content [1]);

							content [0][0] = NULL;
							content [1] = NULL;
							free (content);
							content = NULL;

							if ((*split_size) == 2 || (*split_size) == 3) {
								if (is_type (&split_content [1])) {
									if ((*split_size) == 2) {
										if (is_var (&split_content [2])) {
											_string_size = 0;
											for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
											free (split_content);
											split_content = NULL;
											free (split_size);
											split_size = NULL;
											
											return 1;
										} else {
											_string_size = 0;
											for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
											free (split_content);
											split_content = NULL;
											free (split_size);
											split_size = NULL;
											
											return 0;
										}
									} else {
										if (is_astrik (&split_content [2])) {
											if (is_var (&split_content [3])) {
												_string_size = 0;
												for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
												free (split_content);
												split_content = NULL;
												free (split_size);
												split_size = NULL;

												
												string temp = ";";
												append (_string, 0, &temp);
												
												return 1;
											} else {
												_string_size = 0;
												for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
												free (split_content);
												split_content = NULL;
												free (split_size);
												split_size = NULL;

												return 0;
											}
										} else {
											_string_size = 0;
											for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
											free (split_content);
											split_content = NULL;
											free (split_size);
											split_size = NULL;

											return 0;
										}
									}
								} else {
									_string_size = 0;
									for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
									free (split_content);
									split_content = NULL;
									free (split_size);
									split_size = NULL;
									
									return 0;
								}
							} else {
								_string_size = 0;
								for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
								free (split_content);
								split_content = NULL;
								free (split_size);
								split_size = NULL;

								return 0;
							}
						} else
							return 0;
					} else
						return 0;
				} else
					return 0;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to string which hold the main string.
OUTPUT: Return 1 if the string can represent a function pointer, otherwise return 0.
INFO: This function check if the string can represent a function pointer.
	Example:
		datatype [*] (* var [*]/[n]) ([datatype [*] var], ...);

NOTE: Very Important, We only support one layer of prantheses coverting the function pointer i.e: (* var [*]/[n])
	We do not support that: (*(Var [*]/[n])). Only one layer. In the future we can solve it.
*/
_Bool is_it_ptr_function (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size != 0) {
				string temp = ");";
				if (does_it_end_with (_string, ");")) {
					remove_at_these (_string, 2, (unsigned int []) {(_string_size - 2), (_string_size - 1)});

					void *** content = str_char_split (_string, '(');

					if (content != NULL) {
						unsigned int * split_size = ((unsigned int *) content [0][0]);
						string * split_content = ((string *) content [1]);

						content [1] = NULL;
						content [0][0] = NULL;
						free (content);
						content = NULL;

						if ((*split_size) == 2) {
							if (check_func_parameters (&split_content [2])) {
								content = str_char_split (&split_content [1], ' ');

								if (content != NULL) {
									_string_size = 0;
									for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
									free (split_content);
									split_content = NULL;
									free (split_size);
									split_size = NULL;

									split_content = ((string *) content [1]);
									split_size = ((unsigned int *) content [0][0]);

									if ((*split_size) == 2 || (*split_size) == 3) {
										if (is_type (&split_content [1])) {
											_Bool check_astriks = 0;
											if ((*split_size) == 3)
												check_astriks = 1;
											
											if (check_astriks)
												if (!is_astrik (&split_content [2])) {
													append (_string, 0, &temp);
													_string_size = 0;
													for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
													free (split_content);
													split_content = NULL;
													free (split_size);
													split_size = NULL;

													return 0;
												}

											unsigned char index = 2;
											if (check_astriks)
												index = 3;

											_string_size = str_len (&split_content [index]);
											if (split_content [index][0] == '(' && split_content [index][(_string_size - 1)] == ')')
												remove_at_these (&split_content [index], 2, ((unsigned int []) {0, (_string_size - 1)}));

											content = str_char_split (&split_content [index], ' ');
											
											if (content != NULL) {
												_string_size = 0;
												for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
												free (split_content);
												split_content = NULL;
												free (split_size);
												split_size = NULL;

												split_size = ((unsigned int *) content [0][0]);
												split_content = ((string *) content [1]);

												if ((*split_size) == 2 || (*split_size) == 3) {
													if (str_len (&split_content [1]) == 1 && split_content [1][0] == '*') {
														if (is_var (&split_content [2])) {
															if ((*split_size) == 3) {
																if (is_astrik (&split_content [3])) {
																	append (_string, 0, &temp);
																	_string_size = 0;
																	for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
																	free (split_content);
																	split_content = NULL;
																	free (split_size);
																	split_size = NULL;

																	return 1;
																} else {
																	_string_size = str_len (&split_content [3]);
																	if (split_content [3][0] == '[' && split_content [3][(_string_size - 1)] == ']') {
																		remove_at_these (&split_content [3], 2, ((unsigned int []) {0, (_string_size - 1)}));

																		if (is_number (&split_content [3])) {
																			append (_string, 0, &temp);
																			_string_size = 0;
																			for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
																			free (split_content);
																			split_content = NULL;
																			free (split_size);
																			split_size = NULL;

																			return 1;
																		} else {
																			append (_string, 0, &temp);
																			_string_size = 0;
																			for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
																			free (split_content);
																			split_content = NULL;
																			free (split_size);
																			split_size = NULL;

																			return 0;
																		}
																	} else {
																		append (_string, 0, &temp);
																		_string_size = 0;
																		for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
																		free (split_content);
																		split_content = NULL;
																		free (split_size);
																		split_size = NULL;

																		return 0;
																	}
																}
															} else {
																append (_string, 0, &temp);
																_string_size = 0;
																for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
																free (split_content);
																split_content = NULL;
																free (split_size);
																split_size = NULL;

																return 1;
															}
														} else {
															append (_string, 0, &temp);
															_string_size = 0;
															for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
															free (split_content);
															split_content = NULL;
															free (split_size);
															split_size = NULL;

															return 0;
														}
													} else {
														append (_string, 0, &temp);
														_string_size = 0;
														for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
														free (split_content);
														split_content = NULL;
														free (split_size);
														split_size = NULL;

														return 0;
													}
												} else {
													append (_string, 0, &temp);
													_string_size = 0;
													for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
													free (split_content);
													split_content = NULL;
													free (split_size);
													split_size = NULL;

													return 0;
												}
											} else {
												append (_string, 0, &temp);
												_string_size = 0;
												for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
												free (split_content);
												split_content = NULL;
												free (split_size);
												split_size = NULL;

												return 0;
											}
										} else {
											append (_string, 0, &temp);
											_string_size = 0;
											for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
											free (split_content);
											split_content = NULL;
											free (split_size);
											split_size = NULL;

											return 0;
										}
									} else {
										append (_string, 0, &temp);
										_string_size = 0;
										for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
										free (split_content);
										split_content = NULL;
										free (split_size);
										split_size = NULL;

										return 0;
									}
								} else {
									append (_string, 0, &temp);
									_string_size = 0;
									for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
									free (split_content);
									split_content = NULL;
									free (split_size);
									split_size = NULL;

									return 0;
								}
							} else {
								append (_string, 0, &temp);
								_string_size = 0;
								for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
								free (split_content);
								split_content = NULL;
								free (split_size);
								split_size = NULL;

								return 0;
							}
						} else {
							append (_string, 0, &temp);
							_string_size = 0;
							for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
							free (split_content);
							split_content = NULL;
							free (split_size);
							split_size = NULL;

							return 0;
						}
					} else
						return 0;
				} else
					return 0;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to string which hold the main string.
OUTPUT: Return 1 if the string is a right experssion, otherwise return 0.
INFO: This function is used to check if the string can represent experssion.
	sshi 2^n*n is an experssion. Used in union and struct to create a union or a struct.
*/
_Bool check_experssion (string * experssion) {
	if (experssion != NULL) {
		if ((*experssion) != NULL) {
			unsigned int experssion_size = str_len (experssion);

			if (experssion_size != 0) {
				void *** content = str_char_split (experssion, ' ');

				if (content != NULL) {
					string * split_content = ((string *) content [1]);
					unsigned int * split_size = ((unsigned int *) content [0][0]);

					content [1] = NULL;
					content [0][0] = NULL;
					free (content);
					content = NULL;

					if ((*split_size) == 2) {
						if (is_type (&split_content [1]) && str_len (&split_content [1]) == 4) {
							if (is_constraint (&split_content [2])) {
								experssion_size = 0;
								for (; experssion_size <= (*split_size); free (split_content [experssion_size]), split_content [experssion_size] = NULL, ++experssion_size);
								free (split_content);
								split_content = NULL;
								free (split_size);
								split_size = NULL;

								return 1;
							} else {
								experssion_size = 0;
								for (; experssion_size <= (*split_size); free (split_content [experssion_size]), split_content [experssion_size] = NULL, ++experssion_size);
								free (split_content);
								split_content = NULL;
								free (split_size);
								split_size = NULL;

								return 0;
							}
						} else {
							experssion_size = 0;
							for (; experssion_size <= (*split_size); free (split_content [experssion_size]), split_content [experssion_size] = NULL, ++experssion_size);
							free (split_content);
							split_content = NULL;
							free (split_size);
							split_size = NULL;

							return 0;
						}
					} else {
						experssion_size = 0;
						for (; experssion_size <= (*split_size); free (split_content [experssion_size]), split_content [experssion_size] = NULL, ++experssion_size);
						free (split_content);
						split_content = NULL;
						free (split_size);
						split_size = NULL;

						return 0;
					}
				} else
					return 0;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to a string which hold the main string.
OUTPUT: Return 1 if the string represent function parameters, otherwise return 0.
INFO: This function is used to check the parameters of functions if they are correct.
*/
_Bool check_func_parameters (string * parameters) {
	if (parameters != NULL) {
		if ((*parameters) != NULL) {
			unsigned int parameters_size = str_len (parameters);

			if (parameters_size != 0) {
				void *** content = str_char_split (parameters, ',');

				if (content != NULL) {
					unsigned int * split_size = ((unsigned int *) content [0][0]);
					string * split_content = ((string *) content [1]);

					_Bool all_correct = 1;
					parameters_size = 1;
					for (; parameters_size <= (*split_size); ++parameters_size) {
						string temp = ";";
						if (split_content [parameters_size][0] == ' ')
							remove_at (&split_content [parameters_size], 0);

						if (!compare_string (&split_content [parameters_size], "...")) {
							append (&split_content [parameters_size], 0, &temp);
							if (!is_var_declaration (&split_content [parameters_size])) {
								all_correct = 0;
								break;
							}
						}
					}

					parameters_size = 0;
					for (; parameters_size <= (*split_size); free (split_content [parameters_size]), split_content [parameters_size] = NULL, ++parameters_size);
					free (split_content);
					split_content = NULL;
					free (split_size);
					split_size = NULL;

					return all_correct;
				} else {
					if (is_var_declaration (parameters))
						return 1;

					return 0;
				}
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to a string which hold the main string and a string which will be used to overried the main string must at least of size 1.
OUTPUT: Return 1 if the main string is overriden, otherwise return 0.
INFO: This function is used to override the main string to another type of string. The new string must be of size 1.
	We will create a new copy of the new string. We will not use the same reference for further pointer manipulation error or handling errors.
*/
_Bool string_override (string * main, string new) {
	if (main != NULL && new != NULL) {
		if ((*main) != NULL) {
			unsigned int new_size = str_len (&new);

			if (new_size >= 1) {
				free ((*main));
				(*main) = NULL;

				(*main) = give_me_str_copy (&new);

				return 1;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to a string that will be initialized and string value that will be added to it.
OUTPUT: Return 1 if the string is initialized otherwise return 0.
INFO: This function is used to initialized a string. If the data equal to NULL or contain no elements.
	Then we initialized the string with only one character "C".
*/
_Bool init_string (string * _string, string data) {
	if (_string != NULL) {
		if ((*_string) == NULL) {
			unsigned int data_size = str_len (&data);

			if (data_size != 0) {
				do {(*_string) = (string) malloc (sizeof (char) * (data_size + 1));} while ((*_string) == NULL);
				(*_string) [data_size] = '\0';

				for (unsigned int counter = 0; counter < data_size; (*_string) [counter] = data [counter], ++counter);
			} else {
				do {(*_string) = (string) malloc (sizeof (char) * 2);} while ((*_string) == NULL);
				(*_string) [0] = 'C';
				(*_string) [1] = '\0';
			}

			return 1;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to a string that hold the main string and a pointer to a string which will be added to the end of main. Both must contain data.
OUTPUT: Return 1 if the operation is a success otherwise return 0.
INFO: This function to compine the secondary data to the end of the main data.
*/
_Bool combine (string * main, string * secondary) {
	if (main != NULL && secondary != NULL) {
		if ((*main) != NULL && (*secondary) != NULL) {
			unsigned int main_size = str_len (main);
			unsigned int secondary_size = str_len (secondary);

			if (main_size > 0 && secondary_size > 0) {
				//Allocate a string that hold the content of main, so if the reallocation went wrong, we can use temp for backup.
				string temp = NULL;
				do {temp = (string) malloc (sizeof (char) * (main_size + 1));} while (temp == NULL);
				for (unsigned int counter = 0; counter < main_size; temp [counter] = (*main) [counter], ++counter);
				temp [main_size] = '\0';

				//The reallocation of main.
				_Bool is_null = 0; //Is used to check if the reallocation went wrong, we used to re-fill it with temp.
				do {(*main) = (string) realloc ((*main), (sizeof (char) * (main_size + secondary_size + 1))); if ((*main) == NULL) is_null = 1;} while ((*main) == NULL);

				if (is_null)
					for (unsigned int counter = 0; counter < main_size; (*main) [counter] = temp [counter], ++counter);

				free (temp);
				temp = NULL;

				for (unsigned int counter = 0; counter < secondary_size; (*main) [(main_size + counter)] = (*secondary) [counter], ++counter);

				return 1;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
//Remember To Change The Color. And Additional Informatin If Necessary.
INPUT: A 2d array of the analysis. This 2d can be generated from by calling string_analysis function. See below.
OUTPUT: Return 1 if the analysis is displayed. Otherwise return 0.
INFO: This function is used to display information about the string.

The beginning (Time, Data and Title). #done
First: Print the string. #done
Second: Print the size of the string. #done
Third: Count how alphabitical characters all, then print lower and upper n characters. #done
Fourth: Count the numbers in the string.
Fifth: Count the speical in the string.
Sixth: Count the white spaces in the string and there variation.
Seventh: The occureneces of each characters in the string and their indices.
Eights: The possible type that the string can be converted to.
Ninth: Hash.

The content of the analysis:
	0-The size of the string.
	1-How many alph characters. #done
	2-How many lower case characters and their indices. #done
	3-How many upper case characters and their indices. #done
	4-How many numbers in the string and their indices. #done
	5-How many special in the string and their indices. #done
	6-How many space in the string and their indices. #done
	7-How many Htab in the string and their indices. #done
	8-How many  Vtab in the string and their indices. #done
	9-The sum of white space number. #done
	10-The occurences of each characters in the string and their indices. #done
	11-The possible type of string that can be converted to. #done
	12-The hash of the string. #done
	13-The string it self. Which we can be used to print the string normally and reversed. #done
*/
_Bool display_analysis (unsigned int *** analysis) {
	if (analysis != NULL) {
		//To clear the screen with windows commands
		//Require re-compiling if you running on different operating system.
		#ifdef _WIN32
		system ("cls");
		#endif

		//To clear the screen with linux commands.
		//Require re-compiling if you running on different operating system.
		#ifdef __linux__
		system ("clear");
		#endif

		//Beginning
		printf ("\t\t\t\t\t\tThe Analysis Of String.\n");
		printf ("The Current Date: %s.\n", __DATE__);
		printf ("The Current Time: %s.\n\n", __TIME__);

		//sperator
		printf ("\t\t\t\t======================================================================================================================\n\n");

		//Printing the string. Normally and reversed.
		printf ("The String Normally Is: ");
		for (unsigned int counter = 0; counter < analysis [0][0][0]; printf ("%c", analysis [13][0][counter]), ++counter);
		printf ("\n");
		printf ("The string Reversed Is: ");
		for (unsigned int counter = analysis [0][0][0]; counter <= 0; printf ("%c", analysis [13][0][counter]), --counter);
		printf("\n");
		printf ("The Length Of The String Is: ");
		printf ("%u.\n", analysis [0][0][0]);
		printf ("The String Hash Is: ");
		printf ("%u.\n", analysis [12][0][0]);

		if (analysis [11][0][0] > 0) {
			printf ("The Number Of Type The String Can Convert To Is: ");
			printf ("%u.\n", analysis [11][0][0]);
			for (unsigned int counter = 0; counter < analysis [11][0][0]; ++counter) {
				switch (analysis [11][1][counter]) {
					case 0: printf ("\tThe String Can Be Converted To: "); printf ("Signed Short Int.\n");
					case 1:	printf ("\tThe String Can Be Converted To: "); printf ("Unsigned Short Int.\n");
					case 2: printf ("\tThe String Can Be Converted To: "); printf ("Signed Integer.\n");
					case 3: printf ("\tThe String Can Be Converted To: "); printf ("Unsigned Integer.\n");
					case 4: printf ("\tThe String Can Be Converted To: "); printf ("Signed Long Long Int.\n");
					case 5: printf ("\tThe String Can Be Converted To: "); printf ("Unsigned Long Long Int.\n");
					case 6: printf ("\tThe String Can Be Converted To: "); printf ("Double.\n");
					case 7: printf ("\tThe String Can Be Converted To: "); printf ("Basic Struct.\n");
					case 8: printf ("\tThe String Can Be Converted To: "); printf ("Basic Union.\n");
					default: break;
				}
			}
			printf ("\n");
		} else {
			printf ("The String Can Not Be Coverted To Any Other Type.\n\n");
		}
		
		//sperator
		printf ("\t\t\t\t======================================================================================================================\n\n");
		
		//Print 1-
		printf ("The Number Of Characters In The String Is: ");
		printf ("%u.\n", analysis [1][0][0]);
		if (analysis [1][0][0] > 0) {
			printf ("Their indices are:\n");
			for (unsigned int counter = 0; counter < analysis [1][0][0]; printf ("\tCharacter found at index: %u.\n", analysis [1][1][counter]), ++counter);
			printf ("\nPrinting the characters:\n");
			for (unsigned int counter = 0; counter < analysis [1][0][0]; printf ("\tCharacter fount at index [%u] is: %c.\n", analysis [1][1][counter], analysis [1][2][counter]), ++counter);
		}
		
		printf ("\n");

		//Print 2-
		printf ("The Number Of Lower Case Characters Is: ");
		printf ("%u.\n", analysis [2][0][0]);
		for (unsigned int counter = 0; counter < analysis [2][0][0]; ++counter) {
			printf ("\tThe Lower Case Character In Index [");
			printf ("%u", analysis [2][1][counter]);
			printf ("] Is: ");
			printf ("%c.\n", analysis [2][2][counter]);
		}
		printf ("\n");

		//Print 3-
		printf ("The Number Of Upper Case Characters In The String Is: ");
		printf ("%u.\n", analysis [3][0][0]);
		for (unsigned int counter = 0; counter < analysis [3][0][0]; ++counter) {
			printf ("\tThe Upper Case Character In Index [");
			printf ("%u", analysis [3][1][counter]);
			printf ("] Is: ");
			printf ("%c.\n", analysis [3][2][counter]);	
		}
		printf ("\n");

		//Print 4-
		printf ("The Number Of Numbers In The String Is: ");
		printf ("%u.\n", analysis [4][0][0]);
		for (unsigned int counter = 0; counter < analysis [4][0][0]; ++counter) {
			printf ("\tThe Number In Index [");
			printf ("%u", analysis [4][1][counter]);
			printf ("] Is: ");
			printf ("%c.\n", analysis [4][2][counter]);	
		}
		printf ("\n");

		//Printf 5-
		printf ("The Number Of Special Characters In The String Is: ");
		printf ("%u.\n", analysis [5][0][0]);
		for (unsigned int counter = 0; counter < analysis [5][0][0]; ++counter) {
			printf ("\tThe Special Character In Index [");
			printf ("%u", analysis [5][1][counter]);
			printf ("] Is: ");
			printf ("%c.\n", analysis [5][2][counter]);	
		}
		printf ("\n");

		//Print 9-
		printf ("The Number Of White Spaces Variation Is: ");
		printf ("%u.\n\n", analysis [9][0][0]);

		//Printf 6-
		printf ("The Number Of Spaces In The String Is: ");
		printf ("%u.\n", analysis [6][0][0]);
		for (unsigned int counter = 0; counter < analysis [6][0][0]; ++counter) {
			printf ("\tThere Is Space In Index [");
			printf ("%u.\n", analysis [6][1][counter]);
		}
		printf ("\n");

		//Print 7-
		printf ("The Number Of Horizintal Tabs In The String Is: ");
		printf ("%u.\n", analysis [7][0][0]);
		for (unsigned int counter = 0; counter < analysis [7][0][0]; ++counter) {
			printf ("\tThere Is Horizintal Tab In Index [");
			printf ("%u.\n", analysis [7][1][counter]);
		}
		printf ("\n");

		//Print 8-
		printf ("The Number Of Vertical Tabs In The String Is: ");
		printf ("%u.\n", analysis [8][0][0]);
		for (unsigned int counter = 0; counter < analysis [8][0][0]; ++counter) {
			printf ("\tThere Is Vertical Tab In Index [");
			printf ("%u.\n", analysis [8][1][counter]);
		}
		printf ("\n");

		printf ("\t\t\t\t======================================================================================================================\n\n");

		printf ("The Occurences Of Characters In The String.\n");
		for (unsigned int counter = 0; counter < analysis [0][0][0]; ++counter) {
			printf ("\tThe Character: ");
			printf ("%c", analysis [13][0][counter]);
			printf (" Occured: ");
			if (analysis [8][0][counter] > 0) {
				printf ("%u Times. Which Appeared In The Following Addresses:\n", analysis [8][0][counter]);
				for (unsigned int inside = 0; inside < analysis [8][0][counter]; ++inside)
					printf ("\t%u", analysis [8][(1 + counter)][inside]);
			} else
				printf ("%u Times.\n", analysis [8][0][counter]);
		}
		printf ("\n");

		//The end of the function.
		printf ("\n\n");
		SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 15);
		#ifdef __WIN32
		system ("pause");
		system ("cls");
		#endif

		#ifdef __linux__
		system ("read -p \"Press Enter To Continue...\"");
		system ("clear");
		#endif

		return 1;
	} else
		return 0;
}


/*
INPUT: This  function take a pointer to string and unsigned int which is the index where a character should be removed. The string must contain data.
OUTPUT: Return 1 if the character is removed from the string otherwise return 0.
INFO: This function is used to remove a character at specific index. The index must be between 0 and (length - 1).
*/
_Bool remove_at (string * _string, unsigned int c) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (c <= (_string_size - 1)) {
				string temp = NULL;
				do {temp = (string) malloc (sizeof (char) * (_string_size));} while (temp == NULL);
				unsigned int index = 0;
				for (unsigned int counter = 0; counter < _string_size; ++counter)
					if (c != counter)
						temp [index++] = (*_string) [counter];

				temp [(_string_size - 1)] = '\0';

				free ((*_string));
				(*_string) = NULL;
				(*_string) = temp;

				return 1;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to a string, which contain the main string. And a pointer to string which is used to be checked in the main string.
OUTPUT: Return 1 if the string contain the checker otherwise return 0.
INFO: This functions is used to check if the checker does exist in the main string.
*/
_Bool contains (string * _string, string * checker) {
	if (_string != NULL && checker != NULL) {
		if ((*_string) != NULL && (*checker)) {
			unsigned int _string_size = str_len (_string);
			unsigned int checker_size = str_len (checker);

			if (_string_size > 0 && checker_size > 0 && checker_size <= (_string_size - 1)) {
				string temp = NULL;
				do {temp = (string) malloc (sizeof (char) * (checker_size + 1));} while (temp == NULL);
				temp [checker_size] = '\0';
				
				_Bool it_exists = 0;
				for (unsigned int counter = 0; counter < _string_size; ++counter) {
					temp [(counter % checker_size)] = (*_string) [counter];
					
					if ((counter + 1) % checker_size == 0) {
						if (compare_string (checker, temp)) {
							it_exists = 1;
							break;
						}
					}

				}

				free (temp);
				temp = NULL;

				return it_exists;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to string which contain the data and string which we will check.
OUTPUT: Return 1 if the string end with end otherwise return 0.
INFO: This function is used to check if the string end with character.
*/
_Bool does_it_end_with (string * _string, string end) {
	if (_string != NULL && end != NULL) {
		if ((*_string) != NULL) {
			unsigned int end_size = str_len (&end);
			unsigned int _string_size = str_len (_string);
			
			if (end_size <= _string_size) {
				string temp = NULL;
				do {temp = (string) malloc (sizeof (char) * (end_size + 1));} while (temp == NULL);
				temp [end_size] = '\0';
				for (unsigned int counter = end_size; counter > 0; temp [(end_size - counter)] = (*_string) [(_string_size - counter)], --counter);

				_Bool _return = compare_string (&temp, end);
				free (temp);
				temp = NULL;

				return _return;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to string which contain the main data and string which is used to compare against the main.
OUTPUT: Return 1 if the string are the same otherwise return 0.
INFO: This function is used to compare strings.
*/
_Bool compare_string (string * main, string secondary) {
	if (main != NULL && secondary != NULL) {
		if ((*main) != NULL) {
			unsigned int main_size = str_len (main);
			unsigned int secondary_size = str_len (&secondary);

			if (main_size > 0 && secondary_size > 0 && main_size == secondary_size) {
				for (unsigned int counter = 0; counter <  main_size; ++counter)
					if ((*main) [counter] != secondary [counter])
						return 0;
				
				return 1;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to a string which hold the main string, and a string which hold the string which be tesed if it in the beginning.
OUTPUT: Return 1 if the string with the begin string. Otherwise return 0.
INFO: This function is used to check if the string begin with the a certain set of characters.
*/
_Bool does_it_begin_with (string * _string, string begin) {
	if (_string != NULL && begin != NULL) {
		if ((*_string) != NULL) {
			unsigned int begin_size = str_len (&begin);
			unsigned int _string_size= str_len (_string);

			if (begin_size > 0 && _string_size > 0 && begin_size <= _string_size) {
				string temp = NULL;
				do {temp = (string) malloc (sizeof (char) * (begin_size + 1));} while (temp == NULL);
				temp [begin_size] = '\0';
				for (unsigned int counter = 0; counter < begin_size; temp [counter] = (*_string) [counter], ++counter);

				_Bool _return = compare_string (&temp, begin);
				free (temp);
				temp = NULL;

				return _return;	
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to a string which hold the value and a pointer to a string which hold the type of the string.
OUTPUT: Return 1 if the value match the type, otherwise return 0.
INFO: This function is used to check if the value match the type.

NOTE: We for know use sshi-short-short int - ushi - sint - uint - slli - ulli - string - dble - float - char - _Bool
	for numbers we will use the functions like is_sshi
	for string we check if the value begin with " and end with ";
	for double and float we use is_double function.
	for char we check if the string begin with ' and end with ';
	for _Bool check if the string contain either 0 or 1.

Types Value:
	void = 0
	char - scar = 1
	unsigned char - ucar = 29
	_Bool - bool = 2
	sshi - short - short int = 3
	ushi - unsigned short int = 4
	sint - int = 5
	uint - unsigned int = 6
	slli - long long - long long int = 7
	ulli - unsigned long long int = 8
	string - strn = 9
	float = 10
	dble - double = 11
	union = 12
	struct = 13
	bt = 14
	bst = 15
	list = 16
	tree = 17
	hash = 18
	stack = 19
	queue = 20
	llist = 21
	lstack = 22
	lqueue = 23
	fllist = 24
	dllist = 25
	cllist = 26
	fcllist = 27
	dcllist = 28
*/
_Bool does_value_match_type (string * value, string * type) {
	if (value != NULL && type != NULL) {
		if ((*value) != NULL && (*type) != NULL) {
			unsigned int type_size = str_len (type);
			unsigned int value_size = str_len (value);

			if (type_size > 0 && value_size > 0) {
				if (is_type (type)) {
					type_size = get_type_code (type);

					if ((type_size >= 1 && type_size <= 11) || type_size == 29) {
						if (type_size == 1) {
							if (is_scar (value))
								return 1;
							else if ((*value) [0] != '\'' || !is_char_alpnum ((*value) [1]) || (*value) [2] != '\'' || value_size != 3)
								return 0;
						} else if (type_size == 2) {
							if (value_size != 1 && ((*value) [0] != '0' || (*value) [0] != '1'))
								return 0;
						} else if (type_size == 3) {
							if (!is_sshi (value))
								return 0;
						} else if (type_size == 4) {
							if (!is_ushi (value))
								return 0;
						} else if (type_size == 5) {
							if (!is_sint (value))
								return 0;
						} else if (type_size == 6) {
							if (!is_uint (value))
								return 0;
						} else if (type_size == 7) {
							if (!is_slli (value))
								return 0;
						} else if (type_size == 8) {
							if (!is_ulli (value))
								return 0;
						} else if (type_size == 9) {
							if ((*value) [0] != '\"' && !does_it_end_with (value, "\";"))
								return 0;
						} else if (type_size == 10 || type_size == 11) {
							if (!is_double (value))
								return 0;
						} else if (type_size == 29) {
							if (is_ucar (value))
								return 1;
							else if ((*value) [0] != '\'' || !is_char_alpnum ((*value) [1]) || (*value) [2] != '\''|| value_size != 3)
								return 0;
						} else
							return 0;
					} else
						return 0;

					return 1;
				} else
					return 0;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to a string which hold the main string and a character type which hold the character we check for continues repeat.
OUTPUT: Return 1 if the we removed repeated character or there was no repeatative, otherwise return 0.
INFO: This function is used to check the repeatative of a specfic character in a string and remove them.
	This function is used at the beginning of the str_char_split. To remove redundent spliter.
*/
_Bool remove_repeated_char (string * _string, char spliter) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size > 0) {
				_Bool _1 = 0;
				unsigned int _ = 0;
				unsigned int amount = 0;

				//1   2
				for (unsigned int counter = 0; counter < _string_size; ++counter) {
					if (!_1) {
						if ((*_string) [counter] == spliter) {
							_1 = 1;
							_ = counter;
						}
					} else {
						if (counter == (_ + 1) && (*_string) [counter] == spliter) {
							++amount;
							_ = counter;
							(*_string) [counter] = '~';
						} else
							_1 = 0;
					}
				}

				if (amount > 0) {
					string temp = NULL;
					
					//1  2
					do {temp = (string) malloc (sizeof (char) * (_string_size - amount + 1));} while (temp == NULL);
					temp [(_string_size - amount)] = '\0';

					_ = 0;
					for (unsigned int counter = 0; counter < _string_size; ++counter)
						if ((*_string) [counter] != '~')
							temp [_++] = (*_string) [counter];
					
					free ((*_string));
					(*_string) = NULL;

					(*_string) = temp;
					temp = NULL;
				}

				return 1;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to a string which hold the main string and a character type which hold the STRING we check for continues repeat.
OUTPUT: Return 1 if the we removed repeated string or there was no repeatative, otherwise return 0.
INFO: This function is used to check the repeatative of a specfic character in a string and remove them.
	String must be at least of size 2.
	This function is used at the beginning of str_word_split to remove redundent spliter.
*/
_Bool remove_repeated_word (string * _string, string spliter) {
	if (_string != NULL && spliter != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);
			unsigned int spliter_size = str_len (&spliter);

			if (_string_size > 0 && spliter_size >= 2) {
				unsigned int * indices = indices_of_word (_string, spliter);

				if (indices != NULL) {

					if ((indices [0] / 2) != 1) {
						//1  2  3  .
						//[8-1-2-4-5-7-8-9-10]

						unsigned int _ = 0;
						spliter_size = 2;
						for (unsigned int counter = 3; counter < indices [0]; counter += 2)
							if ((indices [counter] - spliter_size) == indices [(counter - spliter_size)]) {
								for (unsigned int begin = indices [counter]; begin <= indices [(counter + 1)]; (*_string) [begin] = '~', ++begin);
								++_;
								spliter_size +=2;
							}
						

						string temp = NULL;
						do {temp = (string) malloc (sizeof (char) * (_string_size - (_ * str_len (&spliter)) + 1));} while (temp == NULL);
						temp [(_string_size - (_ * str_len (&spliter)))] = '\0';

						_ = 0;
						for (unsigned int counter = 0; counter < _string_size; ++counter)
							if ((*_string) [counter] != '~')
								temp [_++] = (*_string) [counter];
						
						free ((*_string));
						(*_string) = NULL;
						(*_string) = temp;
						temp = NULL;
					}

					free (indices);
					indices = NULL;

					return 1;
				} else
					return 0;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to string which contain the main string, A _Bool type which tells where to append the string [Front[1] - End[0]] and pointer to string which contain
	the string to appended to the main string.

OUTPUT: Return 1 if the sent string is appended to the main string. Otherwise return 0.
*/
_Bool append (string * _string, _Bool type, string * content) {
	if (_string != NULL && content != NULL) {
		if ((*_string) != NULL && (*content) != NULL) {
			unsigned int _string_size = str_len (_string);
			unsigned int content_size = str_len (content);

			if (_string > 0 && content > 0) {
				string temp = NULL;
				do {temp = (string) malloc (sizeof (char) * (_string_size + content_size + 1));} while (temp == NULL);
				temp [(_string_size + content_size)] = '\0';

				if (type) {
					for (unsigned int counter = 0; counter < content_size; temp [counter] = (*content) [counter], ++counter);
					for (unsigned int counter = 0; counter < _string_size; temp [(counter + content_size)] = (*_string) [counter], ++counter);
				} else {
					for (unsigned int counter = 0; counter < _string_size; temp [counter] = (*_string) [counter], ++counter);
					for (unsigned int counter = _string_size; counter < (_string_size + content_size); temp [counter] = (*content) [(counter - _string_size)], ++counter);
				}

				free ((*_string));
				(*_string) = NULL;

				(*_string) = temp;
				return 1;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to string which contains data, unsigned int which hold the index and char.
OUTPUT: Return 1 if the characters is inserted otherwise return 0.
INFO: This function is used to insert character at specific index.
*/
_Bool insert_at (string * _string, unsigned int index, char c) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);
			
			if (index <= (_string_size - 1)) {
				if (is_char_alpha (c) || is_char_number (c) || is_char_special (c) || is_char_white_space (c)) {
					(*_string) [index] = c;
					return 1;
				} else
					return 0;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to a 2d array of string, a pointer to an unsigned int which hold the length of the list and a string which hold the string which we want to checks its existance.
OUTPUT: Return 1 if the string is found, otherwise return 0.
INFO: This function is used to check if the a string exists in a list of string.
*/
_Bool is_str_in (string ** list, unsigned int * len, string str) {
	if (list != NULL && str != NULL) {
		if ((*list) != NULL) {
			unsigned int str_length = str_len (&str);

			if (str_length != 0) {
				for (unsigned int counter = 0; counter < (*len); ++counter)
					if (compare_string (&((*list) [counter]), str))
						return 1;
				
				return 0;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to a string which contain the string which contain the The List Of Strings. And Pointer to unsigned int.
OUTPUT: Return 1 if the split is de-allocated. Otherwise return 0.
INFO: This function is used to terminate the split.
*/
_Bool term_split (string ** split_content, unsigned int ** size) {
	if (split_content != NULL) {
		if ((*split_content) != NULL) {
			if (size != NULL) {
				for (unsigned int counter = 0; counter <= (*size) [0]; ++counter)
					if ((*split_content) [counter] != NULL) {
						free ((*split_content) [counter]);
						(*split_content) [counter] = NULL;
					}

				free ((*split_content));
				(*split_content) = NULL;
				free ((*size));
				(*size) = NULL;
			} else {
				if ((*split_content) [0] != NULL) {
					unsigned int split_size = split_len (&(*split_content) [0]);

					for (unsigned int counter = 0; counter < (split_size + 1); ++counter)
						if ((*split_content) [counter] != NULL) {
							free ((*split_content) [counter]);
							(*split_content) [counter] = NULL;
						}

					free ((*split_content));
					(*split_content) = NULL;
				} else
					return 0;
			}

			return 1;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer  to string which hold the main string, unsigned int which from it the word wil start and a string which is the word that will be inserted.
OUTPUT: Return 1 if the word is inserted. Otherwise return 0.
INFO: This function is used to insert a word at a specific index. The word must be at least greater than or equal 2.
*/
_Bool insert_word_at (string * _string, unsigned int index, string word) {
	if (_string != NULL && word != NULL) {
		if ((*_string) != NULL) {
			unsigned int word_size = str_len (&word);
			unsigned int _string_size = str_len (_string);

			if (word_size <= _string_size && word_size >= 2) {
				if (index <= (_string_size - 1)) {
					if ((index + word_size) > _string_size) {
						string temp = NULL;
						do {temp = (string) malloc (sizeof (char) * (_string_size + ((index + word_size) - _string_size) + 1));} while (temp == NULL);
						temp [(_string_size + ((index + word_size) - _string_size))] = '\0';
						
						for (unsigned int counter = 0; counter < index; temp [counter] = (*_string) [counter], ++counter);
						for (unsigned int counter = 0; counter < word_size; temp [(index + counter)] = word [counter], ++counter);

						free ((*_string));
						(*_string) = NULL;

						(*_string) = temp;
					} else
						for (unsigned int counter = index; counter < _string_size; (*_string) [counter] = word [counter], ++counter);

					return 1;
				} else
					return 0;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to string which contain the main string, unsigned int which is an index where the binding will start after it and a pointer to string which contain the string to be binded.

OUTPUT: Return 1 if the binding is successful. Otherwise return 0.
INFO: This function is used to bind a string to another string starting from a specific index. The binding does not overwride the data it put within the string.
*/
_Bool bind_at (string * _string, unsigned int index, string * to_be_bind) {
	if (_string != NULL && to_be_bind != NULL) {
		if ((*_string) != NULL && (*to_be_bind) != NULL) {
			unsigned int _string_size = str_len (_string);
			unsigned int to_be_bind_size = str_len (to_be_bind);

			if (_string_size > 0 && to_be_bind_size > 0 && index <= (_string_size - 1)) {
				string temp = NULL;
				do {temp = (string) malloc (sizeof (char) * (to_be_bind_size + _string_size + 1));} while (temp == NULL);
				temp [to_be_bind_size + _string_size] = '\0';

				//This Loop will copy the elements from the main string to the new string until the index.
				for (unsigned int counter = 0; counter <= index; temp [counter] = (*_string) [counter], ++counter);

				//This Loop will copy the elements from the to_be_bind string to the new string.
				for (unsigned int counter = 0; counter < to_be_bind_size; temp [(counter + index + 1)] = (*to_be_bind) [counter], ++counter);

				//This Loop will copy the rest of the of the elements from the main string to the new string.
				for (unsigned int counter = (index + 1); counter < _string_size; temp [(index + to_be_bind_size)] = (*_string) [counter], ++counter);

				free ((*_string));
				(*_string) = NULL;
				
				(*_string) = temp;
				return 1;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to string which contain the main string, an unsigned integer which contain the size of the a pointer to list of unsigned integer which contain the
	indices that is need to be changed.

OUTPUT: Return 1 if the at least one character is changes. Otherwise return 0.
INFO: This function is to convert lower case characters to upper case characters. Depending on the indices the user provides.
*/
_Bool to_upper (string * _string, unsigned int size, unsigned int * indices) {
	if (_string != NULL && indices != NULL && size != 0) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size != 0) {
				_Bool at_least_one = 0;

				for (unsigned int counter = 0; counter < size; ++counter)
					if (indices [counter] <= (_string_size - 1))
						if (is_char_alpha ((*_string) [indices [counter]]) && is_char_lower ((*_string) [indices [counter]])) {
							(*_string) [indices [counter]] = (*_string) [indices [counter]] - 32;
							at_least_one = 1;
						}

				return at_least_one;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to string which contain the main string, unsigned integer which contain the size of the list of unsigend integer which contains the indices that
	will be changed.

OUTPUT: Return 1 if at least one character changed from upper case to lower case.
INFO: This function is used to convert upper case characters to lower case characters.
*/
_Bool to_lower (string * _string, unsigned int size, unsigned int * indices) {
	if (_string != NULL && indices != NULL && size != 0) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size > 0) {
				_Bool at_least_one = 0;

				for (unsigned int counter = 0; counter < size; ++counter)
					if (indices [counter] <= (_string_size - 1))
						if (is_char_alpha ((*_string) [indices [counter]]) && is_char_upper ((*_string) [indices [counter]])) {
							(*_string) [indices [counter]] = (*_string) [indices [counter]] + 32;
							at_least_one = 1;
						}
				
				return at_least_one;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to a string which contains the main string, an unsigned integer which contains the size of the array of unsigned integer which contains the
	the indices where the characters from that string will be removed.

OUTPUT: Return 1 if at least one of the characters is removed otherwise return 0.
INFO: This function is used to removes set of characters provided by their indices.
*/
_Bool remove_at_these (string * _string, unsigned int chars_size, unsigned int * cs) {
	if (_string != NULL && chars_size != 0 && cs != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size != 0) {
				unsigned int real_size = 0;

				//used to check if actually the indices in the list is between 0 and string_size - 1.
				for (unsigned int counter = 0; counter < chars_size; ++counter)
					if (cs [counter] <= (_string_size - 1))
						cs [real_size++] = cs [counter];
				
				//If there are actually correct indices.
				if (real_size > 0) {
					string temp = NULL;
					do {temp = (string) malloc (sizeof (char) * (_string_size - real_size + 1));} while (temp == NULL);
					temp [(_string_size - real_size)] = '\0';

					for (unsigned int counter = 0; counter < real_size; (*_string) [cs [counter]] = 0, ++counter);

					real_size = 0;
					for (unsigned int counter = 0; counter < _string_size; ++counter)
						if ((*_string) [counter] != 0)
							temp [real_size++] = (*_string) [counter];
					
					free ((*_string));
					(*_string) = NULL;
					(*_string) = temp;
					return 1;	
				} else
					return 0;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to string which contain the main data, an array of unsigned integers where the first index is the size of the array and apointer to an array of strings
	which contains the word that will be added to that string.

OUTPUT: Return 1 if at least one word is inserted otherwise return 0.
INFO: This function is used to insert a list of word into the main string and the index of the insertion provided from the array of the unsigned integers.
*/
_Bool insert_words_at_these (string * _string, unsigned int * indices, string ** words) {
	if (_string != NULL && words != NULL && indices != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size != 0) {
				_Bool at_least_one = 0;
				for (unsigned int counter = 0; counter < indices [0]; ++counter)
					if (indices [(1 + counter)] <= (_string_size - 1) && (*words) [counter] != NULL)
						if (insert_word_at (_string, indices [(1 + counter)], (*words) [counter]))
							at_least_one = 1;

				return at_least_one;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to a string, an unsigned integer which represent the index where the word start and end index of the word.
OUTPUT: Return 1 if the word is removed otherwise return 0.
INFO: This function is used to remove a word from the existing string, by providing the beginning index of the word and the end index of the word.
*/
_Bool remove_word_at (string * _string, unsigned int begin_index, unsigned int end_index) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size != 0) {
				if (begin_index <= (_string_size - 1) && end_index <= (_string_size - 1) && begin_index <= end_index) {
					string temp = NULL;
					do {temp = (string) malloc (sizeof (char) * (_string_size - (end_index - begin_index + 1) + 1));} while (temp == NULL);
					temp [(_string_size - (end_index - begin_index + 1))] = '\0';

					if ((_string_size - (end_index - begin_index + 1)) == 0) { //This means the user deleted/removed the whole string.
						free ((*_string));
						(*_string) = NULL;
						free (temp);
						temp = NULL;
						return 1;
					} else {
						unsigned int index = 0;
						//ABC 1 2 => A
						for (unsigned int counter = 0; counter < begin_index; temp [index] = (*_string) [counter], ++index, ++counter);
						for (unsigned int counter = (end_index + 1); counter < _string_size; temp [index] = (*_string) [counter], ++index, ++counter);
						free ((*_string));
						(*_string) = NULL;
						(*_string) = temp;
						temp = NULL;

						return 1;
					}
				} else
					return 0;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to string which contain the main string, a _Bool type which tells us where the appending will happen, unsigned int which tells us how amount of characters will be appended and a pointer to string which contain the string to be appended into string.

OUTPUT: Return 1 if the appending is happended. Otherwise return 0.
INFO: This function is used to append a number of characters from the string the user send along with the main the string.
*/
_Bool n_append (string * _string, _Bool type, unsigned int amount_to_append, string * content) {
	if (_string != NULL && content != NULL && amount_to_append != 0) {
		if ((*_string) != NULL && (*content) != NULL) {
			unsigned int _string_size = str_len (_string);
			unsigned int content_size = str_len (content);

			if (_string_size != 0 && content_size != 0 && amount_to_append <= content_size) {
				string temp = NULL;
				do {temp = (string) malloc (sizeof (char) * (_string_size + amount_to_append + 1));} while (temp == NULL);
				temp [(_string_size + amount_to_append)] = '\0';

				if (type) {
					for (unsigned int counter = 0; counter < amount_to_append; temp [counter] = (*content) [counter], ++counter);
					for (unsigned int counter = 0; counter < _string_size; temp [(amount_to_append + counter)] = (*_string) [counter], ++counter);
				} else {
					for (unsigned int counter = 0; counter < _string_size; temp [counter] = (*_string) [counter], ++counter);
					for (unsigned int counter = 0; counter < amount_to_append; temp [(_string_size + counter)] = (*content) [counter], ++counter);
				}
				
				free ((*_string));
				(*_string) = NULL;
				(*_string) = temp;
				temp = NULL;

				return 1;
			} else
				return 0;
		} else
			return 0;
	} else
	       return 0;	
}


/*
INPUT: A pointer to string which is the main string, unsigned int which is the size of the array of unsigned int and string which contains the character to be inserted to the main string.

OUTPUT: Return 1 if at least one character is inserted into the main string, otherwise return 0.
INFO: This function is used to insert multiple characters into the main string.
*/
_Bool insert_at_these (string * _string, unsigned int size, unsigned int * indices, string cs) {
	if (_string != NULL && cs != NULL && indices != NULL) {
		if ((*_string) != NULL) {
			if (size != 0) {
				unsigned int cs_size = str_len (&cs);
				unsigned int _string_size = str_len (_string);

				if (cs_size > 0 && _string_size > 0) {
					_Bool at_least_one = 0;
					for (unsigned int counter = 0; counter < size; ++counter)
						if (indices [counter] <= (cs_size - 1) && indices [counter] <= (_string_size - 1)) {
							at_least_one = 1;
							(*_string) [indices [counter]] = cs [indices [counter]];
						}

					return at_least_one;
				} else
					return 0;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to string which contains the main string, an unsigned integer which is the size of an array of _Bool and the size of the a pointer to an array of strings.

OUTPUT: Return 1 is at least one thing is appended to the main string, otherwise return 0.
INFO: This function is used to append multiple strings to the main string.
*/
_Bool append_these (string * _string, unsigned int how_many, _Bool * type, string ** to_append) {
	if (_string != NULL && type != NULL && to_append != NULL) {
		if ((*_string) != NULL && (*to_append) != NULL && how_many > 0) {
			unsigned int _string_size = str_len (_string);

			if (_string_size > 0) {
				_Bool at_least_one = 0;
				for (unsigned int counter = 0; counter < how_many; ++counter)
					if ((*to_append) [counter] != NULL)
						if (append (_string, type [counter], &(*to_append) [counter]))
							at_least_one = 1;

				return at_least_one;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to string which contains the main string, an unsigned int that contain the size of the array of strings and a pointer to the an array of strings.
OUTPUT: Return 1 if at least one checker is in the main string, otherwise return 0.
INFO: This function checks if at least one of the following of the checkers do exist.
*/
_Bool contains_the_following (string * _string, unsigned int number_of_things, string ** checkers) {
	if (_string != NULL && checkers != NULL) {
		if ((*_string) != NULL && (*checkers) != NULL && number_of_things > 0) {
			_Bool at_least_one = 0;
			for (unsigned int counter = 0; counter < number_of_things; ++counter)
				if ((*checkers) [counter] != NULL)
					if (contains (_string, &(*checkers) [counter]))
						at_least_one = 1;

			return at_least_one;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to a string which contains the main string, an unsigned integer which hold the index where binding will happen, an unsigned integer which hold the amount of characters to be binded in from the pointer to a string.

OUTPUT: Return 1 if the binding is happend otherwise return 0.
INFO: This function is used to bind a set number of characters to the main string.
*/
_Bool n_bind_at (string * _string, unsigned int index, unsigned int amount_to_bind, string * to_be_bind) {
	if (_string != NULL && amount_to_bind > 0 && to_be_bind != NULL) {
		if ((*_string) != NULL && (*to_be_bind) != NULL) {
			unsigned int _string_size = str_len (_string);
			unsigned int _to_be_bind_size = str_len (to_be_bind);

			if (_string_size > 0 && _to_be_bind_size > 0 && index <= (_string_size - 1) && amount_to_bind <= _to_be_bind_size) {
				string temp = NULL;
				do {temp = (string) malloc (sizeof (char) * (_string_size + _to_be_bind_size + 1));} while (temp == NULL);
				temp [(_string_size + _to_be_bind_size)] = '\0';

				//To fill the string before the index.
				for (unsigned int counter = 0; counter <= index; temp [counter] = (*_string) [counter], ++counter);

				//To fill the right amount of characters from to_be_bind variable.
				for (unsigned int counter = 0; counter < amount_to_bind; temp [(counter + index + 1)] = (*to_be_bind) [counter], ++counter);

				//To fill the rest of the main string
				for (unsigned int counter = (index + 1); counter < _string_size; temp [(amount_to_bind + counter)] = (*_string) [counter], ++counter);

				free ((*_string));
				(*_string) = NULL;
				(*_string) = temp;
				temp = NULL;

				return 1;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pounter to string which hold the main string, an unsigned integer which hold the size of the following, an array of an unsigned integer of begin index and an array of unsgined integer of the end index.

OUTPUT: Return 1 if at least one word is removed, otherwise return 0.
INFO: This function is used to removed a set of word from the main string.
*/
_Bool remove_words_at_these (string * _string, unsigned int size, unsigned int * begin, unsigned int * end) {
	if (_string != NULL && begin != NULL && end != NULL && size > 0) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size > 0) {
				_Bool at_least_one = 0;
				for (unsigned int counter = 0; counter < size; ++counter)
					if (remove_word_at (_string, begin [counter], end [counter]))
						at_least_one = 1;
				
				return at_least_one;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to a string which contain the main string, string which hold the string that we will compare against, an unsigned integer which hold the starting index for the main string, an unsigned integer which hold the starting index for the second string and unsigned integer which is amount of characters will be used to compare.

OUTPUT: Return 1 if the comparing is true otherwise return 0.
INFO: This function is used to compare a specfic set of numbers against the main string from a specfic starting point.
*/
_Bool n_compare_string (string * main, string secondary, unsigned int main_start_from, unsigned int secondary_start_from, unsigned int amount) {
	if (main != NULL && secondary != NULL && amount > 0) {
		if ((*main) != NULL) {
			unsigned int main_size = str_len (main);
			unsigned int secondary_size = str_len (&secondary);

			if (main_size > 0 && secondary_size > 0 && amount <= main_size && amount <= secondary_size  && main_start_from < (main_size - 1) && secondary_start_from < (secondary_size - 1)) {
				string temp_main = NULL;
				string temp_secn = NULL;

				do {temp_main = (string) malloc (sizeof (char) * (amount + 1));} while (temp_main == NULL);
				do {temp_secn = (string) malloc (sizeof (char) * (amount + 1));} while (temp_secn == NULL);

				for (unsigned int counter = 0; counter < amount; temp_main [counter] = (*main) [(main_start_from + counter)], ++counter);
				for (unsigned int counter = 0; counter < amount; temp_secn [counter] = secondary [(secondary_start_from + counter)], ++counter);

				for (unsigned int counter = 0; counter < amount; ++counter)
					if (temp_main [counter] != temp_secn [counter]) {
						free (temp_main);
						temp_main = NULL;
						free (temp_secn);
						temp_secn = NULL;
						return 0;
					}
				
				free (temp_main);
				temp_main = NULL;
				free (temp_secn);
				temp_secn = NULL;

				return 1;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


//Function that return char datatype.
/*
INPUT: A character. That must be of lower case characters.
OUTPUT: Return 0 if the input not met otherwise return the upper case of the lower case character.
INFO: Convert lower case character to upper case character. 
*/
char char_to_upper (char c) {
	if (is_char_lower (c))
		return (c - 32);
	else
		return 0;
}


/*
INPUT: A character must be of upper case characters.
OUTPUT: Return 0 if the input is not met otherwise return the lower case charater of the upper case character.
INFO: Convert upper case character to lower case character.
*/
char char_to_lower (char c) {
	if (is_char_upper (c))
		return (c + 32);
	else
		return 0;
}


/*
INPUT: A pointer to a string which hold the main string.
OUTPUT: Return char value, otherwise return 0.
INFO: This function is used to convert the string into char value.
*/
char to_scar (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			if (is_scar (_string)) {
				unsigned char counter = 0;
				char value = 0;

				if ((*_string) [0] == '-')
					counter = 1;

				for (; (*_string) [counter] != '\0'; ++counter)
					value = (value * 10) + ((*_string) [counter] - 48);

				if ((*_string) [0] == '-')
					value *= -1;
				
				return value;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to a string which hold the main string and should represent a type.
OUTPUT: Return a char value represent a type, otherwise return -1.
INFO: This function is used to return the code for the type.
Types:
	bt
	bst
	int
	void
	sshi
	ushi
	sint
	uint
	slli
	ulli
	strn
	dble
	scar
	ucar
	char
	list
	tree
	hash
	_Bool
	stack
	queue
	union
	llist
	float
	short
	double
	lstack
	lqueue
	fllist
	string
	struct
	dllist
	cllist
	fcllist
	dcllist
	short int
	long long
	unsigned int
	unsigned char
	long long int
	unsigned short int
	unsigned long long int
	signed char
	signed int
	signed short
	signed short int
	signed long long
	signed long long int
	
Types Value:
	void = 0
	char - scar - signed char = 1
	ucar - unsigned char = 29
	_Bool - bool = 2
	sshi - short - short int - signed short - signed short int = 3
	ushi - unsigned short int = 4
	sint - int - signed int = 5
	uint - unsigned int = 6
	slli - long long - long long int - signed long long - signed long long int = 7
	ulli - unsigned long long int = 8
	string - strn = 9
	float = 10
	dble - double = 11
	union = 12
	struct = 13
	bt = 14
	bst = 15
	list = 16
	tree = 17
	hash = 18
	stack = 19
	queue = 20
	llist = 21
	lstack = 22
	lqueue = 23
	fllist = 24
	dllist = 25
	cllist = 26
	fcllist = 27
	dcllist = 28
*/
char get_type_code (string * type) {
	if (type != NULL) {
		if ((*type)  != NULL) {
			unsigned int type_size = str_len (type);

			if (type_size > 1) {
				if (is_type (type)) {
					_Bool it_is = 0;
					char counter = 0;
					for (; counter < 49; ++counter) {
						switch (counter) {
							case 0: if (compare_string (type, "bt")) {it_is = 1; counter = 14;} break;
							case 1: if (compare_string (type, "bst")) {it_is = 1; counter = 15;} break;
							case 2: if (compare_string (type, "int")) {it_is = 1; counter = 5;} break;
							case 3: if (compare_string (type, "void")) {it_is = 1; counter = 0;} break;
							case 4: if (compare_string (type, "sshi")) {it_is = 1; counter = 3;} break;
							case 5: if (compare_string (type, "ushi")) {it_is = 1; counter = 4;} break;
							case 6: if (compare_string (type, "sint")) {it_is = 1; counter = 5;} break;
							case 7: if (compare_string (type, "uint")) {it_is = 1; counter = 6;} break;
							case 8: if (compare_string (type, "slli")) {it_is = 1; counter = 7;} break;
							case 9: if (compare_string (type, "ulli")) {it_is = 1; counter = 8;} break;
							case 10: if (compare_string (type, "strn")) {it_is = 1; counter = 9;} break;
							case 11: if (compare_string (type, "bool")) {it_is = 1; counter = 2;} break;
							case 12: if (compare_string (type, "dble")) {it_is = 1; counter = 11;} break;
							case 13: if (compare_string (type, "scar")) {it_is = 1; counter = 1;} break;
							case 14: if (compare_string (type, "ucar")) {it_is = 1; counter = 29;} break;
							case 15: if (compare_string (type, "char")) {it_is = 1; counter = 1;} break;
							case 16: if (compare_string (type, "list")) {it_is = 1; counter = 16;} break;
							case 17: if (compare_string (type, "tree")) {it_is = 1; counter = 17;} break;
							case 18: if (compare_string (type, "hash")) {it_is = 1; counter = 18;} break;
							case 19: if (compare_string (type, "_Bool")) {it_is = 1; counter = 2;} break;
							case 20: if (compare_string (type, "stack")) {it_is = 1; counter = 19;} break;
							case 21: if (compare_string (type, "queue")) {it_is = 1; counter = 20;} break;
							case 22: if (compare_string (type, "union")) {it_is = 1; counter = 12;} break;
							case 23: if (compare_string (type, "llist")) {it_is = 1; counter = 21;} break;
							case 24: if (compare_string (type, "float")) {it_is = 1; counter = 10;} break;
							case 25: if (compare_string (type, "short")) {it_is = 1; counter = 3;} break;
							case 26: if (compare_string (type, "double")) {it_is = 1; counter = 11;} break;
							case 27: if (compare_string (type, "lstack")) {it_is = 1; counter = 22;} break;
							case 28: if (compare_string (type, "lqueue")) {it_is = 1; counter = 23;} break;
							case 29: if (compare_string (type, "fllist")) {it_is = 1; counter = 24;} break;
							case 30: if (compare_string (type, "string")) {it_is = 1; counter = 9;} break;
							case 31: if (compare_string (type, "struct")) {it_is = 1; counter = 13;} break;
							case 32: if (compare_string (type, "dllist")) {it_is = 1; counter = 25;} break;
							case 33: if (compare_string (type, "cllist")) {it_is = 1; counter = 26;} break;
							case 34: if (compare_string (type, "fcllist")) {it_is = 1; counter = 27;} break;
							case 35: if (compare_string (type, "dcllist")) {it_is = 1; counter = 28;} break;
							case 36: if (compare_string (type, "short int")) {it_is = 1; counter = 3;} break;
							case 37: if (compare_string (type, "long long")) {it_is = 1; counter = 7;} break;
							case 38: if (compare_string (type, "unsigned int")) {it_is = 1; counter = 6;} break;
							case 39: if (compare_string (type, "unsigned char")) {it_is = 1; counter = 29;} break;
							case 40: if (compare_string (type, "long long int")) {it_is = 1; counter = 7;} break;
							case 41: if (compare_string (type, "unsigned short int")) {it_is = 1; counter = 4;} break;
							case 42: if (compare_string (type, "unsigned long long int")) {it_is = 1; counter = 8;} break;
							case 43: if (compare_string (type, "signed char")) {it_is = 1; counter = 1;} break;
							case 44: if (compare_string (type, "signed short")) {it_is = 1; counter = 3;} break;
							case 45: if (compare_string (type, "signed short int")) {it_is = 1; counter = 3;} break;
							case 46: if (compare_string (type, "signed int")) {it_is = 1; counter = 5;} break;
							case 47: if (compare_string (type, "signed long long")) {it_is = 1; counter = 7;} break;
							case 48: if (compare_string (type, "signed long long int")) {it_is = 1; counter = 7;} break;
							default: break;
						}

						if (it_is)
							break;
					}

					return counter;
				} else
					return -1;
			} else
				return -1;
		} else
			return -1;
	} else
		return -1;
}


/*
INPUT: A pointer to string which hold the main string.
OUTPUT: Return 1, if the string represent a union or return 2, if the string represent a struct, otherwise return 0.
INFO: This function is used to check if the string can represent a union or a struct.

	Example on struct:
		struct: type constraint, type constraint.
			types that are available are: sshi, ushi, sint, uint, slli, ulli, dble and string
			constrains that are aviable are the type in the function is_constraint take.

			struct: sshi 0, sint 1^40;

	The same goes for the union.

NOTE: We support up to 4 dimension array.
*/
char is_union_struct (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size != 0) {
				string temp = ";";

				if ((*_string) [(_string_size - 1)] == ';') {
					remove_at (_string, (_string_size - 1));
					_Bool is_struct = 0;

					void *** content = str_char_split (_string, ':');

					if (content != NULL) {
						string * split_content = ((string *) content [1]);
						unsigned int * split_size = ((unsigned int *) content [2]);

						content [1] = NULL;
						content [0][0] = NULL;
						free (content);
						content = NULL;

						if ((*split_size) == 2) {
							if (compare_string (&split_content [1], "struct"))
								is_struct = 1;
							else if (!compare_string (&split_content [1], "union")) {
								append (_string, 0, &temp);
								_string_size = 0;
								for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
								free (split_content);
								split_content = NULL;
								free (split_size);
								split_size = NULL;

								return 0;
							}

							
							content = str_char_split (&split_content [2], ',');
							
							if (content != NULL) {
								_string_size = 0;
								for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
								free (split_content);
								split_content = NULL;
								free (split_size);
								split_size = NULL;

								split_content = ((string *) content [1]);
								split_size = ((unsigned int *) content [0][0]);

								_string_size = 1;
								_Bool all_correct = 1;
								for (; _string_size <= (*split_size); ++_string_size) {
									if (split_content [_string_size][0] == ' ')
										remove_at (&split_content [_string_size], 0);

									if (!check_experssion (&split_content [_string_size])) {
										all_correct = 0;
										break;
									}
								}

								append (_string, 0, &temp);
								_string_size = 0;
								for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
								free (split_content);
								split_content = NULL;
								free (split_size);
								split_size = NULL;

								
								if (all_correct) {
									if (is_struct)
										return 2;
									else
										return 1;
								} else
									return 0;
							} else {
								append (_string, 0, &temp);
								_string_size = 0;
								for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
								free (split_content);
								split_content = NULL;
								free (split_size);
								split_size = NULL;

								return 0;
							}
						} else {
							append (_string, 0, &temp);
							_string_size = 0;
							for (; _string_size <= (*split_size); free (split_content [_string_size]), split_content [_string_size] = NULL, ++_string_size);
							free (split_content);
							split_content = NULL;
							free (split_size);
							split_size = NULL;

							return 0;
						}
					} else
						return 0;
				} else
					return 0;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to string and index of type unsigned int. The string must contain info and the index sent
	is between the 0 and length of the string - 1.

OUTPUT: Return 0 if the input is not met. Otherwise return the character.

INFO: This function is used to return a character at specific index from the string.
*/
char char_at (string * _string, unsigned int index) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			if (index <= (str_len (_string) - 1))
				return (*_string) [index];
			else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


//Function that return unsigned char
/*
INPUT: A pointer to a string which hold the main string.
OUTPUT: Return an unsigned char value, otherwise return 0.
INFO: This function is used to convert the string into unsigned char.
*/
unsigned char to_ucar (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			if (is_ucar (_string)) {
				unsigned char number = 0;

				for (unsigned char counter = 0; (*_string) [counter] != '\0'; ++counter)
					number = (number * 10) + ((*_string) [counter] - 48);

				return number;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}

//Function that return string or string * value.
/*
INPUT: A pointer to string which hold the main string.
OUTPUT: Return a string if the input is not null otherwise return null.
INFO: This function is used to return a deep copy of the string.
*/
string give_me_str_copy (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size > 0) {
				string copy = NULL;
				do {copy = (string) malloc (sizeof (char) * (_string_size + 1));} while (copy == NULL);
				copy [_string_size] = '\0';

				for (unsigned int counter = 0; counter < _string_size; copy [counter] = (*_string) [counter], ++counter);

				return copy;
			} else
				return NULL;
		} else
			return NULL;
	} else
		return NULL;
}


/*
INPUT: A pointer to a string which hold the main string and a character which represent the spliter.
OUTPUT: Return a string holding the first split content of the main string, Otherwise return NULL.
INFO: This function is used to return the first split data from the main string, give the spliter characters.

	Example: AB CD
		The return will be AB.
		The size of the string will be: The index of the first spliter character + 1. Which means in the above example: 2 + 1 = 3.
		where the 0 index will hold A, 1 index will hold B and 2 index will hold \0.
*/
string single_char_split (string * _string, char spliter) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size != 0) {
				_string_size = 0;
				for (unsigned int counter = 0; (*_string) [counter] != '\0'; ++counter)
					if ((*_string) [counter] == spliter) {
						_string_size = counter;
						break;
					}

				if (_string_size != 0) {
					string content = NULL;
					do {content = (string) malloc (sizeof (char) * (_string_size + 1));} while (content == NULL);
					content [_string_size] = '\0';

					for (unsigned int counter = 0; counter < _string_size; ++counter)
						content [counter] = (*_string) [counter];

					return content;
				} else
					return NULL;
			} else
				return NULL;
		} else
			return NULL;
	} else
		return NULL;
}


/*
INPUT: A pointer to a string which contains the main data and an array of indices where the first index contain the size of the array.
OUTPUT: Return string contains the characters at the indices that has been sent, otherwise return null.
INFO: This function is used to return a string which contain characters at a specific indices in the main string.
*/
string chars_at (string * _string, unsigned int * indices) {
	if (_string != NULL && indices != NULL) {
		if ((*_string) != NULL && indices [0] > 0) {
			unsigned int _string_size = str_len (_string);

			if (_string_size > 0) {
				unsigned int real_size = 0;
				for (unsigned int counter = 1; counter <= indices [0]; (indices [counter] > (_string_size - 1)) ? ++counter : real_size++, ++counter);
				
				if (real_size > 0) {
					string chars = NULL;
					do {chars = (string) malloc (sizeof (char) * (real_size + 1));} while (chars == NULL);
					chars [real_size] = '\0';

					real_size = 0;
					for (unsigned int counter = 1; counter <= indices [0]; (indices [counter] <= (_string_size - 1)) ? chars [real_size] = (*_string) [indices [counter]], ++real_size, ++counter : ++counter);

					return chars;
				} else
					return NULL;
			} else
				return NULL;
		} else
			return NULL;
	} else
		return NULL;
}


/*
INPUT: A pointer to a string which hold the main string and a string which hold the spliter which must be at least of size 2.
OUTPUT: Return a string holding the first split info index, otherwise return NULL.
INFO: This function is used to return the first split info index from the main string providing the spliter which is a string of at least size equal to 2.
*/
string single_word_split (string * _string, string spliter) {
	if (_string != NULL && spliter != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);
			unsigned int spliter_size = str_len (&spliter);

			if (spliter_size >= 2 && _string_size > 2) {
				unsigned int * index = index_of_word (_string, spliter);
				if (index [0] != 0) {
					spliter_size = index [0];
					
					free (index);
					index = NULL;

					string split = NULL;
					do {split = (string) malloc (sizeof (char) * (spliter_size + 1));} while (split == NULL);
					split [spliter_size] = '\0';

					_string_size = 0;
					for (; _string_size < spliter_size; split [_string_size] = (*_string) [_string_size], ++_string_size);

					return split;
				} else {
					free (index);
					index = NULL;

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
INPUT: A pointer to string which hold the main string, an unsigned integer which hold the starting index and an unsigned integer which hold the end index.
	The end index must be in form n-1. Which means if the input is: ABC and you want to return B, the begin should be 1 and end should be 1.
	And if i want BC the begin should be 1 and end should be 2. It is different from the implementation of the regural substring.

OUTPUT: Return the substring if all is correctly processed, otherwise return null.
INFO: This function is used to return a substring from the main string.
*/
string substring (string * _string, unsigned int begin, unsigned int end) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size > 0 && begin <= (_string_size - 1) && end <= (_string_size - 1)) {
				string sub = NULL;

				if ((end - begin) == 0) {
					do {sub = (string) malloc (sizeof (char) * 2);} while (sub == NULL);
					sub [0] = (*_string) [begin];
					sub [1] = '\0';
				} else {
					do {sub = (string) malloc (sizeof (char) * (end - begin + 2));} while (sub == NULL);
					sub [(end - begin + 1)] = '\0';

					for (unsigned int counter = 0; counter <= (end - begin); sub [counter] = (*_string) [(counter + begin)], ++counter); 
				}

				return sub;
			} else
				return NULL;
		} else
			return NULL;
	} else
		return NULL;
}


/*
INPUT: A pointer to string which hold the main string, an unsigned integer which hold how many substring you want and the size for the array of begins and array of ends which are indices that hold the beginning index and ending index respectivlly.

OUTPUT: Return an array of strings containing substring otherwise return null.
INFO: This function is used to return an array of substrings from the main string. You could use split_len function to get the size of the array of string.
	The real data starts from index 1 as split.
*/
string * substrings (string * _string, unsigned int n_subs, unsigned int * begins, unsigned int * ends) {
	if (_string != NULL && begins != NULL && ends != NULL) {
		if ((*_string) != NULL && n_subs > 0) {
			unsigned int _string_size = str_len (_string);

			if (_string_size > 0) {
				unsigned int real_size = 0;
				for (unsigned int counter = 0; counter < n_subs; ++counter) {
					if (begins [counter] <= (_string_size - 1) && ends [counter] <= (_string_size - 1) && begins [counter] <= ends [counter]) {
						++real_size;
					} else {
						begins [counter] = -1;
						ends [counter] = -1;
					}
				}

				if (real_size > 0) {
					string * subs = NULL;
					do {subs = (string *) malloc (sizeof (string) * (real_size + 1));} while (subs == NULL);
					generate_backup_split_size (&subs, real_size);

					real_size = 1;
					for (unsigned int counter = 0; counter < n_subs; ++counter)
						if (begins [counter] <= (_string_size - 1) && ends [counter] <= (_string_size - 1) && begins [counter] <= ends [counter]) {
							subs [real_size] = substring (_string, begins [counter], ends [counter]);
							++real_size;
						}

					return subs;
				} else
					return NULL;
			} else
				return NULL;
		} else
			return NULL;
	} else
		return NULL;
}


//Function that return unsingend int.
/*
INPUT: A pointer to string which hold the main string.
OUTPUT: Return an unsigned integer represent the number of characters. 0 means nothing is there.
INFO: This function is used to return the size of a string.
*/
unsigned int str_len (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int size = 0;

			for (unsigned int counter = 0; (*_string) [counter] != '\0'; ++size, ++counter);

			return size;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to string which hold the main string. The number in string format must not begin with zero.
OUTPUT: Return the represented number return 0xFFFFFFFF(UINT__ERROR) if string is not an unsigned integer.
INFO: This function is used to convert the string to unsigned int if possible. If the size is equal to UINT__ERROR (0xFFFFFFFF), return UINT__ERROR (0xFFFFFFFF).
*/
unsigned int to_uint (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size > 0) {
				if (is_uint (_string)) {
					_string_size = 0;
					for (unsigned int counter = 0; (*_string) [counter] != '\0'; ++counter)
						_string_size = (10 * _string_size) + ((*_string) [counter] - 48);

					if (_string_size != UINT__ERROR)
						return _string_size;
					else
						return UINT__ERROR;
				} else
					return UINT__ERROR;
			} else
				return UINT__ERROR;
		} else
			return UINT__ERROR;
	} else
		return UINT__ERROR;
}


/*
INPUT: A pointer to the first index of a split or the return from the substrings function.
OUTPUT: Return the size of the split or the size of the return from substrings function, otherwise return 0.
INFO: This function is used to return the size of the split or the return from substrings function. The actual data of split and the return from substrings function
	starts from index 1 to size: e.g if the split size = 3, then you go from 1 to 3. The condition will be i <= split_len ()
*/
unsigned int split_len (string * first) {
	if (first != NULL) {
		if ((*first) != NULL) {
			unsigned int split_size = 0;
			for (unsigned int counter = 0; (*first) [counter] != '\0'; split_size += (*first) [counter], ++counter);
			
			return split_size;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to string which hold the main string.
OUTPUT: An unsigned integer value which represent the amount of alphabitcal characters in the string, otherwise return 0xFFFFFFFF which is the maximum value of unsigned integer.

INFO: This function is used to count how many alphabitcal characters in the string and return its count, otherwise return 0xFFFFFFFF which is the maximum value of unsigned integer.
*/
unsigned int n_alphas (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size > 0) {
				unsigned int alphas = 0;
				for (unsigned int counter = 0; counter < _string_size; (is_char_alpha ((*_string) [counter]) != 0) ? ++alphas, ++counter : ++counter);
				return alphas;
			} else
				return UINT__ERROR;
		} else
			return UINT__ERROR;
	} else
		return UINT__ERROR;
}


/*
INPUT: A pointer to string which hold the main string.
OUTPUT: An unsigned integer which hold the count of lower case characters, otherwise return 0xFFFFFFFF which hold the maximum value of unsigned integer.
INFO: This function is used to return the count of lower case characters.
*/
unsigned int n_Lcases (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size > 0) {
				unsigned int lcases = 0;
				for (unsigned int counter = 0; counter < _string_size; (is_char_lower ((*_string) [counter]) != 0) ? ++lcases, ++counter : ++counter);
				return lcases;
			} else
				return UINT__ERROR;
		} else
			return UINT__ERROR;
	} else
		return UINT__ERROR;
}


/*
INPUT: A pointer to string which hold the main string.
OUTPUT: An unsigned int value which hold the number of upper case characters. Otherwise return 0xFFFFFFFF.
INFO: This function is used to count the number of upper case characters.
*/
unsigned int n_Ucases (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);
			
			if (_string_size > 0) {
				unsigned int ucases = 0;
				for (unsigned int counter = 0; counter < _string_size; (is_char_upper ((*_string) [counter]) != 0) ? ++ucases, ++counter : ++counter);
				return ucases;
			} else
				return UINT__ERROR;
		} else
			return UINT__ERROR;
	} else
		return UINT__ERROR;
}


/*
INPUT: A pointer to string which hold the main string.
OUTPUT: An unsigned integer value which hold the amount of spaces in the string, otherwise return 0xFFFFFFFF.
INFO: This function is used to return the number spaces in the string.
*/
unsigned int n_spaces (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size > 0) {
				unsigned int spaces = 0;
				for (unsigned int counter = 0; counter < _string_size; (is_space ((*_string) [counter]) != 0) ? ++spaces, ++counter : ++counter);
				return spaces;
			} else
				return UINT__ERROR;
		} else
			return UINT__ERROR;
	} else
		return UINT__ERROR;
}


/*
INPUT: A pointer to string which hold the main string.
OUTPUT: An unsigned integer which hold the amount of vertical spaces exists in the main string. Otherwise return 0xFFFFFFFF.
INFO: This function is used to count the number of vertical spaces.
*/
unsigned int n_Vspaces (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size > 0) {
				unsigned int vspaces = 0;
				for (unsigned int counter = 0; counter < _string_size; (is_Vtab ((*_string) [counter]) != 0) ? ++vspaces, ++counter : ++counter);
				return vspaces;
			} else
				return UINT__ERROR;
		} else
			return UINT__ERROR;
	} else
		return UINT__ERROR;
}


/*
INPUT: A pointer to string which hold the main string.
OUTPUT: An unsigned integer which hold the amount of horzinatal spaces (Tabs) in the string, otherwise return 0xFFFFFFFF
INFO: This function is used to return the number of tabs/ horizintal spaces in the string.
*/
unsigned int n_Hspaces (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size > 0) {
				unsigned int hspaces = 0;
				for (unsigned int counter = 0; counter < _string_size; (is_Htab ((*_string) [counter]) != 0) ? ++hspaces, ++counter : ++counter);
				return hspaces;
			} else
				return UINT__ERROR;
		} else
			return UINT__ERROR;
	} else
		return UINT__ERROR;
}


/*
INPUT: A pointer to string which hold the mani string.
OUTPUT: An unsigned integer value hold the amount of number exist in the string, otherwise return 0xFFFFFFFF
INFO: This function is used to return the number of digits exist in a string.
*/
unsigned int n_numbers (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size > 0) {
				unsigned int numbers = 0;
				for (unsigned int counter = 0; counter < _string_size; (is_char_number ((*_string) [counter]) != 0) ? ++numbers, ++counter : ++counter);
				return numbers;
			} else
				return UINT__ERROR;
		} else
			return UINT__ERROR;
	} else
		return UINT__ERROR;
}


/*
INPUT: A pointer to string which hold the main string.
OUTPUT: An unsigned int value which hold the number of specail characters in a string, otherwise return 0xFFFFFFFF.
INFO: This function is used to return the count of special characters exist in a string.
*/
unsigned int n_specials (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size > 0) {
				unsigned int specials = 0;
				for (unsigned int counter = 0; counter < _string_size; (is_char_special ((*_string) [counter]) != 0) ? ++specials, ++counter : ++counter);
				return specials;
			} else
				return UINT__ERROR;
		} else
			return UINT__ERROR;
	} else
		return UINT__ERROR;
}


/*
INPUT: A pointer to a string which hold the main string.
OUTPUT: an unsigned integer which hold the hash of the string, if there is returned a failure has occured.
INFO: This function is used to return the hash of a sent string.
*/
unsigned int hash_string (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size > 0) {
				unsigned int hash = 0;
				for (unsigned int counter = 0; counter < _string_size; hash += hash_char ((*_string) [counter]), ++counter);

				return hash;
			} else
				return 0;
		} else
			return 0;
	} else
		return 0;
}


/*
INPUT: A pointer to string which hold the main string.
OUTPUT: An unsigned integer which hold the total number of white_space which include space - htab - vtab, otherwise return 0xFFFFFFFF.
INFO: This function return the numbers of white spaces included in a string.
*/
unsigned int n_white_spaces (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size > 0) {
				unsigned int real_size = 0;
				for (unsigned int counter = 0; counter < _string_size; (is_char_white_space ((*_string) [counter]) != 0) ? ++real_size, ++counter : ++counter);

				return real_size;
			} else
				return UINT__ERROR;
		} else
			return UINT__ERROR;
	} else
		return UINT__ERROR;
}


/*
INPUT: A pointer to string which hold the string and a character that we will find its first appearnece.
OUTPUT: Return the index of the character, otherwise return 4294967295. The maximum value of unsigned int.
INFO: This function is used to return the index of incoming character if it does exist, otherwise return the maximum value of unsigned integer 4294967295.
*/
unsigned int index_of (string * _string, char c) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size > 0) {
				for (unsigned int counter = 0; counter < _string_size; ++counter)
					if ((*_string) [counter] == c)
						return counter;

				return UINT__ERROR;
			} else
				return UINT__ERROR;
		} else
			return UINT__ERROR;
	} else
		return UINT__ERROR;
}


/*
INPUT: A pointer to string which hold the main string and a character that we will look for its last occurances and return its index.
OUTPUT: Return the last index of the character the user sent, otherwise return 4294967295 which is the maximum value of unsigned integer.
INFO: This function is used to return the last index of character occureance in the main string.
*/
unsigned int last_index_of (string * _string, char _char) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size > 0) {
				if (is_char_alpha (_char) || is_char_number (_char) || is_char_white_space (_char) || is_char_special (_char)) {
					_Bool does_exist = 0;

					for (unsigned int counter = 0; counter < _string_size; ++counter)
						if ((*_string) [counter] == _char) {
							does_exist = 1;
							break;
						}
					
					if (does_exist) {
						unsigned int index = 0;

						for (unsigned int counter = 0; counter < _string_size; ++counter)
							if ((*_string) [counter] == _char)
								index = counter;
						
						return index;
					} else
						return UINT__ERROR;
				} else
					return UINT__ERROR;
			} else
				return UINT__ERROR;
		} else
			return UINT__ERROR;
	} else
		return UINT__ERROR;
}


//Functions of type unsigned int *.
/*
INPUT: A pointer to string which is used to hold the main string.
OUTPUT: An array of unsigned integer which hold the indices of alphabitical characters in the string, otherwise return NULL.
INFO: This function is used to return the location/ indices of alphabitical characters in a string.
*/
unsigned int * indices_of_alphas (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size > 0) {
				unsigned int alphas = n_alphas (_string);

				if (alphas != UINT__ERROR) {
					unsigned int * indices = NULL;
					do {indices = (unsigned int *) malloc (sizeof (unsigned int) * alphas);} while (indices == NULL);
					alphas = 0;

					for (unsigned int counter = 0; counter < _string_size; (is_char_alpha ((*_string) [counter]) != 0) ? indices [alphas] = counter, ++alphas, ++counter : ++counter);


					return indices;
				} else
					return NULL;
			} else
				return NULL;
		} else
			return NULL;
	} else
		return NULL;
}


/*
INPUT: A pointer to string which hold the main string.
OUTPUT: An array of unsigned value which hold the indices of the lower cases characters, otherwise return NULL.
INFO: This function is used to return the locations/ indices of lower case characters.
*/
unsigned int * indices_of_Lcases (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size > 0) {
				unsigned int nLcases = n_Lcases (_string);
				if (nLcases != UINT__ERROR) {
					unsigned int * indices = NULL;
					do {indices = (unsigned int *) malloc (sizeof (unsigned int) * nLcases);} while (indices == NULL);
					nLcases = 0;

					for (unsigned int counter = 0; counter < _string_size; (is_char_lower ((*_string) [counter]) != 0) ? indices [nLcases] = counter, ++nLcases, ++counter : ++counter);

					return indices;

				} else
					return NULL;
			} else
				return NULL;
		} else
			return NULL;
	} else
		return NULL;
}


/*
INPUT: A pointer to string which hold the main string.
OUTPUT: An array of unsigned integer which hold the indices of upper case characters, otherwise return NULL.
INFO: This function is used to return the locations/ indices of upercase characters.
*/
unsigned int * indices_of_Ucases (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size > 0) {
				unsigned int nucases = n_Ucases (_string);

				if (nucases != UINT__ERROR) {
					unsigned int * indices = NULL;
					do {indices = (unsigned int *) malloc (sizeof (unsigned int) * nucases);} while (indices == NULL);
					nucases = 0;

					for (unsigned int counter = 0; counter < _string_size; (is_char_upper ((*_string) [counter]) != 0) ? indices [nucases] = counter, ++nucases, ++counter : ++counter);

					return indices;
				} else
					return NULL;
			} else
				return NULL;
		} else
			return NULL;
	} else
		return NULL;
}


/*
INPUT: A pointer to string which hold the main string.
OUTPUT: An array of unsigned integer which hold the indices of spaces in the string, otherwise return NULL.
INFO: This function is used to return the locations/ indices of spaces.
*/
unsigned int * indices_of_spaces (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size > 0) {
				unsigned int nspaces = n_spaces (_string);

				if (nspaces != UINT__ERROR) {
					unsigned int * indices = NULL;
					do {indices = (unsigned int *) malloc (sizeof (unsigned int) * nspaces);} while (indices == NULL);
					nspaces = 0;

					for (unsigned int counter = 0; counter < _string_size; (is_space ((*_string) [counter]) != 0) ? indices [nspaces] = counter, ++nspaces, ++counter : ++counter);

					return indices;
				} else
					return NULL;
			} else
				return NULL;
		} else
			return NULL;
	} else
		return NULL;
}


/*
INPUT: A pointer to the string which hold the main string.
OUTPUT: An array of unsigened which hold the indices of numbers in the string, otherwise return NULL.
INFO: This function is used to return the locations/ indices of number that exist in the string.
*/
unsigned int * indices_of_numbers (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size > 0) {
				unsigned int numbers = n_numbers (_string);

				if (numbers != UINT__ERROR) {
					unsigned int * indices = NULL;
					do {indices = (unsigned int *) malloc (sizeof (unsigned int) * numbers);} while (indices == NULL);
					numbers = 0;
					
					for (unsigned int counter = 0; counter < _string_size; (is_char_number ((*_string) [counter]) != 0) ? indices [numbers] = counter, ++numbers, ++counter : ++counter);

					return indices;
				} else
					return NULL;
			} else
				return NULL;
		} else
			return NULL;
	} else
		return NULL;
}


/*
INPUT: A pointer to string which hold the main string.
OUTPUT: An array of unsigned integer which hold the indices of vertical spaces, otherwise return NULL.
INFO: This function is used to return the locations/ indices of vertical spaces exist in string
*/
unsigned int * indices_of_Vspaces (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size > 0) {
				unsigned int vspaces = n_Vspaces (_string);

				if (vspaces != UINT__ERROR) {
					unsigned int * indices = NULL;
					do {indices = (unsigned int *) malloc (sizeof (unsigned int) * vspaces);} while (indices == NULL);
					vspaces = 0;

					for (unsigned int counter = 0; counter < _string_size; (is_Vtab ((*_string) [counter]) != 0) ? indices [vspaces] = counter, ++vspaces, ++counter : ++counter);

					return indices;
				} else
					return NULL;
			} else
				return NULL;
		} else
			return NULL;
	} else
		return NULL;
}


/*
INPUT: A pointer to string which hold the main string.
OUTPUT: An array of unsigned integer which hold the indices of horizintal spaces (tabs), otherwise return NULL.
INFO: This function is used to return the locations/ indices of horizintal spaces (tabs) exist in a string.
*/
unsigned int * indices_of_Hspaces (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size > 0) {
				unsigned int hspaces = n_Hspaces (_string);

				if (hspaces != UINT__ERROR) {
					unsigned int * indices = NULL;
					do {indices = (unsigned int *) malloc (sizeof (unsigned int) * hspaces);} while (indices == NULL);
					hspaces = 0;

					for (unsigned int counter = 0; counter < _string_size; (is_Htab ((*_string) [counter]) != 0) ? indices [hspaces] = counter, ++hspaces, ++counter : ++counter);

					return indices;
				} else
					return NULL;
			} else
				return NULL;
		} else
			return NULL;
	} else
		return NULL;
}


/*
INPUT: A pointer to string which hold the main string.
OUTPUT: An array of unsigned integer which hold the indices of special characters in a string, otherwise return NULL.
INFO: This function is used to return the locations/ indices of special characters in a string.
*/
unsigned int * indices_of_specials (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size > 0) {
				unsigned int specials = n_specials (_string);

				if (specials != UINT__ERROR) {
					unsigned int * indices = NULL;
					do {indices = (unsigned int *) malloc (sizeof (unsigned int) * specials);} while (indices == NULL);
					specials = 0;

					for (unsigned int counter = 0; counter < _string_size; (is_char_special ((*_string) [counter]) != 0) ? indices [specials] = counter, ++specials, ++counter : ++counter);

					return indices;
				} else
					return NULL;
			} else
				return NULL;
		} else
			return NULL;
	} else
		return NULL;
}


/*
INPUT: A pointer to string which hold the main string and a character which we will get its n indices.
OUTPUT: Return an array of unsigned integer where the first index is the size and the rest are the indices otherwise return NULL.
INFO: This function is used to return the occurences of a single characters and the first index is the size of the array.
*/
unsigned int * indices_of (string * _string, char _char) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			if (is_char_alpha (_char) || is_char_number (_char) || is_char_special (_char) || is_char_white_space (_char)) {
				unsigned int _string_size = str_len (_string);

				if (_string_size > 0) {
					unsigned int real_size = 0;
					for (unsigned int counter = 0; counter < _string_size; ((*_string) [counter] == _char) ? ++real_size, ++counter : ++counter);

					if (real_size > 0) {
						unsigned int * indices = NULL;
						do {indices = (unsigned int *) malloc (sizeof (unsigned int) * (real_size + 1));} while (indices == NULL);
						indices [0] = real_size;
						
						real_size = 1;
						for (unsigned int counter = 0; counter < _string_size; ((*_string) [counter] == _char) ? indices [real_size] = counter, ++real_size, ++counter : ++counter);

						return indices;
					} else
						return NULL;
				} else
					return NULL;
			} else
				return NULL;
		} else
			return NULL;
	} else
		return NULL;
}


/*
INPUT: A pointer to string which hold the main string and a string which hold the word the we need to get its starting index and ending index. The word size must be at least be 2.
OUTPUT: Return an array of unsigned integer of size 2 the first index contains the starting index of the word and the second index is the ending index of the word, otherwise return NULL.
INFO: This function is used to return the beginnging and the ending index of a word user sent.
*/
unsigned int * index_of_word (string * _string, string word) {
	if (_string != NULL && word != NULL) {
		if ((*_string) != NULL) {
			unsigned int _word_size = str_len (&word);
			unsigned int _string_size = str_len (_string);

			if (_word_size > 2 && _string_size > 0 && _word_size <= _string_size) {
				if (contains (_string, &word)) {
					unsigned int * indices = NULL;
					do {indices = (unsigned int *) calloc (2, sizeof (unsigned int));} while (indices == NULL);

					string temp = NULL;
					do {temp = (string) malloc (sizeof (char) * (_word_size + 1));} while (temp == NULL);
					temp [_word_size] = '\0';
					
					_Bool start = 1;
					for (unsigned int counter = 0; counter < _string_size; ++counter) {
						if (start) {
							start = 0;
							indices [0] = counter;
						}

						temp [(counter % _word_size)] = (*_string) [counter];
						
						if ((counter + 1) % _word_size == 0) {
							indices [1] = counter;
							start = 1;
							if (compare_string (_string, temp))
								break;
						}
					}

					return indices;
				} else
					return NULL;
			} else
				return NULL;
		} else
			return NULL;
	} else
		return NULL;
}


/*
INPUT: A pointer to string which hold the main string and a string which hold a word that we need to find.
OUTPUT: An array of unsigned integer otherwise return NULL.
INFO: This function is used the multiple occurences of a specific word. The first index of the word will tell how many indices.
	The indices is like the output of index_of_word but multiple.
	E.G: Let's the word BC is found two times in the main string the output will be
	---------------------
	| 4 | 0 | 1 | 5 | 6 |
	---------------------
	The 4 means we have four indices we divide it by 2 and we will get the real occurence number. Why it is 4, because when you iterate you know
	if the index - 1 % 2 == 0 then you know you moved to the next occurence of the word.
	0 and 5 is the starting index of a word
	1 and 6 is the ending index of a word.
*/
unsigned int * indices_of_word (string * _string, string word) {
	if (_string != NULL && word != NULL) {
		if ((*_string) != NULL) {
			unsigned int word_size = str_len (&word);
			unsigned int _string_size = str_len (_string);

			if (word_size > 0 && _string_size > 0 && word_size <= _string_size) {
				unsigned int real_size = 0;
				string temp = NULL;
				do {temp = (string) malloc (sizeof (char) * (word_size + 1));} while (temp == NULL);
				temp [word_size] = '\0';
				
				//1  2  3  4.
				unsigned int counter = 0;
				while (counter < (_string_size - word_size + 1)) {
					for (unsigned int inside = 0; inside < word_size; ++inside) {
						temp [inside] = (*_string) [(inside + counter)];
					}
					
					if (compare_string (&temp, word)) {
						++real_size;
						++counter;
					}
					
					++counter;
				}
				
				if (real_size > 0) {
					unsigned int * indices = NULL;
					do {indices = (unsigned int *) malloc (sizeof (unsigned int) * ((real_size * 2) + 1));} while (indices == NULL);
					indices [0] = (real_size * 2);
					real_size = 1;

					counter = 0;
					while (counter < (_string_size - word_size + 1)) {
						for (unsigned int inside = 0; inside < word_size; ++inside)
							temp [inside] = (*_string) [(counter + inside)];

						//1  2
						if (compare_string (&temp, word)) {
							indices [real_size++] = counter;
							indices [real_size++] = (counter + word_size - 1);
							++counter;
						}

						++counter;
					}

					free (temp);
					temp = NULL;
					
					return indices;
				} else {
					free (temp);
					temp = NULL;
					return NULL;
				}
			} else
				return NULL;
		} else
			return NULL;
	} else
		return NULL;
}


//Function that return unsigned int **
/*
INPUT: A pointer to string which hold the main string and string which hold set of characters, space is the spliter which is used to know how many words, characters the user want to know their occurences.

OUTPUT: A 2d array of where the first index contains the size of the array otherwise return NULL
INFO: This function is used to the occurences of character or word in the string seperated by space. The first index hold the size of the list and starting from index 
	1 the information begin each one tells you the occurences and their indices.
*/
unsigned int ** occurences (string * _string, string chars_words) {
	if (_string != NULL && chars_words != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);
			unsigned int chars_words_size = str_len (&chars_words);

			if (_string_size > 0 && chars_words_size > 0) {
				void *** content = str_char_split (&chars_words, ' ');

				if (content != NULL) {
					unsigned int * split_length = (unsigned int *) content [0][0];
					string * split = (string *) content [1];

					content [0][0] = NULL;
					content [1] = NULL;
					free (content);
					content = NULL;

					unsigned int ** oc = NULL;
					do {oc = (unsigned int **) malloc (sizeof (unsigned int *) * ((*split_length) + 1));} while (oc == NULL);
					do {oc [0] = (unsigned int *) malloc (sizeof (unsigned int));} while (oc [0] == NULL);
					oc [0][0] = (*split_length);

					for (unsigned int counter = 1; counter <= oc [0][0]; ++counter) {
						if (str_len (&split [counter]) >= 2) {
							split_length = indices_of_word (_string, split [counter]);
							if (split_length != NULL)
								oc [counter] = split_length;
							else
								do {oc [counter] = (unsigned int *) calloc (1, sizeof (unsigned int));} while (oc [counter] == NULL);
						} else {
							split_length = indices_of (_string, split [counter][0]);
							if (split_length != NULL)
								oc [counter] = split_length;
							else
								do {oc [counter] = (unsigned int *) calloc (1, sizeof (unsigned int));} while (oc [counter] == NULL);

						}
					}

					split_length = NULL;
					return oc;
				} else
					return NULL;
			} else
				return NULL;
		} else
			return NULL;
	} else
		return NULL;
}


/*
INPUT: A pointer to string which hold the main string and string which hold the characters/ words the user want to know their indices. There must be space between each word or character.
OUTPUT:

INFO: This function is used to a 2d array of unsigned integer where the first index in the 2d array is the length of the 2d.
	Starting from index 1 the data about indices start. Where the first index in each 1d array contain the size of that 1d array.
	As user can send a word or a character. If character the size of the 1d will be 2 and if word the size of the 1d will be 3. As we discussed the first index
	in the 1d array will tell you the size of that array if 1, then it is a character and if 2, then it is word. If a string do not exist, then the value will
	be 0.

	E.g:
		If the input was: "C IS King"
		Then we will have three thing to search for C, IS and King
		And let's say we found them the output will be like.

		OUTPUT:
			-----------------
			| 0 | 1 | 2 | 3 |
			|===|===|===|===|
			| 3 | 1 | 2 | 2 |
			|===| 2 | 0 | 3 |
		 	    ====| 1 | 6 |
			        =========
*/
unsigned int ** indices_of_n_strings (string * _string, string cs) {
	if (_string != NULL && cs != NULL) {
		if ((*_string) != NULL) {
			unsigned int cs_size = str_len (&cs);
			unsigned int _string_size = str_len (_string);

			if (cs_size > 0 && _string_size > 0) {
				void *** content = str_char_split (&cs, ' ');
				if (content != NULL) {
					unsigned int * split_length = (unsigned int *) content [0][0];
					string * split = (string *) content [1];

					content [0][0] = NULL;
					content [1] = NULL;
					free (content);
					content = NULL;

					unsigned int ** return_value = NULL;
					do {return_value = (unsigned int **) malloc (sizeof (unsigned int *) * ((*split_length) + 1));} while (return_value == NULL);

					//Allocate the size of the return_value
					do {return_value [0] = (unsigned int *) malloc (sizeof (unsigned int));} while (return_value [0] == NULL);
					return_value [0][0] = (*split_length);

					free (split_length);
					split_length = NULL;

					for (unsigned int counter = 1; counter <= return_value [0][0]; ++counter) {
						unsigned int size = str_len (&split [counter]);

						if (size >= 2) {
							unsigned int * word_indices = index_of_word (_string, split [counter]);
							if (word_indices != NULL) {
								do {return_value [counter] = (unsigned int *) malloc (sizeof (unsigned int) * 3);} while (return_value [counter] == NULL);
								return_value [counter][0] = 2;
								return_value [counter][1] = word_indices [0];
								return_value [counter][2] = word_indices [1];

								free (word_indices);
								word_indices = NULL;
							} else
								//Allocate to indicate that the string do not exist.
								do {return_value [counter] = (unsigned int *) calloc (1, sizeof (unsigned int));} while (return_value [counter] == NULL);
						} else {
							unsigned int index = index_of (_string, split [counter][0]);
							if (index != UINT__ERROR) {
								do {return_value [counter] = (unsigned int *) malloc (sizeof (unsigned int) * 2);} while (return_value [counter] == NULL);
								return_value [counter][0] = 1;
								return_value [counter][1] = index;
							} else
								do {return_value [counter] = (unsigned int *) calloc (1, sizeof (unsigned int));} while (return_value [counter] == NULL);
						}
					}

					return return_value;
				} else
					return NULL;
			} else
				return NULL;
		} else
			return NULL;
	} else
		return NULL;
}


/*
INPUT: A pointer to string which hold the main string and a string which hold set of words seperated by space.
OUTPUT: A 2d array of unsigned integer each one holds the output indices_of_word otherwise return NULL.
INFO: This function is used to find the indices of multiple words the first index is the size of the output of 2d array.
*/
unsigned int ** indices_of_words (string * _string, string words) {
	if (_string != NULL && words != NULL) {
		if ((*_string) != NULL) {
			unsigned int words_size = str_len (&words);
			unsigned int _string_size = str_len (_string);

			if (words_size > 0 && _string_size > 0) {
				void *** content = str_char_split (&words, ' ');

				if (content != NULL) {
					unsigned int * split_length = (unsigned int *) content [0][0];
					string * split = (string *) content [1];

					content [0][0] = NULL;
					content [1] = NULL;
					free (content);
					content = NULL;

					unsigned int ** indices = NULL;
					do {indices = (unsigned int **) malloc (sizeof (unsigned int *) * ((*split_length) + 1));} while (indices == NULL);
					do {indices [0] = (unsigned int *) malloc (sizeof (unsigned int));} while (indices [0] == NULL);
					indices [0][0] = (*split_length);

					for (unsigned int counter = 1; counter <= indices [0][0]; ++counter) {
						split_length = indices_of_word (_string, split [counter]);
						if (split_length != NULL)
							indices [counter] = split_length;
						else {
							do {indices [counter] = (unsigned int *) malloc (sizeof (unsigned int));} while (indices [counter] == NULL);
							indices [counter][0] = 0;
						}
					}

					return indices;
				} else
					return NULL;
			} else
				return NULL;
		} else
			return NULL;
	} else
		return NULL;
}


//Functions of type unsigned int ***.
/*
Require Changes here and in the printing function for the analysis
INPUT: A pointer to a string. Must contain data.
OUTPUT: Return a 3d array of unsinged int contanis analysis about the string.
INFO: This function is used to produce an analysis about the string which is sent.
	The content of the analysis will be as follows:
		0-The size of the string.
		1-How many alph characters.
		2-How many lower case characters and their indices.
		3-How many upper case characters and their indices.
		4-How many numbers in the string and their indices.
		5-How many special in the string and their indices.
		6-How many space in the string and their indices.
		7-How many Htab in the string and their indices.
		8-How many  Vtab in the string and their indices.
		9-The sum of white space number.
		10-The occurences of each characters in the string and their indices.
		11-The possible type of string that can be converted to.
		12-The hash of the string.
		13-The string it self. Which we can be used to print the string normally and reversed.
*/
unsigned int *** string_analysis (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size > 0) {
				unsigned int *** analysis = NULL;
				do {analysis = (unsigned int ***) malloc (sizeof (unsigned int **) * 14);} while (analysis == NULL);

				//Allocate space for the size of the string.
				do {analysis [0] = (unsigned int **) malloc (sizeof (unsigned int *));} while (analysis [0] == NULL);
				do {analysis [0][0] = (unsigned int *) malloc (sizeof (unsigned int));} while (analysis [0][0] == NULL);
				analysis [0][0][0] = _string_size;

				//Allocate space for the size of alphabitcal characters and their locations and value.
				//So we have about 3 2d and each 1d is the size of alphabitcal characters. But first we need to know if there is even
				//alphabitical characters in the string.
				_string_size = n_alphas (_string);
				if (_string_size != UINT__ERROR) {
					do {analysis [1] = (unsigned int **) malloc (sizeof (unsigned int *) * 3);} while (analysis [1] == NULL);
					do {analysis [1][0] = (unsigned int *) malloc (sizeof (unsigned int));} while (analysis [1][0] == NULL);
					analysis [1][0][0] = _string_size;

					analysis [1][1] = indices_of_alphas (_string);

					do {analysis [1][2] = (unsigned int *) malloc (sizeof (unsigned int) * analysis [1][0][0]);} while (analysis [1][2] == NULL);
					for (unsigned int counter = 0; counter < analysis [1][0][0]; analysis [1][2][counter] = (*_string) [analysis [1][1][counter]], ++counter);
				} else {
					do {analysis [1] = (unsigned int **) malloc (sizeof (unsigned int *));} while (analysis [1] == NULL);
					do {analysis [1][0] = (unsigned int *) calloc (1, sizeof (unsigned int));} while (analysis [1][0] == NULL);
				}

				//Allocate space for the lower case characters.
				//1 index is the size, 2 is their indices and 3 is their characters.
				_string_size = n_Lcases (_string);
				if (_string_size != UINT__ERROR) {
					do {analysis [2] = (unsigned int **) malloc (sizeof (unsigned int *) * 3);} while (analysis [2] == NULL);
					do {analysis [2][0] = (unsigned int *) malloc (sizeof (unsigned int));} while (analysis [2][0] == NULL);
					analysis [2][0][0] = _string_size;

					analysis [2][1] = indices_of_Lcases (_string);

					do {analysis [2][2] = (unsigned int *) malloc (sizeof (unsigned int) * _string_size);} while (analysis [2][2] == NULL);
					for (unsigned int counter = 0; counter < _string_size; analysis [2][2][counter] = (*_string) [analysis [2][1][counter]], ++counter);

				} else {
					do {analysis [2] = (unsigned int **) malloc (sizeof (unsigned int *));} while (analysis [2] == NULL);
					do {analysis [2][0] = (unsigned int *) calloc (1, sizeof (unsigned int));} while (analysis [2][0] == NULL);
				}

				//Allocate spaces for the upper case characters.
				//1 index is the size, 2 index is their indices and 3 index is their characters.
				_string_size = n_Ucases (_string);
				if (_string_size != UINT__ERROR) {
					do {analysis [3] = (unsigned int **) malloc (sizeof (unsigned int *) * 3);} while (analysis [3] == NULL);
					do {analysis [3][0] = (unsigned int *) malloc (sizeof (unsigned int));} while (analysis [3][0] == NULL);
					analysis [3][0][0] = _string_size;

					analysis [3][1] = indices_of_Ucases (_string);

					do {analysis [3][2] = (unsigned int *) malloc (sizeof (unsigned int) * _string_size);} while (analysis [3][2] == NULL);
					for (unsigned int counter = 0; counter < _string_size; analysis [3][2][counter] = (*_string) [analysis [3][1][counter]], ++counter);
				} else {
					do {analysis [3] = (unsigned int **) malloc (sizeof (unsigned int *));} while (analysis [3] == NULL);
					do {analysis [3][0] = (unsigned int *) calloc (1, sizeof (unsigned int));} while (analysis [3][0] == NULL);
				}

				//Allocate space for the numbers in a string.
				//1 index is size, 2 index is their indices and 3 index is their numbers.
				_string_size = n_numbers (_string);
				if (_string_size != UINT__ERROR) {
					do {analysis [4] = (unsigned int **) malloc (sizeof (unsigned int *) * 3);} while (analysis [4] == NULL);
					do {analysis [4][0] = (unsigned int *) malloc (sizeof (unsigned int));} while (analysis [4][0] == NULL);
					analysis [4][0][0] = _string_size;

					analysis [4][1] = indices_of_numbers (_string);

					do {analysis [4][2] = (unsigned int *) malloc (sizeof (unsigned int) * _string_size);} while (analysis [4][2] == NULL);
					for (unsigned int counter = 0; counter < _string_size; analysis [4][2][counter] = (*_string) [analysis [4][1][counter]], ++counter);
				} else {
					do {analysis [4] = (unsigned int **) malloc (sizeof (unsigned int *));} while (analysis [4] == NULL);
					do {analysis [4][0] = (unsigned int *) calloc (1, sizeof (unsigned int));} while (analysis [4][0] == NULL);
				}

				//Allocate space for the special characters.
				//1 index is size, 2 index is their indices and 3 index is their characters.
				_string_size = n_specials (_string);
				if (_string_size != UINT__ERROR) {
					do {analysis [5] = (unsigned int **) malloc (sizeof (unsigned int *) * 3);} while (analysis [5] == NULL);
					do {analysis [5][0] = (unsigned int *) malloc (sizeof (unsigned int));} while (analysis [5][0] == NULL);
					analysis [5][0][0] = _string_size;

					analysis [5][1] = indices_of_specials (_string);

					do {analysis [5][2] = (unsigned int *) malloc (sizeof (unsigned int) * _string_size);} while (analysis [5][2] == NULL);
					for (unsigned int counter = 0; counter < _string_size; analysis [4][2][counter] = (*_string) [analysis [5][1][counter]], ++counter);
				} else {
					do {analysis [5] = (unsigned int **) malloc (sizeof (unsigned int *));} while (analysis [5] == NULL);
					do {analysis [5][0] = (unsigned int *) calloc (1, sizeof (unsigned int));} while (analysis [5][0] == NULL);
				}


				//Allocate space for spaces.
				//1 index is size, 2 index is their indices and 3 index is their characters
				_string_size = n_spaces (_string);
				if (_string_size != UINT__ERROR) {
					do {analysis [6] = (unsigned int **) malloc (sizeof (unsigned int *) * 3);} while (analysis [6] == NULL);
					do {analysis [6][0] = (unsigned int *) malloc (sizeof (unsigned int));} while (analysis [6][0] == NULL);
					analysis [6][0][0] = _string_size;

					analysis [6][1] = indices_of_spaces (_string);

					do {analysis [6][2] = (unsigned int *) malloc (sizeof (unsigned int) * _string_size);} while (analysis [6][2] == NULL);
					for (unsigned int counter = 0; counter < _string_size; analysis [6][2][counter] = (*_string) [analysis [6][1][counter]], ++counter);
				} else {
					do {analysis [6] = (unsigned int **) malloc (sizeof (unsigned int *));} while (analysis [6] == NULL);
					do {analysis [6][0] = (unsigned int *) calloc (1, sizeof (unsigned int));} while (analysis [6][0] == NULL);
				}


				//Allocate space for hspaces.
				//1 index is the size, 2 index is their indices and 3 index is their characters.
				_string_size = n_Hspaces (_string);
				if (_string_size != UINT__ERROR) {
					do {analysis [7] = (unsigned int **) malloc (sizeof (unsigned int *) * 3);} while (analysis [7] == NULL);
					do {analysis [7][0] = (unsigned int *) malloc (sizeof (unsigned int));} while (analysis [7][0] == NULL);
					analysis [7][0][0] = _string_size;

					analysis [7][1] = indices_of_Hspaces (_string);

					do {analysis [7][2] = (unsigned int *) malloc (sizeof (unsigned int) * _string_size);} while (analysis [7][2] == NULL);
					for (unsigned int counter = 0; counter < _string_size; analysis [7][2][counter] = (*_string) [analysis [7][1][counter]], ++counter);
				} else {
					do {analysis [7] = (unsigned int **) malloc (sizeof (unsigned int *));} while (analysis [7] == NULL);
					do {analysis [7][0] = (unsigned int *) calloc (1, sizeof (unsigned int));} while (analysis [7][0] == NULL);
				}

				//Allocate space for V space.
				//1 index for size, 2 index for indices and 3 index for their characters.
				_string_size = n_Vspaces (_string);
				if (_string_size != UINT__ERROR) {
					do {analysis [8] = (unsigned int **) malloc (sizeof (unsigned int *) * 3);} while (analysis [8] == NULL);
					do {analysis [8][0] = (unsigned int *) malloc (sizeof (unsigned int));} while (analysis [8][0]);
					analysis [8][0][0] = _string_size;

					analysis [8][1] = indices_of_Vspaces (_string);

					do {analysis [8][2] = (unsigned int *) malloc (sizeof (unsigned int) * _string_size);} while (analysis [8][2] == NULL);
					for (unsigned int counter = 0; counter < _string_size; analysis [8][2][counter] = (*_string) [analysis [8][1][counter]], ++counter);
				} else {
					do {analysis [8] = (unsigned int **) malloc (sizeof (unsigned int *));} while (analysis [8] == NULL);
					do {analysis [8][0] = (unsigned int *) calloc (1, sizeof (unsigned int));} while (analysis [8][0] == NULL);
				}

				//Allocate space for the amount of white_spaces in a string.
				do {analysis [9] = (unsigned int **) malloc (sizeof (unsigned int *));} while (analysis [9] == NULL);
				do {analysis [9][0] = (unsigned int *) malloc (sizeof (unsigned int));} while (analysis [9][0] == NULL);
				if ((analysis [9][0][0] = n_white_spaces (_string)) == UINT__ERROR)
					analysis [9][0][0] = 0;

				//Allocate space for occurences.
				_string_size = str_len (_string);
				do {analysis [10] = (unsigned int **) malloc (sizeof (unsigned int *) * _string_size);} while (analysis [10] == NULL);
				for (unsigned int counter = 0; (*_string) [counter] < _string_size; ++counter) {
					string temp = NULL;
					do {temp = (string) malloc (sizeof (char) * 2);} while (temp == NULL);
					temp [1] = '\0';
					temp [counter] = (*_string) [counter];
					analysis [10][counter] = occurences (_string, temp) [0]; //Index of 0 because i know from occurences function only one dimension array will be need.
					free (temp);
					temp = NULL;
				}

				//Allocate space for the possible convertion type. [Short int - Unsigned Short int - Unsigned int - int - unsigned long long int - long long int - double - struct - union]
				//We need 8 spaces to hold the result in a one dimension array.
				//Not 9 because the function is_union_struct return 1 if it is struct or 2 if it is union and 0 if non of those. So there is need for 9.
				do {analysis [11] = (unsigned int **) malloc (sizeof (unsigned int *));} while (analysis [11] == NULL);
				do {analysis [11][0] = (unsigned int *) malloc (sizeof (unsigned int) * 8);} while (analysis [11][0] == NULL);
				for (unsigned char counter = 0; counter < 8; counter++) {
					switch (counter) {
						case 0: analysis [11][0][counter] = (unsigned int) is_sshi (_string); break;
						case 1: analysis [11][0][counter] = (unsigned int) is_ushi (_string); break;
						case 2: analysis [11][0][counter] = (unsigned int) is_sint (_string); break;
						case 3: analysis [11][0][counter] = (unsigned int) is_uint (_string); break;
						case 4: analysis [11][0][counter] = (unsigned int) is_slli (_string); break;
						case 5: analysis [11][0][counter] = (unsigned int) is_ulli (_string); break;
						case 6: analysis [11][0][counter] = (unsigned int) is_double (_string); break;
						case 7: analysis [11][0][counter] = (unsigned int) is_union_struct (_string); break;
						default:
							break;
					}
				}

				//Allocate space for the hash.
				do {analysis [12] = (unsigned int **) malloc (sizeof (unsigned int *));} while (analysis [12] == NULL);
				do {analysis [12][0] = (unsigned int *) malloc (sizeof (unsigned int));} while (analysis [12][0] == NULL);
				analysis [12][0][0] = hash_string (_string);

				//Allocate space for the string it self.
				//I did not need the size of the string again as it was last stored in the _string_size variable while doing analysis [10] operation.
				do {analysis [13] = (unsigned int **) malloc (sizeof (unsigned int *));} while (analysis [13] == NULL);
				do {analysis [13][0] = (unsigned int *) malloc (sizeof (unsigned int) * _string_size);} while (analysis [13][0] == NULL);
				for (unsigned int counter = 0; counter < _string_size; analysis [13][0][counter] = (*_string) [counter], ++counter);

				return analysis;
			} else
				return NULL;
		} else
			return NULL;
	} else
		return NULL;
}


//Functions that return int value.
/*
INPUT: A pointer to string which hold the main string.
OUTPUT: Return the number, otherwise return (0x7FFFFFFF) PINT__ERROR if the number is positive or (0x80000000) NINT__ERROR if the number is negative.
INFO: This function is used to return the signed integer representation of the string.
	If the number is equal to 0x7FFFFFFF return 0x7FFFFFFF (PINT__ERROR), return 0x7FFFFFFF (PINT__ERROR).
	If the number is equal to 0x80000000 return 0x80000000 (NINT__ERROR), return 0x80000000 (NINT__ERROR).
	If the string is not number, then return 0x7FFFFFFF (PINT__ERROR).
*/
int to_sint (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size > 0) {
				if (is_sint (_string)) {
					_Bool is_negative = 0;
					unsigned int counter = 0;

					if ((*_string) [0] == '-') {
						is_negative = 1;
						counter = 1;
					}

					int number = 0;
					for (; counter < _string_size; ++counter)
						number = (10 * number) + ((*_string) [counter] - 48);

					if (is_negative)
						number *= -1;

					if (number == PINT__ERROR && !is_negative)
						return PINT__ERROR;
					else if (number == NINT__ERROR && is_negative)
						return NINT__ERROR;
					else
						return number;
				} else
					return PINT__ERROR;
			} else
				return PINT__ERROR;
		} else
			return PINT__ERROR;
	} else
		return PINT__ERROR;
}


//Functions that return unsigned short value.
/*
INPUT: A pointer to string which hold the main string.
OUTPUT: Return number if the string represent the number, otherwise return USHI__ERROR (0xFFFF).
INFO: This function is used to return the unsigned short int representation of the string.
*/
unsigned short to_ushi (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size > 0) {
				if (is_ushi (_string)) {
					unsigned short int number = 0;
					for (unsigned int counter = 0; counter < _string_size; ++counter)
						number = (10 * number) + ((*_string) [counter] - 48);

					if (number != USHI__ERROR)
						return number;
					else
						return USHI__ERROR;
				} else
					return USHI__ERROR;
			} else
				return USHI__ERROR;
		} else
			return USHI__ERROR;
	} else
		return USHI__ERROR;
}


//Functions that return short value
/*
INPUT: A pointer to string which hold the main string.
OUTPUT: Return a number which, otherwise return 0x7FFF (PSHI__ERROR) if the number is positive and 0x8000 (NSHI__ERROR) if the number is negative.
INFO: This function is used to return the signed short int representation of the string.
	If the number is equal to 0x7FFF (PSHI__ERROR), return PSHI__ERROR.
	If the number is equal to 0x8000 (NSHI__ERROR), return NSHI__ERROR.
	If the string is not a number, return PSHI__ERROR.
*/
short to_sshi (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size > 0) {
				if (is_sshi (_string)) {
					int counter = 0;
					short number = 0;
					_Bool is_negative = 0;

					if ((*_string) [0] == '-') {
						is_negative = 1;
						counter = 1;
					}

					for (; counter < _string_size; ++counter)
						number = (10 * number) + ((*_string) [counter] - 48);

					if (is_negative)
						number *= -1;

					if (number == PSHI__ERROR && !is_negative)
						return PSHI__ERROR;
					else if (number == NSHI__ERROR && is_negative)
						return NSHI__ERROR;
					else
						return number;
				} else
					return PSHI__ERROR;
			} else
				return PSHI__ERROR;
		} else
			return PSHI__ERROR;
	} else
		return PSHI__ERROR;
}


//Functions that return unsigned long long int.
/*
INPUT: A pointer to string which hold the main string.
OUTPUT: Return the number, otherwise return 0xFFFFFFFFFFFFFFFF (ULLI__ERROR).
INFO: This function return the unsigned long long int representation of the string.
*/
unsigned long long int to_ulli (string *_string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size > 0) {
				if (is_ulli (_string)) {
					unsigned long long int number = 0;
					for (unsigned int counter = 0; counter < _string_size; ++counter)
						number = (10 * number) + ((*_string) [counter] - 48);

					if (number == ULLI__ERROR)
						return ULLI__ERROR;
					else
						return number;
				} else
					return ULLI__ERROR;
			} else
				return ULLI__ERROR;
		} else
			return ULLI__ERROR;
	} else
		return ULLI__ERROR;
}


//Functions that return long long int.
/*
INPUT: A pointer to a string which hold the main string.
OUTPUT: Return the number, otherwise return (PLLI__ERROR) if the number is positive or (NLLI__ERROR) if the number is negative.
INFO: This function is used to return the signed long long int representation of the string.
	If the number is equal to negative number, return NLLI__ERROR.
	If the number is equal to posative number, return PLLI__ERROR.
	If the the string is not a number, return  PLLI__ERROR.
*/
long long int to_slli (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size > 0) {
				if (is_slli (_string)) {
					_Bool is_negative = 0;
					unsigned int counter = 0;
					long long int number = 0;

					if ((*_string) [0] == '-') {
						is_negative = 1;
						counter = 1;
					}

					for (; counter < _string_size; ++counter)
						number = (10 * number) + ((*_string) [counter] - 48);

					if (is_negative)
						number *= -1;

					if (number == PLLI__ERROR && !is_negative)
						return PLLI__ERROR;
					else if (number == NLLI__ERROR && is_negative)
						return NLLI__ERROR;
					else
						return number;
				} else
					return PLLI__ERROR;
			} else
				return PLLI__ERROR;
		} else
			return PLLI__ERROR;
	} else
		return PLLI__ERROR;
}


//Functions that return double.
/*
INPUT: A pointer to a string, which hold the main string.
OUTPUT: Return the number, otherwise return PDBL__ERROR if the number is positive or NDBL__ERROR if the number is negative.
INFO: This function is used to return signed double representation of the string.
	If the number is equal PDBL__ERROR, return PDBL__ERROR.
	If the number is equal NDBL__ERROR, return NDBL__ERROR.
	If the string is not number, then return PDBL__ERROR.
*/
double to_double (string * _string) {
	if (_string != NULL) {
		if ((*_string) != NULL) {
			unsigned int _string_size = str_len (_string);

			if (_string_size > 0) {
				if (is_double (_string)) {
					double number = 0;
					_Bool is_negative = 0;
					unsigned int counter = 0;

					void *** content = str_char_split (_string, '.');

					if (content != NULL) {
						unsigned int * split_size = ((unsigned int *) content [0][0]);
						string * split_content = ((string *) content [1]);
						content [0][0] = NULL;
						content [1] = NULL;
						free (content);
						content = NULL;
						free (split_content [0]);
						split_content [0] = NULL;

						if ((*_string) [0] == '-') {
							counter = 1;
							is_negative = 1;
						}

						_string_size = str_len (&split_content [1]);
						for (; counter < _string_size; ++counter)
							number = (10 * number) + (split_content [1][counter] - 48);

						_string_size = str_len (&split_content [2]);
						counter = 0;
						for (; counter < _string_size; ++counter)
							number = (10 * number) + (split_content [2][counter] - 48);

						number /= power_of_ten (_string_size);

						free (split_size);
						split_size = NULL;

						free (split_content [1]);
						split_content [1] = NULL;
						free (split_content [2]);
						split_content [2] = NULL;
						free (split_content);
						split_content = NULL;

						if (is_negative)
							number *= -1;

						if (number == PDBL__ERROR && !is_negative)
							return PDBL__ERROR;
						else if (number == NDBL__ERROR && is_negative)
							return NDBL__ERROR;
						else
							return number;
					} else
						return PDBL__ERROR;
				} else
					return PDBL__ERROR;
			} else
				return PDBL__ERROR;
		} else
			return PDBL__ERROR;
	} else
		return PDBL__ERROR;
}


//Functions the user does not need to use, but if he/ she understands, then it is okay.
/*
INPUT: A pointer to a pointer to a string and unsigned integer. Address of list of strings and the size of that list
OUTPUT: NONE.
INFO: This function is used to make the first index of the list of string contain the size of the split list.
	As we know the maximum number char can hold is 127. So we create an array of chars where the first index
	tells us how many times we need to add up to get the real split list size. And it end with \0.

	Example: If we have two lists we create another list at the beginning so we have three and the first one tells us the size of the split list size.
	-------------------
	|  0  |  1  |  2  |
	-------------------
	|  2  |  .  |  .  |
	| \0  |  .  |  .  |
	-------------------
	Another Example if we have 230 list we create another so we have 231
	-------------------
	|  0  | ... | 230 |
	-------------------
	| 127 |  .  |  .  |
	| 103 |  .  |  .  |
	| \0  |  .  |  .  |
	-------------------
*/
void generate_backup_split_size (string ** split_list, unsigned int size_of_split_list) {
	if (size_of_split_list > 127) {
		unsigned int how_many = 0;
		while (size_of_split_list > 127) {
			how_many++;
			size_of_split_list -= 127;
		}
		how_many++;

		do {(*split_list) [0] = (string) malloc (sizeof (char) * (how_many + 1));} while ((*split_list) [0] == NULL);

		for (unsigned int counter = 0; counter < (how_many + 1); counter++) {
			if ((counter + 1) == how_many)
				(*split_list) [0][counter] = (char) size_of_split_list;
			else
				(*split_list) [0][counter] = 127;
		}
		
		(*split_list) [0][how_many] = '\0';
						
	} else {
		do {(*split_list) [0] = (string) malloc (sizeof (char) * 2);} while ((*split_list) [0] == NULL);
		(*split_list) [0][0] = (char) (size_of_split_list);
		(*split_list) [0][1] = '\0';
	}
}


/*
INPUT: A pointer to a void * which hold either BASIC_UNION or BASIC_STRUCT, _Bool type which tells us either what sent is union or struct and string which represent an experssion which we will
	analyze the string to allocate variables inside a union or struct.

	If the value of type is 1 then it is struct otherwise it is union.

OUTPUT: Return 1 if the allocation is successful, otherwise return 0.
INFO: This function is used to allocate variables in either a struct or a union with the right constraints.

Type codes:
	sshi = 3
	ushi = 4
	sint = 5
	uint = 6
	slli = 7
	ulli = 8
	strn = 9
	dble = 11
*/
_Bool allocate_union_struct_var (void ** union_struct, _Bool type, string * experssion) {
	if ((*experssion) [0] == ' ')
		remove_at (experssion, 0);
	
	//type how_many_dimension^size_for_each_dimension* ...
	unsigned int how_many_dimensions = ((*experssion) [5] - 48);

	//After this function the string will be: typesize_for_each_dimension* ...
	remove_at_these (experssion, 2, ((unsigned int []) {4, 5, 6}));

	string dtype = NULL;
	dtype = ((char []) {(*experssion) [0], (*experssion) [1], (*experssion) [2], (*experssion) [3], '\0'});

	//Afther this function the string will be: size_for_each_dimension* ...
	remove_at_these (experssion, 4, ((unsigned int []) {0, 1, 2, 3}));

	char dtype_code = get_type_code (&dtype);

	free (dtype);
	dtype = NULL;

	unsigned int ** size_for_each_dimension = NULL;

	if (dtype_code == 3) {
		if (type) {
			if (__BASIC__STRUCT__(union_struct) -> sshi != NULL)
				return 1;
		} else {
			if (__BASIC__UNION__(union_struct) -> sshi != NULL)
				return 1;
		}

		if (how_many_dimensions == 0) {
			if (type) {
				do {__BASIC__STRUCT__(union_struct) -> sshi = (short int ****) malloc (sizeof (short int ***));} while (__BASIC__STRUCT__(union_struct) -> sshi == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> sshi) [0] = (short int ***) malloc (sizeof (short int **));} while ((__BASIC__STRUCT__(union_struct) -> sshi) [0] == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> sshi) [0][0] = (short int **) malloc (sizeof (short int *));} while ((__BASIC__STRUCT__(union_struct) -> sshi) [0][0] == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> sshi) [0][0][0] = (short int *) calloc (1, sizeof (short int));} while ((__BASIC__STRUCT__(union_struct) -> sshi) [0][0][0] == NULL);
			} else {
				do {__BASIC__UNION__(union_struct) -> sshi = (short int ****) malloc (sizeof (short int ***));} while (__BASIC__UNION__(union_struct) -> sshi == NULL);
				do {(__BASIC__UNION__(union_struct) -> sshi) [0] = (short int ***) malloc (sizeof (short int **));} while ((__BASIC__UNION__(union_struct) -> sshi) [0] == NULL);
				do {(__BASIC__UNION__(union_struct) -> sshi) [0][0] = (short int **) malloc (sizeof (short int *));} while ((__BASIC__UNION__(union_struct) -> sshi) [0][0] == NULL);
				do {(__BASIC__UNION__(union_struct) -> sshi) [0][0][0] = (short int *) calloc (1, sizeof (short int));} while ((__BASIC__UNION__(union_struct) -> sshi) [0][0][0] == NULL);
			}
		} else if (how_many_dimensions == 1) {
			if (type) {
				do {__BASIC__STRUCT__(union_struct) -> sshi = (short int ****) malloc (sizeof (short int ***));} while (__BASIC__STRUCT__(union_struct) -> sshi == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> sshi) [0] = (short int ***) malloc (sizeof (short int **));} while ((__BASIC__STRUCT__(union_struct) -> sshi) [0] == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> sshi) [0][0] = (short int **) malloc (sizeof (short int *));} while ((__BASIC__STRUCT__(union_struct) -> sshi) [0][0] == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> sshi) [0][0][0] = (short int *) calloc (to_uint (experssion), sizeof (short int));} while ((__BASIC__STRUCT__(union_struct) -> sshi) [0][0][0] == NULL);
			} else {
				do {__BASIC__UNION__(union_struct) -> sshi = (short int ****) malloc (sizeof (short int ***));} while (__BASIC__UNION__(union_struct) -> sshi == NULL);
				do {(__BASIC__UNION__(union_struct) -> sshi) [0] = (short int ***) malloc (sizeof (short int **));} while ((__BASIC__UNION__(union_struct) -> sshi) [0] == NULL);
				do {(__BASIC__UNION__(union_struct) -> sshi) [0][0] = (short int **) malloc (sizeof (short int *));} while ((__BASIC__UNION__(union_struct) -> sshi) [0][0] == NULL);
				do {(__BASIC__UNION__(union_struct) -> sshi) [0][0][0] = (short int *) calloc (to_uint (experssion), sizeof (short int));} while ((__BASIC__UNION__(union_struct) -> sshi) [0][0][0] == NULL);
			}
		} else if (how_many_dimensions == 2) {
			size_for_each_dimension = sizes_for_union_struct_var_alloc (experssion);

			if (size_for_each_dimension != NULL) {
				//3*[1-2-3] or 3*2
				if (type) {
					do {__BASIC__STRUCT__(union_struct) -> sshi = (short int ****) malloc (sizeof (short int ***));} while (__BASIC__STRUCT__(union_struct) -> sshi == NULL);
					do {(__BASIC__STRUCT__(union_struct) -> sshi) [0] = (short int ***) malloc (sizeof (short int **));} while ((__BASIC__STRUCT__(union_struct) -> sshi) [0] == NULL);
					do {(__BASIC__STRUCT__(union_struct) -> sshi) [0][0] = (short int **) malloc (sizeof (short int *) * size_for_each_dimension [1][0]);} while ((__BASIC__STRUCT__(union_struct) -> sshi) [0][0] == NULL);
					
					if (size_for_each_dimension [0][0] == 1)
						for (unsigned int counter = 0; counter < size_for_each_dimension [1][0]; ++counter)
							do {(__BASIC__STRUCT__(union_struct) -> sshi) [0][0][counter] = (short int *) calloc (size_for_each_dimension [2][counter], sizeof (short int));} while ((__BASIC__STRUCT__(union_struct) -> sshi) [0][0][counter] == NULL);
					else
						for (unsigned int counter = 0; counter < size_for_each_dimension [1][0]; ++counter)
							do {(__BASIC__STRUCT__(union_struct) -> sshi) [0][0][counter] = (short int *) calloc (size_for_each_dimension [2][0], sizeof (short int));} while ((__BASIC__STRUCT__(union_struct) -> sshi) [0][0][counter] == NULL);
				} else {
					do {__BASIC__UNION__(union_struct) -> sshi = (short int ****) malloc (sizeof (short int ***));} while (__BASIC__UNION__(union_struct) -> sshi == NULL);
					do {(__BASIC__UNION__(union_struct) -> sshi) [0] = (short int ***) malloc (sizeof (short int **));} while ((__BASIC__UNION__(union_struct) -> sshi) [0] == NULL);
					do {(__BASIC__UNION__(union_struct) -> sshi) [0][0] = (short int **) malloc (sizeof (short int *) * size_for_each_dimension [1][0]);} while ((__BASIC__UNION__(union_struct) -> sshi) [0][0] == NULL);
					
					if (size_for_each_dimension [0][0] == 1)
						for (unsigned int counter = 0; counter < size_for_each_dimension [1][0]; ++counter)
							do {(__BASIC__UNION__(union_struct) -> sshi) [0][0][counter] = (short int *) calloc (size_for_each_dimension [2][counter], sizeof (short int));} while ((__BASIC__UNION__(union_struct) -> sshi) [0][0][counter] == NULL);
					else
						for (unsigned int counter = 0; counter < size_for_each_dimension [1][0]; ++counter)
							do {(__BASIC__UNION__(union_struct) -> sshi) [0][0][counter] = (short int *) calloc (size_for_each_dimension [2][0], sizeof (short int));} while ((__BASIC__UNION__(union_struct) -> sshi) [0][0][counter] == NULL);
				}
			} else
				return 0;

		} else if (how_many_dimensions == 3) {
			size_for_each_dimension = sizes_for_union_struct_var_alloc (experssion);

			if (size_for_each_dimension != NULL) {
				if (type) {
					do {__BASIC__STRUCT__(union_struct) -> sshi = (short int ****) malloc (sizeof (short int ***));} while (__BASIC__STRUCT__(union_struct) -> sshi == NULL);
					
					//Allocate 3d array
					do {(__BASIC__STRUCT__(union_struct) -> sshi) [0] = (short int ***) malloc (sizeof (short int **) * size_for_each_dimension [1][0]);} while ((__BASIC__STRUCT__(union_struct) -> sshi) [0] == NULL);

					//Allocate 2d for each index of 3d
					for (unsigned int _3 = 0; _3 < size_for_each_dimension [1][0]; _3++) {
						if (size_for_each_dimension [0][0] == 1)
							do {(__BASIC__STRUCT__(union_struct) -> sshi) [0][_3] = (short int **) malloc (sizeof (short int *) * size_for_each_dimension [2][_3]);} while ((__BASIC__STRUCT__(union_struct) -> sshi) [0][_3] == NULL);
						else
							do {(__BASIC__STRUCT__(union_struct) -> sshi) [0][_3] = (short int **) malloc (sizeof (short int *) * size_for_each_dimension [2][0]);} while ((__BASIC__STRUCT__(union_struct) -> sshi) [0][_3] == NULL);
					}

					//Allocate 1d for each index of 2d.
					unsigned int start_from = 0;
					for (unsigned int _3 = 0; _3 < size_for_each_dimension [1][0]; ++_3) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned int _2 = 0; _2 < size_for_each_dimension [2][_3]; _2++) {
								if (size_for_each_dimension [1][0] == 1) {
									do {(__BASIC__STRUCT__(union_struct) -> sshi) [0][_3][_2] = (short int *) calloc (size_for_each_dimension [3][start_from], sizeof (short int));} while ((__BASIC__STRUCT__(union_struct) -> sshi) [0][_3][_2] == NULL);
									++start_from;
								} else
									do {(__BASIC__STRUCT__(union_struct) -> sshi) [0][_3][_2] = (short int *) calloc (size_for_each_dimension [3][0], sizeof (short int));} while ((__BASIC__STRUCT__(union_struct) -> sshi) [0][_3][_2] == NULL);
							}
						} else {
							for (unsigned int _2 = 0; _2 < size_for_each_dimension [2][0]; _2++) {
								if (size_for_each_dimension [1][0] == 1)
									do {(__BASIC__STRUCT__(union_struct) -> sshi) [0][_3][_2] = (short int *) calloc (size_for_each_dimension [3][_2], sizeof (short int));} while ((__BASIC__STRUCT__(union_struct) -> sshi) [0][_3][_2] == NULL);
								else
									do {(__BASIC__STRUCT__(union_struct) -> sshi) [0][_3][_2] = (short int *) calloc (size_for_each_dimension [3][0], sizeof (short int));} while ((__BASIC__STRUCT__(union_struct) -> sshi) [0][_3][_2] == NULL);
							}
						}
					}
				} else {	
					do {__BASIC__UNION__(union_struct) -> sshi = (short int ****) malloc (sizeof (short int ***));} while (__BASIC__UNION__(union_struct) -> sshi == NULL);
					
					//Allocate 3d array
					do {(__BASIC__UNION__(union_struct) -> sshi) [0] = (short int ***) malloc (sizeof (short int **) * size_for_each_dimension [1][0]);} while ((__BASIC__UNION__(union_struct) -> sshi) [0] == NULL);

					//Allocate 2d for each index of 3d
					for (unsigned int _3 = 0; _3 < size_for_each_dimension [1][0]; _3++) {
						if (size_for_each_dimension [0][0] == 1)
							do {(__BASIC__UNION__(union_struct) -> sshi) [0][_3] = (short int **) malloc (sizeof (short int *) * size_for_each_dimension [2][_3]);} while ((__BASIC__UNION__(union_struct) -> sshi) [0][_3] == NULL);
						else
							do {(__BASIC__UNION__(union_struct) -> sshi) [0][_3] = (short int **) malloc (sizeof (short int *) * size_for_each_dimension [2][0]);} while ((__BASIC__UNION__(union_struct) -> sshi) [0][_3] == NULL);
					}

					//Allocate 1d for each index of 2d.
					unsigned int start_from = 0;
					for (unsigned int _3 = 0; _3 < size_for_each_dimension [1][0]; ++_3) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned int _2 = 0; _2 < size_for_each_dimension [2][_3]; _2++) {
								if (size_for_each_dimension [1][0] == 1) {
									do {(__BASIC__UNION__(union_struct) -> sshi) [0][_3][_2] = (short int *) calloc (size_for_each_dimension [3][start_from], sizeof (short int));} while ((__BASIC__UNION__(union_struct) -> sshi) [0][_3][_2] == NULL);
									++start_from;
								} else
									do {(__BASIC__UNION__(union_struct) -> sshi) [0][_3][_2] = (short int *) calloc (size_for_each_dimension [3][0], sizeof (short int));} while ((__BASIC__UNION__(union_struct) -> sshi) [0][_3][_2] == NULL);
							}
						} else {
							for (unsigned int _2 = 0; _2 < size_for_each_dimension [2][0]; _2++) {
								if (size_for_each_dimension [1][0] == 1)
									do {(__BASIC__UNION__(union_struct) -> sshi) [0][_3][_2] = (short int *) calloc (size_for_each_dimension [3][_2], sizeof (short int));} while ((__BASIC__UNION__(union_struct) -> sshi) [0][_3][_2] == NULL);
								else
									do {(__BASIC__UNION__(union_struct) -> sshi) [0][_3][_2] = (short int *) calloc (size_for_each_dimension [3][0], sizeof (short int));} while ((__BASIC__UNION__(union_struct) -> sshi) [0][_3][_2] == NULL);
							}
						}
					}
				}
			} else
				return 0;

		} else {
			size_for_each_dimension = sizes_for_union_struct_var_alloc (experssion);

			if (size_for_each_dimension != NULL) {
				if (type) {
					do {__BASIC__STRUCT__(union_struct) -> sshi = (short int ****) malloc (sizeof (short int ***) * size_for_each_dimension [1][0]);} while (__BASIC__STRUCT__(union_struct) -> sshi == NULL);

					//Allocate 3d for each index of 4d array.
					for (unsigned int _4 = 0; _4 < size_for_each_dimension [1][0]; _4++) {
						if (size_for_each_dimension [0][0] == 1)
							do {(__BASIC__STRUCT__(union_struct) -> sshi) [_4] = (short int ***) malloc (sizeof (short int **) * size_for_each_dimension [2][_4]);} while ((__BASIC__STRUCT__(union_struct) -> sshi) [_4] == NULL);
						else
							do {(__BASIC__STRUCT__(union_struct) -> sshi) [_4] = (short int ***) malloc (sizeof (short int **) * size_for_each_dimension [2][0]);} while ((__BASIC__STRUCT__(union_struct) -> sshi) [_4] == NULL);
					}

					unsigned int start_from = 0;
					
					//Allocate 2d for each index of 3d array.
					for (unsigned int _4 = 0; _4 < size_for_each_dimension [1][0]; _4++) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][_4]; _3++) {
								if (size_for_each_dimension [0][1] == 1) {
									do {(__BASIC__STRUCT__(union_struct) -> sshi) [_4][_3] = (short int **) malloc (sizeof (short int *) * size_for_each_dimension [3][start_from]);} while ((__BASIC__STRUCT__(union_struct) -> sshi) [_4][_3] == NULL);
									++start_from;
								} else
									do {(__BASIC__STRUCT__(union_struct) -> sshi) [_4][_3] = (short int **) malloc (sizeof (short int *) * size_for_each_dimension [3][0]);} while ((__BASIC__STRUCT__(union_struct) -> sshi) [_4][_3] == NULL);
							}
						} else {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][0]; _3++) {
								if (size_for_each_dimension [0][1] == 1)
									do {(__BASIC__STRUCT__(union_struct) -> sshi) [_4][_3] = (short int **) malloc (sizeof (short int *) * size_for_each_dimension [3][_3]);} while ((__BASIC__STRUCT__(union_struct) -> sshi) [_4][_3] == NULL);
								else
									do {(__BASIC__STRUCT__(union_struct) -> sshi) [_4][_3] = (short int **) malloc (sizeof (short int *) * size_for_each_dimension [3][0]);} while ((__BASIC__STRUCT__(union_struct) -> sshi) [_4][_3] == NULL);
							}
						}
					}

					start_from = 0;

					//Allocate 1d for each index of 2d array.
					for (unsigned int _4 = 0; _4 < size_for_each_dimension [1][0]; ++_4) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][_4]; ++_3) {
								if (size_for_each_dimension [0][1] == 1) {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][_3]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__STRUCT__(union_struct) -> sshi) [_4][_3][_2] = (short int *) calloc (size_for_each_dimension [4][start_from], sizeof (short int));} while ((__BASIC__STRUCT__(union_struct) -> sshi) [_4][_3][_2] == NULL);
											++start_from;
										} else
											do {(__BASIC__STRUCT__(union_struct) -> sshi) [_4][_3][_2] = (short int *) calloc (size_for_each_dimension [4][0], sizeof (short int));} while ((__BASIC__STRUCT__(union_struct) -> sshi) [_4][_3][_2] == NULL);
									}
								} else {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][0]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__STRUCT__(union_struct) -> sshi) [_4][_3][_2] = (short int *) calloc (size_for_each_dimension [4][_2], sizeof (short int));} while ((__BASIC__STRUCT__(union_struct) -> sshi) [_4][_3][_2] == NULL);
										} else
											do {(__BASIC__STRUCT__(union_struct) -> sshi) [_4][_3][_2] = (short int *) calloc (size_for_each_dimension [4][0], sizeof (short int));} while ((__BASIC__STRUCT__(union_struct) -> sshi) [_4][_3][_2] == NULL);
									}
								}
							}
						} else {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][0]; ++_3) {
								if (size_for_each_dimension [0][1] == 1) {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][_3]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__STRUCT__(union_struct) -> sshi) [_4][_3][_2] = (short int *) calloc (size_for_each_dimension [4][start_from], sizeof (short int));} while ((__BASIC__STRUCT__(union_struct) -> sshi) [_4][_3][_2] == NULL);
											++start_from;
										} else
											do {(__BASIC__STRUCT__(union_struct) -> sshi) [_4][_3][_2] = (short int *) calloc (size_for_each_dimension [4][0], sizeof (short int));} while ((__BASIC__STRUCT__(union_struct) -> sshi) [_4][_3][_2] == NULL);
									}
								} else {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][0]; ++_2) {
										if (size_for_each_dimension [0][2] == 1)
											do {(__BASIC__STRUCT__(union_struct) -> sshi) [_4][_3][_2] = (short int *) calloc (size_for_each_dimension [4][_2], sizeof (short int));} while ((__BASIC__STRUCT__(union_struct) -> sshi) [_4][_3][_2] == NULL);
										else
											do {(__BASIC__STRUCT__(union_struct) -> sshi) [_4][_3][_2] = (short int *) calloc (size_for_each_dimension [4][0], sizeof (short int));} while ((__BASIC__STRUCT__(union_struct) -> sshi) [_4][_3][_2] == NULL);
									}
								}
							}
						}
					}
				} else {
					do {__BASIC__UNION__(union_struct) -> sshi = (short int ****) malloc (sizeof (short int ***) * size_for_each_dimension [1][0]);} while (__BASIC__UNION__(union_struct) -> sshi == NULL);

					//Allocate 3d for each index of 4d array.
					for (unsigned int _4 = 0; _4 < size_for_each_dimension [1][0]; _4++) {
						if (size_for_each_dimension [0][0] == 1)
							do {(__BASIC__UNION__(union_struct) -> sshi) [_4] = (short int ***) malloc (sizeof (short int **) * size_for_each_dimension [2][_4]);} while ((__BASIC__UNION__(union_struct) -> sshi) [_4] == NULL);
						else
							do {(__BASIC__UNION__(union_struct) -> sshi) [_4] = (short int ***) malloc (sizeof (short int **) * size_for_each_dimension [2][0]);} while ((__BASIC__UNION__(union_struct) -> sshi) [_4] == NULL);
					}

					unsigned int start_from = 0;
					
					//Allocate 2d for each index of 3d array.
					for (unsigned int _4 = 0; _4 < size_for_each_dimension [1][0]; _4++) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][_4]; _3++) {
								if (size_for_each_dimension [0][1] == 1) {
									do {(__BASIC__UNION__(union_struct) -> sshi) [_4][_3] = (short int **) malloc (sizeof (short int *) * size_for_each_dimension [3][start_from]);} while ((__BASIC__UNION__(union_struct) -> sshi) [_4][_3] == NULL);
									++start_from;
								} else
									do {(__BASIC__UNION__(union_struct) -> sshi) [_4][_3] = (short int **) malloc (sizeof (short int *) * size_for_each_dimension [3][0]);} while ((__BASIC__UNION__(union_struct) -> sshi) [_4][_3] == NULL);
							}
						} else {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][0]; _3++) {
								if (size_for_each_dimension [0][1] == 1)
									do {(__BASIC__UNION__(union_struct) -> sshi) [_4][_3] = (short int **) malloc (sizeof (short int *) * size_for_each_dimension [3][_3]);} while ((__BASIC__UNION__(union_struct) -> sshi) [_4][_3] == NULL);
								else
									do {(__BASIC__UNION__(union_struct) -> sshi) [_4][_3] = (short int **) malloc (sizeof (short int *) * size_for_each_dimension [3][0]);} while ((__BASIC__UNION__(union_struct) -> sshi) [_4][_3] == NULL);
							}
						}
					}

					start_from = 0;

					//Allocate 1d for each index of 2d array.
					for (unsigned int _4 = 0; _4 < size_for_each_dimension [1][0]; ++_4) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][_4]; ++_3) {
								if (size_for_each_dimension [0][1] == 1) {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][_3]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__UNION__(union_struct) -> sshi) [_4][_3][_2] = (short int *) calloc (size_for_each_dimension [4][start_from], sizeof (short int));} while ((__BASIC__UNION__(union_struct) -> sshi) [_4][_3][_2] == NULL);
											++start_from;
										} else
											do {(__BASIC__UNION__(union_struct) -> sshi) [_4][_3][_2] = (short int *) calloc (size_for_each_dimension [4][0], sizeof (short int));} while ((__BASIC__UNION__(union_struct) -> sshi) [_4][_3][_2] == NULL);
									}
								} else {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][0]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__UNION__(union_struct) -> sshi) [_4][_3][_2] = (short int *) calloc (size_for_each_dimension [4][_2], sizeof (short int));} while ((__BASIC__UNION__(union_struct) -> sshi) [_4][_3][_2] == NULL);
										} else
											do {(__BASIC__UNION__(union_struct) -> sshi) [_4][_3][_2] = (short int *) calloc (size_for_each_dimension [4][0], sizeof (short int));} while ((__BASIC__UNION__(union_struct) -> sshi) [_4][_3][_2] == NULL);
									}
								}
							}
						} else {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][0]; ++_3) {
								if (size_for_each_dimension [0][1] == 1) {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][_3]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__UNION__(union_struct) -> sshi) [_4][_3][_2] = (short int *) calloc (size_for_each_dimension [4][start_from], sizeof (short int));} while ((__BASIC__UNION__(union_struct) -> sshi) [_4][_3][_2] == NULL);
											++start_from;
										} else
											do {(__BASIC__UNION__(union_struct) -> sshi) [_4][_3][_2] = (short int *) calloc (size_for_each_dimension [4][0], sizeof (short int));} while ((__BASIC__UNION__(union_struct) -> sshi) [_4][_3][_2] == NULL);
									}
								} else {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][0]; ++_2) {
										if (size_for_each_dimension [0][2] == 1)
											do {(__BASIC__UNION__(union_struct) -> sshi) [_4][_3][_2] = (short int *) calloc (size_for_each_dimension [4][_2], sizeof (short int));} while ((__BASIC__UNION__(union_struct) -> sshi) [_4][_3][_2] == NULL);
										else
											do {(__BASIC__UNION__(union_struct) -> sshi) [_4][_3][_2] = (short int *) calloc (size_for_each_dimension [4][0], sizeof (short int));} while ((__BASIC__UNION__(union_struct) -> sshi) [_4][_3][_2] == NULL);
									}
								}
							}
						}
					}
				}
			}
		}
	} else if (dtype_code == 4) {
		if (type) {
			if (__BASIC__STRUCT__(union_struct) -> ushi != NULL)
				return 1;
		} else {
			if (__BASIC__UNION__(union_struct) -> ushi != NULL)
				return 1;
		}

		if (how_many_dimensions == 0) {
			if (type) {
				do {__BASIC__STRUCT__(union_struct) -> ushi = (unsigned short int ****) malloc (sizeof (unsigned short int ***));} while (__BASIC__STRUCT__(union_struct) -> ushi == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> ushi) [0] = (unsigned short int ***) malloc (sizeof (unsigned short int **));} while ((__BASIC__STRUCT__(union_struct) -> ushi) [0] == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> ushi) [0][0] = (unsigned short int **) malloc (sizeof (unsigned short int *));} while ((__BASIC__STRUCT__(union_struct) -> ushi) [0][0] == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> ushi) [0][0][0] = (unsigned short int *) calloc (1, sizeof (unsigned short int));} while ((__BASIC__STRUCT__(union_struct) -> ushi) [0][0][0] == NULL);
			} else {
				do {__BASIC__UNION__(union_struct) -> ushi = (unsigned short int ****) malloc (sizeof (unsigned short int ***));} while (__BASIC__UNION__(union_struct) -> ushi == NULL);
				do {(__BASIC__UNION__(union_struct) -> ushi) [0] = (unsigned short int ***) malloc (sizeof (unsigned short int **));} while ((__BASIC__UNION__(union_struct) -> ushi) [0] == NULL);
				do {(__BASIC__UNION__(union_struct) -> ushi) [0][0] = (unsigned short int **) malloc (sizeof (unsigned short int *));} while ((__BASIC__UNION__(union_struct) -> ushi) [0][0] == NULL);
				do {(__BASIC__UNION__(union_struct) -> ushi) [0][0][0] = (unsigned short int *) calloc (1, sizeof (unsigned short int));} while ((__BASIC__UNION__(union_struct) -> ushi) [0][0][0] == NULL);
			}
		} else if (how_many_dimensions == 1) {
			if (type) {
				do {__BASIC__STRUCT__(union_struct) -> ushi = (unsigned short int ****) malloc (sizeof (unsigned short int ***));} while (__BASIC__STRUCT__(union_struct) -> ushi == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> ushi) [0] = (unsigned short int ***) malloc (sizeof (unsigned short int **));} while ((__BASIC__STRUCT__(union_struct) -> ushi) [0] == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> ushi) [0][0] = (unsigned short int **) malloc (sizeof (unsigned short int *));} while ((__BASIC__STRUCT__(union_struct) -> ushi) [0][0] == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> ushi) [0][0][0] = (unsigned short int *) calloc (to_uint (experssion), sizeof (unsigned short int));} while ((__BASIC__STRUCT__(union_struct) -> ushi) [0][0][0] == NULL);
			} else {
				do {__BASIC__UNION__(union_struct) -> ushi = (unsigned short int ****) malloc (sizeof (unsigned short int ***));} while (__BASIC__UNION__(union_struct) -> ushi == NULL);
				do {(__BASIC__UNION__(union_struct) -> ushi) [0] = (unsigned short int ***) malloc (sizeof (unsigned short int **));} while ((__BASIC__UNION__(union_struct) -> ushi) [0] == NULL);
				do {(__BASIC__UNION__(union_struct) -> ushi) [0][0] = (unsigned short int **) malloc (sizeof (unsigned short int *));} while ((__BASIC__UNION__(union_struct) -> ushi) [0][0] == NULL);
				do {(__BASIC__UNION__(union_struct) -> ushi) [0][0][0] = (unsigned short int *) calloc (to_uint (experssion), sizeof (unsigned short int));} while ((__BASIC__UNION__(union_struct) -> ushi) [0][0][0] == NULL);
			}
		} else if (how_many_dimensions == 2) {
			size_for_each_dimension = sizes_for_union_struct_var_alloc (experssion);

			if (size_for_each_dimension != NULL) {
				//3*[1-2-3] or 3*2
				if (type) {
					do {__BASIC__STRUCT__(union_struct) -> ushi = (unsigned short int ****) malloc (sizeof (unsigned short int ***));} while (__BASIC__STRUCT__(union_struct) -> ushi == NULL);
					do {(__BASIC__STRUCT__(union_struct) -> ushi) [0] = (unsigned short int ***) malloc (sizeof (unsigned short int **));} while ((__BASIC__STRUCT__(union_struct) -> ushi) [0] == NULL);
					do {(__BASIC__STRUCT__(union_struct) -> ushi) [0][0] = (unsigned short int **) malloc (sizeof (unsigned short int *) * size_for_each_dimension [1][0]);} while ((__BASIC__STRUCT__(union_struct) -> ushi) [0][0] == NULL);
					
					if (size_for_each_dimension [0][0] == 1)
						for (unsigned int counter = 0; counter < size_for_each_dimension [1][0]; ++counter)
							do {(__BASIC__STRUCT__(union_struct) -> ushi) [0][0][counter] = (unsigned short int *) calloc (size_for_each_dimension [2][counter], sizeof (unsigned short int));} while ((__BASIC__STRUCT__(union_struct) -> ushi) [0][0][counter] == NULL);
					else
						for (unsigned int counter = 0; counter < size_for_each_dimension [1][0]; ++counter)
							do {(__BASIC__STRUCT__(union_struct) -> ushi [0][0][counter]) = (unsigned short int *) calloc (size_for_each_dimension [2][0], sizeof (unsigned short int));} while ((__BASIC__STRUCT__(union_struct) -> ushi) [0][0][counter] == NULL);
				} else {
					do {__BASIC__UNION__(union_struct) -> ushi = (unsigned short int ****) malloc (sizeof (unsigned short int ***));} while (__BASIC__UNION__(union_struct) -> ushi == NULL);
					do {(__BASIC__UNION__(union_struct) -> ushi) [0] = (unsigned short int ***) malloc (sizeof (unsigned short int **));} while ((__BASIC__UNION__(union_struct) -> ushi) [0] == NULL);
					do {(__BASIC__UNION__(union_struct) -> ushi) [0][0] = (unsigned short int **) malloc (sizeof (unsigned short int *) * size_for_each_dimension [1][0]);} while ((__BASIC__UNION__(union_struct) -> ushi) [0][0] == NULL);
					
					if (size_for_each_dimension [0][0] == 1)
						for (unsigned int counter = 0; counter < size_for_each_dimension [1][0]; ++counter)
							do {(__BASIC__UNION__(union_struct) -> ushi) [0][0][counter] = (unsigned short int *) calloc (size_for_each_dimension [2][counter], sizeof (unsigned short int));} while ((__BASIC__UNION__(union_struct) -> ushi) [0][0][counter] == NULL);
					else
						for (unsigned int counter = 0; counter < size_for_each_dimension [1][0]; ++counter)
							do {(__BASIC__UNION__(union_struct) -> ushi) [0][0][counter] = (unsigned short int *) calloc (size_for_each_dimension [2][0], sizeof (unsigned short int));} while ((__BASIC__UNION__(union_struct) -> ushi) [0][0][counter] == NULL);
				}
			} else
				return 0;

		} else if (how_many_dimensions == 3) {
			size_for_each_dimension = sizes_for_union_struct_var_alloc (experssion);

			if (size_for_each_dimension != NULL) {
				if (type) {
					do {__BASIC__STRUCT__(union_struct) -> ushi = (unsigned short int ****) malloc (sizeof (unsigned short int ***));} while (__BASIC__STRUCT__(union_struct) -> ushi == NULL);
					
					//Allocate 3d array
					do {(__BASIC__STRUCT__(union_struct) -> ushi) [0] = (unsigned short int ***) malloc (sizeof (unsigned short int **) * size_for_each_dimension [1][0]);} while ((__BASIC__STRUCT__(union_struct) -> ushi) [0] == NULL);

					//Allocate 2d for each index of 3d
					for (unsigned int _3 = 0; _3 < size_for_each_dimension [1][0]; _3++) {
						if (size_for_each_dimension [0][0] == 1)
							do {(__BASIC__STRUCT__(union_struct) -> ushi) [0][_3] = (unsigned short int **) malloc (sizeof (unsigned short int *) * size_for_each_dimension [2][_3]);} while ((__BASIC__STRUCT__(union_struct) -> ushi) [0][_3] == NULL);
						else
							do {(__BASIC__STRUCT__(union_struct) -> ushi) [0][_3] = (unsigned short int **) malloc (sizeof (unsigned short int *) * size_for_each_dimension [2][0]);} while ((__BASIC__STRUCT__(union_struct) -> ushi) [0][_3] == NULL);
					}

					//Allocate 1d for each index of 2d.
					unsigned int start_from = 0;
					for (unsigned int _3 = 0; _3 < size_for_each_dimension [1][0]; ++_3) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned int _2 = 0; _2 < size_for_each_dimension [2][_3]; _2++) {
								if (size_for_each_dimension [1][0] == 1) {
									do {(__BASIC__STRUCT__(union_struct) -> ushi) [0][_3][_2] = (unsigned short int *) calloc (size_for_each_dimension [3][start_from], sizeof (unsigned short int));} while ((__BASIC__STRUCT__(union_struct) -> ushi) [0][_3][_2] == NULL);
									++start_from;
								} else
									do {(__BASIC__STRUCT__(union_struct) -> ushi) [0][_3][_2] = (unsigned short int *) calloc (size_for_each_dimension [3][0], sizeof (unsigned short int));} while ((__BASIC__STRUCT__(union_struct) -> ushi) [0][_3][_2] == NULL);
							}
						} else {
							for (unsigned int _2 = 0; _2 < size_for_each_dimension [2][0]; _2++) {
								if (size_for_each_dimension [1][0] == 1)
									do {(__BASIC__STRUCT__(union_struct) -> ushi) [0][_3][_2] = (unsigned short int *) calloc (size_for_each_dimension [3][_2], sizeof (unsigned short int));} while ((__BASIC__STRUCT__(union_struct) -> ushi) [0][_3][_2] == NULL);
								else
									do {(__BASIC__STRUCT__(union_struct) -> ushi) [0][_3][_2] = (unsigned short int *) calloc (size_for_each_dimension [3][0], sizeof (unsigned short int));} while ((__BASIC__STRUCT__(union_struct) -> ushi) [0][_3][_2] == NULL);
							}
						}
					}
				} else {
					do {__BASIC__UNION__(union_struct) -> ushi = (unsigned short int ****) malloc (sizeof (unsigned short int ***));} while (__BASIC__UNION__(union_struct) -> ushi == NULL);
					
					//Allocate 3d array
					do {(__BASIC__UNION__(union_struct) -> ushi) [0] = (unsigned short int ***) malloc (sizeof (unsigned short int **) * size_for_each_dimension [1][0]);} while ((__BASIC__UNION__(union_struct) -> ushi) [0] == NULL);

					//Allocate 2d for each index of 3d
					for (unsigned int _3 = 0; _3 < size_for_each_dimension [1][0]; _3++) {
						if (size_for_each_dimension [0][0] == 1)
							do {(__BASIC__UNION__(union_struct) -> ushi) [0][_3] = (unsigned short int **) malloc (sizeof (unsigned short int *) * size_for_each_dimension [2][_3]);} while ((__BASIC__UNION__(union_struct) -> ushi) [0][_3] == NULL);
						else
							do {(__BASIC__UNION__(union_struct) -> ushi) [0][_3] = (unsigned short int **) malloc (sizeof (unsigned short int *) * size_for_each_dimension [2][0]);} while ((__BASIC__UNION__(union_struct) -> ushi) [0][_3] == NULL);
					}

					//Allocate 1d for each index of 2d.
					unsigned int start_from = 0;
					for (unsigned int _3 = 0; _3 < size_for_each_dimension [1][0]; ++_3) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned int _2 = 0; _2 < size_for_each_dimension [2][_3]; _2++) {
								if (size_for_each_dimension [1][0] == 1) {
									do {(__BASIC__UNION__(union_struct) -> ushi) [0][_3][_2] = (unsigned short int *) calloc (size_for_each_dimension [3][start_from], sizeof (unsigned short int));} while ((__BASIC__UNION__(union_struct) -> ushi) [0][_3][_2] == NULL);
									++start_from;
								} else
									do {(__BASIC__UNION__(union_struct) -> ushi) [0][_3][_2] = (unsigned short int *) calloc (size_for_each_dimension [3][0], sizeof (unsigned short int));} while ((__BASIC__UNION__(union_struct) -> ushi) [0][_3][_2] == NULL);
							}
						} else {
							for (unsigned int _2 = 0; _2 < size_for_each_dimension [2][0]; _2++) {
								if (size_for_each_dimension [1][0] == 1)
									do {(__BASIC__UNION__(union_struct) -> ushi) [0][_3][_2] = (unsigned short int *) calloc (size_for_each_dimension [3][_2], sizeof (unsigned short int));} while ((__BASIC__UNION__(union_struct) -> ushi) [0][_3][_2] == NULL);
								else
									do {(__BASIC__UNION__(union_struct) -> ushi) [0][_3][_2] = (unsigned short int *) calloc (size_for_each_dimension [3][0], sizeof (unsigned short int));} while ((__BASIC__UNION__(union_struct) -> ushi) [0][_3][_2] == NULL);
							}
						}
					}
				}
			} else
				return 0;

		} else {
			size_for_each_dimension = sizes_for_union_struct_var_alloc (experssion);

			if (size_for_each_dimension != NULL) {
				if (type) {
					do {__BASIC__STRUCT__(union_struct) -> ushi = (unsigned short int ****) malloc (sizeof (unsigned short int ***) * size_for_each_dimension [1][0]);} while (__BASIC__STRUCT__(union_struct) -> ushi == NULL);

					//Allocate 3d for each index of 4d array.
					for (unsigned int _4 = 0; _4 < size_for_each_dimension [1][0]; _4++) {
						if (size_for_each_dimension [0][0] == 1)
							do {(__BASIC__STRUCT__(union_struct) -> ushi) [_4] = (unsigned short int ***) malloc (sizeof (unsigned short int **) * size_for_each_dimension [2][_4]);} while ((__BASIC__STRUCT__(union_struct) -> ushi) [_4] == NULL);
						else
							do {(__BASIC__STRUCT__(union_struct) -> ushi) [_4] = (unsigned short int ***) malloc (sizeof (unsigned short int **) * size_for_each_dimension [2][0]);} while ((__BASIC__STRUCT__(union_struct) -> ushi) [_4] == NULL);
					}

					unsigned int start_from = 0;
					
					//Allocate 2d for each index of 3d array.
					for (unsigned int _4 = 0; _4 < size_for_each_dimension [1][0]; _4++) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][_4]; _3++) {
								if (size_for_each_dimension [0][1] == 1) {
									do {(__BASIC__STRUCT__(union_struct) -> ushi) [_4][_3] = (unsigned short int **) malloc (sizeof (unsigned short int *) * size_for_each_dimension [3][start_from]);} while ((__BASIC__STRUCT__(union_struct) -> ushi) [_4][_3] == NULL);
									++start_from;
								} else
									do {(__BASIC__STRUCT__(union_struct) -> ushi) [_4][_3] = (unsigned short int **) malloc (sizeof (unsigned short int *) * size_for_each_dimension [3][0]);} while ((__BASIC__STRUCT__(union_struct) -> ushi) [_4][_3] == NULL);
							}
						} else {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][0]; _3++) {
								if (size_for_each_dimension [0][1] == 1)
									do {(__BASIC__STRUCT__(union_struct) -> ushi) [_4][_3] = (unsigned short int **) malloc (sizeof (unsigned short int *) * size_for_each_dimension [3][_3]);} while ((__BASIC__STRUCT__(union_struct) -> ushi) [_4][_3] == NULL);
								else
									do {(__BASIC__STRUCT__(union_struct) -> ushi) [_4][_3] = (unsigned short int **) malloc (sizeof (unsigned short int *) * size_for_each_dimension [3][0]);} while ((__BASIC__STRUCT__(union_struct) -> ushi) [_4][_3] == NULL);
							}
						}
					}

					start_from = 0;

					//Allocate 1d for each index of 2d array.
					for (unsigned int _4 = 0; _4 < size_for_each_dimension [1][0]; ++_4) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][_4]; ++_3) {
								if (size_for_each_dimension [0][1] == 1) {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][_3]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__STRUCT__(union_struct) -> ushi) [_4][_3][_2] = (unsigned short int *) calloc (size_for_each_dimension [4][start_from], sizeof (unsigned short int));} while ((__BASIC__STRUCT__(union_struct) -> ushi) [_4][_3][_2] == NULL);
											++start_from;
										} else
											do {(__BASIC__STRUCT__(union_struct) -> ushi) [_4][_3][_2] = (unsigned short int *) calloc (size_for_each_dimension [4][0], sizeof (unsigned short int));} while ((__BASIC__STRUCT__(union_struct) -> ushi) [_4][_3][_2] == NULL);
									}
								} else {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][0]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__STRUCT__(union_struct) -> ushi) [_4][_3][_2] = (unsigned short int *) calloc (size_for_each_dimension [4][_2], sizeof (unsigned short int));} while ((__BASIC__STRUCT__(union_struct) -> ushi) [_4][_3][_2] == NULL);
										} else
											do {(__BASIC__STRUCT__(union_struct) -> ushi) [_4][_3][_2] = (unsigned short int *) calloc (size_for_each_dimension [4][0], sizeof (unsigned short int));} while ((__BASIC__STRUCT__(union_struct) -> ushi) [_4][_3][_2] == NULL);
									}
								}
							}
						} else {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][0]; ++_3) {
								if (size_for_each_dimension [0][1] == 1) {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][_3]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__STRUCT__(union_struct) -> ushi) [_4][_3][_2] = (unsigned short int *) calloc (size_for_each_dimension [4][start_from], sizeof (unsigned short int));} while ((__BASIC__STRUCT__(union_struct) -> ushi) [_4][_3][_2] == NULL);
											++start_from;
										} else
											do {(__BASIC__STRUCT__(union_struct) -> ushi) [_4][_3][_2] = (unsigned short int *) calloc (size_for_each_dimension [4][0], sizeof (unsigned short int));} while ((__BASIC__STRUCT__(union_struct) -> ushi) [_4][_3][_2] == NULL);
									}
								} else {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][0]; ++_2) {
										if (size_for_each_dimension [0][2] == 1)
											do {(__BASIC__STRUCT__(union_struct) -> ushi) [_4][_3][_2] = (unsigned short int *) calloc (size_for_each_dimension [4][_2], sizeof (unsigned short int));} while ((__BASIC__STRUCT__(union_struct) -> ushi) [_4][_3][_2] == NULL);
										else
											do {(__BASIC__STRUCT__(union_struct) -> ushi) [_4][_3][_2] = (unsigned short int *) calloc (size_for_each_dimension [4][0], sizeof (unsigned short int));} while ((__BASIC__STRUCT__(union_struct) -> ushi) [_4][_3][_2] == NULL);
									}
								}
							}
						}
					}
				} else {
					do {__BASIC__UNION__(union_struct) -> ushi = (unsigned short int ****) malloc (sizeof (unsigned short int ***) * size_for_each_dimension [1][0]);} while (__BASIC__UNION__(union_struct) -> ushi == NULL);

					//Allocate 3d for each index of 4d array.
					for (unsigned int _4 = 0; _4 < size_for_each_dimension [1][0]; _4++) {
						if (size_for_each_dimension [0][0] == 1)
							do {(__BASIC__UNION__(union_struct) -> ushi) [_4] = (unsigned short int ***) malloc (sizeof (unsigned short int **) * size_for_each_dimension [2][_4]);} while ((__BASIC__UNION__(union_struct) -> ushi) [_4] == NULL);
						else
							do {(__BASIC__UNION__(union_struct) -> ushi) [_4] = (unsigned short int ***) malloc (sizeof (unsigned short int **) * size_for_each_dimension [2][0]);} while ((__BASIC__UNION__(union_struct) -> ushi) [_4] == NULL);
					}

					unsigned int start_from = 0;
					
					//Allocate 2d for each index of 3d array.
					for (unsigned int _4 = 0; _4 < size_for_each_dimension [1][0]; _4++) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][_4]; _3++) {
								if (size_for_each_dimension [0][1] == 1) {
									do {(__BASIC__UNION__(union_struct) -> ushi) [_4][_3] = (unsigned short int **) malloc (sizeof (unsigned short int *) * size_for_each_dimension [3][start_from]);} while ((__BASIC__UNION__(union_struct) -> ushi) [_4][_3] == NULL);
									++start_from;
								} else
									do {(__BASIC__UNION__(union_struct) -> ushi) [_4][_3] = (unsigned short int **) malloc (sizeof (unsigned short int *) * size_for_each_dimension [3][0]);} while ((__BASIC__UNION__(union_struct) -> ushi) [_4][_3] == NULL);
							}
						} else {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][0]; _3++) {
								if (size_for_each_dimension [0][1] == 1)
									do {(__BASIC__UNION__(union_struct) -> ushi) [_4][_3] = (unsigned short int **) malloc (sizeof (unsigned short int *) * size_for_each_dimension [3][_3]);} while ((__BASIC__UNION__(union_struct) -> ushi) [_4][_3] == NULL);
								else
									do {(__BASIC__UNION__(union_struct) -> ushi) [_4][_3] = (unsigned short int **) malloc (sizeof (unsigned short int *) * size_for_each_dimension [3][0]);} while ((__BASIC__UNION__(union_struct) -> ushi) [_4][_3] == NULL);
							}
						}
					}

					start_from = 0;

					//Allocate 1d for each index of 2d array.
					for (unsigned int _4 = 0; _4 < size_for_each_dimension [1][0]; ++_4) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][_4]; ++_3) {
								if (size_for_each_dimension [0][1] == 1) {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][_3]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__UNION__(union_struct) -> ushi) [_4][_3][_2] = (unsigned short int *) calloc (size_for_each_dimension [4][start_from], sizeof (unsigned short int));} while ((__BASIC__UNION__(union_struct) -> ushi) [_4][_3][_2] == NULL);
											++start_from;
										} else
											do {(__BASIC__UNION__(union_struct) -> ushi) [_4][_3][_2] = (unsigned short int *) calloc (size_for_each_dimension [4][0], sizeof (unsigned short int));} while ((__BASIC__UNION__(union_struct) -> ushi) [_4][_3][_2] == NULL);
									}
								} else {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][0]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__UNION__(union_struct) -> ushi) [_4][_3][_2] = (unsigned short int *) calloc (size_for_each_dimension [4][_2], sizeof (unsigned short int));} while ((__BASIC__UNION__(union_struct) -> ushi) [_4][_3][_2] == NULL);
										} else
											do {(__BASIC__UNION__(union_struct) -> ushi) [_4][_3][_2] = (unsigned short int *) calloc (size_for_each_dimension [4][0], sizeof (unsigned short int));} while ((__BASIC__UNION__(union_struct) -> ushi) [_4][_3][_2] == NULL);
									}
								}
							}
						} else {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][0]; ++_3) {
								if (size_for_each_dimension [0][1] == 1) {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][_3]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__UNION__(union_struct) -> ushi) [_4][_3][_2] = (unsigned short int *) calloc (size_for_each_dimension [4][start_from], sizeof (unsigned short int));} while ((__BASIC__UNION__(union_struct) -> ushi) [_4][_3][_2] == NULL);
											++start_from;
										} else
											do {(__BASIC__UNION__(union_struct) -> ushi) [_4][_3][_2] = (unsigned short int *) calloc (size_for_each_dimension [4][0], sizeof (unsigned short int));} while ((__BASIC__UNION__(union_struct) -> ushi) [_4][_3][_2] == NULL);
									}
								} else {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][0]; ++_2) {
										if (size_for_each_dimension [0][2] == 1)
											do {(__BASIC__UNION__(union_struct) -> ushi) [_4][_3][_2] = (unsigned short int *) calloc (size_for_each_dimension [4][_2], sizeof (unsigned short int));} while ((__BASIC__UNION__(union_struct) -> ushi) [_4][_3][_2] == NULL);
										else
											do {(__BASIC__UNION__(union_struct) -> ushi) [_4][_3][_2] = (unsigned short int *) calloc (size_for_each_dimension [4][0], sizeof (unsigned short int));} while ((__BASIC__UNION__(union_struct) -> ushi) [_4][_3][_2] == NULL);
									}
								}
							}
						}
					}
				}
			}
		}
	} else if (dtype_code == 5) {
		if (type) {
			if (__BASIC__STRUCT__(union_struct) -> sint != NULL)
				return 1;
		} else {
			if (__BASIC__UNION__(union_struct) -> sint != NULL)
				return 1;
		}

		if (how_many_dimensions == 0) {
			if (type) {
				do {__BASIC__STRUCT__(union_struct) -> sint = (int ****) malloc (sizeof (int ***));} while (__BASIC__STRUCT__(union_struct) -> sint == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> sint) [0] = (int ***) malloc (sizeof (int **));} while ((__BASIC__STRUCT__(union_struct) -> sint) [0] == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> sint) [0][0] = (int **) malloc (sizeof (int *));} while ((__BASIC__STRUCT__(union_struct) -> sint) [0][0] == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> sint) [0][0][0] = (int *) calloc (1, sizeof (int));} while ((__BASIC__STRUCT__(union_struct) -> sint) [0][0][0] == NULL);
			} else {
				do {__BASIC__UNION__(union_struct) -> sint = (int ****) malloc (sizeof (int ***));} while (__BASIC__UNION__(union_struct) -> sint == NULL);
				do {(__BASIC__UNION__(union_struct) -> sint) [0] = (int ***) malloc (sizeof (int **));} while ((__BASIC__UNION__(union_struct) -> sint) [0] == NULL);
				do {(__BASIC__UNION__(union_struct) -> sint) [0][0] = (int **) malloc (sizeof (int *));} while ((__BASIC__UNION__(union_struct) -> sint) [0][0] == NULL);
				do {(__BASIC__UNION__(union_struct) -> sint) [0][0][0] = (int *) calloc (1, sizeof (int));} while ((__BASIC__UNION__(union_struct) -> sint) [0][0][0] == NULL);
			}
		} else if (how_many_dimensions == 1) {
			if (type) {
				do {__BASIC__STRUCT__(union_struct) -> sint = (int ****) malloc (sizeof (int ***));} while (__BASIC__STRUCT__(union_struct) -> sint == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> sint) [0] = (int ***) malloc (sizeof (int **));} while ((__BASIC__STRUCT__(union_struct) -> sint) [0] == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> sint) [0][0] = (int **) malloc (sizeof (int *));} while ((__BASIC__STRUCT__(union_struct) -> sint) [0][0] == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> sint) [0][0][0] = (int *) calloc (to_uint (experssion), sizeof (int));} while ((__BASIC__STRUCT__(union_struct) -> sint) [0][0][0] == NULL);
			} else {
				do {__BASIC__UNION__(union_struct) -> sint = (int ****) malloc (sizeof (int ***));} while (__BASIC__UNION__(union_struct) -> sint == NULL);
				do {(__BASIC__UNION__(union_struct) -> sint) [0] = (int ***) malloc (sizeof (int **));} while ((__BASIC__UNION__(union_struct) -> sint) [0] == NULL);
				do {(__BASIC__UNION__(union_struct) -> sint) [0][0] = (int **) malloc (sizeof (int *));} while ((__BASIC__UNION__(union_struct) -> sint) [0][0] == NULL);
				do {(__BASIC__UNION__(union_struct) -> sint) [0][0][0] = (int *) calloc (to_uint (experssion), sizeof (int));} while ((__BASIC__UNION__(union_struct) -> sint) [0][0][0] == NULL);
			}
		} else if (how_many_dimensions == 2) {
			size_for_each_dimension = sizes_for_union_struct_var_alloc (experssion);

			if (size_for_each_dimension != NULL) {
				//3*[1-2-3] or 3*2
				if (type) {
					do {__BASIC__STRUCT__(union_struct) -> sint = (int ****) malloc (sizeof (int ***));} while (__BASIC__STRUCT__(union_struct) -> sint == NULL);
					do {(__BASIC__STRUCT__(union_struct) -> sint) [0] = (int ***) malloc (sizeof (int **));} while ((__BASIC__STRUCT__(union_struct) -> sint) [0] == NULL);
					do {(__BASIC__STRUCT__(union_struct) -> sint) [0][0] = (int **) malloc (sizeof (int *) * size_for_each_dimension [1][0]);} while ((__BASIC__STRUCT__(union_struct) -> sint) [0][0] == NULL);
					
					if (size_for_each_dimension [0][0] == 1)
						for (unsigned int counter = 0; counter < size_for_each_dimension [1][0]; ++counter)
							do {(__BASIC__STRUCT__(union_struct) -> sint) [0][0][counter] = (int *) calloc (size_for_each_dimension [2][counter], sizeof (int));} while ((__BASIC__STRUCT__(union_struct) -> sint) [0][0][counter] == NULL);
					else
						for (unsigned int counter = 0; counter < size_for_each_dimension [1][0]; ++counter)
							do {(__BASIC__STRUCT__(union_struct) -> sint) [0][0][counter] = (int *) calloc (size_for_each_dimension [2][0], sizeof (int));} while ((__BASIC__STRUCT__(union_struct) -> sint) [0][0][counter] == NULL);
				} else {
					do {__BASIC__UNION__(union_struct) -> sint = (int ****) malloc (sizeof (int ***));} while (__BASIC__UNION__(union_struct) -> sint == NULL);
					do {(__BASIC__UNION__(union_struct) -> sint) [0] = (int ***) malloc (sizeof (int **));} while ((__BASIC__UNION__(union_struct) -> sint) [0] == NULL);
					do {(__BASIC__UNION__(union_struct) -> sint) [0][0] = (int **) malloc (sizeof (int *) * size_for_each_dimension [1][0]);} while ((__BASIC__UNION__(union_struct) -> sint) [0][0] == NULL);
					
					if (size_for_each_dimension [0][0] == 1)
						for (unsigned int counter = 0; counter < size_for_each_dimension [1][0]; ++counter)
							do {(__BASIC__UNION__(union_struct) -> sint) [0][0][counter] = (int *) calloc (size_for_each_dimension [2][counter], sizeof (int));} while ((__BASIC__UNION__(union_struct) -> sint) [0][0][counter] == NULL);
					else
						for (unsigned int counter = 0; counter < size_for_each_dimension [1][0]; ++counter)
							do {(__BASIC__UNION__(union_struct) -> sint) [0][0][counter] = (int *) calloc (size_for_each_dimension [2][0], sizeof (int));} while ((__BASIC__UNION__(union_struct) -> sint) [0][0][counter] == NULL);
				}
			} else
				return 0;

		} else if (how_many_dimensions == 3) {
			size_for_each_dimension = sizes_for_union_struct_var_alloc (experssion);

			if (size_for_each_dimension != NULL) {
				if (type) {
					do {__BASIC__STRUCT__(union_struct) -> sint = (int ****) malloc (sizeof (int ***));} while (__BASIC__STRUCT__(union_struct) -> sint == NULL);
					
					//Allocate 3d array
					do {(__BASIC__STRUCT__(union_struct) -> sint) [0] = (int ***) malloc (sizeof (int **) * size_for_each_dimension [1][0]);} while ((__BASIC__STRUCT__(union_struct) -> sint) [0] == NULL);

					//Allocate 2d for each index of 3d
					for (unsigned int _3 = 0; _3 < size_for_each_dimension [1][0]; _3++) {
						if (size_for_each_dimension [0][0] == 1)
							do {(__BASIC__STRUCT__(union_struct) -> sint) [0][_3] = (int **) malloc (sizeof (int *) * size_for_each_dimension [2][_3]);} while ((__BASIC__STRUCT__(union_struct) -> sint) [0][_3] == NULL);
						else
							do {(__BASIC__STRUCT__(union_struct) -> sint) [0][_3] = (int **) malloc (sizeof (int *) * size_for_each_dimension [2][0]);} while ((__BASIC__STRUCT__(union_struct) -> sint) [0][_3] == NULL);
					}

					//Allocate 1d for each index of 2d.
					unsigned int start_from = 0;
					for (unsigned int _3 = 0; _3 < size_for_each_dimension [1][0]; ++_3) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned int _2 = 0; _2 < size_for_each_dimension [2][_3]; _2++) {
								if (size_for_each_dimension [1][0] == 1) {
									do {(__BASIC__STRUCT__(union_struct) -> sint) [0][_3][_2] = (int *) calloc (size_for_each_dimension [3][start_from], sizeof (int));} while ((__BASIC__STRUCT__(union_struct) -> sint) [0][_3][_2] == NULL);
									++start_from;
								} else
									do {(__BASIC__STRUCT__(union_struct) -> sint) [0][_3][_2] = (int *) calloc (size_for_each_dimension [3][0], sizeof (int));} while ((__BASIC__STRUCT__(union_struct) -> sint) [0][_3][_2] == NULL);
							}
						} else {
							for (unsigned int _2 = 0; _2 < size_for_each_dimension [2][0]; _2++) {
								if (size_for_each_dimension [1][0] == 1)
									do {(__BASIC__STRUCT__(union_struct) -> sint) [0][_3][_2] = (int *) calloc (size_for_each_dimension [3][_2], sizeof (int));} while ((__BASIC__STRUCT__(union_struct) -> sint) [0][_3][_2] == NULL);
								else
									do {(__BASIC__STRUCT__(union_struct) -> sint) [0][_3][_2] = (int *) calloc (size_for_each_dimension [3][0], sizeof (int));} while ((__BASIC__STRUCT__(union_struct) -> sint) [0][_3][_2] == NULL);
							}
						}
					}
				} else {
					do {__BASIC__UNION__(union_struct) -> sint = (int ****) malloc (sizeof (int ***));} while (__BASIC__UNION__(union_struct) -> sint == NULL);
					
					//Allocate 3d array
					do {(__BASIC__UNION__(union_struct) -> sint) [0] = (int ***) malloc (sizeof (int **) * size_for_each_dimension [1][0]);} while ((__BASIC__UNION__(union_struct) -> sint) [0] == NULL);

					//Allocate 2d for each index of 3d
					for (unsigned int _3 = 0; _3 < size_for_each_dimension [1][0]; _3++) {
						if (size_for_each_dimension [0][0] == 1)
							do {(__BASIC__UNION__(union_struct) -> sint) [0][_3] = (int **) malloc (sizeof (int *) * size_for_each_dimension [2][_3]);} while ((__BASIC__UNION__(union_struct) -> sint) [0][_3] == NULL);
						else
							do {(__BASIC__UNION__(union_struct) -> sint) [0][_3] = (int **) malloc (sizeof (int *) * size_for_each_dimension [2][0]);} while ((__BASIC__UNION__(union_struct) -> sint) [0][_3] == NULL);
					}

					//Allocate 1d for each index of 2d.
					unsigned int start_from = 0;
					for (unsigned int _3 = 0; _3 < size_for_each_dimension [1][0]; ++_3) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned int _2 = 0; _2 < size_for_each_dimension [2][_3]; _2++) {
								if (size_for_each_dimension [1][0] == 1) {
									do {(__BASIC__UNION__(union_struct) -> sint) [0][_3][_2] = (int *) calloc (size_for_each_dimension [3][start_from], sizeof (int));} while ((__BASIC__UNION__(union_struct) -> sint) [0][_3][_2] == NULL);
									++start_from;
								} else
									do {(__BASIC__UNION__(union_struct) -> sint) [0][_3][_2] = (int *) calloc (size_for_each_dimension [3][0], sizeof (int));} while ((__BASIC__UNION__(union_struct) -> sint) [0][_3][_2] == NULL);
							}
						} else {
							for (unsigned int _2 = 0; _2 < size_for_each_dimension [2][0]; _2++) {
								if (size_for_each_dimension [1][0] == 1)
									do {(__BASIC__UNION__(union_struct) -> sint) [0][_3][_2] = (int *) calloc (size_for_each_dimension [3][_2], sizeof (int));} while ((__BASIC__UNION__(union_struct) -> sint) [0][_3][_2] == NULL);
								else
									do {(__BASIC__UNION__(union_struct) -> sint) [0][_3][_2] = (int *) calloc (size_for_each_dimension [3][0], sizeof (int));} while ((__BASIC__UNION__(union_struct) -> sint) [0][_3][_2] == NULL);
							}
						}
					}
				}
			} else
				return 0;

		} else {
			size_for_each_dimension = sizes_for_union_struct_var_alloc (experssion);

			if (size_for_each_dimension != NULL) {
				if (type) {
					do {__BASIC__STRUCT__(union_struct) -> sint = (int ****) malloc (sizeof (int ***) * size_for_each_dimension [1][0]);} while (__BASIC__STRUCT__(union_struct) -> sint == NULL);

					//Allocate 3d for each index of 4d array.
					for (unsigned int _4 = 0; _4 < size_for_each_dimension [1][0]; _4++) {
						if (size_for_each_dimension [0][0] == 1)
							do {(__BASIC__STRUCT__(union_struct) -> sint) [_4] = (int ***) malloc (sizeof (int **) * size_for_each_dimension [2][_4]);} while ((__BASIC__STRUCT__(union_struct) -> sint) [_4] == NULL);
						else
							do {(__BASIC__STRUCT__(union_struct) -> sint) [_4] = (int ***) malloc (sizeof (int **) * size_for_each_dimension [2][0]);} while ((__BASIC__STRUCT__(union_struct) -> sint) [_4] == NULL);
					}

					unsigned int start_from = 0;
					
					//Allocate 2d for each index of 3d array.
					for (unsigned int _4 = 0; _4 < size_for_each_dimension [1][0]; _4++) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][_4]; _3++) {
								if (size_for_each_dimension [0][1] == 1) {
									do {(__BASIC__STRUCT__(union_struct) -> sint) [_4][_3] = (int **) malloc (sizeof (int *) * size_for_each_dimension [3][start_from]);} while ((__BASIC__STRUCT__(union_struct) -> sint) [_4][_3] == NULL);
									++start_from;
								} else
									do {(__BASIC__STRUCT__(union_struct) -> sint) [_4][_3] = (int **) malloc (sizeof (int *) * size_for_each_dimension [3][0]);} while ((__BASIC__STRUCT__(union_struct) -> sint) [_4][_3] == NULL);
							}
						} else {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][0]; _3++) {
								if (size_for_each_dimension [0][1] == 1)
									do {(__BASIC__STRUCT__(union_struct) -> sint) [_4][_3] = (int **) malloc (sizeof (int *) * size_for_each_dimension [3][_3]);} while ((__BASIC__STRUCT__(union_struct) -> sint) [_4][_3] == NULL);
								else
									do {(__BASIC__STRUCT__(union_struct) -> sint) [_4][_3] = (int **) malloc (sizeof (int *) * size_for_each_dimension [3][0]);} while ((__BASIC__STRUCT__(union_struct) -> sint) [_4][_3] == NULL);
							}
						}
					}

					start_from = 0;

					//Allocate 1d for each index of 2d array.
					for (unsigned int _4 = 0; _4 < size_for_each_dimension [1][0]; ++_4) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][_4]; ++_3) {
								if (size_for_each_dimension [0][1] == 1) {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][_3]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__STRUCT__(union_struct) -> sint) [_4][_3][_2] = (int *) calloc (size_for_each_dimension [4][start_from], sizeof (int));} while ((__BASIC__STRUCT__(union_struct) -> sint) [_4][_3][_2] == NULL);
											++start_from;
										} else
											do {(__BASIC__STRUCT__(union_struct) -> sint) [_4][_3][_2] = (int *) calloc (size_for_each_dimension [4][0], sizeof (int));} while ((__BASIC__STRUCT__(union_struct) -> sint) [_4][_3][_2] == NULL);
									}
								} else {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][0]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__STRUCT__(union_struct) -> sint) [_4][_3][_2] = (int *) calloc (size_for_each_dimension [4][_2], sizeof (int));} while ((__BASIC__STRUCT__(union_struct) -> sint) [_4][_3][_2] == NULL);
										} else
											do {(__BASIC__STRUCT__(union_struct) -> sint) [_4][_3][_2] = (int *) calloc (size_for_each_dimension [4][0], sizeof (int));} while ((__BASIC__STRUCT__(union_struct) -> sint) [_4][_3][_2] == NULL);
									}
								}
							}
						} else {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][0]; ++_3) {
								if (size_for_each_dimension [0][1] == 1) {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][_3]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__STRUCT__(union_struct) -> sint) [_4][_3][_2] = (int *) calloc (size_for_each_dimension [4][start_from], sizeof (int));} while ((__BASIC__STRUCT__(union_struct) -> sint) [_4][_3][_2] == NULL);
											++start_from;
										} else
											do {(__BASIC__STRUCT__(union_struct) -> sint) [_4][_3][_2] = (int *) calloc (size_for_each_dimension [4][0], sizeof (int));} while ((__BASIC__STRUCT__(union_struct) -> sint) [_4][_3][_2] == NULL);
									}
								} else {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][0]; ++_2) {
										if (size_for_each_dimension [0][2] == 1)
											do {(__BASIC__STRUCT__(union_struct) -> sint) [_4][_3][_2] = (int *) calloc (size_for_each_dimension [4][_2], sizeof (int));} while ((__BASIC__STRUCT__(union_struct) -> sint) [_4][_3][_2] == NULL);
										else
											do {(__BASIC__STRUCT__(union_struct) -> sint) [_4][_3][_2] = (int *) calloc (size_for_each_dimension [4][0], sizeof (int));} while ((__BASIC__STRUCT__(union_struct) -> sint) [_4][_3][_2] == NULL);
									}
								}
							}
						}
					}
				} else {
					do {__BASIC__UNION__(union_struct) -> sint = (int ****) malloc (sizeof (int ***) * size_for_each_dimension [1][0]);} while (__BASIC__UNION__(union_struct) -> sint == NULL);

					//Allocate 3d for each index of 4d array.
					for (unsigned int _4 = 0; _4 < size_for_each_dimension [1][0]; _4++) {
						if (size_for_each_dimension [0][0] == 1)
							do {(__BASIC__UNION__(union_struct) -> sint) [_4] = (int ***) malloc (sizeof (int **) * size_for_each_dimension [2][_4]);} while ((__BASIC__UNION__(union_struct) -> sint) [_4] == NULL);
						else
							do {(__BASIC__UNION__(union_struct) -> sint) [_4] = (int ***) malloc (sizeof (int **) * size_for_each_dimension [2][0]);} while ((__BASIC__UNION__(union_struct) -> sint) [_4] == NULL);
					}

					unsigned int start_from = 0;
					
					//Allocate 2d for each index of 3d array.
					for (unsigned int _4 = 0; _4 < size_for_each_dimension [1][0]; _4++) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][_4]; _3++) {
								if (size_for_each_dimension [0][1] == 1) {
									do {(__BASIC__UNION__(union_struct) -> sint) [_4][_3] = (int **) malloc (sizeof (int *) * size_for_each_dimension [3][start_from]);} while ((__BASIC__UNION__(union_struct) -> sint) [_4][_3] == NULL);
									++start_from;
								} else
									do {(__BASIC__UNION__(union_struct) -> sint) [_4][_3] = (int **) malloc (sizeof (int *) * size_for_each_dimension [3][0]);} while ((__BASIC__UNION__(union_struct) -> sint) [_4][_3] == NULL);
							}
						} else {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][0]; _3++) {
								if (size_for_each_dimension [0][1] == 1)
									do {(__BASIC__UNION__(union_struct) -> sint) [_4][_3] = (int **) malloc (sizeof (int *) * size_for_each_dimension [3][_3]);} while ((__BASIC__UNION__(union_struct) -> sint) [_4][_3] == NULL);
								else
									do {(__BASIC__UNION__(union_struct) -> sint) [_4][_3] = (int **) malloc (sizeof (int *) * size_for_each_dimension [3][0]);} while ((__BASIC__UNION__(union_struct) -> sint) [_4][_3] == NULL);
							}
						}
					}

					start_from = 0;

					//Allocate 1d for each index of 2d array.
					for (unsigned int _4 = 0; _4 < size_for_each_dimension [1][0]; ++_4) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][_4]; ++_3) {
								if (size_for_each_dimension [0][1] == 1) {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][_3]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__UNION__(union_struct) -> sint) [_4][_3][_2] = (int *) calloc (size_for_each_dimension [4][start_from], sizeof (int));} while ((__BASIC__UNION__(union_struct) -> sint) [_4][_3][_2] == NULL);
											++start_from;
										} else
											do {(__BASIC__UNION__(union_struct) -> sint) [_4][_3][_2] = (int *) calloc (size_for_each_dimension [4][0], sizeof (int));} while ((__BASIC__UNION__(union_struct) -> sint) [_4][_3][_2] == NULL);
									}
								} else {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][0]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__UNION__(union_struct) -> sint) [_4][_3][_2] = (int *) calloc (size_for_each_dimension [4][_2], sizeof (int));} while ((__BASIC__UNION__(union_struct) -> sint) [_4][_3][_2] == NULL);
										} else
											do {(__BASIC__UNION__(union_struct) -> sint) [_4][_3][_2] = (int *) calloc (size_for_each_dimension [4][0], sizeof (int));} while ((__BASIC__UNION__(union_struct) -> sint) [_4][_3][_2] == NULL);
									}
								}
							}
						} else {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][0]; ++_3) {
								if (size_for_each_dimension [0][1] == 1) {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][_3]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__UNION__(union_struct) -> sint) [_4][_3][_2] = (int *) calloc (size_for_each_dimension [4][start_from], sizeof (int));} while ((__BASIC__UNION__(union_struct) -> sint) [_4][_3][_2] == NULL);
											++start_from;
										} else
											do {(__BASIC__UNION__(union_struct) -> sint) [_4][_3][_2] = (int *) calloc (size_for_each_dimension [4][0], sizeof (int));} while ((__BASIC__UNION__(union_struct) -> sint) [_4][_3][_2] == NULL);
									}
								} else {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][0]; ++_2) {
										if (size_for_each_dimension [0][2] == 1)
											do {(__BASIC__UNION__(union_struct) -> sint) [_4][_3][_2] = (int *) calloc (size_for_each_dimension [4][_2], sizeof (int));} while ((__BASIC__UNION__(union_struct) -> sint) [_4][_3][_2] == NULL);
										else
											do {(__BASIC__UNION__(union_struct) -> sint) [_4][_3][_2] = (int *) calloc (size_for_each_dimension [4][0], sizeof (int));} while ((__BASIC__UNION__(union_struct) -> sint) [_4][_3][_2] == NULL);
									}
								}
							}
						}
					}
				}
			}
		}
	} else if (dtype_code == 6) {
		if (type) {
			if (__BASIC__STRUCT__(union_struct) -> _uint != NULL)
				return 1;
		else
			if (__BASIC__UNION__(union_struct) -> _uint != NULL)
				return 1;
		}

		if (how_many_dimensions == 0) {
			if (type) {
				do {__BASIC__STRUCT__(union_struct) -> _uint = (unsigned int ****) malloc (sizeof (unsigned int ***));} while (__BASIC__STRUCT__(union_struct) -> _uint == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> _uint) [0] = (unsigned int ***) malloc (sizeof (unsigned int **));} while ((__BASIC__STRUCT__(union_struct) -> _uint) [0] == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> _uint) [0][0] = (unsigned int **) malloc (sizeof (unsigned int *));} while ((__BASIC__STRUCT__(union_struct) -> _uint) [0][0] == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> _uint) [0][0][0] = (unsigned int *) calloc (1, sizeof (unsigned int));} while ((__BASIC__STRUCT__(union_struct) -> _uint) [0][0][0] == NULL);
			} else {
				do {__BASIC__UNION__(union_struct) -> _uint = (unsigned int ****) malloc (sizeof (unsigned int ***));} while (__BASIC__UNION__(union_struct) -> _uint == NULL);
				do {(__BASIC__UNION__(union_struct) -> _uint) [0] = (unsigned int ***) malloc (sizeof (unsigned int **));} while ((__BASIC__UNION__(union_struct) -> _uint) [0] == NULL);
				do {(__BASIC__UNION__(union_struct) -> _uint) [0][0] = (unsigned int **) malloc (sizeof (unsigned int *));} while ((__BASIC__UNION__(union_struct) -> _uint) [0][0] == NULL);
				do {(__BASIC__UNION__(union_struct) -> _uint) [0][0][0] = (unsigned int *) calloc (1, sizeof (unsigned int));} while ((__BASIC__UNION__(union_struct) -> _uint) [0][0][0] == NULL);
			}
		} else if (how_many_dimensions == 1) {
			if (type) {
				do {__BASIC__STRUCT__(union_struct) -> _uint = (unsigned int ****) malloc (sizeof (unsigned int ***));} while (__BASIC__STRUCT__(union_struct) -> _uint == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> _uint) [0] = (unsigned int ***) malloc (sizeof (unsigned int **));} while ((__BASIC__STRUCT__(union_struct) -> _uint) [0] == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> _uint) [0][0] = (unsigned int **) malloc (sizeof (unsigned int *));} while ((__BASIC__STRUCT__(union_struct) -> _uint) [0][0] == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> _uint) [0][0][0] = (unsigned int *) calloc (to_uint (experssion), sizeof (unsigned int));} while ((__BASIC__STRUCT__(union_struct) -> _uint) [0][0][0] == NULL);
			} else {
				do {__BASIC__UNION__(union_struct) -> _uint = (unsigned int ****) malloc (sizeof (unsigned int ***));} while (__BASIC__UNION__(union_struct) -> _uint == NULL);
				do {(__BASIC__UNION__(union_struct) -> _uint) [0] = (unsigned int ***) malloc (sizeof (unsigned int **));} while ((__BASIC__UNION__(union_struct) -> _uint) [0] == NULL);
				do {(__BASIC__UNION__(union_struct) -> _uint) [0][0] = (unsigned int **) malloc (sizeof (unsigned int *));} while ((__BASIC__UNION__(union_struct) -> _uint) [0][0] == NULL);
				do {(__BASIC__UNION__(union_struct) -> _uint) [0][0][0] = (unsigned int *) calloc (to_uint (experssion), sizeof (unsigned int));} while ((__BASIC__UNION__(union_struct) -> _uint) [0][0][0] == NULL);
			}
		} else if (how_many_dimensions == 2) {
			size_for_each_dimension = sizes_for_union_struct_var_alloc (experssion);

			if (size_for_each_dimension != NULL) {
				//3*[1-2-3] or 3*2
				if (type) {
					do {__BASIC__STRUCT__(union_struct) -> _uint = (unsigned int ****) malloc (sizeof (unsigned int ***));} while (__BASIC__STRUCT__(union_struct) -> _uint == NULL);
					do {(__BASIC__STRUCT__(union_struct) -> _uint) [0] = (unsigned int ***) malloc (sizeof (unsigned int **));} while ((__BASIC__STRUCT__(union_struct) -> _uint) [0] == NULL);
					do {(__BASIC__STRUCT__(union_struct) -> _uint) [0][0] = (unsigned int **) malloc (sizeof (unsigned int *) * size_for_each_dimension [1][0]);} while ((__BASIC__STRUCT__(union_struct) -> _uint) [0][0] == NULL);
					
					if (size_for_each_dimension [0][0] == 1)
						for (unsigned int counter = 0; counter < size_for_each_dimension [1][0]; ++counter)
							do {(__BASIC__STRUCT__(union_struct) -> _uint) [0][0][counter] = (unsigned int *) calloc (size_for_each_dimension [2][counter], sizeof (unsigned int));} while ((__BASIC__STRUCT__(union_struct) -> _uint) [0][0][counter] == NULL);
					else
						for (unsigned int counter = 0; counter < size_for_each_dimension [1][0]; ++counter)
							do {(__BASIC__STRUCT__(union_struct) -> _uint) [0][0][counter] = (unsigned int *) calloc (size_for_each_dimension [2][0], sizeof (unsigned int));} while ((__BASIC__STRUCT__(union_struct) -> _uint) [0][0][counter] == NULL);
				} else {
					do {__BASIC__UNION__(union_struct) -> _uint = (unsigned int ****) malloc (sizeof (unsigned int ***));} while (__BASIC__UNION__(union_struct) -> _uint == NULL);
					do {(__BASIC__UNION__(union_struct) -> _uint) [0] = (unsigned int ***) malloc (sizeof (unsigned int **));} while ((__BASIC__UNION__(union_struct) -> _uint) [0] == NULL);
					do {(__BASIC__UNION__(union_struct) -> _uint) [0][0] = (unsigned int **) malloc (sizeof (unsigned int *) * size_for_each_dimension [1][0]);} while ((__BASIC__UNION__(union_struct) -> _uint) [0][0] == NULL);
					
					if (size_for_each_dimension [0][0] == 1)
						for (unsigned int counter = 0; counter < size_for_each_dimension [1][0]; ++counter)
							do {(__BASIC__UNION__(union_struct) -> _uint) [0][0][counter] = (unsigned int *) calloc (size_for_each_dimension [2][counter], sizeof (unsigned int));} while ((__BASIC__UNION__(union_struct) -> _uint) [0][0][counter] == NULL);
					else
						for (unsigned int counter = 0; counter < size_for_each_dimension [1][0]; ++counter)
							do {(__BASIC__UNION__(union_struct) -> _uint) [0][0][counter] = (unsigned int *) calloc (size_for_each_dimension [2][0], sizeof (unsigned int));} while ((__BASIC__UNION__(union_struct) -> _uint) [0][0][counter] == NULL);
				}
			} else
				return 0;

		} else if (how_many_dimensions == 3) {
			size_for_each_dimension = sizes_for_union_struct_var_alloc (experssion);

			if (size_for_each_dimension != NULL) {
				if (type) {
					do {__BASIC__STRUCT__(union_struct) -> _uint = (unsigned int ****) malloc (sizeof (unsigned int ***));} while (__BASIC__STRUCT__(union_struct) -> _uint == NULL);
					
					//Allocate 3d array
					do {(__BASIC__STRUCT__(union_struct) -> _uint) [0] = (unsigned int ***) malloc (sizeof (unsigned int **) * size_for_each_dimension [1][0]);} while ((__BASIC__STRUCT__(union_struct) -> _uint) [0] == NULL);

					//Allocate 2d for each index of 3d
					for (unsigned int _3 = 0; _3 < size_for_each_dimension [1][0]; _3++) {
						if (size_for_each_dimension [0][0] == 1)
							do {(__BASIC__STRUCT__(union_struct) -> _uint) [0][_3] = (unsigned int **) malloc (sizeof (unsigned int *) * size_for_each_dimension [2][_3]);} while ((__BASIC__STRUCT__(union_struct) -> _uint) [0][_3] == NULL);
						else
							do {(__BASIC__STRUCT__(union_struct) -> _uint) [0][_3] = (unsigned int **) malloc (sizeof (unsigned int *) * size_for_each_dimension [2][0]);} while ((__BASIC__STRUCT__(union_struct) -> _uint) [0][_3] == NULL);
					}

					//Allocate 1d for each index of 2d.
					unsigned int start_from = 0;
					for (unsigned int _3 = 0; _3 < size_for_each_dimension [1][0]; ++_3) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned int _2 = 0; _2 < size_for_each_dimension [2][_3]; _2++) {
								if (size_for_each_dimension [1][0] == 1) {
									do {(__BASIC__STRUCT__(union_struct) -> _uint) [0][_3][_2] = (unsigned int *) calloc (size_for_each_dimension [3][start_from], sizeof (unsigned int));} while ((__BASIC__STRUCT__(union_struct) -> _uint) [0][_3][_2] == NULL);
									++start_from;
								} else
									do {(__BASIC__STRUCT__(union_struct) -> _uint) [0][_3][_2] = (unsigned int *) calloc (size_for_each_dimension [3][0], sizeof (unsigned int));} while ((__BASIC__STRUCT__(union_struct) -> _uint) [0][_3][_2] == NULL);
							}
						} else {
							for (unsigned int _2 = 0; _2 < size_for_each_dimension [2][0]; _2++) {
								if (size_for_each_dimension [1][0] == 1)
									do {(__BASIC__STRUCT__(union_struct) -> _uint) [0][_3][_2] = (unsigned int *) calloc (size_for_each_dimension [3][_2], sizeof (unsigned int));} while ((__BASIC__STRUCT__(union_struct) -> _uint) [0][_3][_2] == NULL);
								else
									do {(__BASIC__STRUCT__(union_struct) -> _uint) [0][_3][_2] = (unsigned int *) calloc (size_for_each_dimension [3][0], sizeof (unsigned int));} while ((__BASIC__STRUCT__(union_struct) -> _uint) [0][_3][_2] == NULL);
							}
						}
					}
				} else {
					do {__BASIC__UNION__(union_struct) -> _uint = (unsigned int ****) malloc (sizeof (unsigned int ***));} while (__BASIC__UNION__(union_struct) -> _uint == NULL);
					
					//Allocate 3d array
					do {(__BASIC__UNION__(union_struct) -> _uint) [0] = (unsigned int ***) malloc (sizeof (unsigned int **) * size_for_each_dimension [1][0]);} while ((__BASIC__UNION__(union_struct) -> _uint) [0] == NULL);

					//Allocate 2d for each index of 3d
					for (unsigned int _3 = 0; _3 < size_for_each_dimension [1][0]; _3++) {
						if (size_for_each_dimension [0][0] == 1)
							do {(__BASIC__UNION__(union_struct) -> _uint) [0][_3] = (unsigned int **) malloc (sizeof (unsigned int *) * size_for_each_dimension [2][_3]);} while ((__BASIC__UNION__(union_struct) -> _uint) [0][_3] == NULL);
						else
							do {(__BASIC__UNION__(union_struct) -> _uint) [0][_3] = (unsigned int **) malloc (sizeof (unsigned int *) * size_for_each_dimension [2][0]);} while ((__BASIC__UNION__(union_struct) -> _uint) [0][_3] == NULL);
					}

					//Allocate 1d for each index of 2d.
					unsigned int start_from = 0;
					for (unsigned int _3 = 0; _3 < size_for_each_dimension [1][0]; ++_3) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned int _2 = 0; _2 < size_for_each_dimension [2][_3]; _2++) {
								if (size_for_each_dimension [1][0] == 1) {
									do {(__BASIC__UNION__(union_struct) -> _uint) [0][_3][_2] = (unsigned int *) calloc (size_for_each_dimension [3][start_from], sizeof (unsigned int));} while ((__BASIC__UNION__(union_struct) -> _uint) [0][_3][_2] == NULL);
									++start_from;
								} else
									do {(__BASIC__UNION__(union_struct) -> _uint) [0][_3][_2] = (unsigned int *) calloc (size_for_each_dimension [3][0], sizeof (unsigned int));} while ((__BASIC__UNION__(union_struct) -> _uint) [0][_3][_2] == NULL);
							}
						} else {
							for (unsigned int _2 = 0; _2 < size_for_each_dimension [2][0]; _2++) {
								if (size_for_each_dimension [1][0] == 1)
									do {(__BASIC__UNION__(union_struct) -> _uint) [0][_3][_2] = (unsigned int *) calloc (size_for_each_dimension [3][_2], sizeof (unsigned int));} while ((__BASIC__UNION__(union_struct) -> _uint) [0][_3][_2] == NULL);
								else
									do {(__BASIC__UNION__(union_struct) -> _uint) [0][_3][_2] = (unsigned int *) calloc (size_for_each_dimension [3][0], sizeof (unsigned int));} while ((__BASIC__UNION__(union_struct) -> _uint) [0][_3][_2] == NULL);
							}
						}
					}
				}
			} else
				return 0;

		} else {
			size_for_each_dimension = sizes_for_union_struct_var_alloc (experssion);

			if (size_for_each_dimension != NULL) {
				if (type) {
					do {__BASIC__STRUCT__(union_struct) -> _uint = (unsigned int ****) malloc (sizeof (unsigned int ***) * size_for_each_dimension [1][0]);} while (__BASIC__STRUCT__(union_struct) -> _uint == NULL);

					//Allocate 3d for each index of 4d array.
					for (unsigned int _4 = 0; _4 < size_for_each_dimension [1][0]; _4++) {
						if (size_for_each_dimension [0][0] == 1)
							do {(__BASIC__STRUCT__(union_struct) -> _uint) [_4] = (unsigned int ***) malloc (sizeof (unsigned int **) * size_for_each_dimension [2][_4]);} while ((__BASIC__STRUCT__(union_struct) -> _uint) [_4] == NULL);
						else
							do {(__BASIC__STRUCT__(union_struct) -> _uint) [_4] = (unsigned int ***) malloc (sizeof (unsigned int **) * size_for_each_dimension [2][0]);} while ((__BASIC__STRUCT__(union_struct) -> _uint) [_4] == NULL);
					}

					unsigned int start_from = 0;
					
					//Allocate 2d for each index of 3d array.
					for (unsigned int _4 = 0; _4 < size_for_each_dimension [1][0]; _4++) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][_4]; _3++) {
								if (size_for_each_dimension [0][1] == 1) {
									do {(__BASIC__STRUCT__(union_struct) -> _uint) [_4][_3] = (unsigned int **) malloc (sizeof (unsigned int *) * size_for_each_dimension [3][start_from]);} while ((__BASIC__STRUCT__(union_struct) -> _uint) [_4][_3] == NULL);
									++start_from;
								} else
									do {(__BASIC__STRUCT__(union_struct) -> _uint) [_4][_3] = (unsigned int **) malloc (sizeof (unsigned int *) * size_for_each_dimension [3][0]);} while ((__BASIC__STRUCT__(union_struct) -> _uint) [_4][_3] == NULL);
							}
						} else {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][0]; _3++) {
								if (size_for_each_dimension [0][1] == 1)
									do {(__BASIC__STRUCT__(union_struct) -> _uint) [_4][_3] = (unsigned int **) malloc (sizeof (unsigned int *) * size_for_each_dimension [3][_3]);} while ((__BASIC__STRUCT__(union_struct) -> _uint) [_4][_3] == NULL);
								else
									do {(__BASIC__STRUCT__(union_struct) -> _uint) [_4][_3] = (unsigned int **) malloc (sizeof (unsigned int *) * size_for_each_dimension [3][0]);} while ((__BASIC__STRUCT__(union_struct) -> _uint) [_4][_3] == NULL);
							}
						}
					}

					start_from = 0;

					//Allocate 1d for each index of 2d array.
					for (unsigned int _4 = 0; _4 < size_for_each_dimension [1][0]; ++_4) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][_4]; ++_3) {
								if (size_for_each_dimension [0][1] == 1) {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][_3]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__STRUCT__(union_struct) -> _uint) [_4][_3][_2] = (unsigned int *) calloc (size_for_each_dimension [4][start_from], sizeof (unsigned int));} while ((__BASIC__STRUCT__(union_struct) -> _uint) [_4][_3][_2] == NULL);
											++start_from;
										} else
											do {(__BASIC__STRUCT__(union_struct) -> _uint) [_4][_3][_2] = (unsigned int *) calloc (size_for_each_dimension [4][0], sizeof (unsigned int));} while ((__BASIC__STRUCT__(union_struct) -> _uint) [_4][_3][_2] == NULL);
									}
								} else {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][0]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__STRUCT__(union_struct) -> _uint) [_4][_3][_2] = (unsigned int *) calloc (size_for_each_dimension [4][_2], sizeof (unsigned int));} while ((__BASIC__STRUCT__(union_struct) -> _uint) [_4][_3][_2] == NULL);
										} else
											do {(__BASIC__STRUCT__(union_struct) -> _uint) [_4][_3][_2] = (unsigned int *) calloc (size_for_each_dimension [4][0], sizeof (unsigned int));} while ((__BASIC__STRUCT__(union_struct) -> _uint) [_4][_3][_2] == NULL);
									}
								}
							}
						} else {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][0]; ++_3) {
								if (size_for_each_dimension [0][1] == 1) {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][_3]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__STRUCT__(union_struct) -> _uint) [_4][_3][_2] = (unsigned int *) calloc (size_for_each_dimension [4][start_from], sizeof (unsigned int));} while ((__BASIC__STRUCT__(union_struct) -> _uint) [_4][_3][_2] == NULL);
											++start_from;
										} else
											do {(__BASIC__STRUCT__(union_struct) -> _uint) [_4][_3][_2] = (unsigned int *) calloc (size_for_each_dimension [4][0], sizeof (unsigned int));} while ((__BASIC__STRUCT__(union_struct) -> _uint) [_4][_3][_2] == NULL);
									}
								} else {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][0]; ++_2) {
										if (size_for_each_dimension [0][2] == 1)
											do {(__BASIC__STRUCT__(union_struct) -> _uint) [_4][_3][_2] = (unsigned int *) calloc (size_for_each_dimension [4][_2], sizeof (unsigned int));} while ((__BASIC__STRUCT__(union_struct) -> _uint) [_4][_3][_2] == NULL);
										else
											do {(__BASIC__STRUCT__(union_struct) -> _uint) [_4][_3][_2] = (unsigned int *) calloc (size_for_each_dimension [4][0], sizeof (unsigned int));} while ((__BASIC__STRUCT__(union_struct) -> _uint) [_4][_3][_2] == NULL);
									}
								}
							}
						}
					}
				} else {
					do {__BASIC__UNION__(union_struct) -> _uint = (unsigned int ****) malloc (sizeof (unsigned int ***) * size_for_each_dimension [1][0]);} while (__BASIC__UNION__(union_struct) -> _uint == NULL);

					//Allocate 3d for each index of 4d array.
					for (unsigned int _4 = 0; _4 < size_for_each_dimension [1][0]; _4++) {
						if (size_for_each_dimension [0][0] == 1)
							do {(__BASIC__UNION__(union_struct) -> _uint) [_4] = (unsigned int ***) malloc (sizeof (unsigned int **) * size_for_each_dimension [2][_4]);} while ((__BASIC__UNION__(union_struct) -> _uint) [_4] == NULL);
						else
							do {(__BASIC__UNION__(union_struct) -> _uint) [_4] = (unsigned int ***) malloc (sizeof (unsigned int **) * size_for_each_dimension [2][0]);} while ((__BASIC__UNION__(union_struct) -> _uint) [_4] == NULL);
					}

					unsigned int start_from = 0;
					
					//Allocate 2d for each index of 3d array.
					for (unsigned int _4 = 0; _4 < size_for_each_dimension [1][0]; _4++) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][_4]; _3++) {
								if (size_for_each_dimension [0][1] == 1) {
									do {(__BASIC__UNION__(union_struct) -> _uint) [_4][_3] = (unsigned int **) malloc (sizeof (unsigned int *) * size_for_each_dimension [3][start_from]);} while ((__BASIC__UNION__(union_struct) -> _uint) [_4][_3] == NULL);
									++start_from;
								} else
									do {(__BASIC__UNION__(union_struct) -> _uint) [_4][_3] = (unsigned int **) malloc (sizeof (unsigned int *) * size_for_each_dimension [3][0]);} while ((__BASIC__UNION__(union_struct) -> _uint) [_4][_3] == NULL);
							}
						} else {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][0]; _3++) {
								if (size_for_each_dimension [0][1] == 1)
									do {(__BASIC__UNION__(union_struct) -> _uint) [_4][_3] = (unsigned int **) malloc (sizeof (unsigned int *) * size_for_each_dimension [3][_3]);} while ((__BASIC__UNION__(union_struct) -> _uint) [_4][_3] == NULL);
								else
									do {(__BASIC__UNION__(union_struct) -> _uint) [_4][_3] = (unsigned int **) malloc (sizeof (unsigned int *) * size_for_each_dimension [3][0]);} while ((__BASIC__UNION__(union_struct) -> _uint) [_4][_3] == NULL);
							}
						}
					}

					start_from = 0;

					//Allocate 1d for each index of 2d array.
					for (unsigned int _4 = 0; _4 < size_for_each_dimension [1][0]; ++_4) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][_4]; ++_3) {
								if (size_for_each_dimension [0][1] == 1) {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][_3]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__UNION__(union_struct) -> _uint) [_4][_3][_2] = (unsigned int *) calloc (size_for_each_dimension [4][start_from], sizeof (unsigned int));} while ((__BASIC__UNION__(union_struct) -> _uint) [_4][_3][_2] == NULL);
											++start_from;
										} else
											do {(__BASIC__UNION__(union_struct) -> _uint) [_4][_3][_2] = (unsigned int *) calloc (size_for_each_dimension [4][0], sizeof (unsigned int));} while ((__BASIC__UNION__(union_struct) -> _uint) [_4][_3][_2] == NULL);
									}
								} else {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][0]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__UNION__(union_struct) -> _uint) [_4][_3][_2] = (unsigned int *) calloc (size_for_each_dimension [4][_2], sizeof (unsigned int));} while ((__BASIC__UNION__(union_struct) -> _uint) [_4][_3][_2] == NULL);
										} else
											do {(__BASIC__UNION__(union_struct) -> _uint) [_4][_3][_2] = (unsigned int *) calloc (size_for_each_dimension [4][0], sizeof (unsigned int));} while ((__BASIC__UNION__(union_struct) -> _uint) [_4][_3][_2] == NULL);
									}
								}
							}
						} else {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][0]; ++_3) {
								if (size_for_each_dimension [0][1] == 1) {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][_3]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__UNION__(union_struct) -> _uint) [_4][_3][_2] = (unsigned int *) calloc (size_for_each_dimension [4][start_from], sizeof (unsigned int));} while ((__BASIC__UNION__(union_struct) -> _uint) [_4][_3][_2] == NULL);
											++start_from;
										} else
											do {(__BASIC__UNION__(union_struct) -> _uint) [_4][_3][_2] = (unsigned int *) calloc (size_for_each_dimension [4][0], sizeof (unsigned int));} while ((__BASIC__UNION__(union_struct) -> _uint) [_4][_3][_2] == NULL);
									}
								} else {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][0]; ++_2) {
										if (size_for_each_dimension [0][2] == 1)
											do {(__BASIC__UNION__(union_struct) -> _uint) [_4][_3][_2] = (unsigned int *) calloc (size_for_each_dimension [4][_2], sizeof (unsigned int));} while ((__BASIC__UNION__(union_struct) -> _uint) [_4][_3][_2] == NULL);
										else
											do {(__BASIC__UNION__(union_struct) -> _uint) [_4][_3][_2] = (unsigned int *) calloc (size_for_each_dimension [4][0], sizeof (unsigned int));} while ((__BASIC__UNION__(union_struct) -> _uint) [_4][_3][_2] == NULL);
									}
								}
							}
						}
					}
				}
			}
		}
	} else if (dtype_code == 7) {
		if (type) {
			if (__BASIC__STRUCT__(union_struct) -> slli != NULL)
				return 1;
		} else {
			if (__BASIC__UNION__(union_struct) -> slli != NULL)
				return 1;
		}

		if (how_many_dimensions == 0) {
			if (type) {
				do {__BASIC__STRUCT__(union_struct) -> slli = (long long int ****) malloc (sizeof (long long int ***));} while (__BASIC__STRUCT__(union_struct) -> slli == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> slli) [0] = (long long int ***) malloc (sizeof (long long int **));} while ((__BASIC__STRUCT__(union_struct) -> slli) [0] == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> slli) [0][0] = (long long int **) malloc (sizeof (long long int *));} while ((__BASIC__STRUCT__(union_struct) -> slli) [0][0] == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> slli) [0][0][0] = (long long int *) calloc (1, sizeof (long long int));} while ((__BASIC__STRUCT__(union_struct) -> slli) [0][0][0] == NULL);
			} else {
				do {__BASIC__UNION__(union_struct) -> slli = (long long int ****) malloc (sizeof (long long int ***));} while (__BASIC__UNION__(union_struct) -> slli == NULL);
				do {(__BASIC__UNION__(union_struct) -> slli) [0] = (long long int ***) malloc (sizeof (long long int **));} while ((__BASIC__UNION__(union_struct) -> slli) [0] == NULL);
				do {(__BASIC__UNION__(union_struct) -> slli) [0][0] = (long long int **) malloc (sizeof (long long int *));} while ((__BASIC__UNION__(union_struct) -> slli) [0][0] == NULL);
				do {(__BASIC__UNION__(union_struct) -> slli) [0][0][0] = (long long int *) calloc (1, sizeof (long long int));} while ((__BASIC__UNION__(union_struct) -> slli) [0][0][0] == NULL);
			}
		} else if (how_many_dimensions == 1) {
			if (type) {
				do {__BASIC__STRUCT__(union_struct) -> slli = (long long int ****) malloc (sizeof (long long int ***));} while (__BASIC__STRUCT__(union_struct) -> slli == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> slli) [0] = (long long int ***) malloc (sizeof (long long int **));} while ((__BASIC__STRUCT__(union_struct) -> slli) [0] == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> slli) [0][0] = (long long int **) malloc (sizeof (long long int *));} while ((__BASIC__STRUCT__(union_struct) -> slli) [0][0] == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> slli) [0][0][0] = (long long int *) calloc (to_uint (experssion), sizeof (long long int));} while ((__BASIC__STRUCT__(union_struct) -> slli) [0][0][0] == NULL);
			} else {
				do {__BASIC__UNION__(union_struct) -> slli = (long long int ****) malloc (sizeof (long long int ***));} while (__BASIC__UNION__(union_struct) -> slli == NULL);
				do {(__BASIC__UNION__(union_struct) -> slli) [0] = (long long int ***) malloc (sizeof (long long int **));} while ((__BASIC__UNION__(union_struct) -> slli) [0] == NULL);
				do {(__BASIC__UNION__(union_struct) -> slli) [0][0] = (long long int **) malloc (sizeof (long long int *));} while ((__BASIC__UNION__(union_struct) -> slli) [0][0] == NULL);
				do {(__BASIC__UNION__(union_struct) -> slli) [0][0][0] = (long long int *) calloc (to_uint (experssion), sizeof (long long int));} while ((__BASIC__UNION__(union_struct) -> slli) [0][0][0] == NULL);
			}
		} else if (how_many_dimensions == 2) {
			size_for_each_dimension = sizes_for_union_struct_var_alloc (experssion);

			if (size_for_each_dimension != NULL) {
				//3*[1-2-3] or 3*2
				if (type) {
					do {__BASIC__STRUCT__(union_struct) -> slli = (long long int ****) malloc (sizeof (long long int ***));} while (__BASIC__STRUCT__(union_struct) -> slli == NULL);
					do {(__BASIC__STRUCT__(union_struct) -> slli) [0] = (long long int ***) malloc (sizeof (long long int **));} while ((__BASIC__STRUCT__(union_struct) -> slli) [0] == NULL);
					do {(__BASIC__STRUCT__(union_struct) -> slli) [0][0] = (long long int **) malloc (sizeof (long long int *) * size_for_each_dimension [1][0]);} while ((__BASIC__STRUCT__(union_struct) -> slli) [0][0] == NULL);
					
					if (size_for_each_dimension [0][0] == 1)
						for (unsigned int counter = 0; counter < size_for_each_dimension [1][0]; ++counter)
							do {(__BASIC__STRUCT__(union_struct) -> slli) [0][0][counter] = (long long int *) calloc (size_for_each_dimension [2][counter], sizeof (long long int));} while ((__BASIC__STRUCT__(union_struct) -> slli) [0][0][counter] == NULL);
					else
						for (unsigned int counter = 0; counter < size_for_each_dimension [1][0]; ++counter)
							do {(__BASIC__STRUCT__(union_struct) -> slli) [0][0][counter] = (long long int *) calloc (size_for_each_dimension [2][0], sizeof (long long int));} while ((__BASIC__STRUCT__(union_struct) -> slli) [0][0][counter] == NULL);
				} else {
					do {__BASIC__UNION__(union_struct) -> slli = (long long int ****) malloc (sizeof (long long int ***));} while (__BASIC__UNION__(union_struct) -> slli == NULL);
					do {(__BASIC__UNION__(union_struct) -> slli) [0] = (long long int ***) malloc (sizeof (long long int **));} while ((__BASIC__UNION__(union_struct) -> slli) [0] == NULL);
					do {(__BASIC__UNION__(union_struct) -> slli) [0][0] = (long long int **) malloc (sizeof (long long int *) * size_for_each_dimension [1][0]);} while ((__BASIC__UNION__(union_struct) -> slli) [0][0] == NULL);
					
					if (size_for_each_dimension [0][0] == 1)
						for (unsigned int counter = 0; counter < size_for_each_dimension [1][0]; ++counter)
							do {(__BASIC__UNION__(union_struct) -> slli) [0][0][counter] = (long long int *) calloc (size_for_each_dimension [2][counter], sizeof (long long int));} while ((__BASIC__UNION__(union_struct) -> slli) [0][0][counter] == NULL);
					else
						for (unsigned int counter = 0; counter < size_for_each_dimension [1][0]; ++counter)
							do {(__BASIC__UNION__(union_struct) -> slli) [0][0][counter] = (long long int *) calloc (size_for_each_dimension [2][0], sizeof (long long int));} while ((__BASIC__UNION__(union_struct) -> slli) [0][0][counter] == NULL);
				}
			} else
				return 0;

		} else if (how_many_dimensions == 3) {
			size_for_each_dimension = sizes_for_union_struct_var_alloc (experssion);

			if (size_for_each_dimension != NULL) {
				if (type) {
					do {__BASIC__STRUCT__(union_struct) -> slli = (long long int ****) malloc (sizeof (long long int ***));} while (__BASIC__STRUCT__(union_struct) -> slli == NULL);
					
					//Allocate 3d array
					do {(__BASIC__STRUCT__(union_struct) -> slli) [0] = (long long int ***) malloc (sizeof (long long int **) * size_for_each_dimension [1][0]);} while ((__BASIC__STRUCT__(union_struct) -> slli) [0] == NULL);

					//Allocate 2d for each index of 3d
					for (unsigned int _3 = 0; _3 < size_for_each_dimension [1][0]; _3++) {
						if (size_for_each_dimension [0][0] == 1)
							do {(__BASIC__STRUCT__(union_struct) -> slli) [0][_3] = (long long int **) malloc (sizeof (long long int *) * size_for_each_dimension [2][_3]);} while ((__BASIC__STRUCT__(union_struct) -> slli) [0][_3] == NULL);
						else
							do {(__BASIC__STRUCT__(union_struct) -> slli) [0][_3] = (long long int **) malloc (sizeof (long long int *) * size_for_each_dimension [2][0]);} while ((__BASIC__STRUCT__(union_struct) -> slli) [0][_3] == NULL);
					}

					//Allocate 1d for each index of 2d.
					unsigned int start_from = 0;
					for (unsigned int _3 = 0; _3 < size_for_each_dimension [1][0]; ++_3) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned int _2 = 0; _2 < size_for_each_dimension [2][_3]; _2++) {
								if (size_for_each_dimension [1][0] == 1) {
									do {(__BASIC__STRUCT__(union_struct) -> slli) [0][_3][_2] = (long long int *) calloc (size_for_each_dimension [3][start_from], sizeof (long long int));} while ((__BASIC__STRUCT__(union_struct) -> slli) [0][_3][_2] == NULL);
									++start_from;
								} else
									do {(__BASIC__STRUCT__(union_struct) -> slli) [0][_3][_2] = (long long int *) calloc (size_for_each_dimension [3][0], sizeof (long long int));} while ((__BASIC__STRUCT__(union_struct) -> slli) [0][_3][_2] == NULL);
							}
						} else {
							for (unsigned int _2 = 0; _2 < size_for_each_dimension [2][0]; _2++) {
								if (size_for_each_dimension [1][0] == 1)
									do {(__BASIC__STRUCT__(union_struct) -> slli) [0][_3][_2] = (long long int *) calloc (size_for_each_dimension [3][_2], sizeof (long long int));} while ((__BASIC__STRUCT__(union_struct) -> slli) [0][_3][_2] == NULL);
								else
									do {(__BASIC__STRUCT__(union_struct) -> slli) [0][_3][_2] = (long long int *) calloc (size_for_each_dimension [3][0], sizeof (long long int));} while ((__BASIC__STRUCT__(union_struct) -> slli) [0][_3][_2] == NULL);
							}
						}
					}
				} else {
					do {__BASIC__UNION__(union_struct) -> slli = (long long int ****) malloc (sizeof (long long int ***));} while (__BASIC__UNION__(union_struct) -> slli == NULL);
					
					//Allocate 3d array
					do {(__BASIC__UNION__(union_struct) -> slli) [0] = (long long int ***) malloc (sizeof (long long int **) * size_for_each_dimension [1][0]);} while ((__BASIC__UNION__(union_struct) -> slli) [0] == NULL);

					//Allocate 2d for each index of 3d
					for (unsigned int _3 = 0; _3 < size_for_each_dimension [1][0]; _3++) {
						if (size_for_each_dimension [0][0] == 1)
							do {(__BASIC__UNION__(union_struct) -> slli) [0][_3] = (long long int **) malloc (sizeof (long long int *) * size_for_each_dimension [2][_3]);} while ((__BASIC__UNION__(union_struct) -> slli) [0][_3] == NULL);
						else
							do {(__BASIC__UNION__(union_struct) -> slli) [0][_3] = (long long int **) malloc (sizeof (long long int *) * size_for_each_dimension [2][0]);} while ((__BASIC__UNION__(union_struct) -> slli) [0][_3] == NULL);
					}

					//Allocate 1d for each index of 2d.
					unsigned int start_from = 0;
					for (unsigned int _3 = 0; _3 < size_for_each_dimension [1][0]; ++_3) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned int _2 = 0; _2 < size_for_each_dimension [2][_3]; _2++) {
								if (size_for_each_dimension [1][0] == 1) {
									do {(__BASIC__UNION__(union_struct) -> slli) [0][_3][_2] = (long long int *) calloc (size_for_each_dimension [3][start_from], sizeof (long long int));} while ((__BASIC__UNION__(union_struct) -> slli) [0][_3][_2] == NULL);
									++start_from;
								} else
									do {(__BASIC__UNION__(union_struct) -> slli) [0][_3][_2] = (long long int *) calloc (size_for_each_dimension [3][0], sizeof (long long int));} while ((__BASIC__UNION__(union_struct) -> slli) [0][_3][_2] == NULL);
							}
						} else {
							for (unsigned int _2 = 0; _2 < size_for_each_dimension [2][0]; _2++) {
								if (size_for_each_dimension [1][0] == 1)
									do {(__BASIC__UNION__(union_struct) -> slli) [0][_3][_2] = (long long int *) calloc (size_for_each_dimension [3][_2], sizeof (long long int));} while ((__BASIC__UNION__(union_struct) -> slli) [0][_3][_2] == NULL);
								else
									do {(__BASIC__UNION__(union_struct) -> slli) [0][_3][_2] = (long long int *) calloc (size_for_each_dimension [3][0], sizeof (long long int));} while ((__BASIC__UNION__(union_struct) -> slli) [0][_3][_2] == NULL);
							}
						}
					}
				}
			} else
				return 0;

		} else {
			size_for_each_dimension = sizes_for_union_struct_var_alloc (experssion);

			if (size_for_each_dimension != NULL) {
				if (type) {
					do {__BASIC__STRUCT__(union_struct) -> slli = (long long int ****) malloc (sizeof (long long int ***) * size_for_each_dimension [1][0]);} while (__BASIC__STRUCT__(union_struct) -> slli == NULL);

					//Allocate 3d for each index of 4d array.
					for (unsigned int _4 = 0; _4 < size_for_each_dimension [1][0]; _4++) {
						if (size_for_each_dimension [0][0] == 1)
							do {(__BASIC__STRUCT__(union_struct) -> slli) [_4] = (long long int ***) malloc (sizeof (long long int **) * size_for_each_dimension [2][_4]);} while ((__BASIC__STRUCT__(union_struct) -> slli) [_4] == NULL);
						else
							do {(__BASIC__STRUCT__(union_struct) -> slli) [_4] = (long long int ***) malloc (sizeof (long long int **) * size_for_each_dimension [2][0]);} while ((__BASIC__STRUCT__(union_struct) -> slli) [_4] == NULL);
					}

					unsigned int start_from = 0;
					
					//Allocate 2d for each index of 3d array.
					for (unsigned int _4 = 0; _4 < size_for_each_dimension [1][0]; _4++) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][_4]; _3++) {
								if (size_for_each_dimension [0][1] == 1) {
									do {(__BASIC__STRUCT__(union_struct) -> slli) [_4][_3] = (long long int **) malloc (sizeof (long long int *) * size_for_each_dimension [3][start_from]);} while ((__BASIC__STRUCT__(union_struct) -> slli) [_4][_3] == NULL);
									++start_from;
								} else
									do {(__BASIC__STRUCT__(union_struct) -> slli) [_4][_3] = (long long int **) malloc (sizeof (long long int *) * size_for_each_dimension [3][0]);} while ((__BASIC__STRUCT__(union_struct) -> slli) [_4][_3] == NULL);
							}
						} else {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][0]; _3++) {
								if (size_for_each_dimension [0][1] == 1)
									do {(__BASIC__STRUCT__(union_struct) -> slli) [_4][_3] = (long long int **) malloc (sizeof (long long int *) * size_for_each_dimension [3][_3]);} while ((__BASIC__STRUCT__(union_struct) -> slli) [_4][_3] == NULL);
								else
									do {(__BASIC__STRUCT__(union_struct) -> slli) [_4][_3] = (long long int **) malloc (sizeof (long long int *) * size_for_each_dimension [3][0]);} while ((__BASIC__STRUCT__(union_struct) -> slli) [_4][_3] == NULL);
							}
						}
					}

					start_from = 0;

					//Allocate 1d for each index of 2d array.
					for (unsigned int _4 = 0; _4 < size_for_each_dimension [1][0]; ++_4) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][_4]; ++_3) {
								if (size_for_each_dimension [0][1] == 1) {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][_3]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__STRUCT__(union_struct) -> slli) [_4][_3][_2] = (long long int *) calloc (size_for_each_dimension [4][start_from], sizeof (long long int));} while ((__BASIC__STRUCT__(union_struct) -> slli) [_4][_3][_2] == NULL);
											++start_from;
										} else
											do {(__BASIC__STRUCT__(union_struct) -> slli) [_4][_3][_2] = (long long int *) calloc (size_for_each_dimension [4][0], sizeof (long long int));} while ((__BASIC__STRUCT__(union_struct) -> slli) [_4][_3][_2] == NULL);
									}
								} else {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][0]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__STRUCT__(union_struct) -> slli) [_4][_3][_2] = (long long int *) calloc (size_for_each_dimension [4][_2], sizeof (long long int));} while ((__BASIC__STRUCT__(union_struct) -> slli) [_4][_3][_2] == NULL);
										} else
											do {(__BASIC__STRUCT__(union_struct) -> slli) [_4][_3][_2] = (long long int *) calloc (size_for_each_dimension [4][0], sizeof (long long int));} while ((__BASIC__STRUCT__(union_struct) -> slli) [_4][_3][_2] == NULL);
									}
								}
							}
						} else {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][0]; ++_3) {
								if (size_for_each_dimension [0][1] == 1) {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][_3]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__STRUCT__(union_struct) -> slli) [_4][_3][_2] = (long long int *) calloc (size_for_each_dimension [4][start_from], sizeof (long long int));} while ((__BASIC__STRUCT__(union_struct) -> slli) [_4][_3][_2] == NULL);
											++start_from;
										} else
											do {(__BASIC__STRUCT__(union_struct) -> slli) [_4][_3][_2] = (long long int *) calloc (size_for_each_dimension [4][0], sizeof (long long int));} while ((__BASIC__STRUCT__(union_struct) -> slli) [_4][_3][_2] == NULL);
									}
								} else {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][0]; ++_2) {
										if (size_for_each_dimension [0][2] == 1)
											do {(__BASIC__STRUCT__(union_struct) -> slli) [_4][_3][_2] = (long long int *) calloc (size_for_each_dimension [4][_2], sizeof (long long int));} while ((__BASIC__STRUCT__(union_struct) -> slli) [_4][_3][_2] == NULL);
										else
											do {(__BASIC__STRUCT__(union_struct) -> slli) [_4][_3][_2] = (long long int *) calloc (size_for_each_dimension [4][0], sizeof (long long int));} while ((__BASIC__STRUCT__(union_struct) -> slli) [_4][_3][_2] == NULL);
									}
								}
							}
						}
					}
				} else {
					do {__BASIC__UNION__(union_struct) -> slli = (long long int ****) malloc (sizeof (long long int ***) * size_for_each_dimension [1][0]);} while (__BASIC__UNION__(union_struct) -> slli == NULL);

					//Allocate 3d for each index of 4d array.
					for (unsigned int _4 = 0; _4 < size_for_each_dimension [1][0]; _4++) {
						if (size_for_each_dimension [0][0] == 1)
							do {(__BASIC__UNION__(union_struct) -> slli) [_4] = (long long int ***) malloc (sizeof (long long int **) * size_for_each_dimension [2][_4]);} while ((__BASIC__UNION__(union_struct) -> slli) [_4] == NULL);
						else
							do {(__BASIC__UNION__(union_struct) -> slli) [_4] = (long long int ***) malloc (sizeof (long long int **) * size_for_each_dimension [2][0]);} while ((__BASIC__UNION__(union_struct) -> slli) [_4] == NULL);
					}

					unsigned int start_from = 0;
					
					//Allocate 2d for each index of 3d array.
					for (unsigned int _4 = 0; _4 < size_for_each_dimension [1][0]; _4++) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][_4]; _3++) {
								if (size_for_each_dimension [0][1] == 1) {
									do {(__BASIC__UNION__(union_struct) -> slli) [_4][_3] = (long long int **) malloc (sizeof (long long int *) * size_for_each_dimension [3][start_from]);} while ((__BASIC__UNION__(union_struct) -> slli) [_4][_3] == NULL);
									++start_from;
								} else
									do {(__BASIC__UNION__(union_struct) -> slli) [_4][_3] = (long long int **) malloc (sizeof (long long int *) * size_for_each_dimension [3][0]);} while ((__BASIC__UNION__(union_struct) -> slli) [_4][_3] == NULL);
							}
						} else {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][0]; _3++) {
								if (size_for_each_dimension [0][1] == 1)
									do {(__BASIC__UNION__(union_struct) -> slli) [_4][_3] = (long long int **) malloc (sizeof (long long int *) * size_for_each_dimension [3][_3]);} while ((__BASIC__UNION__(union_struct) -> slli) [_4][_3] == NULL);
								else
									do {(__BASIC__UNION__(union_struct) -> slli) [_4][_3] = (long long int **) malloc (sizeof (long long int *) * size_for_each_dimension [3][0]);} while ((__BASIC__UNION__(union_struct) -> slli) [_4][_3] == NULL);
							}
						}
					}

					start_from = 0;

					//Allocate 1d for each index of 2d array.
					for (unsigned int _4 = 0; _4 < size_for_each_dimension [1][0]; ++_4) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][_4]; ++_3) {
								if (size_for_each_dimension [0][1] == 1) {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][_3]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__UNION__(union_struct) -> slli) [_4][_3][_2] = (long long int *) calloc (size_for_each_dimension [4][start_from], sizeof (long long int));} while ((__BASIC__UNION__(union_struct) -> slli) [_4][_3][_2] == NULL);
											++start_from;
										} else
											do {(__BASIC__UNION__(union_struct) -> slli) [_4][_3][_2] = (long long int *) calloc (size_for_each_dimension [4][0], sizeof (long long int));} while ((__BASIC__UNION__(union_struct) -> slli) [_4][_3][_2] == NULL);
									}
								} else {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][0]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__UNION__(union_struct) -> slli) [_4][_3][_2] = (long long int *) calloc (size_for_each_dimension [4][_2], sizeof (long long int));} while ((__BASIC__UNION__(union_struct) -> slli) [_4][_3][_2] == NULL);
										} else
											do {(__BASIC__UNION__(union_struct) -> slli) [_4][_3][_2] = (long long int *) calloc (size_for_each_dimension [4][0], sizeof (long long int));} while ((__BASIC__UNION__(union_struct) -> slli) [_4][_3][_2] == NULL);
									}
								}
							}
						} else {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][0]; ++_3) {
								if (size_for_each_dimension [0][1] == 1) {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][_3]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__UNION__(union_struct) -> slli) [_4][_3][_2] = (long long int *) calloc (size_for_each_dimension [4][start_from], sizeof (long long int));} while ((__BASIC__UNION__(union_struct) -> slli) [_4][_3][_2] == NULL);
											++start_from;
										} else
											do {(__BASIC__UNION__(union_struct) -> slli) [_4][_3][_2] = (long long int *) calloc (size_for_each_dimension [4][0], sizeof (long long int));} while ((__BASIC__UNION__(union_struct) -> slli) [_4][_3][_2] == NULL);
									}
								} else {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][0]; ++_2) {
										if (size_for_each_dimension [0][2] == 1)
											do {(__BASIC__UNION__(union_struct) -> slli) [_4][_3][_2] = (long long int *) calloc (size_for_each_dimension [4][_2], sizeof (long long int));} while ((__BASIC__UNION__(union_struct) -> slli) [_4][_3][_2] == NULL);
										else
											do {(__BASIC__UNION__(union_struct) -> slli) [_4][_3][_2] = (long long int *) calloc (size_for_each_dimension [4][0], sizeof (long long int));} while ((__BASIC__UNION__(union_struct) -> slli) [_4][_3][_2] == NULL);
									}
								}
							}
						}
					}
				}
			}
		}
	} else if (dtype_code == 8) {
		if (type) {
			if (__BASIC__STRUCT__(union_struct) -> ulli != NULL)
				return 1;
		} else {
			if (__BASIC__UNION__(union_struct) -> ulli != NULL)
				return 1;
		}

		if (how_many_dimensions == 0) {
			if (type) {
				do {__BASIC__STRUCT__(union_struct) -> ulli = (unsigned long long int ****) malloc (sizeof (unsigned long long int ***));} while (__BASIC__STRUCT__(union_struct) -> ulli == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> ulli) [0] = (unsigned long long int ***) malloc (sizeof (unsigned long long int **));} while ((__BASIC__STRUCT__(union_struct) -> ulli) [0] == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> ulli) [0][0] = (unsigned long long int **) malloc (sizeof (unsigned long long int *));} while ((__BASIC__STRUCT__(union_struct) -> ulli) [0][0] == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> ulli) [0][0][0] = (unsigned long long int *) calloc (1, sizeof (unsigned long long int));} while ((__BASIC__STRUCT__(union_struct) -> ulli) [0][0][0] == NULL);
			} else {
				do {__BASIC__UNION__(union_struct) -> ulli = (unsigned long long int ****) malloc (sizeof (unsigned long long int ***));} while (__BASIC__UNION__(union_struct) -> ulli == NULL);
				do {(__BASIC__UNION__(union_struct) -> ulli) [0] = (unsigned long long int ***) malloc (sizeof (unsigned long long int **));} while ((__BASIC__UNION__(union_struct) -> ulli) [0] == NULL);
				do {(__BASIC__UNION__(union_struct) -> ulli) [0][0] = (unsigned long long int **) malloc (sizeof (unsigned long long int *));} while ((__BASIC__UNION__(union_struct) -> ulli) [0][0] == NULL);
				do {(__BASIC__UNION__(union_struct) -> ulli) [0][0][0] = (unsigned long long int *) calloc (1, sizeof (unsigned long long int));} while ((__BASIC__UNION__(union_struct) -> ulli) [0][0][0] == NULL);
			}
		} else if (how_many_dimensions == 1) {
			if (type) {
				do {__BASIC__STRUCT__(union_struct) -> ulli = (unsigned long long int ****) malloc (sizeof (unsigned long long int ***));} while (__BASIC__STRUCT__(union_struct) -> ulli == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> ulli) [0] = (unsigned long long int ***) malloc (sizeof (unsigned long long int **));} while ((__BASIC__STRUCT__(union_struct) -> ulli) [0] == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> ulli) [0][0] = (unsigned long long int **) malloc (sizeof (unsigned long long int *));} while ((__BASIC__STRUCT__(union_struct) -> ulli) [0][0] == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> ulli) [0][0][0] = (unsigned long long int *) calloc (to_uint (experssion), sizeof (unsigned long long int));} while ((__BASIC__STRUCT__(union_struct) -> ulli) [0][0][0] == NULL);
			} else {
				do {__BASIC__UNION__(union_struct) -> ulli = (unsigned long long int ****) malloc (sizeof (unsigned long long int ***));} while (__BASIC__UNION__(union_struct) -> ulli == NULL);
				do {(__BASIC__UNION__(union_struct) -> ulli) [0] = (unsigned long long int ***) malloc (sizeof (unsigned long long int **));} while ((__BASIC__UNION__(union_struct) -> ulli) [0] == NULL);
				do {(__BASIC__UNION__(union_struct) -> ulli) [0][0] = (unsigned long long int **) malloc (sizeof (unsigned long long int *));} while ((__BASIC__UNION__(union_struct) -> ulli) [0][0] == NULL);
				do {(__BASIC__UNION__(union_struct) -> ulli) [0][0][0] = (unsigned long long int *) calloc (to_uint (experssion), sizeof (unsigned long long int));} while ((__BASIC__UNION__(union_struct) -> ulli) [0][0][0] == NULL);
			}
		} else if (how_many_dimensions == 2) {
			size_for_each_dimension = sizes_for_union_struct_var_alloc (experssion);

			if (size_for_each_dimension != NULL) {
				//3*[1-2-3] or 3*2
				if (type) {
					do {__BASIC__STRUCT__(union_struct) -> ulli = (unsigned long long int ****) malloc (sizeof (unsigned long long int ***));} while (__BASIC__STRUCT__(union_struct) -> ulli == NULL);
					do {(__BASIC__STRUCT__(union_struct) -> ulli) [0] = (unsigned long long int ***) malloc (sizeof (unsigned long long int **));} while ((__BASIC__STRUCT__(union_struct) -> ulli) [0] == NULL);
					do {(__BASIC__STRUCT__(union_struct) -> ulli) [0][0] = (unsigned long long int **) malloc (sizeof (unsigned long long int *) * size_for_each_dimension [1][0]);} while ((__BASIC__STRUCT__(union_struct) -> ulli) [0][0] == NULL);
					
					if (size_for_each_dimension [0][0] == 1)
						for (unsigned long long int counter = 0; counter < size_for_each_dimension [1][0]; ++counter)
							do {(__BASIC__STRUCT__(union_struct) -> ulli) [0][0][counter] = (unsigned long long int *) calloc (size_for_each_dimension [2][counter], sizeof (unsigned long long int));} while ((__BASIC__STRUCT__(union_struct) -> ulli) [0][0][counter] == NULL);
					else
						for (unsigned long long int counter = 0; counter < size_for_each_dimension [1][0]; ++counter)
							do {(__BASIC__STRUCT__(union_struct) -> ulli) [0][0][counter] = (unsigned long long int *) calloc (size_for_each_dimension [2][0], sizeof (unsigned long long int));} while ((__BASIC__STRUCT__(union_struct) -> ulli) [0][0][counter] == NULL);
				} else {
					do {__BASIC__UNION__(union_struct) -> ulli = (unsigned long long int ****) malloc (sizeof (unsigned long long int ***));} while (__BASIC__UNION__(union_struct) -> ulli == NULL);
					do {(__BASIC__UNION__(union_struct) -> ulli) [0] = (unsigned long long int ***) malloc (sizeof (unsigned long long int **));} while ((__BASIC__UNION__(union_struct) -> ulli) [0] == NULL);
					do {(__BASIC__UNION__(union_struct) -> ulli) [0][0] = (unsigned long long int **) malloc (sizeof (unsigned long long int *) * size_for_each_dimension [1][0]);} while ((__BASIC__UNION__(union_struct) -> ulli) [0][0] == NULL);
					
					if (size_for_each_dimension [0][0] == 1)
						for (unsigned long long int counter = 0; counter < size_for_each_dimension [1][0]; ++counter)
							do {(__BASIC__UNION__(union_struct) -> ulli) [0][0][counter] = (unsigned long long int *) calloc (size_for_each_dimension [2][counter], sizeof (unsigned long long int));} while ((__BASIC__UNION__(union_struct) -> ulli) [0][0][counter] == NULL);
					else
						for (unsigned long long int counter = 0; counter < size_for_each_dimension [1][0]; ++counter)
							do {(__BASIC__UNION__(union_struct) -> ulli) [0][0][counter] = (unsigned long long int *) calloc (size_for_each_dimension [2][0], sizeof (unsigned long long int));} while ((__BASIC__UNION__(union_struct) -> ulli) [0][0][counter] == NULL);
				}
			} else
				return 0;

		} else if (how_many_dimensions == 3) {
			size_for_each_dimension = sizes_for_union_struct_var_alloc (experssion);

			if (size_for_each_dimension != NULL) {
				if (type) {
					do {__BASIC__STRUCT__(union_struct) -> ulli = (unsigned long long int ****) malloc (sizeof (unsigned long long int ***));} while (__BASIC__STRUCT__(union_struct) -> ulli == NULL);
					
					//Allocate 3d array
					do {(__BASIC__STRUCT__(union_struct) -> ulli) [0] = (unsigned long long int ***) malloc (sizeof (unsigned long long int **) * size_for_each_dimension [1][0]);} while ((__BASIC__STRUCT__(union_struct) -> ulli) [0] == NULL);

					//Allocate 2d for each index of 3d
					for (unsigned long long int _3 = 0; _3 < size_for_each_dimension [1][0]; _3++) {
						if (size_for_each_dimension [0][0] == 1)
							do {(__BASIC__STRUCT__(union_struct) -> ulli) [0][_3] = (unsigned long long int **) malloc (sizeof (unsigned long long int *) * size_for_each_dimension [2][_3]);} while ((__BASIC__STRUCT__(union_struct) -> ulli) [0][_3] == NULL);
						else
							do {(__BASIC__STRUCT__(union_struct) -> ulli) [0][_3] = (unsigned long long int **) malloc (sizeof (unsigned long long int *) * size_for_each_dimension [2][0]);} while ((__BASIC__STRUCT__(union_struct) -> ulli) [0][_3] == NULL);
					}

					//Allocate 1d for each index of 2d.
					unsigned int start_from = 0;
					for (unsigned long long int _3 = 0; _3 < size_for_each_dimension [1][0]; ++_3) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned long long int _2 = 0; _2 < size_for_each_dimension [2][_3]; _2++) {
								if (size_for_each_dimension [1][0] == 1) {
									do {(__BASIC__STRUCT__(union_struct) -> ulli) [0][_3][_2] = (unsigned long long int *) calloc (size_for_each_dimension [3][start_from], sizeof (unsigned long long int));} while ((__BASIC__STRUCT__(union_struct) -> ulli) [0][_3][_2] == NULL);
									++start_from;
								} else
									do {(__BASIC__STRUCT__(union_struct) -> ulli) [0][_3][_2] = (unsigned long long int *) calloc (size_for_each_dimension [3][0], sizeof (unsigned long long int));} while ((__BASIC__STRUCT__(union_struct) -> ulli) [0][_3][_2] == NULL);
							}
						} else {
							for (unsigned long long int _2 = 0; _2 < size_for_each_dimension [2][0]; _2++) {
								if (size_for_each_dimension [1][0] == 1)
									do {(__BASIC__STRUCT__(union_struct) -> ulli) [0][_3][_2] = (unsigned long long int *) calloc (size_for_each_dimension [3][_2], sizeof (unsigned long long int));} while ((__BASIC__STRUCT__(union_struct) -> ulli) [0][_3][_2] == NULL);
								else
									do {(__BASIC__STRUCT__(union_struct) -> ulli) [0][_3][_2] = (unsigned long long int *) calloc (size_for_each_dimension [3][0], sizeof (unsigned long long int));} while ((__BASIC__STRUCT__(union_struct) -> ulli) [0][_3][_2] == NULL);
							}
						}
					}
				} else {
					do {__BASIC__UNION__(union_struct) -> ulli = (unsigned long long int ****) malloc (sizeof (unsigned long long int ***));} while (__BASIC__UNION__(union_struct) -> ulli == NULL);
					
					//Allocate 3d array
					do {(__BASIC__UNION__(union_struct) -> ulli) [0] = (unsigned long long int ***) malloc (sizeof (unsigned long long int **) * size_for_each_dimension [1][0]);} while ((__BASIC__UNION__(union_struct) -> ulli) [0] == NULL);

					//Allocate 2d for each index of 3d
					for (unsigned long long int _3 = 0; _3 < size_for_each_dimension [1][0]; _3++) {
						if (size_for_each_dimension [0][0] == 1)
							do {(__BASIC__UNION__(union_struct) -> ulli) [0][_3] = (unsigned long long int **) malloc (sizeof (unsigned long long int *) * size_for_each_dimension [2][_3]);} while ((__BASIC__UNION__(union_struct) -> ulli) [0][_3] == NULL);
						else
							do {(__BASIC__UNION__(union_struct) -> ulli) [0][_3] = (unsigned long long int **) malloc (sizeof (unsigned long long int *) * size_for_each_dimension [2][0]);} while ((__BASIC__UNION__(union_struct) -> ulli) [0][_3] == NULL);
					}

					//Allocate 1d for each index of 2d.
					unsigned int start_from = 0;
					for (unsigned long long int _3 = 0; _3 < size_for_each_dimension [1][0]; ++_3) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned long long int _2 = 0; _2 < size_for_each_dimension [2][_3]; _2++) {
								if (size_for_each_dimension [1][0] == 1) {
									do {(__BASIC__UNION__(union_struct) -> ulli) [0][_3][_2] = (unsigned long long int *) calloc (size_for_each_dimension [3][start_from], sizeof (unsigned long long int));} while ((__BASIC__UNION__(union_struct) -> ulli) [0][_3][_2] == NULL);
									++start_from;
								} else
									do {(__BASIC__UNION__(union_struct) -> ulli) [0][_3][_2] = (unsigned long long int *) calloc (size_for_each_dimension [3][0], sizeof (unsigned long long int));} while ((__BASIC__UNION__(union_struct) -> ulli) [0][_3][_2] == NULL);
							}
						} else {
							for (unsigned long long int _2 = 0; _2 < size_for_each_dimension [2][0]; _2++) {
								if (size_for_each_dimension [1][0] == 1)
									do {(__BASIC__UNION__(union_struct) -> ulli) [0][_3][_2] = (unsigned long long int *) calloc (size_for_each_dimension [3][_2], sizeof (unsigned long long int));} while ((__BASIC__UNION__(union_struct) -> ulli) [0][_3][_2] == NULL);
								else
									do {(__BASIC__UNION__(union_struct) -> ulli) [0][_3][_2] = (unsigned long long int *) calloc (size_for_each_dimension [3][0], sizeof (unsigned long long int));} while ((__BASIC__UNION__(union_struct) -> ulli) [0][_3][_2] == NULL);
							}
						}
					}
				}
			} else
				return 0;

		} else {
			size_for_each_dimension = sizes_for_union_struct_var_alloc (experssion);

			if (size_for_each_dimension != NULL) {
				if (type) {
					do {__BASIC__STRUCT__(union_struct) -> ulli = (unsigned long long int ****) malloc (sizeof (unsigned long long int ***) * size_for_each_dimension [1][0]);} while (__BASIC__STRUCT__(union_struct) -> ulli == NULL);

					//Allocate 3d for each index of 4d array.
					for (unsigned long long int _4 = 0; _4 < size_for_each_dimension [1][0]; _4++) {
						if (size_for_each_dimension [0][0] == 1)
							do {(__BASIC__STRUCT__(union_struct) -> ulli) [_4] = (unsigned long long int ***) malloc (sizeof (unsigned long long int **) * size_for_each_dimension [2][_4]);} while ((__BASIC__STRUCT__(union_struct) -> ulli) [_4] == NULL);
						else
							do {(__BASIC__STRUCT__(union_struct) -> ulli) [_4] = (unsigned long long int ***) malloc (sizeof (unsigned long long int **) * size_for_each_dimension [2][0]);} while ((__BASIC__STRUCT__(union_struct) -> ulli) [_4] == NULL);
					}

					unsigned int start_from = 0;
					
					//Allocate 2d for each index of 3d array.
					for (unsigned long long int _4 = 0; _4 < size_for_each_dimension [1][0]; _4++) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned long long int _3 = 0; _3 < size_for_each_dimension [2][_4]; _3++) {
								if (size_for_each_dimension [0][1] == 1) {
									do {(__BASIC__STRUCT__(union_struct) -> ulli) [_4][_3] = (unsigned long long int **) malloc (sizeof (unsigned long long int *) * size_for_each_dimension [3][start_from]);} while ((__BASIC__STRUCT__(union_struct) -> ulli) [_4][_3] == NULL);
									++start_from;
								} else
									do {(__BASIC__STRUCT__(union_struct) -> ulli) [_4][_3] = (unsigned long long int **) malloc (sizeof (unsigned long long int *) * size_for_each_dimension [3][0]);} while ((__BASIC__STRUCT__(union_struct) -> ulli) [_4][_3] == NULL);
							}
						} else {
							for (unsigned long long int _3 = 0; _3 < size_for_each_dimension [2][0]; _3++) {
								if (size_for_each_dimension [0][1] == 1)
									do {(__BASIC__STRUCT__(union_struct) -> ulli) [_4][_3] = (unsigned long long int **) malloc (sizeof (unsigned long long int *) * size_for_each_dimension [3][_3]);} while ((__BASIC__STRUCT__(union_struct) -> ulli) [_4][_3] == NULL);
								else
									do {(__BASIC__STRUCT__(union_struct) -> ulli) [_4][_3] = (unsigned long long int **) malloc (sizeof (unsigned long long int *) * size_for_each_dimension [3][0]);} while ((__BASIC__STRUCT__(union_struct) -> ulli) [_4][_3] == NULL);
							}
						}
					}

					start_from = 0;

					//Allocate 1d for each index of 2d array.
					for (unsigned long long int _4 = 0; _4 < size_for_each_dimension [1][0]; ++_4) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned long long int _3 = 0; _3 < size_for_each_dimension [2][_4]; ++_3) {
								if (size_for_each_dimension [0][1] == 1) {
									for (unsigned long long int _2 = 0; _2 < size_for_each_dimension [3][_3]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__STRUCT__(union_struct) -> ulli) [_4][_3][_2] = (unsigned long long int *) calloc (size_for_each_dimension [4][start_from], sizeof (unsigned long long int));} while ((__BASIC__STRUCT__(union_struct) -> ulli) [_4][_3][_2] == NULL);
											++start_from;
										} else
											do {(__BASIC__STRUCT__(union_struct) -> ulli) [_4][_3][_2] = (unsigned long long int *) calloc (size_for_each_dimension [4][0], sizeof (unsigned long long int));} while ((__BASIC__STRUCT__(union_struct) -> ulli) [_4][_3][_2] == NULL);
									}
								} else {
									for (unsigned long long int _2 = 0; _2 < size_for_each_dimension [3][0]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__STRUCT__(union_struct) -> ulli) [_4][_3][_2] = (unsigned long long int *) calloc (size_for_each_dimension [4][_2], sizeof (unsigned long long int));} while ((__BASIC__STRUCT__(union_struct) -> ulli) [_4][_3][_2] == NULL);
										} else
											do {(__BASIC__STRUCT__(union_struct) -> ulli) [_4][_3][_2] = (unsigned long long int *) calloc (size_for_each_dimension [4][0], sizeof (unsigned long long int));} while ((__BASIC__STRUCT__(union_struct) -> ulli) [_4][_3][_2] == NULL);
									}
								}
							}
						} else {
							for (unsigned long long int _3 = 0; _3 < size_for_each_dimension [2][0]; ++_3) {
								if (size_for_each_dimension [0][1] == 1) {
									for (unsigned long long int _2 = 0; _2 < size_for_each_dimension [3][_3]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__STRUCT__(union_struct) -> ulli) [_4][_3][_2] = (unsigned long long int *) calloc (size_for_each_dimension [4][start_from], sizeof (unsigned long long int));} while ((__BASIC__STRUCT__(union_struct) -> ulli) [_4][_3][_2] == NULL);
											++start_from;
										} else
											do {(__BASIC__STRUCT__(union_struct) -> ulli) [_4][_3][_2] = (unsigned long long int *) calloc (size_for_each_dimension [4][0], sizeof (unsigned long long int));} while ((__BASIC__STRUCT__(union_struct) -> ulli) [_4][_3][_2] == NULL);
									}
								} else {
									for (unsigned long long int _2 = 0; _2 < size_for_each_dimension [3][0]; ++_2) {
										if (size_for_each_dimension [0][2] == 1)
											do {(__BASIC__STRUCT__(union_struct) -> ulli) [_4][_3][_2] = (unsigned long long int *) calloc (size_for_each_dimension [4][_2], sizeof (unsigned long long int));} while ((__BASIC__STRUCT__(union_struct) -> ulli) [_4][_3][_2] == NULL);
										else
											do {(__BASIC__STRUCT__(union_struct) -> ulli) [_4][_3][_2] = (unsigned long long int *) calloc (size_for_each_dimension [4][0], sizeof (unsigned long long int));} while ((__BASIC__STRUCT__(union_struct) -> ulli) [_4][_3][_2] == NULL);
									}
								}
							}
						}
					}
				} else {
					do {__BASIC__UNION__(union_struct) -> ulli = (unsigned long long int ****) malloc (sizeof (unsigned long long int ***) * size_for_each_dimension [1][0]);} while (__BASIC__UNION__(union_struct) -> ulli == NULL);

					//Allocate 3d for each index of 4d array.
					for (unsigned long long int _4 = 0; _4 < size_for_each_dimension [1][0]; _4++) {
						if (size_for_each_dimension [0][0] == 1)
							do {(__BASIC__UNION__(union_struct) -> ulli) [_4] = (unsigned long long int ***) malloc (sizeof (unsigned long long int **) * size_for_each_dimension [2][_4]);} while ((__BASIC__UNION__(union_struct) -> ulli) [_4] == NULL);
						else
							do {(__BASIC__UNION__(union_struct) -> ulli) [_4] = (unsigned long long int ***) malloc (sizeof (unsigned long long int **) * size_for_each_dimension [2][0]);} while ((__BASIC__UNION__(union_struct) -> ulli) [_4] == NULL);
					}

					unsigned int start_from = 0;
					
					//Allocate 2d for each index of 3d array.
					for (unsigned long long int _4 = 0; _4 < size_for_each_dimension [1][0]; _4++) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned long long int _3 = 0; _3 < size_for_each_dimension [2][_4]; _3++) {
								if (size_for_each_dimension [0][1] == 1) {
									do {(__BASIC__UNION__(union_struct) -> ulli) [_4][_3] = (unsigned long long int **) malloc (sizeof (unsigned long long int *) * size_for_each_dimension [3][start_from]);} while ((__BASIC__UNION__(union_struct) -> ulli) [_4][_3] == NULL);
									++start_from;
								} else
									do {(__BASIC__UNION__(union_struct) -> ulli) [_4][_3] = (unsigned long long int **) malloc (sizeof (unsigned long long int *) * size_for_each_dimension [3][0]);} while ((__BASIC__UNION__(union_struct) -> ulli) [_4][_3] == NULL);
							}
						} else {
							for (unsigned long long int _3 = 0; _3 < size_for_each_dimension [2][0]; _3++) {
								if (size_for_each_dimension [0][1] == 1)
									do {(__BASIC__UNION__(union_struct) -> ulli) [_4][_3] = (unsigned long long int **) malloc (sizeof (unsigned long long int *) * size_for_each_dimension [3][_3]);} while ((__BASIC__UNION__(union_struct) -> ulli) [_4][_3] == NULL);
								else
									do {(__BASIC__UNION__(union_struct) -> ulli) [_4][_3] = (unsigned long long int **) malloc (sizeof (unsigned long long int *) * size_for_each_dimension [3][0]);} while ((__BASIC__UNION__(union_struct) -> ulli) [_4][_3] == NULL);
							}
						}
					}

					start_from = 0;

					//Allocate 1d for each index of 2d array.
					for (unsigned long long int _4 = 0; _4 < size_for_each_dimension [1][0]; ++_4) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned long long int _3 = 0; _3 < size_for_each_dimension [2][_4]; ++_3) {
								if (size_for_each_dimension [0][1] == 1) {
									for (unsigned long long int _2 = 0; _2 < size_for_each_dimension [3][_3]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__UNION__(union_struct) -> ulli) [_4][_3][_2] = (unsigned long long int *) calloc (size_for_each_dimension [4][start_from], sizeof (unsigned long long int));} while ((__BASIC__UNION__(union_struct) -> ulli) [_4][_3][_2] == NULL);
											++start_from;
										} else
											do {(__BASIC__UNION__(union_struct) -> ulli) [_4][_3][_2] = (unsigned long long int *) calloc (size_for_each_dimension [4][0], sizeof (unsigned long long int));} while ((__BASIC__UNION__(union_struct) -> ulli) [_4][_3][_2] == NULL);
									}
								} else {
									for (unsigned long long int _2 = 0; _2 < size_for_each_dimension [3][0]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__UNION__(union_struct) -> ulli) [_4][_3][_2] = (unsigned long long int *) calloc (size_for_each_dimension [4][_2], sizeof (unsigned long long int));} while ((__BASIC__UNION__(union_struct) -> ulli) [_4][_3][_2] == NULL);
										} else
											do {(__BASIC__UNION__(union_struct) -> ulli) [_4][_3][_2] = (unsigned long long int *) calloc (size_for_each_dimension [4][0], sizeof (unsigned long long int));} while ((__BASIC__UNION__(union_struct) -> ulli) [_4][_3][_2] == NULL);
									}
								}
							}
						} else {
							for (unsigned long long int _3 = 0; _3 < size_for_each_dimension [2][0]; ++_3) {
								if (size_for_each_dimension [0][1] == 1) {
									for (unsigned long long int _2 = 0; _2 < size_for_each_dimension [3][_3]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__UNION__(union_struct) -> ulli) [_4][_3][_2] = (unsigned long long int *) calloc (size_for_each_dimension [4][start_from], sizeof (unsigned long long int));} while ((__BASIC__UNION__(union_struct) -> ulli) [_4][_3][_2] == NULL);
											++start_from;
										} else
											do {(__BASIC__UNION__(union_struct) -> ulli) [_4][_3][_2] = (unsigned long long int *) calloc (size_for_each_dimension [4][0], sizeof (unsigned long long int));} while ((__BASIC__UNION__(union_struct) -> ulli) [_4][_3][_2] == NULL);
									}
								} else {
									for (unsigned long long int _2 = 0; _2 < size_for_each_dimension [3][0]; ++_2) {
										if (size_for_each_dimension [0][2] == 1)
											do {(__BASIC__UNION__(union_struct) -> ulli) [_4][_3][_2] = (unsigned long long int *) calloc (size_for_each_dimension [4][_2], sizeof (unsigned long long int));} while ((__BASIC__UNION__(union_struct) -> ulli) [_4][_3][_2] == NULL);
										else
											do {(__BASIC__UNION__(union_struct) -> ulli) [_4][_3][_2] = (unsigned long long int *) calloc (size_for_each_dimension [4][0], sizeof (unsigned long long int));} while ((__BASIC__UNION__(union_struct) -> ulli) [_4][_3][_2] == NULL);
									}
								}
							}
						}
					}
				}
			}
		}
	} else if (dtype_code == 9) {
		if (type) {
			if (__BASIC__STRUCT__(union_struct) -> strn != NULL)
				return 1;
		} else {
			if (__BASIC__UNION__(union_struct) -> strn != NULL)
				return 1;
		}

		if (how_many_dimensions == 0) {
			if (type) {
				do {__BASIC__STRUCT__(union_struct) -> strn = (string ***) malloc (sizeof (string **));} while (__BASIC__STRUCT__(union_struct) -> strn == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> strn) [0] = (string **) malloc (sizeof (string *));} while ((__BASIC__STRUCT__(union_struct) -> strn) [0] == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> strn) [0][0] = (string *) malloc (sizeof (string));} while ((__BASIC__STRUCT__(union_struct) -> strn) [0][0] == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> strn) [0][0][0] = (string) calloc (1, sizeof (char));} while ((__BASIC__STRUCT__(union_struct) -> strn) [0][0][0] == NULL);
			} else {
				do {__BASIC__UNION__(union_struct) -> strn = (string ***) malloc (sizeof (string **));} while (__BASIC__UNION__(union_struct) -> strn == NULL);
				do {(__BASIC__UNION__(union_struct) -> strn) [0] = (string **) malloc (sizeof (string *));} while ((__BASIC__UNION__(union_struct) -> strn) [0] == NULL);
				do {(__BASIC__UNION__(union_struct) -> strn) [0][0] = (string *) malloc (sizeof (string));} while ((__BASIC__UNION__(union_struct) -> strn) [0][0] == NULL);
				do {(__BASIC__UNION__(union_struct) -> strn) [0][0][0] = (string) calloc (1, sizeof (char));} while ((__BASIC__UNION__(union_struct) -> strn) [0][0][0] == NULL);
			}
		} else if (how_many_dimensions == 1) {
			if (type) {
				do {__BASIC__STRUCT__(union_struct) -> strn = (string ***) malloc (sizeof (string **));} while (__BASIC__STRUCT__(union_struct) -> strn == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> strn) [0] = (string **) malloc (sizeof (string *));} while ((__BASIC__STRUCT__(union_struct) -> strn) [0] == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> strn) [0][0] = (string *) malloc (sizeof (string));} while ((__BASIC__STRUCT__(union_struct) -> strn) [0][0] == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> strn) [0][0][0] = (string) calloc (to_uint (experssion), sizeof (char));} while ((__BASIC__STRUCT__(union_struct) -> strn) [0][0][0] == NULL);
			} else {
				do {__BASIC__UNION__(union_struct) -> strn = (string ***) malloc (sizeof (string **));} while (__BASIC__UNION__(union_struct) -> strn == NULL);
				do {(__BASIC__UNION__(union_struct) -> strn) [0] = (string **) malloc (sizeof (string *));} while ((__BASIC__UNION__(union_struct) -> strn) [0] == NULL);
				do {(__BASIC__UNION__(union_struct) -> strn) [0][0] = (string *) malloc (sizeof (string));} while ((__BASIC__UNION__(union_struct) -> strn) [0][0] == NULL);
				do {(__BASIC__UNION__(union_struct) -> strn) [0][0][0] = (string) calloc (to_uint (experssion), sizeof (char));} while ((__BASIC__UNION__(union_struct) -> strn) [0][0][0] == NULL);
			}
		} else if (how_many_dimensions == 2) {
			size_for_each_dimension = sizes_for_union_struct_var_alloc (experssion);

			if (size_for_each_dimension != NULL) {
				//3*[1-2-3] or 3*2
				if (type) {
					do {__BASIC__STRUCT__(union_struct) -> strn = (string ***) malloc (sizeof (string **));} while (__BASIC__STRUCT__(union_struct) -> strn == NULL);
					do {(__BASIC__STRUCT__(union_struct) -> strn) [0] = (string **) malloc (sizeof (string *));} while ((__BASIC__STRUCT__(union_struct) -> strn) [0] == NULL);
					do {(__BASIC__STRUCT__(union_struct) -> strn) [0][0] = (string *) malloc (sizeof (string) * size_for_each_dimension [1][0]);} while ((__BASIC__STRUCT__(union_struct) -> strn) [0][0] == NULL);
					
					if (size_for_each_dimension [0][0] == 1)
						for (unsigned int counter = 0; counter < size_for_each_dimension [1][0]; ++counter)
							do {(__BASIC__STRUCT__(union_struct) -> strn) [0][0][counter] = (string) calloc (size_for_each_dimension [2][counter], sizeof (char));} while ((__BASIC__STRUCT__(union_struct) -> strn) [0][0][counter] == NULL);
					else
						for (unsigned int counter = 0; counter < size_for_each_dimension [1][0]; ++counter)
							do {(__BASIC__STRUCT__(union_struct) -> strn) [0][0][counter] = (string) calloc (size_for_each_dimension [2][0], sizeof (char));} while ((__BASIC__STRUCT__(union_struct) -> strn) [0][0][counter] == NULL);
				} else {
					do {__BASIC__UNION__(union_struct) -> strn = (string ***) malloc (sizeof (string **));} while (__BASIC__UNION__(union_struct) -> strn == NULL);
					do {(__BASIC__UNION__(union_struct) -> strn) [0] = (string **) malloc (sizeof (string *));} while ((__BASIC__UNION__(union_struct) -> strn) [0] == NULL);
					do {(__BASIC__UNION__(union_struct) -> strn) [0][0] = (string *) malloc (sizeof (string) * size_for_each_dimension [1][0]);} while ((__BASIC__UNION__(union_struct) -> strn) [0][0] == NULL);
					
					if (size_for_each_dimension [0][0] == 1)
						for (unsigned int counter = 0; counter < size_for_each_dimension [1][0]; ++counter)
							do {(__BASIC__UNION__(union_struct) -> strn) [0][0][counter] = (string) calloc (size_for_each_dimension [2][counter], sizeof (char));} while ((__BASIC__UNION__(union_struct) -> strn) [0][0][counter] == NULL);
					else
						for (unsigned int counter = 0; counter < size_for_each_dimension [1][0]; ++counter)
							do {(__BASIC__UNION__(union_struct) -> strn) [0][0][counter] = (string) calloc (size_for_each_dimension [2][0], sizeof (char));} while ((__BASIC__UNION__(union_struct) -> strn) [0][0][counter] == NULL);
				}
			} else
				return 0;

		} else if (how_many_dimensions == 3) {
			size_for_each_dimension = sizes_for_union_struct_var_alloc (experssion);

			if (size_for_each_dimension != NULL) {
				if (type) {
					do {__BASIC__STRUCT__(union_struct) -> strn = (string ***) malloc (sizeof (string **));} while (__BASIC__STRUCT__(union_struct) -> strn == NULL);
					
					//Allocate 3d array
					do {(__BASIC__STRUCT__(union_struct) -> strn) [0] = (string **) malloc (sizeof (string *) * size_for_each_dimension [1][0]);} while ((__BASIC__STRUCT__(union_struct) -> strn) [0] == NULL);

					//Allocate 2d for each index of 3d
					for (unsigned int _3 = 0; _3 < size_for_each_dimension [1][0]; _3++) {
						if (size_for_each_dimension [0][0] == 1)
							do {(__BASIC__STRUCT__(union_struct) -> strn) [0][_3] = (string *) malloc (sizeof (string) * size_for_each_dimension [2][_3]);} while ((__BASIC__STRUCT__(union_struct) -> strn) [0][_3] == NULL);
						else
							do {(__BASIC__STRUCT__(union_struct) -> strn) [0][_3] = (string *) malloc (sizeof (string) * size_for_each_dimension [2][0]);} while ((__BASIC__STRUCT__(union_struct) -> strn) [0][_3] == NULL);
					}

					//Allocate 1d for each index of 2d.
					unsigned int start_from = 0;
					for (unsigned int  _3 = 0; _3 < size_for_each_dimension [1][0]; ++_3) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned int _2 = 0; _2 < size_for_each_dimension [2][_3]; _2++) {
								if (size_for_each_dimension [1][0] == 1) {
									do {(__BASIC__STRUCT__(union_struct) -> strn) [0][_3][_2] = (string) calloc (size_for_each_dimension [3][start_from], sizeof (char));} while ((__BASIC__STRUCT__(union_struct) -> strn) [0][_3][_2] == NULL);
									++start_from;
								} else
									do {(__BASIC__STRUCT__(union_struct) -> strn) [0][_3][_2] = (string) calloc (size_for_each_dimension [3][0], sizeof (char));} while ((__BASIC__STRUCT__(union_struct) -> strn) [0][_3][_2] == NULL);
							}
						} else {
							for (unsigned int _2 = 0; _2 < size_for_each_dimension [2][0]; _2++) {
								if (size_for_each_dimension [1][0] == 1)
									do {(__BASIC__STRUCT__(union_struct) -> strn) [0][_3][_2] = (string) calloc (size_for_each_dimension [3][_2], sizeof (char));} while ((__BASIC__STRUCT__(union_struct) -> strn) [0][_3][_2] == NULL);
								else
									do {(__BASIC__STRUCT__(union_struct) -> strn) [0][_3][_2] = (string) calloc (size_for_each_dimension [3][0], sizeof (char));} while ((__BASIC__STRUCT__(union_struct) -> strn) [0][_3][_2] == NULL);
							}
						}
					}
				} else {
					do {__BASIC__UNION__(union_struct) -> strn = (string ***) malloc (sizeof (string **));} while (__BASIC__UNION__(union_struct) -> strn == NULL);
					
					//Allocate 3d array
					do {(__BASIC__UNION__(union_struct) -> strn) [0] = (string **) malloc (sizeof (string *) * size_for_each_dimension [1][0]);} while ((__BASIC__UNION__(union_struct) -> strn) [0] == NULL);

					//Allocate 2d for each index of 3d
					for (unsigned int _3 = 0; _3 < size_for_each_dimension [1][0]; _3++) {
						if (size_for_each_dimension [0][0] == 1)
							do {(__BASIC__UNION__(union_struct) -> strn) [0][_3] = (string *) malloc (sizeof (string) * size_for_each_dimension [2][_3]);} while ((__BASIC__UNION__(union_struct) -> strn) [0][_3] == NULL);
						else
							do {(__BASIC__UNION__(union_struct) -> strn) [0][_3] = (string *) malloc (sizeof (string) * size_for_each_dimension [2][0]);} while ((__BASIC__UNION__(union_struct) -> strn) [0][_3] == NULL);
					}

					//Allocate 1d for each index of 2d.
					unsigned int start_from = 0;
					for (unsigned int _3 = 0; _3 < size_for_each_dimension [1][0]; ++_3) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned int _2 = 0; _2 < size_for_each_dimension [2][_3]; _2++) {
								if (size_for_each_dimension [1][0] == 1) {
									do {(__BASIC__UNION__(union_struct) -> strn) [0][_3][_2] = (string) calloc (size_for_each_dimension [3][start_from], sizeof (char));} while ((__BASIC__UNION__(union_struct) -> strn) [0][_3][_2] == NULL);
									++start_from;
								} else
									do {(__BASIC__UNION__(union_struct) -> strn) [0][_3][_2] = (string) calloc (size_for_each_dimension [3][0], sizeof (char));} while ((__BASIC__UNION__(union_struct) -> strn) [0][_3][_2] == NULL);
							}
						} else {
							for (unsigned int _2 = 0; _2 < size_for_each_dimension [2][0]; _2++) {
								if (size_for_each_dimension [1][0] == 1)
									do {(__BASIC__UNION__(union_struct) -> strn) [0][_3][_2] = (string) calloc (size_for_each_dimension [3][_2], sizeof (char));} while ((__BASIC__UNION__(union_struct) -> strn) [0][_3][_2] == NULL);
								else
									do {(__BASIC__UNION__(union_struct) -> strn) [0][_3][_2] = (string) calloc (size_for_each_dimension [3][0], sizeof (char));} while ((__BASIC__UNION__(union_struct) -> strn) [0][_3][_2] == NULL);
							}
						}
					}
				}
			} else
				return 0;

		} else {
			size_for_each_dimension = sizes_for_union_struct_var_alloc (experssion);

			if (size_for_each_dimension != NULL) {
				if (type) {
					do {__BASIC__STRUCT__(union_struct) -> strn = (string ***) malloc (sizeof (string **) * size_for_each_dimension [1][0]);} while (__BASIC__STRUCT__(union_struct) -> strn == NULL);

					//Allocate 3d for each index of 4d array.
					for (unsigned int _4 = 0; _4 < size_for_each_dimension [1][0]; _4++) {
						if (size_for_each_dimension [0][0] == 1)
							do {(__BASIC__STRUCT__(union_struct) -> strn) [_4] = (string **) malloc (sizeof (string *) * size_for_each_dimension [2][_4]);} while ((__BASIC__STRUCT__(union_struct) -> strn) [_4] == NULL);
						else
							do {(__BASIC__STRUCT__(union_struct) -> strn) [_4] = (string **) malloc (sizeof (string *) * size_for_each_dimension [2][0]);} while ((__BASIC__STRUCT__(union_struct) -> strn) [_4] == NULL);
					}

					unsigned int start_from = 0;
					
					//Allocate 2d for each index of 3d array.
					for (unsigned int _4 = 0; _4 < size_for_each_dimension [1][0]; _4++) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][_4]; _3++) {
								if (size_for_each_dimension [0][1] == 1) {
									do {(__BASIC__STRUCT__(union_struct) -> strn) [_4][_3] = (string *) malloc (sizeof (string) * size_for_each_dimension [3][start_from]);} while ((__BASIC__STRUCT__(union_struct) -> strn) [_4][_3] == NULL);
									++start_from;
								} else
									do {(__BASIC__STRUCT__(union_struct) -> strn) [_4][_3] = (string *) malloc (sizeof (string) * size_for_each_dimension [3][0]);} while ((__BASIC__STRUCT__(union_struct) -> strn) [_4][_3] == NULL);
							}
						} else {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][0]; _3++) {
								if (size_for_each_dimension [0][1] == 1)
									do {(__BASIC__STRUCT__(union_struct) -> strn) [_4][_3] = (string *) malloc (sizeof (string) * size_for_each_dimension [3][_3]);} while ((__BASIC__STRUCT__(union_struct) -> strn) [_4][_3] == NULL);
								else
									do {(__BASIC__STRUCT__(union_struct) -> strn) [_4][_3] = (string *) malloc (sizeof (string) * size_for_each_dimension [3][0]);} while ((__BASIC__STRUCT__(union_struct) -> strn) [_4][_3] == NULL);
							}
						}
					}

					start_from = 0;

					//Allocate 1d for each index of 2d array.
					for (unsigned int _4 = 0; _4 < size_for_each_dimension [1][0]; ++_4) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][_4]; ++_3) {
								if (size_for_each_dimension [0][1] == 1) {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][_3]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__STRUCT__(union_struct) -> strn) [_4][_3][_2] = (string) calloc (size_for_each_dimension [4][start_from], sizeof (char));} while ((__BASIC__STRUCT__(union_struct) -> strn) [_4][_3][_2] == NULL);
											++start_from;
										} else
											do {(__BASIC__STRUCT__(union_struct) -> strn) [_4][_3][_2] = (string) calloc (size_for_each_dimension [4][0], sizeof (char));} while ((__BASIC__STRUCT__(union_struct) -> strn) [_4][_3][_2] == NULL);
									}
								} else {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][0]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__STRUCT__(union_struct) -> strn) [_4][_3][_2] = (string) calloc (size_for_each_dimension [4][_2], sizeof (char));} while ((__BASIC__STRUCT__(union_struct) -> strn) [_4][_3][_2] == NULL);
										} else
											do {(__BASIC__STRUCT__(union_struct) -> strn) [_4][_3][_2] = (string) calloc (size_for_each_dimension [4][0], sizeof (char));} while ((__BASIC__STRUCT__(union_struct) -> strn) [_4][_3][_2] == NULL);
									}
								}
							}
						} else {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][0]; ++_3) {
								if (size_for_each_dimension [0][1] == 1) {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][_3]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__STRUCT__(union_struct) -> strn) [_4][_3][_2] = (string) calloc (size_for_each_dimension [4][start_from], sizeof (char));} while ((__BASIC__STRUCT__(union_struct) -> strn) [_4][_3][_2] == NULL);
											++start_from;
										} else
											do {(__BASIC__STRUCT__(union_struct) -> strn) [_4][_3][_2] = (string) calloc (size_for_each_dimension [4][0], sizeof (char));} while ((__BASIC__STRUCT__(union_struct) -> strn) [_4][_3][_2] == NULL);
									}
								} else {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][0]; ++_2) {
										if (size_for_each_dimension [0][2] == 1)
											do {(__BASIC__STRUCT__(union_struct) -> strn) [_4][_3][_2] = (string) calloc (size_for_each_dimension [4][_2], sizeof (char));} while ((__BASIC__STRUCT__(union_struct) -> strn) [_4][_3][_2] == NULL);
										else
											do {(__BASIC__STRUCT__(union_struct) -> strn) [_4][_3][_2] = (string) calloc (size_for_each_dimension [4][0], sizeof (char));} while ((__BASIC__STRUCT__(union_struct) -> strn) [_4][_3][_2] == NULL);
									}
								}
							}
						}
					}
				} else {
					do {__BASIC__UNION__(union_struct) -> strn = (string ***) malloc (sizeof (string **) * size_for_each_dimension [1][0]);} while (__BASIC__UNION__(union_struct) -> strn == NULL);

					//Allocate 3d for each index of 4d array.
					for (unsigned int _4 = 0; _4 < size_for_each_dimension [1][0]; _4++) {
						if (size_for_each_dimension [0][0] == 1)
							do {(__BASIC__UNION__(union_struct) -> strn) [_4] = (string **) malloc (sizeof (string *) * size_for_each_dimension [2][_4]);} while ((__BASIC__UNION__(union_struct) -> strn) [_4] == NULL);
						else
							do {(__BASIC__UNION__(union_struct) -> strn) [_4] = (string **) malloc (sizeof (string *) * size_for_each_dimension [2][0]);} while ((__BASIC__UNION__(union_struct) -> strn) [_4] == NULL);
					}

					unsigned int start_from = 0;
					
					//Allocate 2d for each index of 3d array.
					for (unsigned int _4 = 0; _4 < size_for_each_dimension [1][0]; _4++) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][_4]; _3++) {
								if (size_for_each_dimension [0][1] == 1) {
									do {(__BASIC__UNION__(union_struct) -> strn) [_4][_3] = (string *) malloc (sizeof (string) * size_for_each_dimension [3][start_from]);} while ((__BASIC__UNION__(union_struct) -> strn) [_4][_3] == NULL);
									++start_from;
								} else
									do {(__BASIC__UNION__(union_struct) -> strn) [_4][_3] = (string *) malloc (sizeof (string) * size_for_each_dimension [3][0]);} while ((__BASIC__UNION__(union_struct) -> strn) [_4][_3] == NULL);
							}
						} else {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][0]; _3++) {
								if (size_for_each_dimension [0][1] == 1)
									do {(__BASIC__UNION__(union_struct) -> strn) [_4][_3] = (string *) malloc (sizeof (string) * size_for_each_dimension [3][_3]);} while ((__BASIC__UNION__(union_struct) -> strn) [_4][_3] == NULL);
								else
									do {(__BASIC__UNION__(union_struct) -> strn) [_4][_3] = (string *) malloc (sizeof (string) * size_for_each_dimension [3][0]);} while ((__BASIC__UNION__(union_struct) -> strn) [_4][_3] == NULL);
							}
						}
					}

					start_from = 0;

					//Allocate 1d for each index of 2d array.
					for (unsigned int _4 = 0; _4 < size_for_each_dimension [1][0]; ++_4) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][_4]; ++_3) {
								if (size_for_each_dimension [0][1] == 1) {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][_3]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__UNION__(union_struct) -> strn) [_4][_3][_2] = (string) calloc (size_for_each_dimension [4][start_from], sizeof (char));} while ((__BASIC__UNION__(union_struct) -> strn) [_4][_3][_2] == NULL);
											++start_from;
										} else
											do {(__BASIC__UNION__(union_struct) -> strn) [_4][_3][_2] = (string) calloc (size_for_each_dimension [4][0], sizeof (char));} while ((__BASIC__UNION__(union_struct) -> strn) [_4][_3][_2] == NULL);
									}
								} else {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][0]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__UNION__(union_struct) -> strn) [_4][_3][_2] = (string) calloc (size_for_each_dimension [4][_2], sizeof (char));} while ((__BASIC__UNION__(union_struct) -> strn) [_4][_3][_2] == NULL);
										} else
											do {(__BASIC__UNION__(union_struct) -> strn) [_4][_3][_2] = (string) calloc (size_for_each_dimension [4][0], sizeof (char));} while ((__BASIC__UNION__(union_struct) -> strn) [_4][_3][_2] == NULL);
									}
								}
							}
						} else {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][0]; ++_3) {
								if (size_for_each_dimension [0][1] == 1) {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][_3]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__UNION__(union_struct) -> strn) [_4][_3][_2] = (string) calloc (size_for_each_dimension [4][start_from], sizeof (char));} while ((__BASIC__UNION__(union_struct) -> strn) [_4][_3][_2] == NULL);
											++start_from;
										} else
											do {(__BASIC__UNION__(union_struct) -> strn) [_4][_3][_2] = (string) calloc (size_for_each_dimension [4][0], sizeof (char));} while ((__BASIC__UNION__(union_struct) -> strn) [_4][_3][_2] == NULL);
									}
								} else {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][0]; ++_2) {
										if (size_for_each_dimension [0][2] == 1)
											do {(__BASIC__UNION__(union_struct) -> strn) [_4][_3][_2] = (string) calloc (size_for_each_dimension [4][_2], sizeof (char));} while ((__BASIC__UNION__(union_struct) -> strn) [_4][_3][_2] == NULL);
										else
											do {(__BASIC__UNION__(union_struct) -> strn) [_4][_3][_2] = (string) calloc (size_for_each_dimension [4][0], sizeof (char));} while ((__BASIC__UNION__(union_struct) -> strn) [_4][_3][_2] == NULL);
									}
								}
							}
						}
					}
				}
			}
		}
	} else {
		if (type) {
			if (__BASIC__STRUCT__(union_struct) -> dble != NULL)
				return 1;
		} else {
			if (__BASIC__UNION__(union_struct) -> dble != NULL)
				return 1;
		}

		if (how_many_dimensions == 0) {
			if (type) {
				do {__BASIC__STRUCT__(union_struct) -> dble = (double ****) malloc (sizeof (double ***));} while (__BASIC__STRUCT__(union_struct) -> dble == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> dble) [0] = (double ***) malloc (sizeof (double **));} while ((__BASIC__STRUCT__(union_struct) -> dble) [0] == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> dble) [0][0] = (double **) malloc (sizeof (double *));} while ((__BASIC__STRUCT__(union_struct) -> dble) [0][0] == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> dble) [0][0][0] = (double *) calloc (1, sizeof (double));} while ((__BASIC__STRUCT__(union_struct) -> dble) [0][0][0] == NULL);
			} else {
				do {__BASIC__UNION__(union_struct) -> dble = (double ****) malloc (sizeof (double ***));} while (__BASIC__UNION__(union_struct) -> dble == NULL);
				do {(__BASIC__UNION__(union_struct) -> dble) [0] = (double ***) malloc (sizeof (double **));} while ((__BASIC__UNION__(union_struct) -> dble) [0] == NULL);
				do {(__BASIC__UNION__(union_struct) -> dble) [0][0] = (double **) malloc (sizeof (double *));} while ((__BASIC__UNION__(union_struct) -> dble) [0][0] == NULL);
				do {(__BASIC__UNION__(union_struct) -> dble) [0][0][0] = (double *) calloc (1, sizeof (double));} while ((__BASIC__UNION__(union_struct) -> dble) [0][0][0] == NULL);
			}
		} else if (how_many_dimensions == 1) {
			if (type) {
				do {__BASIC__STRUCT__(union_struct) -> dble = (double ****) malloc (sizeof (double ***));} while (__BASIC__STRUCT__(union_struct) -> dble == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> dble) [0] = (double ***) malloc (sizeof (double **));} while ((__BASIC__STRUCT__(union_struct) -> dble) [0] == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> dble) [0][0] = (double **) malloc (sizeof (double *));} while ((__BASIC__STRUCT__(union_struct) -> dble) [0][0] == NULL);
				do {(__BASIC__STRUCT__(union_struct) -> dble) [0][0][0] = (double *) calloc (to_uint (experssion), sizeof (double));} while ((__BASIC__STRUCT__(union_struct) -> dble) [0][0][0] == NULL);
			} else {
				do {__BASIC__UNION__(union_struct) -> dble = (double ****) malloc (sizeof (double ***));} while (__BASIC__UNION__(union_struct) -> dble == NULL);
				do {(__BASIC__UNION__(union_struct) -> dble) [0] = (double ***) malloc (sizeof (double **));} while ((__BASIC__UNION__(union_struct) -> dble) [0] == NULL);
				do {(__BASIC__UNION__(union_struct) -> dble) [0][0] = (double **) malloc (sizeof (double *));} while ((__BASIC__UNION__(union_struct) -> dble) [0][0] == NULL);
				do {(__BASIC__UNION__(union_struct) -> dble) [0][0][0] = (double *) calloc (to_uint (experssion), sizeof (double));} while ((__BASIC__UNION__(union_struct) -> dble) [0][0][0] == NULL);
			}
		} else if (how_many_dimensions == 2) {
			size_for_each_dimension = sizes_for_union_struct_var_alloc (experssion);

			if (size_for_each_dimension != NULL) {
				//3*[1-2-3] or 3*2
				if (type) {
					do {__BASIC__STRUCT__(union_struct) -> dble = (double ****) malloc (sizeof (double ***));} while (__BASIC__STRUCT__(union_struct) -> dble == NULL);
					do {(__BASIC__STRUCT__(union_struct) -> dble) [0] = (double ***) malloc (sizeof (double **));} while ((__BASIC__STRUCT__(union_struct) -> dble) [0] == NULL);
					do {(__BASIC__STRUCT__(union_struct) -> dble) [0][0] = (double **) malloc (sizeof (double *) * size_for_each_dimension [1][0]);} while ((__BASIC__STRUCT__(union_struct) -> dble) [0][0] == NULL);
					
					if (size_for_each_dimension [0][0] == 1)
						for (unsigned int counter = 0; counter < size_for_each_dimension [1][0]; ++counter)
							do {(__BASIC__STRUCT__(union_struct) -> dble) [0][0][counter] = (double *) calloc (size_for_each_dimension [2][counter], sizeof (double));} while ((__BASIC__STRUCT__(union_struct) -> dble) [0][0][counter] == NULL);
					else
						for (unsigned int counter = 0; counter < size_for_each_dimension [1][0]; ++counter)
							do {(__BASIC__STRUCT__(union_struct) -> dble) [0][0][counter] = (double *) calloc (size_for_each_dimension [2][0], sizeof (double));} while ((__BASIC__STRUCT__(union_struct) -> dble) [0][0][counter] == NULL);
				} else {
					do {__BASIC__UNION__(union_struct) -> dble = (double ****) malloc (sizeof (double ***));} while (__BASIC__UNION__(union_struct) -> dble == NULL);
					do {(__BASIC__UNION__(union_struct) -> dble) [0] = (double ***) malloc (sizeof (double **));} while ((__BASIC__UNION__(union_struct) -> dble) [0] == NULL);
					do {(__BASIC__UNION__(union_struct) -> dble) [0][0] = (double **) malloc (sizeof (double *) * size_for_each_dimension [1][0]);} while ((__BASIC__UNION__(union_struct) -> dble) [0][0] == NULL);
					
					if (size_for_each_dimension [0][0] == 1)
						for (unsigned int counter = 0; counter < size_for_each_dimension [1][0]; ++counter)
							do {(__BASIC__UNION__(union_struct) -> dble) [0][0][counter] = (double *) calloc (size_for_each_dimension [2][counter], sizeof (double));} while ((__BASIC__UNION__(union_struct) -> dble) [0][0][counter] == NULL);
					else
						for (unsigned int counter = 0; counter < size_for_each_dimension [1][0]; ++counter)
							do {(__BASIC__UNION__(union_struct) -> dble) [0][0][counter] = (double *) calloc (size_for_each_dimension [2][0], sizeof (double));} while ((__BASIC__UNION__(union_struct) -> dble) [0][0][counter] == NULL);
				}
			} else
				return 0;

		} else if (how_many_dimensions == 3) {
			size_for_each_dimension = sizes_for_union_struct_var_alloc (experssion);

			if (size_for_each_dimension != NULL) {
				if (type) {
					do {__BASIC__STRUCT__(union_struct) -> dble = (double ****) malloc (sizeof (double ***));} while (__BASIC__STRUCT__(union_struct) -> dble == NULL);
					
					//Allocate 3d array
					do {(__BASIC__STRUCT__(union_struct) -> dble) [0] = (double ***) malloc (sizeof (double **) * size_for_each_dimension [1][0]);} while ((__BASIC__STRUCT__(union_struct) -> dble) [0] == NULL);

					//Allocate 2d for each index of 3d
					for (unsigned int _3 = 0; _3 < size_for_each_dimension [1][0]; _3++) {
						if (size_for_each_dimension [0][0] == 1)
							do {(__BASIC__STRUCT__(union_struct) -> dble) [0][_3] = (double **) malloc (sizeof (double *) * size_for_each_dimension [2][_3]);} while ((__BASIC__STRUCT__(union_struct) -> dble) [0][_3] == NULL);
						else
							do {(__BASIC__STRUCT__(union_struct) -> dble) [0][_3] = (double **) malloc (sizeof (double *) * size_for_each_dimension [2][0]);} while ((__BASIC__STRUCT__(union_struct) -> dble) [0][_3] == NULL);
					}

					//Allocate 1d for each index of 2d.
					unsigned int start_from = 0;
					for (unsigned int _3 = 0; _3 < size_for_each_dimension [1][0]; ++_3) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned int _2 = 0; _2 < size_for_each_dimension [2][_3]; _2++) {
								if (size_for_each_dimension [1][0] == 1) {
									do {(__BASIC__STRUCT__(union_struct) -> dble) [0][_3][_2] = (double *) calloc (size_for_each_dimension [3][start_from], sizeof (double));} while ((__BASIC__STRUCT__(union_struct) -> dble) [0][_3][_2] == NULL);
									++start_from;
								} else
									do {(__BASIC__STRUCT__(union_struct) -> dble) [0][_3][_2] = (double *) calloc (size_for_each_dimension [3][0], sizeof (double));} while ((__BASIC__STRUCT__(union_struct) -> dble) [0][_3][_2] == NULL);
							}
						} else {
							for (unsigned int _2 = 0; _2 < size_for_each_dimension [2][0]; _2++) {
								if (size_for_each_dimension [1][0] == 1)
									do {(__BASIC__STRUCT__(union_struct) -> dble) [0][_3][_2] = (double *) calloc (size_for_each_dimension [3][_2], sizeof (double));} while ((__BASIC__STRUCT__(union_struct) -> dble) [0][_3][_2] == NULL);
								else
									do {(__BASIC__STRUCT__(union_struct) -> dble) [0][_3][_2] = (double *) calloc (size_for_each_dimension [3][0], sizeof (double));} while ((__BASIC__STRUCT__(union_struct) -> dble) [0][_3][_2] == NULL);
							}
						}
					}
				} else {
					do {__BASIC__UNION__(union_struct) -> dble = (double ****) malloc (sizeof (double ***));} while (__BASIC__UNION__(union_struct) -> dble == NULL);
					
					//Allocate 3d array
					do {(__BASIC__UNION__(union_struct) -> dble) [0] = (double ***) malloc (sizeof (double **) * size_for_each_dimension [1][0]);} while ((__BASIC__UNION__(union_struct) -> dble) [0] == NULL);

					//Allocate 2d for each index of 3d
					for (unsigned int _3 = 0; _3 < size_for_each_dimension [1][0]; _3++) {
						if (size_for_each_dimension [0][0] == 1)
							do {(__BASIC__UNION__(union_struct) -> dble) [0][_3] = (double **) malloc (sizeof (double *) * size_for_each_dimension [2][_3]);} while ((__BASIC__UNION__(union_struct) -> dble) [0][_3] == NULL);
						else
							do {(__BASIC__UNION__(union_struct) -> dble) [0][_3] = (double **) malloc (sizeof (double *) * size_for_each_dimension [2][0]);} while ((__BASIC__UNION__(union_struct) -> dble) [0][_3] == NULL);
					}

					//Allocate 1d for each index of 2d.
					unsigned int start_from = 0;
					for (unsigned int _3 = 0; _3 < size_for_each_dimension [1][0]; ++_3) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned int _2 = 0; _2 < size_for_each_dimension [2][_3]; _2++) {
								if (size_for_each_dimension [1][0] == 1) {
									do {(__BASIC__UNION__(union_struct) -> dble) [0][_3][_2] = (double *) calloc (size_for_each_dimension [3][start_from], sizeof (double));} while ((__BASIC__UNION__(union_struct) -> dble) [0][_3][_2] == NULL);
									++start_from;
								} else
									do {(__BASIC__UNION__(union_struct) -> dble) [0][_3][_2] = (double *) calloc (size_for_each_dimension [3][0], sizeof (double));} while ((__BASIC__UNION__(union_struct) -> dble) [0][_3][_2] == NULL);
							}
						} else {
							for (unsigned int _2 = 0; _2 < size_for_each_dimension [2][0]; _2++) {
								if (size_for_each_dimension [1][0] == 1)
									do {(__BASIC__UNION__(union_struct) -> dble) [0][_3][_2] = (double *) calloc (size_for_each_dimension [3][_2], sizeof (double));} while ((__BASIC__UNION__(union_struct) -> dble) [0][_3][_2] == NULL);
								else
									do {(__BASIC__UNION__(union_struct) -> dble) [0][_3][_2] = (double *) calloc (size_for_each_dimension [3][0], sizeof (double));} while ((__BASIC__UNION__(union_struct) -> dble) [0][_3][_2] == NULL);
							}
						}
					}
				}
			} else
				return 0;

		} else {
			size_for_each_dimension = sizes_for_union_struct_var_alloc (experssion);

			if (size_for_each_dimension != NULL) {
				if (type) {
					do {__BASIC__STRUCT__(union_struct) -> dble = (double ****) malloc (sizeof (double ***) * size_for_each_dimension [1][0]);} while (__BASIC__STRUCT__(union_struct) -> dble == NULL);

					//Allocate 3d for each index of 4d array.
					for (unsigned int _4 = 0; _4 < size_for_each_dimension [1][0]; _4++) {
						if (size_for_each_dimension [0][0] == 1)
							do {(__BASIC__STRUCT__(union_struct) -> dble) [_4] = (double ***) malloc (sizeof (double **) * size_for_each_dimension [2][_4]);} while ((__BASIC__STRUCT__(union_struct) -> dble) [_4] == NULL);
						else
							do {(__BASIC__STRUCT__(union_struct) -> dble) [_4] = (double ***) malloc (sizeof (double **) * size_for_each_dimension [2][0]);} while ((__BASIC__STRUCT__(union_struct) -> dble) [_4] == NULL);
					}

					unsigned int start_from = 0;
					
					//Allocate 2d for each index of 3d array.
					for (unsigned int _4 = 0; _4 < size_for_each_dimension [1][0]; _4++) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][_4]; _3++) {
								if (size_for_each_dimension [0][1] == 1) {
									do {(__BASIC__STRUCT__(union_struct) -> dble) [_4][_3] = (double **) malloc (sizeof (double *) * size_for_each_dimension [3][start_from]);} while ((__BASIC__STRUCT__(union_struct) -> dble) [_4][_3] == NULL);
									++start_from;
								} else
									do {(__BASIC__STRUCT__(union_struct) -> dble) [_4][_3] = (double **) malloc (sizeof (double *) * size_for_each_dimension [3][0]);} while ((__BASIC__STRUCT__(union_struct) -> dble) [_4][_3] == NULL);
							}
						} else {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][0]; _3++) {
								if (size_for_each_dimension [0][1] == 1)
									do {(__BASIC__STRUCT__(union_struct) -> dble) [_4][_3] = (double **) malloc (sizeof (double *) * size_for_each_dimension [3][_3]);} while ((__BASIC__STRUCT__(union_struct) -> dble) [_4][_3] == NULL);
								else
									do {(__BASIC__STRUCT__(union_struct) -> dble) [_4][_3] = (double **) malloc (sizeof (double *) * size_for_each_dimension [3][0]);} while ((__BASIC__STRUCT__(union_struct) -> dble) [_4][_3] == NULL);
							}
						}
					}

					start_from = 0;

					//Allocate 1d for each index of 2d array.
					for (unsigned int _4 = 0; _4 < size_for_each_dimension [1][0]; ++_4) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][_4]; ++_3) {
								if (size_for_each_dimension [0][1] == 1) {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][_3]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__STRUCT__(union_struct) -> dble) [_4][_3][_2] = (double *) calloc (size_for_each_dimension [4][start_from], sizeof (double));} while ((__BASIC__STRUCT__(union_struct) -> dble) [_4][_3][_2] == NULL);
											++start_from;
										} else
											do {(__BASIC__STRUCT__(union_struct) -> dble) [_4][_3][_2] = (double *) calloc (size_for_each_dimension [4][0], sizeof (double));} while ((__BASIC__STRUCT__(union_struct) -> dble) [_4][_3][_2] == NULL);
									}
								} else {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][0]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__STRUCT__(union_struct) -> dble) [_4][_3][_2] = (double *) calloc (size_for_each_dimension [4][_2], sizeof (double));} while ((__BASIC__STRUCT__(union_struct) -> dble) [_4][_3][_2] == NULL);
										} else
											do {(__BASIC__STRUCT__(union_struct) -> dble) [_4][_3][_2] = (double *) calloc (size_for_each_dimension [4][0], sizeof (double));} while ((__BASIC__STRUCT__(union_struct) -> dble) [_4][_3][_2] == NULL);
									}
								}
							}
						} else {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][0]; ++_3) {
								if (size_for_each_dimension [0][1] == 1) {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][_3]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__STRUCT__(union_struct) -> dble) [_4][_3][_2] = (double *) calloc (size_for_each_dimension [4][start_from], sizeof (double));} while ((__BASIC__STRUCT__(union_struct) -> dble) [_4][_3][_2] == NULL);
											++start_from;
										} else
											do {(__BASIC__STRUCT__(union_struct) -> dble) [_4][_3][_2] = (double *) calloc (size_for_each_dimension [4][0], sizeof (double));} while ((__BASIC__STRUCT__(union_struct) -> dble) [_4][_3][_2] == NULL);
									}
								} else {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][0]; ++_2) {
										if (size_for_each_dimension [0][2] == 1)
											do {(__BASIC__STRUCT__(union_struct) -> dble) [_4][_3][_2] = (double *) calloc (size_for_each_dimension [4][_2], sizeof (double));} while ((__BASIC__STRUCT__(union_struct) -> dble) [_4][_3][_2] == NULL);
										else
											do {(__BASIC__STRUCT__(union_struct) -> dble) [_4][_3][_2] = (double *) calloc (size_for_each_dimension [4][0], sizeof (double));} while ((__BASIC__STRUCT__(union_struct) -> dble) [_4][_3][_2] == NULL);
									}
								}
							}
						}
					}
				} else {
					do {__BASIC__UNION__(union_struct) -> dble = (double ****) malloc (sizeof (double ***) * size_for_each_dimension [1][0]);} while (__BASIC__UNION__(union_struct) -> dble == NULL);

					//Allocate 3d for each index of 4d array.
					for (unsigned int _4 = 0; _4 < size_for_each_dimension [1][0]; _4++) {
						if (size_for_each_dimension [0][0] == 1)
							do {(__BASIC__UNION__(union_struct) -> dble) [_4] = (double ***) malloc (sizeof (double **) * size_for_each_dimension [2][_4]);} while ((__BASIC__UNION__(union_struct) -> dble) [_4] == NULL);
						else
							do {(__BASIC__UNION__(union_struct) -> dble) [_4] = (double ***) malloc (sizeof (double **) * size_for_each_dimension [2][0]);} while ((__BASIC__UNION__(union_struct) -> dble) [_4] == NULL);
					}

					unsigned int start_from = 0;
					
					//Allocate 2d for each index of 3d array.
					for (unsigned int _4 = 0; _4 < size_for_each_dimension [1][0]; _4++) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][_4]; _3++) {
								if (size_for_each_dimension [0][1] == 1) {
									do {(__BASIC__UNION__(union_struct) -> dble) [_4][_3] = (double **) malloc (sizeof (double *) * size_for_each_dimension [3][start_from]);} while ((__BASIC__UNION__(union_struct) -> dble) [_4][_3] == NULL);
									++start_from;
								} else
									do {(__BASIC__UNION__(union_struct) -> dble) [_4][_3] = (double **) malloc (sizeof (double *) * size_for_each_dimension [3][0]);} while ((__BASIC__UNION__(union_struct) -> dble) [_4][_3] == NULL);
							}
						} else {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][0]; _3++) {
								if (size_for_each_dimension [0][1] == 1)
									do {(__BASIC__UNION__(union_struct) -> dble) [_4][_3] = (double **) malloc (sizeof (double *) * size_for_each_dimension [3][_3]);} while ((__BASIC__UNION__(union_struct) -> dble) [_4][_3] == NULL);
								else
									do {(__BASIC__UNION__(union_struct) -> dble) [_4][_3] = (double **) malloc (sizeof (double *) * size_for_each_dimension [3][0]);} while ((__BASIC__UNION__(union_struct) -> dble) [_4][_3] == NULL);
							}
						}
					}

					start_from = 0;

					//Allocate 1d for each index of 2d array.
					for (unsigned int _4 = 0; _4 < size_for_each_dimension [1][0]; ++_4) {
						if (size_for_each_dimension [0][0] == 1) {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][_4]; ++_3) {
								if (size_for_each_dimension [0][1] == 1) {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][_3]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__UNION__(union_struct) -> dble) [_4][_3][_2] = (double *) calloc (size_for_each_dimension [4][start_from], sizeof (double));} while ((__BASIC__UNION__(union_struct) -> dble) [_4][_3][_2] == NULL);
											++start_from;
										} else
											do {(__BASIC__UNION__(union_struct) -> dble) [_4][_3][_2] = (double *) calloc (size_for_each_dimension [4][0], sizeof (double));} while ((__BASIC__UNION__(union_struct) -> dble) [_4][_3][_2] == NULL);
									}
								} else {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][0]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__UNION__(union_struct) -> dble) [_4][_3][_2] = (double *) calloc (size_for_each_dimension [4][_2], sizeof (double));} while ((__BASIC__UNION__(union_struct) -> dble) [_4][_3][_2] == NULL);
										} else
											do {(__BASIC__UNION__(union_struct) -> dble) [_4][_3][_2] = (double *) calloc (size_for_each_dimension [4][0], sizeof (double));} while ((__BASIC__UNION__(union_struct) -> dble) [_4][_3][_2] == NULL);
									}
								}
							}
						} else {
							for (unsigned int _3 = 0; _3 < size_for_each_dimension [2][0]; ++_3) {
								if (size_for_each_dimension [0][1] == 1) {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][_3]; ++_2) {
										if (size_for_each_dimension [0][2] == 1) {
											do {(__BASIC__UNION__(union_struct) -> dble) [_4][_3][_2] = (double *) calloc (size_for_each_dimension [4][start_from], sizeof (double));} while ((__BASIC__UNION__(union_struct) -> dble) [_4][_3][_2] == NULL);
											++start_from;
										} else
											do {(__BASIC__UNION__(union_struct) -> dble) [_4][_3][_2] = (double *) calloc (size_for_each_dimension [4][0], sizeof (double));} while ((__BASIC__UNION__(union_struct) -> dble) [_4][_3][_2] == NULL);
									}
								} else {
									for (unsigned int _2 = 0; _2 < size_for_each_dimension [3][0]; ++_2) {
										if (size_for_each_dimension [0][2] == 1)
											do {(__BASIC__UNION__(union_struct) -> dble) [_4][_3][_2] = (double *) calloc (size_for_each_dimension [4][_2], sizeof (double));} while ((__BASIC__UNION__(union_struct) -> dble) [_4][_3][_2] == NULL);
										else
											do {(__BASIC__UNION__(union_struct) -> dble) [_4][_3][_2] = (double *) calloc (size_for_each_dimension [4][0], sizeof (double));} while ((__BASIC__UNION__(union_struct) -> dble) [_4][_3][_2] == NULL);
									}
								}
							}
						}
					}
				}
			}
		}
	}

	if (size_for_each_dimension != NULL) {
		for (unsigned int counter = 0; counter <= how_many_dimensions; free (size_for_each_dimension [counter]), size_for_each_dimension [counter] = NULL, ++counter);
		free (size_for_each_dimension);
		size_for_each_dimension = NULL;
	}
	
	return 1;
}


/*
INPUT: A character.
OUTPUT: A unsigned int value represent this character.
INFO: This function is used to hash a character and return that hash.
*/
unsigned int hash_char (char c) {
	unsigned int hash = c;
	
	while (c != 0) {
		hash += (c % 10);
		hash *= 10;
	}

	srand (time (NULL));

	hash += (((rand ()) % RAND_MAX) + 1);
	hash %= 4294967295;

	return hash;
}


/*
INPUT: An unsigned integer whic hold the size.
OUTPUT: Return the power ten of the size.
INFO: This function is used to return the power of ten.
*/
unsigned int power_of_ten (unsigned int size) {
	unsigned int power = 10;
	--size;

	while (size != 0) {
		power *= 10;
		--size;
	}

	return power;
}


/*
INPUT: A pointer to a string which hold the size for each dimension.
OUTPUT: Depending on the dimension passed to the function:
	****IMPORTANT*****: The size of the output match the dimension size the user passed + 1.

INFO: This function is used to get the size for each dimension for a single datatype experssion.

Example: 
1-Let's say we have passed: 2*2 the output be like:
	---------------------------
	|______|__________|_______|
	|   1  |    2     |   1   |
	|______|__________|_______|
					  |   2   |
					  |_______|

	This means we have 2 2d array in the first 2d array we have 1d array of size 1 and in the second 2d array we have 1d array of size 2.

2-Let's say we have passed: 3*[1-2-4]*2 the output be like:
	----------------------------------------
	|_____|_______|___________|____________|
	|  1  |   3   |     1     |     2      |
	|_____|_______|___________|____________|
	|  0  |		  |	    2     |
	|_____|       |___________|
			      |     4     |
				  |___________|

	This means we have 3 3d array:
		In the first 3d array:
			We 1 2d array in it:
				We have a 1d array of size 2.
		
		In the second 3d array:
			We have 2 2d arrays in them:
				We have a 1d array of size 2.
		
		In the third 3d array:
			We have 4 2d array in them:
				We have a 1d array of size 2.


3-Let's say we have passed: 4*2*[1-2]*[2-3-5]:
	---------------------------------------------------------
	|_______|_________|________|___________|________________|
	|   0   |    4    |    2   |     1     |       2        |
	|_______|_________|________|___________|________________|
	|   1   |                  |     2     |       3        |
	|_______|        		   |___________|________________|
	|   1   |                  			   |       5        |
	|_______|                    		   |________________|

	This means we have 4d array of size 4:
		For each index of the 4d array we have 3d array of size 2:
			For the first array of 3d array we have a 2d array of size 1:
				In the 2d array we 1d array of size 2.
			For the second array of 3d array we have a 2d array of size 2:
				In the first 2d array we have 1d array of size 3.
				In the second 2d array we have 1d array of size 5.
*/
unsigned int ** sizes_for_union_struct_var_alloc (string * experssion) {
	void *** content = str_char_split (experssion, '*');

	if (content != NULL) {
		string * split = ((string *) content [1]);
		unsigned int * size = ((unsigned int *) content [0][0]);

		content [1] = NULL;
		content [0][0] = NULL;
		free (content);
		content = NULL;

		free (split [0]);
		split [0] = NULL;

		unsigned int dimension_size = (*size);
		free (size);
		size = NULL;

		unsigned int ** dimension_info = NULL;
		do {dimension_info = (unsigned int **) malloc (sizeof (unsigned int *) * (dimension_size + 1));} while (dimension_info == NULL);
		do {dimension_info [0] = (unsigned int *) malloc (sizeof (unsigned int) * (dimension_size - 1));} while (dimension_info [0] == NULL);
		

		//This loop to fill the first index of the dimension_info.
		for (unsigned int counter = 1; counter <= (dimension_size - 1); ++counter) {
			if (split [(counter + 1)][0] == '[')
				dimension_info [0][(counter - 1)] = 1;
			else
				dimension_info [0][(counter - 1)] = 0;
		}

		_Bool invalid = 0;
		unsigned int dependence = 0;
		unsigned int used_indices = 1;
		//Dimension size = 3.
		//2*[1-2]*[3-6-2]
		for (unsigned int counter = 0; counter < dimension_size; ++counter) {
			if (split [(counter + 1)][0] == '[') {
				remove_at_these (&split [(counter + 1)], 2, ((unsigned int []) {0, (str_len (&split [(counter + 1)]) - 1)}));
				content = str_char_split (&split [(counter + 1)], '-');
				string * temp = ((string *) content [1]);
				unsigned int * temp_size = ((unsigned int *) content [0][0]);

				content [0][0] = NULL;
				content [1] = NULL;
				free (content);
				content = NULL;

				if ((*temp_size) != dependence) {
					invalid = 1;
					for (unsigned int inside = 0; inside <= (*temp_size); free (temp [inside]), temp [inside] = NULL, ++inside);
					free (temp);
					temp = NULL;
					free (temp_size);
					temp_size = NULL;
					break;
				}

				dependence = 0;
				do {dimension_info [(counter + 1)] = (unsigned int *) malloc (sizeof (unsigned int) * (*temp_size));} while (dimension_info [(counter + 1)] == NULL);
				for (unsigned int inside = 0; inside < (*temp_size); dependence += to_uint (&temp [(inside + 1)]), dimension_info [(counter + 1)][inside] = to_uint (&temp [(inside + 1)]), ++inside);
				used_indices++;

				for (unsigned int inside = 0; inside <= (*temp_size); free (temp [inside]), temp [inside] = NULL, ++inside);
				free (temp);
				temp = NULL;
				free (temp_size);
				temp_size = NULL;
			} else {
				dependence = to_uint (&split [(counter + 1)]);
				do {dimension_info [(counter + 1)] = (unsigned int *) malloc (sizeof (unsigned int));} while (dimension_info [(counter + 1)] == NULL);
				dimension_info [(counter + 1)][0] = dependence;
				used_indices++;
			}
		}
		
		if (invalid) {
			for (unsigned int inside = 1; inside <= dimension_size; free (split [inside]), split [inside] = NULL, ++inside);
			free (split);
			split = NULL;
			
			for (unsigned int inside = 0; inside < used_indices; free (dimension_info [inside]), dimension_info [inside] = NULL, ++inside);
			free (dimension_info);
			dimension_info = NULL;

			return NULL;
		} else 
			return dimension_info;
	} else
		return NULL;
}