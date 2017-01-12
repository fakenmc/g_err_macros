/*
 * The MIT License (MIT)
 * Copyright (c) 2017 Nuno Fachada
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * */

/**
 * @file
 *
 * Tests for the error-handling macros for applications using Glib.
 *
 * Building the tests executable (requires CMake, Glib and a C compiler):
 *
 *    1. Create a build folder (e.g. mkdir build)
 *    2. CD into the build folder (e.g. cd build) and run CMake (e.g. cmake ..)
 *    3. Compile the tests file (e.g. make)
 *
 * Performing the tests (two options):
 *
 *    1. Run the tests executable directly (e.g. ./tests)
 *    2. Run the tests with gtester (e.g. gtester tests)
 *
 * @author Nuno Fachada
 * @date 2017
 * @copyright [The MIT License (MIT)](https://opensource.org/licenses/MIT)
 * */

#include "_g_err_macros.h"

/** Resolves to error category identifying string, in this case an error
 * in the GErrorf tests. */
#define TEST_G_ERROR_MACROS test_error_handling_error_quark()

/**
 * Test error codes.
 * */
enum test_error_handling_error_codes {
	TEST_G_ERROR_MACROS_SUCCESS = 0,
	TEST_G_ERROR_MACROS_1 = -1,
	TEST_G_ERROR_MACROS_2 = -2
};

/**
 * Resolves to error category identifying string, in this case an
 * error in the GErrorf tests.
 *
 * @return A GQuark structure defined by category identifying string,
 * which identifies the error as a gerrof tests generated error.
 */
GQuark test_error_handling_error_quark() {
	return g_quark_from_static_string("test-error-handling-error-quark");
}

/* ************** */
/* Aux. functions */
/* ************** */

/* This function can create an error. */
static gboolean error_l2_aux(
	int code, const char* xtramsg, GError **err) {

	/* Return status variable. */
	gboolean status;

	/* Check for error. */
	g_if_err_create_goto(*err, TEST_G_ERROR_MACROS,
		code != TEST_G_ERROR_MACROS_SUCCESS, code, error_handler,
		"Big error in level %d function: %s", 2, xtramsg);

	/* If we got here, everything is OK. */
	g_assert(err == NULL || *err == NULL);
	status = TRUE;
	goto finish;

error_handler:

	/* If we got here there was an error, verify that it is so. */
	g_assert(err == NULL || *err != NULL);
	status = FALSE;

finish:

	/* Return status. */
	return status;

}

/* This function can propagate an error created by another function. */
static gboolean error_l1_aux(int code, GError **err) {

	/* Return status variable. */
	gboolean status;

	/* Internal error handling variable. */
	GError* err_internal = NULL;

	/* Call sub-function, check for error. */
	error_l2_aux(code, "called by error_l1_aux", &err_internal);
	g_if_err_propagate_goto(err, err_internal, error_handler);

	/* If we got here, everything is OK. */
	g_assert(err == NULL || *err == NULL);
	status = TRUE;
	goto finish;

error_handler:

	/* If we got here there was an error, verify that it is so. */
	g_assert(err == NULL || *err != NULL);
	status = FALSE;

finish:

	/* Return status. */
	return status;

}

/* ************** */
/* Test functions */
/* ************** */

/**
 * Test one level error handling.
 * */
static void error_one_level_test() {

	/* Error handling object. */
	GError *err = NULL;

	/* Status variable. */
	gboolean status;

	/* Call function which can create an error, and force it to create
	 * error. */
	status = error_l2_aux(TEST_G_ERROR_MACROS_1,
		"called by error_one_level_test", &err);
	g_if_err_goto(err, error_handler);

	/* Function should throw error, so we shouldn't get here. */
	g_assert_not_reached();
	goto finish;

error_handler:

	/* If we got here there was an error, verify that it is so. */
	g_assert_error(err, TEST_G_ERROR_MACROS, TEST_G_ERROR_MACROS_1);
	g_assert_cmpint(status, ==, FALSE);
	g_assert_cmpstr(err->message, ==,
		"Big error in level 2 function: called by error_one_level_test");
	g_error_free(err);

finish:

	/* Return. */
	return;

}

/**
 * Test two level error handling.
 * */
static void error_two_level_test() {

	/* Error handling object. */
	GError *err = NULL;

	/* Status variable. */
	gboolean status;

	/* Call function which can propagate an error thrown by a
	 * sub-function. */
	status = error_l1_aux(TEST_G_ERROR_MACROS_2, &err);
	g_if_err_goto(err, error_handler);

	/* Function should throw error, so we shouldn't get here. */
	g_assert_not_reached();
	goto finish;

error_handler:

	/* If we got here there was an error, verify that it is so. */
	g_assert_error(err, TEST_G_ERROR_MACROS, TEST_G_ERROR_MACROS_2);
	g_assert_cmpint(status, ==, FALSE);
	g_assert_cmpstr(err->message, ==,
		"Big error in level 2 function: called by error_l1_aux");
	g_error_free(err);

finish:

	/* Return. */
	return;

}

/**
 * Test no errors.
 * */
static void error_none_test() {

	/* Error handling object. */
	GError *err = NULL;

	/* Status variable. */
	gboolean status;

	/* Call a function which will not throw an error. */
	status = error_l2_aux(TEST_G_ERROR_MACROS_SUCCESS,
		"called by error_one_level_test", &err);
	g_if_err_goto(err, error_handler);

	/* If we got here, everything is OK. */
	goto finish;

error_handler:

	/* No error should be thrown. */
	g_assert_not_reached();

finish:

	/* Confirm no error was thrown. */
	g_assert_no_error(err);
	g_assert_cmpint(status, ==, TRUE);

	/* Return. */
	return;
}

/**
 * Test an error without additional arguments.
 * */
static void error_no_vargs_test() {

	/* Error handling object. */
	GError *err = NULL;

	/* Create an error without additional arguments. */
	g_if_err_create_goto(err, TEST_G_ERROR_MACROS, 1,
		TEST_G_ERROR_MACROS_1, error_handler,
		"I have no additional arguments");

	/* We shouldn't get here. */
	g_assert_not_reached();
	goto finish;

error_handler:

	/* Check error properties. */
	g_assert_error(err, TEST_G_ERROR_MACROS, TEST_G_ERROR_MACROS_1);
	g_assert_cmpstr(err->message, ==, "I have no additional arguments");
	g_error_free(err);

finish:

	/* Return. */
	return;
}

/**
 * Main function.
 * @param[in] argc Number of command line arguments.
 * @param[in] argv Command line arguments.
 * @return Result of test run.
 * */
int main(int argc, char** argv) {

	g_test_init(&argc, &argv, NULL);

	g_test_add_func("/utils/error-onelevel", error_one_level_test);
	g_test_add_func("/utils/error-twolevel", error_two_level_test);
	g_test_add_func("/utils/error-none", error_none_test);
	g_test_add_func("/utils/error-novargs", error_no_vargs_test);

	return g_test_run();
}
