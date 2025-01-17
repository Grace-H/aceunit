#ifndef ACEUNIT_H
#define ACEUNIT_H

#include <stddef.h>

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
#include <stdbool.h>
#elif !defined(__bool_true_false_are_defined)
#define bool int
#define true 1
#define false 0
#define __bool_true_false_are_defined 1
#endif

/** Function pointer to a function used by AceUnit.
 * This is used for testcase functions as well as fixture functions (beforeAll, beforeEach, afterEach, afterAll).
 */
typedef void (*aceunit_func_t)(void);

/** Fixture information.
 * This will usually be auto-generated by `gentest.sh`.
 */
typedef struct {
    /** Pointer to the `beforeAll(void)` method of the fixture, if present, otherwise `NULL`.
     * `beforeAll(void)` will be called once per fixture, before anything else.
     */
    const aceunit_func_t beforeAll;

    /** Pointer to the `afterAll(void)` method of the fixture, if present, otherwise `NULL`.
     * `afterAll(void)` will be called once per fixture, after anything else.
     */
    const aceunit_func_t afterAll;

    /** Pointer to the `beforeEach(void)` method of the fixture, if present, otherwise `NULL`.
     * `beforeEach(void)` will be called once per testcase, before the testcase.
     */
    const aceunit_func_t beforeEach;

    /** Pointer to the `afterEach(void)` method of the fixture, if present, otherwise `NULL`.
     * `afterEach(void)` will be called once per testcase, after the testcase.
     */
    const aceunit_func_t afterEach;

    /** Pointer to the array of test cases. */
    const aceunit_func_t *testcases;
} AceUnit_Fixture_t;

/** Result of a test run. */
typedef struct {
    /** The number of test cases that were executed. */
    int testCaseCount;

    /** The number of test cases that completed successfully. */
    int successCount;

    /** The number of failures in testcases and fixture methods. */
    int failureCount;
} AceUnit_Result_t;

/** The global fixtures. */
extern const AceUnit_Fixture_t *fixtures[];

/** Run AceUnit for the given list of fixtures.
 *
 * @note Don't forget to initialize \p results.
 * The function does not initialize \p results intentionally.
 * This way, multiple runs can be collected together in the same results.
 *
 * @param fixtures Pointer to the fixtures to run, must not be `NULL`, must be terminated with `NULL`.
 * @param result Where to store the results, must not be `NULL`.
 */
extern void AceUnit_run(const AceUnit_Fixture_t *fixtures[], AceUnit_Result_t *result);

/** Run a function, catching some types of errors.
 * Implementations must guarantee that they only ever return `true` upon successful completion of the function.
 * Implementations are allowed to not return at all in case of errors.
 * What types of errors are caught and converted to `false` is implementation defined.
 * @param code Function to run.
 * @return Whether the function was executed successfully.
 * @retval true when the function was executed successfully.
 * @retval false otherwise
 */
extern bool runCatching(void(*code)(void));

/** Fail and abort a test case.
 * Implementations may use different mechanisms for how to actually fail the test case.
 * Popular implementations could be:
 * - `longjmp()` which would work anywhere including most freestanding environments.
 * - `abort()` which would work on all hosted environments.
 * - `exit()` which would work with `AceUnit_Fork`.
 */
extern void AceUnit_fail(void);

#ifndef assert
/** Assert a condition.
 * If the condition is \c false, this will call #AceUnit_fail().
 * @param cond  Condition to assert.
 */
#if defined(__STDC_HOSTED__) && __STDC_HOSTED__ == 1
#include <stdio.h>
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
#define assert(cond) \
    do { \
        if (!(cond)) { \
            fprintf(stderr, "%s:%d: %s: Assertion `%s' failed.\n", __FILE__, __LINE__, __func__, #cond); \
            AceUnit_fail(); \
        } \
    } while (false)
#else
#define assert(cond) \
    do { \
        if (!(cond)) { \
            fprintf(stderr, "%s:%d: Assertion `%s' failed.\n", __FILE__, __LINE__, #cond); \
            AceUnit_fail(); \
        } \
    } while (false)
#endif
#else
#define assert(cond) \
    do { \
        if (!(cond)) { \
            AceUnit_fail(); \
        } \
    } while (false)
#endif
#endif

#endif
