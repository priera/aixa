#ifndef ALSAPLAYGROUND_NOTESETTER_H
#define ALSAPLAYGROUND_NOTESETTER_H

class AudioWorker;

class NoteSetter {
public:
    NoteSetter(AudioWorker & audioWorker) : worker(&audioWorker) {}

    void setNote(float steps);

private:
    AudioWorker *worker;
};

#endif //ALSAPLAYGROUND_NOTESETTER_H
