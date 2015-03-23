/* 
 *  	Created on: 12.8.2011
 *      Author: Antti Kamppi
 * 		filename: configurableelementdelegate.cpp
 *		Project: Kactus 2
 */

#include "configurableelementdelegate.h"
#include "ConfigurableElementsColumns.h"

#include <IPXACTmodels/Enumeration.h>

#include <editors/ComponentEditor/common/IPXactSystemVerilogParser.h>
#include <editors/ComponentEditor/parameters/Array/ArrayView.h>
#include <editors/ComponentEditor/parameters/Array/ParameterArrayModel.h>
#include <editors/ComponentEditor/parameters/Array/ArrayDelegate.h>

#include <QScrollArea>
#include <QComboBox>

//-----------------------------------------------------------------------------
// Function: configurableelementdelegate::ConfigurableElemenetDelegate()
//-----------------------------------------------------------------------------
ConfigurableElementDelegate::ConfigurableElementDelegate(QCompleter* parameterCompleter,
    QSharedPointer<ParameterFinder> parameterFinder, QSharedPointer<ExpressionFormatter> expressionFormatter,
    QObject *parent):
ChoiceCreatorDelegate(parameterCompleter, parameterFinder, parent),
expressionFormatter_(expressionFormatter)
{

}

//-----------------------------------------------------------------------------
// Function: configurableelementdelegate::~ConfigurableElementDelegate()
//-----------------------------------------------------------------------------
ConfigurableElementDelegate::~ConfigurableElementDelegate() 
{

}

//-----------------------------------------------------------------------------
// Function: configurableelementdelegate::createEditor()
//-----------------------------------------------------------------------------
QWidget* ConfigurableElementDelegate::createEditor(QWidget* parent, QStyleOptionViewItem const& option,
    QModelIndex const& index) const
{
    if (index.column() == valueColumn() && valueIsArray(index))
    {
        ArrayView* editor = new ArrayView(parent);

        QScrollArea* scrollingWidget = new QScrollArea(parent);
        scrollingWidget->setWidgetResizable(true);
        scrollingWidget->setWidget(editor);
        scrollingWidget->parent()->installEventFilter(editor);

        return scrollingWidget;
    }

    else if (isIndexForValueUsingChoice(index))
    {
        return createEnumerationSelector(parent, index);
    }

    else
    {
        return ChoiceCreatorDelegate::createEditor(parent, option, index);
    }
}

//-----------------------------------------------------------------------------
// Function: configurableelementdelegate::setEditorData()
//-----------------------------------------------------------------------------
void ConfigurableElementDelegate::setEditorData(QWidget* editor, QModelIndex const& index) const
{
    if (index.column() == valueColumn() && valueIsArray(index))
    {
        createArrayEditor(editor, index);
    }

    else if (isIndexForValueUsingChoice(index))
    {
        QString text = index.model()->data(index, Qt::DisplayRole).toString();
        QComboBox* combo = qobject_cast<QComboBox*>(editor);

        int comboIndex = combo->findText(text, Qt::MatchEndsWith);
        if (comboIndex == -1)
        {
            comboIndex = combo->findText(text, Qt::MatchStartsWith);
        }
        combo->setCurrentIndex(comboIndex);
    }

    else
    {
        ChoiceCreatorDelegate::setEditorData(editor, index);
    }
}

//-----------------------------------------------------------------------------
// Function: configurableelementdelegate::setModelData()
//-----------------------------------------------------------------------------
void ConfigurableElementDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
    QModelIndex const& index) const
{
    if (index.column() == valueColumn() && valueIsArray(index))
    {
        QScrollArea* scrollWidget = qobject_cast<QScrollArea*>(editor);
        ArrayView* arrayTable = qobject_cast<ArrayView*>(scrollWidget->widget());
        ParameterArrayModel* arrayModel = qobject_cast<ParameterArrayModel*>(arrayTable->model());

        QString arrayValue = arrayModel->getArrayData();
        model->setData(index, arrayValue, Qt::EditRole);
    }

    else if (isIndexForValueUsingChoice(index))
    {
        QComboBox* combo = qobject_cast<QComboBox*>(editor);
        QString text = combo->currentText();
        text = text.left(text.indexOf(':'));
        model->setData(index, text, Qt::EditRole);
    }

    else
    {
        ChoiceCreatorDelegate::setModelData(editor, model, index);
    }
}

//-----------------------------------------------------------------------------
// Function: configurableelementdelegate::setChoices()
//-----------------------------------------------------------------------------
void ConfigurableElementDelegate::setChoices(QSharedPointer<QList<QSharedPointer<Choice> > > choices)
{
    ChoiceCreatorDelegate::setChoices(choices);
}

//-----------------------------------------------------------------------------
// Function: configurableelementdelegate::columnAcceptsExpression()
//-----------------------------------------------------------------------------
bool ConfigurableElementDelegate::columnAcceptsExpression(int column) const
{
    return column == ConfigurableElementsColumns::VALUE;
}

//-----------------------------------------------------------------------------
// Function: configurableelementdelegate::choiceNameOnRow()
//-----------------------------------------------------------------------------
QString ConfigurableElementDelegate::choiceNameOnRow(QModelIndex const& index) const
{
    return index.sibling(index.row(), ConfigurableElementsColumns::CHOICE).data(Qt::DisplayRole).toString();
}

//-----------------------------------------------------------------------------
// Function: configurableelementdelegate::valueColumn()
//-----------------------------------------------------------------------------
int ConfigurableElementDelegate::valueColumn() const
{
    return ConfigurableElementsColumns::VALUE;
}

//-----------------------------------------------------------------------------
// Function: configurableelementdelegate::valueIsArray()
//-----------------------------------------------------------------------------
bool ConfigurableElementDelegate::valueIsArray(QModelIndex const& index) const
{
    QModelIndex arraySizeIndex = index.sibling(index.row(), ConfigurableElementsColumns::ARRAY_SIZE);
    bool arraySizeIsOk = true;
    int arraySize = arraySizeIndex.data(Qt::DisplayRole).toInt(&arraySizeIsOk);

    return arraySizeIsOk && arraySize > 0;
}

//-----------------------------------------------------------------------------
// Function: configurableelementdelegate::updateEditorGeometry()
//-----------------------------------------------------------------------------
void ConfigurableElementDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
    const QModelIndex &index) const
{
    ExpressionDelegate::updateEditorGeometry(editor, option, index);

    if (index.column() == valueColumn() && valueIsArray(index))
    {
        repositionAndResizeEditor(editor, option, index);
    }
}

//-----------------------------------------------------------------------------
// Function: configurableelementdelegate::repositionAndResizeEditor()
//-----------------------------------------------------------------------------
void ConfigurableElementDelegate::repositionAndResizeEditor(QWidget* editor, QStyleOptionViewItem const& option,
    QModelIndex const& index) const
{
    QModelIndex arraySizeIndex = index.sibling(index.row(), ConfigurableElementsColumns::ARRAY_SIZE);
    int arraySize = arraySizeIndex.data(Qt::DisplayRole).toInt();
    int editorMinimumSize = 24 * (arraySize + 1);

    editor->setFixedWidth(200);

    const int PARENT_HEIGHT = editor->parentWidget()->height();
    const int AVAILABLE_HEIGHT_BELOW = PARENT_HEIGHT - option.rect.top();

    if (AVAILABLE_HEIGHT_BELOW > editorMinimumSize)
    {
        editor->move(option.rect.topLeft());
    }
    else
    {
        int editorNewY = PARENT_HEIGHT - editorMinimumSize;
        if (editorNewY < 0)
        {
            editorNewY = 0;
        }

        editor->move(option.rect.left(), editorNewY);
    }

    if (editorMinimumSize > PARENT_HEIGHT)
    {
        editor->setFixedHeight(PARENT_HEIGHT);
    }
    else
    {
        editor->setFixedHeight(editorMinimumSize);
    }
}

//-----------------------------------------------------------------------------
// Function: configurableelementdelegate::createArrayEditor()
//-----------------------------------------------------------------------------
void ConfigurableElementDelegate::createArrayEditor(QWidget* editor, QModelIndex const& index) const
{
    ArrayView* view = dynamic_cast<ArrayView*>(dynamic_cast<QScrollArea*>(editor)->widget());
    QModelIndex arraySizeIndex = index.sibling(index.row(), ConfigurableElementsColumns::ARRAY_SIZE);
    int arraySize = arraySizeIndex.data(Qt::DisplayRole).toInt();

    QSharedPointer<IPXactSystemVerilogParser> expressionParser(new IPXactSystemVerilogParser(getParameterFinder()));
    QSharedPointer<Choice> selectedChoice = findChoice(index);
    ParameterArrayModel* model = new ParameterArrayModel(arraySize, expressionParser, getParameterFinder(),
        expressionFormatter_, selectedChoice, view);
    
    QModelIndex valueIndex = index.sibling(index.row(), valueColumn());
    QString parameterValue = valueIndex.data(Qt::EditRole).toString();
    model->setArrayData(parameterValue);

    view->setItemDelegate(new ArrayDelegate(getNameCompleter(), getParameterFinder(), selectedChoice, this->parent()));

    QModelIndex typeIndex = index.sibling(index.row(), ConfigurableElementsColumns::TYPE);
    QString parameterType = typeIndex.data(Qt::DisplayRole).toString();
    model->setParameterType(parameterType);

    view->setItemDelegate(new ArrayDelegate(getNameCompleter(), getParameterFinder(), selectedChoice,
        this->parent()));
    view->setModel(model);
    view->setSortingEnabled(false);
    view->resizeColumnsToContents();

    connect(view->itemDelegate(), SIGNAL(increaseReferences(QString)),
        this, SIGNAL(increaseReferences(QString)), Qt::UniqueConnection);
    connect(view->itemDelegate(), SIGNAL(decreaseReferences(QString)),
        this, SIGNAL(decreaseReferences(QString)), Qt::UniqueConnection);
}