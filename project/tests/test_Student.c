#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "student.h"
#include "utils.h"

int tests_run = 0;
int tests_passed = 0;

#define RUN_TEST(name) \
    do { \
        tests_run++; \
        name(); \
        tests_passed++; \
    } while (0)

void check_student_ok(const Students *s, const char *case_name)
{
    ErrorCode e = validateStudent(s);
    if (e != ERROR_OK) {
        printf("[FAIL] %s -> validateStudent error code: %d\n",
               case_name, (int)e);
    }
    assert(e == ERROR_OK);
}

Students sample_student(void)
{
    Students s = {0};
    s.student_id = 2020001;
    strcpy(s.first_name, "Mehmet");
    strcpy(s.last_name, "Huseyinn");
    strcpy(s.s_mail, "mehmetn@university.edu");
    strcpy(s.s_phone, "555-1234");
    s.enrollment_year = 2020;
    strcpy(s.major, "Computer Science");
    return s;
}

void test_init_students(void)
{
    printf("-> init_Students\n");
    init_Students();

    assert(arrStudents != NULL);
    assert(student_count == 0);
}

void test_validation_cases(void)
{
    printf("-> validateStudent cases\n");

    Students s = sample_student();
    check_student_ok(&s, "valid student");

    Students t = s;
    t.student_id = 242424245;
    assert(validateStudent(&t) == ERROR_STUDENT_ID);

    t = s;
    t.first_name[0] = '\0';
    assert(validateStudent(&t) == ERROR_STUDENT_NAME);

    t = s;
    strcpy(t.s_mail, "johnuniversity.edu");
    assert(validateStudent(&t) == ERROR_STUDENT_MAIL);

    t = s;
    strcpy(t.s_phone, "123-4567");
    assert(validateStudent(&t) == ERROR_STUDENT_PHONE);

    t = s;
    strcpy(t.s_phone, "4444422");
    assert(validateStudent(&t) == ERROR_STUDENT_PHONE);
}

void test_add_search_delete(void)
{
    printf("-> add/search/delete\n");

    Students s1 = sample_student();
    assert(addStudent(&s1) == 1);
    assert(student_count == 1);

    Students dup = s1;
    strcpy(dup.s_mail, "diff@university.edu");
    assert(addStudent(&dup) == 0);

    Students s2 = {0};
    s2.student_id = 2021002;
    strcpy(s2.first_name, "Hüseyin");
    strcpy(s2.last_name, "ARABOĞA");
    strcpy(s2.s_mail, "hüseyinn@university.edu");
    strcpy(s2.s_phone, "555-3622");
    s2.enrollment_year = 2023;
    strcpy(s2.major, "Electrical Engineering");

    check_student_ok(&s2, "student #2");
    assert(addStudent(&s2) == 1);
    assert(student_count == 2);

    Students *f = search_student_id(2021002);
    assert(f && strcmp(f->first_name, "Hüseyin") == 0);

    assert(delete_Student(2020001) == 1);
    assert(search_student_id(2020001) == NULL);
}

int main(void)
{
    printf("=== student tests ===\n");

    RUN_TEST(test_init_students);
    RUN_TEST(test_validation_cases);
    RUN_TEST(test_add_search_delete);

    printf("\nTest summary:\n");
    printf("  Tests run    : %d\n", tests_run);
    printf("  Tests passed : %d\n", tests_passed);

    freeStudent();

    printf("student tests: OK\n");
    return 0;
}
