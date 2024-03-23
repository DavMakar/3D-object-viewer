#ifndef SHAPES_LIST_VIEW_DELEGATE_HPP
#define SHAPES_LIST_VIEW_DELEGATE_HPP

#include <QStyledItemDelegate>

class ShapesListViewDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    ShapesListViewDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

signals:
    void deleteButtonClicked(const QModelIndex &index);
private:
    int deleteButtonSize = 20;
};

#endif // SHAPES_LIST_VIEW_DELEGATE_HPP