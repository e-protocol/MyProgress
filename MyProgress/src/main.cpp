#include "src/ui/mainwindow.h"
#include <QApplication>
#include <QDebug>

/*
 * This block is used to detect memory leaks.
 * Comment it, if build final version.
 * NOTE! In Qt: delete calls and new calls are not equal!
 */
//------------------------------------------------------------------------------
struct MemoryUsage
{
    uint32_t allocated = 0;
    uint32_t diallocated = 0;

    uint32_t getMemoryUsage() { return allocated - diallocated; }
};

static MemoryUsage _memoryUsage;

static void printMemoryUsage()
{
    qDebug() << "Memory used:" << _memoryUsage.getMemoryUsage()
             << "bytes" << "allocated:" << _memoryUsage.allocated
             << "diallocated:" << _memoryUsage.diallocated;
}

void *operator new(size_t size)
{
    _memoryUsage.allocated += size;
    return malloc(size);
}

void operator delete(void *memory, size_t size)
{
    _memoryUsage.diallocated += size;
    free(memory);
}

void operator delete[](void *memory, size_t size)
{
    ::operator delete(memory);
}
//------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    QApplication *app = new QApplication(argc, argv);
    MainWindow *w = new MainWindow;
    w->showMaximized();
    printMemoryUsage();
    int error = app->exec();
    delete w;
    delete app;
    printMemoryUsage();
    return error;
}
