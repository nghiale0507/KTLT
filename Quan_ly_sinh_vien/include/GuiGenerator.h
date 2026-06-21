#ifndef GUI_GENERATOR_H
#define GUI_GENERATOR_H

#include "LinkedList.h"
#include "Models.h"
#include <string>
using std::string;

bool exportGuiDashboard(LinkedList<Student>& students,
                        LinkedList<Course>& courses,
                        LinkedList<ClassSection>& sections,
                        LinkedList<Grade>& grades,
                        const string& outputPath);

#endif
