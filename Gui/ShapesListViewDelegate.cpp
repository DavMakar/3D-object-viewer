#include "ShapesListViewDelegate.hpp"
#include <QPainter>
#include <QMouseEvent>

ShapesListViewDelegate::ShapesListViewDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

void ShapesListViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter, option, index);

    if (option.state & QStyle::State_Selected)
    {
        QRect rect = option.rect;
        QRect buttonRect(rect.right() - deleteButtonSize, rect.top(), deleteButtonSize, rect.height());
        painter->fillRect(buttonRect, Qt::red);
        painter->drawText(buttonRect, Qt::AlignCenter, "X");
    }
}

bool ShapesListViewDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        QRect itemRect = option.rect;
        
        QRect buttonRect(itemRect.right() - deleteButtonSize, itemRect.top(), deleteButtonSize, itemRect.height());
        if (buttonRect.contains(mouseEvent->pos()))
        {
            emit deleteButtonClicked(index);
            return true;
        }
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}
