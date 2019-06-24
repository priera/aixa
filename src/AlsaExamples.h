//
// Created by pedro on 24/06/19.
//

#ifndef ALSAPLAYGROUND_ALSAEXAMPLES_H
#define ALSAPLAYGROUND_ALSAEXAMPLES_H

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


#endif //ALSAPLAYGROUND_ALSAEXAMPLES_H
