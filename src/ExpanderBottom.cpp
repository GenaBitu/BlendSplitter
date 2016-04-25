#include "include/QtSplitter/ExpanderBottom.hpp"

#include "include/Splitter.hpp"
#include "include/QtSplitter/Overlay.hpp"
#include "include/QtSplitter/SplitterWidgetDecorator.hpp"

ExpanderBottom::ExpanderBottom(SplitterWidgetDecorator* parent) : Expander(parent)
{
    QTransform rot;
    *pixmap = pixmap->transformed(rot.rotate(180), Qt::FastTransformation);
    setPixmap(*pixmap);
    QPolygon mask;
    mask << QPoint{0, 0} << QPoint{size/10, 0} << QPoint{size, size * 9 / 10} << QPoint{size, size} << QPoint{0, size};
    setMask(QRegion{mask});
}

void ExpanderBottom::reposition()
{
    move(0, parentWidget()->height() - height());
    Expander::reposition();
}

void ExpanderBottom::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        SplitterWidgetDecorator* parentSplitWidget{qobject_cast<SplitterWidgetDecorator*>(parentWidget())};
        if(parentSplitWidget == 0)
        {
            std::cerr << "Bad cast caused by having an expander not properly inside a SplitWidget inside a Splitter." << std::endl;
            qApp->exit(-1);
        }
        Splitter* parentSplitter{qobject_cast<Splitter*>(parentSplitWidget->parentWidget())};
        if(parentSplitter == 0)
        {
            std::cerr << "Bad cast caused by having an expander not properly inside a SplitWidget inside a Splitter." << std::endl;
            qApp->exit(-1);
        }
        if(parentSplitter->orientation() == Qt::Horizontal and event->x() > size and event->y() < size and (event->x()) > size - event->y())
        {
            QList<int> sizes{parentSplitter->sizes()};
            int index{parentSplitter->indexOf(parentSplitWidget)};
            sizes.insert(index, size);
            sizes[index + 1] -= size + 1;
            parentSplitter->insertWidget(index);
            parentSplitter->setSizes(sizes);
            parentSplitter->handle(index + 1)->grabMouse();
        }
        else if(parentSplitter->orientation() == Qt::Vertical and event->x() > 0 and event->y() < 0 and (event->x()) < size - event->y())
        {
            QList<int> sizes{parentSplitter->sizes()};
            int index{parentSplitter->indexOf(parentSplitWidget)};
            sizes[index] -= size + 1;
            sizes.insert(index + 1, size);
            parentSplitter->insertWidget(index + 1);
            parentSplitter->setSizes(sizes);
            parentSplitter->handle(index + 1)->grabMouse();
        }
        else if(parentSplitter->orientation() == Qt::Horizontal and event->x() < size and event->y() > size and (size - event->x()) < event->y())
        {
            /*Splitter* newSplitter{new Splitter{parentSplitter->defaultWidget, Qt::Vertical}};
            QList<int> sizes{parentSplitter->sizes()};
            parentSplitter->insertSplitter(parentSplitter->indexOf(parentSplitWidget), newSplitter);
            newSplitter->addWidget();
            newSplitter->addDecoratedWidget(parentSplitWidget);
            parentSplitter->setSizes(sizes);
            newSplitter->handle(1)->grabMouse();*/
        }
        else if(parentSplitter->orientation() == Qt::Vertical and event->x() < 0 and event->y() > 0 and (size - event->x()) > event->y())
        {
            /*Splitter* newSplitter{new Splitter{parentSplitter->defaultWidget, Qt::Horizontal}};
            QList<int> sizes{parentSplitter->sizes()};
            parentSplitter->insertSplitter(parentSplitter->indexOf(parentSplitWidget), newSplitter);
            newSplitter->addDecoratedWidget(parentSplitWidget);
            newSplitter->addWidget();
            parentSplitter->setSizes(sizes);
            newSplitter->handle(1)->grabMouse();*/
        }
        /*if(parentSplitter->orientation() == Qt::Horizontal and event->x() > size and event->y() > 0 and event->y() < parentSplitWidget->height())
        {
            if(overlay == nullptr)
            {
                overlay = new Overlay{parentSplitter->widget(parentSplitter->indexOf(parentSplitWidget) + 1)};
                overlay->show();
            }
        }
        else if(parentSplitter->orientation() == Qt::Vertical and event->y() < 0 and event->x() < size and -(event->x()) < parentSplitWidget->width() - size)
        {
            if(overlay == nullptr)
            {
                overlay = new Overlay{parentSplitter->widget(parentSplitter->indexOf(parentSplitWidget) - 1)};
                overlay->show();
            }
        }
        else if(overlay != nullptr)
        {
            delete overlay;
            overlay = nullptr;
        }*/
    }
}

void ExpanderBottom::mouseReleaseEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        SplitterWidgetDecorator* parentSplitWidget{qobject_cast<SplitterWidgetDecorator*>(parentWidget())};
        if(parentSplitWidget == 0)
        {
            std::cerr << "Bad cast caused by having an expander not properly inside a SplitWidget inside a Splitter." << std::endl;
            qApp->exit(-1);
        }
        Splitter* parentSplitter{qobject_cast<Splitter*>(parentSplitWidget->parentWidget())};
        if(parentSplitter == 0)
        {
            std::cerr << "Bad cast caused by having an expander not properly inside a SplitWidget inside a Splitter." << std::endl;
            qApp->exit(-1);
        }
        if(parentSplitter->orientation() == Qt::Horizontal and event->x() > size and event->y() > 0 and event->y() < parentSplitWidget->height())
        {
            QList<int> sizes{parentSplitter->sizes()};
            int index{parentSplitter->indexOf(parentSplitWidget)};
            sizes[index] += sizes[index + 1] + 1;
            sizes.removeAt(index + 1);
            delete parentSplitter->widget(index + 1);
            if(parentSplitter->count() == 1 and parentSplitter->parentWidget()->inherits("SplitterDecorator"))
            {
                Splitter* newParent{qobject_cast<Splitter*>(parentSplitter->parentWidget()->parentWidget())};
                if(newParent == 0)
                {
                    std::cerr << "Bad cast caused by having an expander not properly inside a SplitWidget inside a Splitter." << std::endl;
                    qApp->exit(-1);
                }
                QList<int> sizes2{newParent->sizes()};
                newParent->insertDecoratedWidget(newParent->indexOf(parentSplitter->parentWidget()), parentSplitWidget);
                delete parentSplitter->parentWidget();
                newParent->setSizes(sizes2);
            }
            else
            {
                parentSplitter->setSizes(sizes);
            }
            overlay = nullptr;
        }
        else if(parentSplitter->orientation() == Qt::Vertical and event->y() < 0 and event->x() < size and -(event->x()) < parentSplitWidget->width() - size)
        {
            QList<int> sizes{parentSplitter->sizes()};
            int index{parentSplitter->indexOf(parentSplitWidget)};
            sizes[index] += sizes[index - 1] + 1;
            sizes.removeAt(index - 1);
            delete parentSplitter->widget(index - 1);
            if(parentSplitter->count() == 1 and parentSplitter->parentWidget()->inherits("SplitterDecorator"))
            {
                Splitter* newParent{qobject_cast<Splitter*>(parentSplitter->parentWidget()->parentWidget())};
                if(newParent == 0)
                {
                    std::cerr << "Bad cast caused by having an expander not properly inside a SplitWidget inside a Splitter." << std::endl;
                    qApp->exit(-1);
                }
                QList<int> sizes2{newParent->sizes()};
                newParent->insertDecoratedWidget(newParent->indexOf(parentSplitter->parentWidget()), parentSplitWidget);
                delete parentSplitter->parentWidget();
                newParent->setSizes(sizes2);
            }
            else
            {
                parentSplitter->setSizes(sizes);
             }
            overlay = nullptr;
        }
        releaseMouse();
    }
}
