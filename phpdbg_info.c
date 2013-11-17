/*
   +----------------------------------------------------------------------+
   | PHP Version 5                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2013 The PHP Group                                |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Authors: Felipe Pena <felipe@php.net>                                |
   | Authors: Joe Watkins <joe.watkins@live.co.uk>                        |
   +----------------------------------------------------------------------+
*/

#include "php.h"
#include "phpdbg_utils.h"
#include "phpdbg_info.h"

PHPDBG_INFO(files) /* {{{ */
{
    HashPosition pos;
    char *fname;

	phpdbg_notice("Included files: %d",
		zend_hash_num_elements(&EG(included_files)));

	zend_hash_internal_pointer_reset_ex(&EG(included_files), &pos);
	while (zend_hash_get_current_key_ex(&EG(included_files), &fname,
		NULL, NULL, 0, &pos) == HASH_KEY_IS_STRING) {
		phpdbg_writeln("File: %s", fname);
		zend_hash_move_forward_ex(&EG(included_files), &pos);
	}

	return SUCCESS;
} /* }}} */

PHPDBG_INFO(error) /* {{{ */
{
	return SUCCESS;
} /* }}} */

PHPDBG_INFO(vars) /* {{{ */
{
	HashPosition pos;
	char *var;
	zval **data;

	if (!EG(active_symbol_table)) {
		zend_rebuild_symbol_table(TSRMLS_C);

		if (!EG(active_symbol_table)) {
			phpdbg_error("No active symbol table!");
			return SUCCESS;
		}
	}

	phpdbg_notice("Variables: %d",
		zend_hash_num_elements(EG(active_symbol_table)));

	zend_hash_internal_pointer_reset_ex(EG(active_symbol_table), &pos);
	while (zend_hash_get_current_key_ex(EG(active_symbol_table), &var,
		NULL, NULL, 0, &pos) == HASH_KEY_IS_STRING) {
		zend_hash_get_current_data_ex(EG(active_symbol_table), (void **)&data, &pos);

		if (*var != '_') {
			phpdbg_write("Var: %s = ", var, *data == NULL ? "NULL" : "");
			if (data) {
				zend_print_flat_zval_r(*data TSRMLS_CC);
				phpdbg_writeln(EMPTY);
			}
		}
		zend_hash_move_forward_ex(EG(active_symbol_table), &pos);
	}

	return SUCCESS;
} /* }}} */
