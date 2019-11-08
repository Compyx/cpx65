/* vim: set et ts=4 sw=4 sts=4 fdm=marker syntax=c.doxygen: */

/** \file   unit.c
 * \brief   Unit testing module
 *
 * Allows some rudimentary unit testing of code.
 */

#include <stdio.h>
#include <stdlib.h>

#include "../base/debug.h"
#include "../base/mem.h"
#include "../base/error.h"

#include "unit.h"


/** \brief  Maximum number of unit test modules
 *
 * Increment this number if unit_module_add() exist the unit-tester bin
 */
#define UNIT_MODULE_MAX 256

/*
 * Forward declarations
 */
static void unit_dump_test(const unit_test_t *test);
static unit_module_t *unit_module_find_by_name(const char *name);


/** \brief  Array of unit test module references
 */
static unit_module_t *modules[UNIT_MODULE_MAX] = { NULL };

/** \brief  Number of registered modules
 */
static size_t modules_count = 0;

/** \brief  Total number of tests run for all modules
 */
static int cummulative_tests_total = 0;

/** \brief  Total number of passed tests of the cummulative tests
 */
static int cummulative_tests_passed = 0;

/** \brief  Total number of test modules run
 */
static int cummulative_modules = 0;


/** \brief  Add a unit test module
 *
 * \param[in,out]   module  unit test module
 *
 * \note    Calls exit(1) in case the array of modules is full, if that happens
 *          please update #UNIT_MODULE_MAX
 */
void unit_module_add(unit_module_t *module)
{
    if (modules_count >= UNIT_MODULE_MAX) {
        fprintf(stderr,
                "%s(): maximum number of %d modules reached, please"
                " increment UNIT_MODULE_MAX in src/unit.c.",
                __func__, UNIT_MODULE_MAX);
        exit(1);
    }
    modules[modules_count++] = module;
}


/** \brief  Find unit test module by \a name
 *
 * \param[in]   name    unit module name
 *
 * \return  pointer to unit_module_t, or `NULL` on error
 */
static unit_module_t *unit_module_find_by_name(const char *name)
{
    if (modules_count > 0) {
        for (size_t i = 0; i < modules_count; i++) {
            if (strcmp(modules[i]->name, name) == 0) {
                return modules[i];
            }
        }
    }
    return NULL;
}

/** \brief  List registered unit test modules on stdout
 */
void unit_dump_module_list(void)
{
    size_t i = 0;

    while (i < modules_count) {
        const unit_module_t *mod = modules[i];
        printf("  %s\n", mod->name);
        printf("    %s\n", mod->description);
        unit_dump_module_test_list(mod);
        i++;
    }
}


/** \brief  Dump info on \a test
 *
 * \param[in]   test    unit test
 */
static void unit_dump_test(const unit_test_t *test)
{
    printf("      %s\n", test->name);
    printf("        %s\n", test->description);
}


/** \brief  Run a test
 *
 * Runs \a test, contained in \a module.
 *
 * The \a module argument is currently required to be able to update the number
 * of tests run and the number of those tests that passed.
 *
 * \param[in,out]   module  unit test module containing \a test
 * \param[in]       test    unit test
 *
 * \return  boolean
 */
static bool unit_module_test_exec(unit_module_t *module, unit_test_t *test)
{
    int total = 0;
    int passed = 0;
    bool result;

    base_debug_unittest("running test '%s'.", test->name);
    result = test->func(&total, &passed);
    base_debug_unittest("result = %s.", result ? "pass" : "fail");
    module->tests_total += total;
    module->tests_passed += passed;

    return result;
}


/** \brief  Dump all tests in \a module on stdout
 *
 * \param[in]   module  unit test module
 */
void unit_dump_module_test_list(const unit_module_t *module)
{
    size_t i;

    for (i = 0; module->tests[i].name != NULL; i++) {
        unit_dump_test(&(module->tests[i]));
    }
}


/** \brief  Dump all test in module with \a name on stdout
 *
 * \param[in]   name    module name
 */
void unit_dump_module_test_list_by_name(const char *name)
{
    unit_module_t *module = unit_module_find_by_name(name);
    if (module != NULL) {
        unit_dump_module_test_list(module);
    }
}


/** \brief  Initialize module for use
 *
 * \param[in,out]   module  unit test module
 *
 * \return  boolean
 */
bool unit_module_init(unit_module_t *module)
{
    bool result = true;

    base_debug_unittest("Initializing module '%s'.", module->name);

    module->tests_total = 0;
    module->tests_passed = 0;

    if (module->setup != NULL) {
        base_debug_unittest(".. Running setup().'");
        result = module->setup();
    }
    return result;
}


/** \brief  Clean up \a module
 *
 * Calls the \a module's teardown function, if available.
 *
 * \param[in]   module  unit test module
 *
 * \return  boolean
 */
bool unit_module_exit(unit_module_t *module)
{
    if (module->teardown != NULL) {
        return module->teardown();
    }
    return true;
}


/** \brief  Run all tests in \a module
 *
 * \param[in,out]   module  unit test module
 *
 * \return  boolean
 */
bool unit_module_exec(unit_module_t *module)
{
    printf(".. running module '%s'.\n", module->name);
    if (!unit_module_init(module)) {
        return false;
    }

    cummulative_modules++;

    for (unit_test_t *test = module->tests; test->name != NULL; test++) {
        bool result;

        printf(".... running test '%s'.\n", test->name);
        result = unit_module_test_exec(module, test);

        if (!result && !test->keep_going) {
            printf("...... test failed and marked fatal, exiting.");
            return false;
        }
    }

    cummulative_tests_total += module->tests_total;
    cummulative_tests_passed += module->tests_passed;

    printf(".. total = %d, passed = %d (%.2f%%)\n",
            module->tests_total,
            module->tests_passed,
            (float)module->tests_passed / (float)(module->tests_total) * 100.0);

    return unit_module_exit(module);
}


/** \brief  Run tests in a module by \a name
 *
 * Wrapper for unit_module_exit().
 *
 * \param[in]   name    name of unit test module
 *
 * \return  boolean
 */
bool unit_module_exec_by_name(const char *name)
{
    unit_module_t *module = unit_module_find_by_name(name);

    if (module == NULL) {
        fprintf(stderr, "failed to find module '%s'.\n", name);
        return false;
    }
    return unit_module_exec(module);
}


/** \brief  Dump end results of running unit tests on stdout
 */
void unit_dump_results(void)
{
    printf("----=> results <=----\n");
    printf("    Modules run : %d\n", cummulative_modules);
    printf("    Total tests : %d\n", cummulative_tests_total);
    printf("    Tests passed: %d\n", cummulative_tests_passed);
    printf("    Pass rate   : %.2f%%\n",
            (float)cummulative_tests_passed / (float)cummulative_tests_total
                * 100.0);
}


/** \brief  Run all units in all modules
 *
 * \return  boolean
 */
bool unit_module_run_all(void)
{
    for (size_t i = 0; modules[i]->name != NULL; i++) {
        printf("Running module '%s':\n", modules[i]->name);
        if (unit_module_exec(modules[i])) {
            return false;
        }
    }
    unit_dump_results();
    return true;
}
