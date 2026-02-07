#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "grade.h"
#include "utils.h"

static void reset_grades(void)
{
    if (arrGrades != NULL) {
        freeGrade();
    }
    init_Grades();
    assert(arrGrades != NULL);
    assert(grade_count == 0);
}

static grades make_grade(int grade_id, int enrollment_id, int student_id,
                          int course_id, const char *letter,
                          double numeric, const char *semester)
{
    grades g;
    memset(&g, 0, sizeof(g));

    g.grade_id = grade_id;
    g.enrollment_id = enrollment_id;
    g.student_id = student_id;
    g.course_id = course_id;

    if (letter) {
        strncpy(g.letter_grade, letter, sizeof(g.letter_grade) - 1);
        g.letter_grade[sizeof(g.letter_grade) - 1] = '\0';
    }

    g.numeric_grade = numeric;

    if (semester) {
        strncpy(g.semester, semester, sizeof(g.semester) - 1);
        g.semester[sizeof(g.semester) - 1] = '\0';
    }

    return g;
}

void test_init_Grades(void)
{
    printf("-> init_Grades\n");
    reset_grades();
    assert(arrGrades != NULL);
    assert(grade_count == 0);
}

void test_validateSemester(void)
{
    printf("-> validateSemester\n");

    assert(validateSemester("2023-FALL") == 1);
    assert(validateSemester("2022-SPRING") == 1);
    assert(validateSemester(NULL) == 0);
    assert(validateSemester("") == 0);
    assert(validateSemester("2023-WINTER") == 0);
}

void test_checkletter_numeric(void)
{
    printf("-> checkletter_numeric\n");

    assert(checkletter_numeric("A", 95.0) == 1);
}

void test_validateGrade(void)
{
    printf("-> validateGrade\n");

    grades base = make_grade(10001, 1, 2020001, 101, "A", 95.0, "2023-FALL");

    assert(validateGrade(&base) == GRADE_OK);
    assert(validateGrade(NULL) == GRADE_NULL);

    grades t = base;
    t.grade_id = 999;
    assert(validateGrade(&t) == GRADE_ID);

    t = base; t.grade_id = 100000;
    assert(validateGrade(&t) == GRADE_ID);

    t = base; t.enrollment_id = 0;
    assert(validateGrade(&t) == GRADE_ENROLLMENT);

    t = base; t.student_id = 999999;
    assert(validateGrade(&t) == GRADE_STUDENT);

    t = base; t.student_id = 2025001;
    assert(validateGrade(&t) == GRADE_STUDENT);

    t = base; t.course_id = 0;
    assert(validateGrade(&t) == GRADE_COURSE);

    t = base; strcpy(t.letter_grade, "D");
    assert(validateGrade(&t) == GRADE_LETTER);

    t = base; strcpy(t.letter_grade, "A+");
    assert(validateGrade(&t) == GRADE_LETTER);

    t = base; t.numeric_grade = -5.0;
    assert(validateGrade(&t) == GRADE_NUMERIC);

    t = base; t.numeric_grade = 105.0;
    assert(validateGrade(&t) == GRADE_NUMERIC);

    t = base; strcpy(t.semester, "2023-WINTER");
    assert(validateGrade(&t) == GRADE_SEMESTER);

    t = base; strcpy(t.letter_grade, "A"); t.numeric_grade = 80.0;
    assert(validateGrade(&t) == GRADE_NUMERIC);
}

void test_search_and_delete(void)
{
    printf("-> search & delete\n");

    reset_grades();

    arrGrades[0] = make_grade(10001, 1, 2020001, 101, "A", 95.0, "2023-FALL");
    grade_count = 1;

    assert(search_grade_id(10001) != NULL);
    assert(seachGrade_byEnrollmentid(1) != NULL);
    assert(seachGrade_byEnrollmentid(999) == NULL);

    assert(deleteGradeBy_EnrollmentId(1) == 1);
    assert(grade_count == 0);
    assert(seachGrade_byEnrollmentid(1) == NULL);

    assert(deleteGradeBy_EnrollmentId(999) == 0);
}

int main(void)
{
    printf("=== GRADE ASSERT TESTS ===\n");

    test_init_Grades();
    test_validateSemester();
    test_checkletter_numeric();
    test_validateGrade();
    test_search_and_delete();

    if (arrGrades != NULL)
        freeGrade();

    printf("=== ALL GRADE TESTS PASSED ===\n");
    return 0;
}
