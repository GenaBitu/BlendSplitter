#include "include/implementation/ExpanderBottom.hpp"

#include "include/BlendSplitter.hpp"
#include "include/implementation/Overlay.hpp"
#include "include/implementation/WidgetDecorator.hpp"

ExpanderBottom::ExpanderBottom(WidgetDecorator* parent) : Expander(parent)
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
        WidgetDecorator* parentDecorator{qobject_cast<WidgetDecorator*>(parentWidget())};
        if(parentDecorator == 0)
        {
            std::cerr << "Bad cast caused by having an expander not properly inside a SplitWidget inside a Splitter." << std::endl;
            qApp->exit(-1);
        }
        BlendSplitter* parentSplitter{qobject_cast<BlendSplitter*>(parentDecorator->parentWidget())};
        if(parentSplitter == 0)
        {
            std::cerr << "Bad cast caused by having an expander not properly inside a SplitWidget inside a Splitter." << std::endl;
            qApp->exit(-1);
        }
        if(parentSplitter->orientation() == Qt::Horizontal and event->x() > size and event->y() < size and event->x() > (size - event->y()))
        {
            QList<int> sizes{parentSplitter->sizes()};
            int index{parentSplitter->indexOf(parentDecorator)};
            sizes.insert(index, size);
            sizes[index + 1] -= size + 1;
            parentSplitter->insertWidget(index);
            parentSplitter->setSizes(sizes);
            parentSplitter->handle(index + 1)->grabMouse();
        }
        else if(parentSplitter->orientation() == Qt::Vertical and event->x() > 0 and event->y() < 0 and event->x() < (size - event->y()))
        {
            QList<int> sizes{parentSplitter->sizes()};
            int index{parentSplitter->indexOf(parentDecorator)};
            sizes[index] -= size + 1;
            sizes.insert(index + 1, size);
            parentSplitter->insertWidget(index + 1);
            parentSplitter->setSizes(sizes);
            parentSplitter->handle(index + 1)->grabMouse();
        }
        else if(parentSplitter->orientation() == Qt::Horizontal and event->x() > 0 and event->y() < 0 and event->x() < (size - event->y()))
        {
            BlendSplitter* newSplitter{new BlendSplitter{parentSplitter->defaultWidget, Qt::Vertical}};
            QList<int> sizes{parentSplitter->sizes()};
            parentSplitter->insertSplitter(parentSplitter->indexOf(parentDecorator), newSplitter);
            newSplitter->addDecoratedWidget(parentDecorator);
            newSplitter->addWidget();
            parentSplitter->setSizes(sizes);
            newSplitter->handle(1)->grabMouse();
        }
        else if(parentSplitter->orientation() == Qt::Vertical and event->x() > size and event->y() < size and event->x() > (size - event->y()))
        {
            BlendSplitter* newSplitter{new BlendSplitter{parentSplitter->defaultWidget, Qt::Horizontal}};
            QList<int> sizes{parentSplitter->sizes()};
            parentSplitter->insertSplitter(parentSplitter->indexOf(parentDecorator), newSplitter);
            newSplitter->addWidget();
            newSplitter->addDecoratedWidget(parentDecorator);
            parentSplitter->setSizes(sizes);
            newSplitter->handle(1)->grabMouse();
        }
        if(parentSplitter->orientation() == Qt::Horizontal and event->x() < 0 and event->y() < size and (size - event->y()) < parentDecorator->height())
        {
            if(overlay == nullptr and parentSplitter->indexOf(parentDecorator) > 0)
            {
                overlay = new Overlay{parentSplitter->widget(parentSplitter->indexOf(parentDecorator) - 1)};
                overlay->show();
            }
        }
        else if(parentSplitter->orientation() == Qt::Vertical and event->x() > 0 and event->y() > size and event->x() < parentDecorator->width())
        {
            if(overlay == nullptr and parentSplitter->indexOf(parentDecorator) + 1 < parentSplitter->count())
            {
                overlay = new Overlay{parentSplitter->widget(parentSplitter->indexOf(parentDecorator) + 1)};
                overlay->show();
            }
        }
        else if(overlay != nullptr)
        {
            delete overlay;
            overlay = nullptr;
        }
    }
}