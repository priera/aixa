#include "SyntheticStreamFactory.h"

#include "SyntheticStream.h"

std::shared_ptr<Stream> SyntheticStreamFactory::probe() {
    return std::make_shared<SyntheticStream>();
}
