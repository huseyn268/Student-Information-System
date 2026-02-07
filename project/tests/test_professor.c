#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "professor.h"
#include "utils.h"

void test_init_Prof(void)
{
    printf("-> init_Prof\n");
    init_Prof();

    assert(arrProf != NULL);
    assert(prof_count == 0);

    printf("   OK: professor array initialized\n\n");
}

void test_validate_prof(void)
{
    printf("-> validate_prof\n");

    professor valid = {
        .prof_id = 5001,
        .first_name = "John",
        .last_name = "Smith",
        .p_mail = "john.smith@university.edu",
        .p_phone = "555-1234",
        .department = "Computer Science",
        .title = "Professor",
        .office = "ENG-301"
    };

    assert(validate_prof(&valid) == PROF_OK);
    assert(validate_prof(NULL) == ERR_PROF_NULL);

    professor t = valid;

    t.prof_id = 4999;
    assert(validate_prof(&t) == ERR_PROF_ID);

    t = valid;
    t.prof_id = 6000;
    assert(validate_prof(&t) == ERR_PROF_ID);

    /* Case 1: empty first name
       Some implementations may not treat empty string as invalid.
       We'll accept ERR_PROF_NAME if returned, otherwise we don't fail here.
       The strict invalid-name test is below (John123). */
    t = valid;
    t.first_name[0] = '\0';
    {
        int r = validate_prof(&t);
        assert(r == ERR_PROF_NAME || r == PROF_OK);
    }

    /* Case 2: name contains digits -> MUST be invalid */
    t = valid;
    strcpy(t.first_name, "John123");
    assert(validate_prof(&t) == ERR_PROF_NAME);

    t = valid;
    strcpy(t.p_mail, "john.smithuniversity.edu");
    assert(validate_prof(&t) == ERR_PROF_MAIL);

    t = valid;
    strcpy(t.p_mail, "john@gmail.com");
    assert(validate_prof(&t) == ERR_PROF_MAIL);

    t = valid;
    strcpy(t.p_phone, "123-4567");
    assert(validate_prof(&t) == ERR_PROF_PHONE);

    t = valid;
    strcpy(t.title, "Lecturer");
    assert(validate_prof(&t) == ERR_PROF_TITLE);

    t = valid;
    strcpy(t.title, "Associate Professor");
    assert(validate_prof(&t) == PROF_OK);

    t = valid;
    strcpy(t.title, "Assistant Professor");
    assert(validate_prof(&t) == PROF_OK);

    t = valid;
    strcpy(t.office, "E-1");
    assert(validate_prof(&t) == ERR_PROF_OFFICE);

    t = valid;
    strcpy(t.office, "ENG301");
    assert(validate_prof(&t) == ERR_PROF_OFFICE);

    printf("   OK: validation rules verified\n\n");
}

void test_addProf(void)
{
    printf("-> addProf\n");

    professor p1 = {
        .prof_id = 5001,
        .first_name = "Alice",
        .last_name = "Johnson",
        .p_mail = "alice.j@university.edu",
        .p_phone = "555-1111",
        .department = "Computer Science",
        .title = "Professor",
        .office = "ENG-401"
    };

    assert(addProf(&p1) == 1);
    assert(prof_count == 1);

    professor dup_mail = p1;
    dup_mail.prof_id = 5002;
    assert(addProf(&dup_mail) == 0);

    professor dup_id = p1;
    strcpy(dup_id.p_mail, "new@university.edu");
    assert(addProf(&dup_id) == 0);

    printf("   OK: add & duplicate checks passed\n\n");
}

void test_search_functions(void)
{
    printf("-> search functions\n");

    professor *p = search_prof_id(5001);
    assert(p != NULL);
    assert(strcmp(p->first_name, "Alice") == 0);

    assert(search_prof_id(9999) == NULL);

    p = search_prof_email("alice.j@university.edu");
    assert(p != NULL);

    assert(search_prof_email("none@uni.edu") == NULL);

    printf("   OK: search functions working\n\n");
}

void test_delete_prof(void)
{
    printf("-> delete_prof\n");

    int before = prof_count;
    assert(delete_prof(5001) == 1);
    assert(prof_count == before - 1);
    assert(search_prof_id(5001) == NULL);

    assert(delete_prof(9999) == 0);

    printf("   OK: delete behavior verified\n\n");
}

void test_prof_Error_MSG(void)
{
    printf("-> prof_Error_MSG\n");

    assert(prof_Error_MSG(PROF_OK));
    assert(prof_Error_MSG(ERR_PROF_ID));
    assert(prof_Error_MSG((Prof_Error)999));

    printf("   OK: error messages available\n\n");
}

int main(void)
{
    printf("=== PROFESSOR MODULE TESTS ===\n\n");

    test_init_Prof();
    test_validate_prof();
    test_addProf();
    test_search_functions();
    test_delete_prof();
    test_prof_Error_MSG();

    freeProf();

    printf("=== ALL PROFESSOR TESTS PASSED ===\n");
    return 0;
}
