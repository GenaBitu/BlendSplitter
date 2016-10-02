#pragma once

#include "Global.hpp"

#include "Expander.hpp"
class WidgetDecorator;

class ExpanderTop : public Expander
{
    Q_OBJECT
public:
    ExpanderTop() = delete;
    explicit ExpanderTop(WidgetDecorator* parent);
    virtual void reposition();
protected slots:
    virtual void mouseMoveEvent(QMouseEvent* event);
};
