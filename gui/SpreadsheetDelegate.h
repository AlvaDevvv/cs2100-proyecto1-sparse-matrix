#pragma once
#include <QStyledItemDelegate>
#include <QLineEdit>

class SpreadsheetDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    explicit SpreadsheetDelegate(QObject* parent = nullptr)
        : QStyledItemDelegate(parent) {}

    QWidget* createEditor(QWidget* parent,
                          const QStyleOptionViewItem&,
                          const QModelIndex&) const override {
        QLineEdit* ed = new QLineEdit(parent);
        ed->setFrame(false);
        ed->setStyleSheet(
            "QLineEdit { border: 2px solid #4a90d9; background: #ffffff; color: #1a1a1a; padding: 0 3px; }");
        return ed;
    }

    void setEditorData(QWidget* editor, const QModelIndex& index) const override {
        static_cast<QLineEdit*>(editor)->setText(
            index.model()->data(index, Qt::EditRole).toString());
    }

    void setModelData(QWidget* editor, QAbstractItemModel* model,
                      const QModelIndex& index) const override {
        model->setData(index, static_cast<QLineEdit*>(editor)->text(), Qt::EditRole);
    }

    void updateEditorGeometry(QWidget* editor,
                              const QStyleOptionViewItem& option,
                              const QModelIndex&) const override {
        editor->setGeometry(option.rect);
    }
};
