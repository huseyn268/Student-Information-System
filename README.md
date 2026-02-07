# Student Information System (C)

This repository contains a console-based **Student Information System**
implemented in the C programming language as a term project for the
CSE102 – Computer Programming course.

The system provides basic functionality to manage academic entities such as
students, courses, professors, enrollments, and grades through a
menu-driven terminal interface.

---

## General Information

The project is designed using a modular programming approach.
Each major component of the system is implemented in separate source and
header files to improve code organization and maintainability.

All data is stored in text-based CSV files, allowing information to persist
between program executions and making the data easy to inspect and manage.

---

## Directory Structure

The project follows the directory structure specified in the assignment:

project/

├── src/

│ ├── main.c


│ ├── student.c

│ ├── student.h

│ ├── course.c

│ ├── course.h

│ ├── professor.c

│ ├── professor.h

│ ├── enrollment.c

│ ├── enrollment.h

│ ├── grade.c

│ ├── grade.h

│ ├── menu.c

│ ├── menu.h


│ ├── utils.c

│ └── utils.h
│

├── tests/

│ ├── test_student.c

│ ├── test_course.c


│ ├── test_enrollment.c

│

├── data/

│ ├── students.csv

│ ├── courses.csv

│ ├── professors.csv

│ ├── enrollments.csv

│ └── grades.csv

│

├── docs/

│ ├── project_report.pdf

│ ├── user_manual.pdf

│ └── quick_start_guide.pdf

│

├── build.sh

└── README.md

## Functionality Overview

The system supports the following operations:

- Managing student records
- Managing course and professor records
- Enrolling and removing students from courses
- Entering and updating grades
- Generating basic academic reports
- Saving and loading system data from files

---

## Build and Run

To compile the project, use the provided build script:

bash
chmod +x build.sh


./build.sh

To run the program:

bash

./sis

To clean generated files:

bash

./build.sh clean

Notes

The project uses standard C and does not rely on external libraries.

Input validation and error handling are implemented to ensure correct usage.

The focus of the project is on structure, correctness, and maintainability.
