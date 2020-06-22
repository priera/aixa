//
// Created by pedro on 24/06/19.
//

#ifndef AIXA_ALSAEXAMPLES_H
#define AIXA_ALSAEXAMPLES_H

#include <vector>
#include <string>


class AlsaExamples {
public:
    AlsaExamples(int argc, char ** argv);
    void run();

private:

    int completeExample();
    void help();

    int argc;
    char ** argv;

};


#endif //AIXA_ALSAEXAMPLES_H
