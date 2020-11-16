#ifndef AIXA_SRC_MAINLIB_GUI_GRAPHICSENVIRONMENTFACTORY_H
#define AIXA_SRC_MAINLIB_GUI_GRAPHICSENVIRONMENTFACTORY_H

#include <QtCore/QSize>

#include "GraphicsEnvironment.h"

class GraphicsEnvironmentFactory {
public:
    GraphicsEnvironmentFactory() = delete;

    static std::unique_ptr<GraphicsEnvironment> build(const QSize &appInitialSize);
};


#endif //AIXA_SRC_MAINLIB_GUI_GRAPHICSENVIRONMENTFACTORY_H
