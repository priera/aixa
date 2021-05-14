#ifndef AIXA_AIXA_SRC_MAINLIB_STREAM_IN_BITREADERSOURCE_H
#define AIXA_AIXA_SRC_MAINLIB_STREAM_IN_BITREADERSOURCE_H

template<typename ActualType>
concept BitReaderSource  = requires(ActualType t, char* c, std::size_t s) {
    { t.readNBytes(c, s) } -> std::convertible_to<std::streamsize>;
    { t.skipNBytes(s) } -> std::same_as<void>;
    { t.ended() } -> std::same_as<bool>;
    { t.seekToBeginning() } -> std::same_as<void>;
};

#endif //AIXA_AIXA_SRC_MAINLIB_STREAM_IN_BITREADERSOURCE_H
