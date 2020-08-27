#ifndef AIXA_SRC_MAINLIB_AUDIO_NOTE_TEST_CHORDBUILDER_UTEST_H
#define AIXA_SRC_MAINLIB_AUDIO_NOTE_TEST_CHORDBUILDER_UTEST_H


#include <QtCore/QObject>

class ChordBuilder_UTest  : public QObject {
    Q_OBJECT

private slots:
    void test_triad_data();
    void test_triad();

    void test_seventh_data();
    void test_seventh();
};


#endif //AIXA_SRC_MAINLIB_AUDIO_NOTE_TEST_CHORDBUILDER_UTEST_H
