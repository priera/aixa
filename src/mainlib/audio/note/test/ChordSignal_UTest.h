#ifndef AIXA_SRC_MAINLIB_AUDIO_NOTE_TEST_CHORDSIGNAL_UTEST_H
#define AIXA_SRC_MAINLIB_AUDIO_NOTE_TEST_CHORDSIGNAL_UTEST_H

#include <QtCore/QObject>

#include <filesystem>

class ChordSignal_UTest : public QObject {
    Q_OBJECT
private slots:
    void initTestCase();

    void test_chordSignal_data();
    void test_chordSignal();

private:
    std::filesystem::path storeAt;
};


#endif //AIXA_SRC_MAINLIB_AUDIO_NOTE_TEST_CHORDSIGNAL_UTEST_H
