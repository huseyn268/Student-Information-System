#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "courses.h"
#include "utils.h"

void print_case(const char *name)
{
    printf("-> %s\n", name);
}

void expect_course_error(Error_Code got, Error_Code expected, const char *case_name)
{
    if (got != expected) {
        printf("[FAIL] %s: expected %d, got %d (%s)\n",
               case_name, expected, got, validation_errorMSG(got));
    }
    assert(got == expected);
}

void test_init_Courses(void)
{
    print_case("init_Courses");

    init_Courses();
    assert(arrCourses != NULL);
    assert(course_count == 0);
}

void test_validate_Course(void)
{
    print_case("validate_Course");

    courses base = {
        .course_id = 101,
        .course_code = "CS101",
        .course_name = "Introduction to Programming",
        .credit = 3,
        .capasity = 30,
        .department = "Computer Science",
        .prerquistis = ""
    };

    expect_course_error(validate_Course(&base), COURSE_OK, "valid course");
    expect_course_error(validate_Course(NULL), ERR_COURSE_NULL, "NULL course");

    courses t = base;

    t.course_id = -5;
    expect_course_error(validate_Course(&t), ERR_COURSE_ID, "invalid id");

    t = base;
    t.course_code[0] = '\0';
    expect_course_error(validate_Course(&t), ERR_COURSE_CODE_EMPTY, "empty code");

    t = base;
    strcpy(t.course_code, "CS050");
    expect_course_error(validate_Course(&t), ERR_COURSE_CODE_LEVEL, "level < 100");

    t = base;
    strcpy(t.course_code, "CS500");
    expect_course_error(validate_Course(&t), ERR_COURSE_CODE_LEVEL, "level > 400");

    t = base;
    t.course_name[0] = '\0';
    expect_course_error(validate_Course(&t), ERR_COURSE_NAME_EMPTY, "empty name");

    t = base;
    t.credit = 2;
    expect_course_error(validate_Course(&t), ERR_COURSE_CREDIT, "credit < 3");

    t = base;
    t.credit = 5;
    expect_course_error(validate_Course(&t), ERR_COURSE_CREDIT, "credit > 4");

    t = base;
    t.credit = 4;
    expect_course_error(validate_Course(&t), COURSE_OK, "credit 4 valid");

    t = base;
    t.capasity = 0;
    expect_course_error(validate_Course(&t), ERR_COURSE_CAPACITY, "capacity 0");

    t = base;
    t.department[0] = '\0';
    expect_course_error(validate_Course(&t), ERR_COURSE_DEPARTMENT, "empty department");
}

void test_addCourse(void)
{
    print_case("addCourse");

    courses c1 = {
        .course_id = 101,
        .course_code = "CS101",
        .course_name = "Programming I",
        .credit = 3,
        .capasity = 40,
        .department = "Computer Science",
        .prerquistis = ""
    };

    assert(addCourse(&c1) == 1);
    assert(course_count == 1);

    courses dup_id = c1;
    strcpy(dup_id.course_code, "CS102");
    assert(addCourse(&dup_id) == 0);
    assert(course_count == 1);

    courses dup_code = c1;
    dup_code.course_id = 102;
    assert(addCourse(&dup_code) == 0);
    assert(course_count == 1);

    courses c2 = {
        .course_id = 201,
        .course_code = "CS201",
        .course_name = "Data Structures",
        .credit = 4,
        .capasity = 35,
        .department = "Computer Science",
        .prerquistis = "CS101"
    };

    assert(addCourse(&c2) == 1);
    assert(course_count == 2);

    courses c3 = {
        .course_id = 102,
        .course_code = "MATH101",
        .course_name = "Calculus I",
        .credit = 4,
        .capasity = 50,
        .department = "Mathematics",
        .prerquistis = ""
    };

    assert(addCourse(&c3) == 1);
    assert(course_count == 3);
}

void test_search_functions(void)
{
    print_case("search functions");

    courses *c = search_course_id(101);
    assert(c != NULL);
    assert(strcmp(c->course_code, "CS101") == 0);

    assert(search_course_id(9999) == NULL);

    c = search_course_code("CS201");
    assert(c != NULL);
    assert(strcmp(c->course_name, "Data Structures") == 0);

    assert(search_course_code("CS999") == NULL);

    c = search_course_name("Calculus I");
    assert(c != NULL);
    assert(strcmp(c->course_code, "MATH101") == 0);

    assert(search_course_name("Nonexistent Course") == NULL);
}

void test_delete_Course(void)
{
    print_case("delete_Course");

    int before = course_count;

    assert(delete_Course(101) == 1);
    assert(course_count == before - 1);
    assert(search_course_id(101) == NULL);

    assert(search_course_id(201) != NULL);
    assert(delete_Course(9999) == 0);
}

void test_prerequisites(void)
{
    print_case("prerequisite field");

    courses *with_p = search_course_code("CS201");
    assert(with_p != NULL);
    assert(strcmp(with_p->prerquistis, "CS101") == 0);

    courses *no_p = search_course_code("MATH101");
    assert(no_p != NULL);
    assert(no_p->prerquistis[0] == '\0');
}

int main(void)
{
    printf("=== COURSES MODULE TESTS ===\n\n");

    test_init_Courses();
    test_validate_Course();
    test_addCourse();
    test_search_functions();
    test_delete_Course();
    test_prerequisites();

    printf("\n=== COURSES TESTS PASSED ===\n");
    return 0;
}
