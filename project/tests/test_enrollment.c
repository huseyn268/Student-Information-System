#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "enrollment.h"
#include "courses.h"
#include "utils.h"


courses make_course(int id, const char *code, const char *name,
                    int credit, int cap, const char *prereq_csv)
{
    courses c;
    memset(&c, 0, sizeof(c));

    c.course_id = id;
    strncpy(c.course_code, code, sizeof(c.course_code) - 1);
    strncpy(c.course_name, name, sizeof(c.course_name) - 1);
    c.credit = credit;
    c.capasity = cap;

    if (prereq_csv)
        strncpy(c.prerquistis, prereq_csv, sizeof(c.prerquistis) - 1);
    else
        c.prerquistis[0] = '\0';

    return c;
}

static enrollment make_enrollment(int eid, int sid, int cid,
                                  int pid, const char *sem,
                                  const char *date, const char *status)
{
    enrollment e;
    memset(&e, 0, sizeof(e));

    e.enrollment_id = eid;
    e.student_id = sid;
    e.course_id = cid;
    e.professor_id = pid;

    strncpy(e.semester, sem, sizeof(e.semester) - 1);
    strncpy(e.enrollment_date, date, sizeof(e.enrollment_date) - 1);
    strncpy(e.status, status, sizeof(e.status) - 1);

    return e;
}

void reset_enrollments(void)
{
    freeEnrollment();
    init_Enrollments();
}

void reset_courses(void)
{
    freeCourses();
    init_Courses();
}


void test_sanity(void)
{
    printf("-> sanity\n");

    init_Enrollments();
    assert(arrEnrollments != NULL);
    assert(enrollment_count == 0);

    reset_enrollments();
    assert(arrEnrollments != NULL);
    assert(enrollment_count == 0);

    freeEnrollment();
    assert(arrEnrollments == NULL);
    assert(enrollment_count == 0);
}

void test_validate_date(void)
{
    printf("-> validate_date\n");

    assert(validate_date("2024-01-15") == 1);
    assert(validate_date("2020-12-31") == 1);

    assert(validate_date(NULL) == 0);
    assert(validate_date("2024/01/15") == 0);
    assert(validate_date("2024-13-01") == 0);
    assert(validate_date("2024-01-32") == 0);
    assert(validate_date("1899-01-01") == 0);
}

void test_validateEnrollment(void)
{
    printf("-> validateEnrollment\n");

    enrollment e = make_enrollment(
        1, 2024001, 101, 5500,
        "2024-FALL", "2024-09-01", "Enrolled"
    );

    assert(validateEnrollment(NULL) == ENROLLMENT_NULL);
    assert(validateEnrollment(&e) == ENROLLMENT_OK);

    e.enrollment_id = 0;
    assert(validateEnrollment(&e) == ENROLLMENT_ID);
    e.enrollment_id = 1;

    e.student_id = 999999;
    assert(validateEnrollment(&e) == ENROLLMENT_STUDENT_ID);
    e.student_id = 2025001;
    assert(validateEnrollment(&e) == ENROLLMENT_STUDENT_ID);
    e.student_id = 2024001;

    e.course_id = 0;
    assert(validateEnrollment(&e) == ENROLLMENT_COURSE_ID);
    e.course_id = 101;

    e.professor_id = 4999;
    assert(validateEnrollment(&e) == ENROLLMENT_PROFESSOR_ID);
    e.professor_id = 5500;

    strcpy(e.semester, "2024-WINTER");
    assert(validateEnrollment(&e) == ENROLLMENT_SEMESTER);

    strcpy(e.semester, "2024-FALL");
    strcpy(e.enrollment_date, "2024/09/01");
    assert(validateEnrollment(&e) == ENROLLMENT_DATE);

    strcpy(e.enrollment_date, "2024-09-01");
    strcpy(e.status, "Pending");
    assert(validateEnrollment(&e) == ENROLLMENT_STATUS);

    strcpy(e.status, "Completed");
    assert(validateEnrollment(&e) == ENROLLMENT_OK);
}

void test_prerequisites(void)
{
    printf("-> prerequisites\n");

    reset_courses();
    reset_enrollments();

    arrCourses[0] = make_course(101, "CS101", "Intro", 3, 30, "");
    arrCourses[1] = make_course(201, "CS201", "DS", 3, 30, "CS101");
    course_count = 2;

    assert(check_prerquisties(2024001, 201) == 0);

    arrEnrollments[0] = make_enrollment(
        1, 2024001, 101, 5500,
        "2023-FALL", "2023-12-20", "Completed"
    );
    enrollment_count = 1;

    assert(check_prerquisties(2024001, 201) == 1);
}

void test_capacity(void)
{
    printf("-> capacity\n");

    reset_courses();
    reset_enrollments();
    arrCourses[0] = make_course(101, "CS101", "Intro", 3, 2, "");
    course_count = 1;
    assert(count_enrolledCourse(101, "2024-FALL") == 0);
    assert(checkCourse_capacity(101, "2024-FALL") == 1);
    arrEnrollments[0] = make_enrollment(1, 2024001, 101, 5500, "2024-FALL", "2024-09-01", "Enrolled");
    arrEnrollments[1] = make_enrollment(2, 2024002, 101, 5500, "2024-FALL", "2024-09-01", "Enrolled");
    enrollment_count = 2;
    assert(count_enrolledCourse(101, "2024-FALL") == 2);
    assert(checkCourse_capacity(101, "2024-FALL") == 0);
    arrEnrollments[1].status[0] = '\0';
    strcpy(arrEnrollments[1].status, "Completed");

    assert(count_enrolledCourse(101, "2024-FALL") == 1);
    assert(checkCourse_capacity(101, "2024-FALL") == 1);
}

void test_deleteEnrollment(void)
{
    printf("-> deleteEnrollment\n");

    reset_enrollments();
    arrEnrollments[0] = make_enrollment(1, 1, 101, 5500, "2024-FALL", "2024-09-01", "Enrolled");
    arrEnrollments[1] = make_enrollment(2, 2, 101, 5500, "2024-FALL", "2024-09-01", "Enrolled");
    arrEnrollments[2] = make_enrollment(3, 3, 101, 5500, "2024-FALL", "2024-09-01", "Enrolled");
    enrollment_count = 3;

    assert(deleteEnrollment(2) == 1);
    assert(enrollment_count == 2);
    assert(arrEnrollments[1].enrollment_id == 3);

    assert(deleteEnrollment(999) == 0);
}
int main(void)
{
    printf("=== ENROLLMENT ASSERT TESTS ===\n");

    test_sanity();
    test_validate_date();
    test_validateEnrollment();
    test_prerequisites();
    test_capacity();
    test_deleteEnrollment();

    printf("=== ALL ENROLLMENT TESTS PASSED ===\n");
    return 0;
}
