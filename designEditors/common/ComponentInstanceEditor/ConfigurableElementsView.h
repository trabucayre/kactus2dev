//-----------------------------------------------------------------------------
// File: ConfigurableElementsView.h
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Mikko Teuho
// Date: 30.03.2015
//
// Description:
// A view for configurable element tree model.
//-----------------------------------------------------------------------------

#ifndef CONFIGURABLEELEMENTSVIEW_H
#define CONFIGURABLEELEMENTSVIEW_H

#include <QTreeView>

#include <QContextMenuEvent>
#include <QModelIndex>
#include <QAction>

//-----------------------------------------------------------------------------
//! A tree view for the configurable element tree model.
//-----------------------------------------------------------------------------
class ConfigurableElementsView : public QTreeView
{
    Q_OBJECT

public:
   
    /*!
     *  The constructor.
     *
     *      @param [in] parent  The owner of this view.
     */
    ConfigurableElementsView(QWidget* parent);

    /*!
     *  Destructor.
     */
    ~ConfigurableElementsView();

signals:

    /*!
     *  Emitted when the user attempts to remove an item.
     *
     *      @param [in] index   The index of the item to be removed.
     */
    void removeItem(QModelIndex const& index);

    /*!
     *  Emitted when the user attempts to remove all sub items.
     *
     *      @param [in] index   The index of the item whose sub items are removed.
     */
    void removeAllSubItems(QModelIndex const& index);

    /*!
     *  Emitted when a configurable element change command should be created.
     *
     *      @param [in] oldValue    Old value of the indexed element.
     *      @param [in] newValue    New value of the indexed element.
     *      @param [in] index       The selected index.
     *      @param [in] cevModel    Model containing the configurable elements.
     */
    void createElementChangeCommand(QString const& oldValue, QString const& newValue, QModelIndex const& index,
        QAbstractItemModel* cevModel) const;

protected:
    
    /*!
     *  Draws a row and a separator line under it.
     *
     *      @param [in] painter   The painter used for the drawing.
     *      @param [in] option    Options for the drawing.
     *      @param [in] index     Index of the item to draw.
     */
    virtual void drawRow(QPainter* painter, QStyleOptionViewItem const& option, QModelIndex const& index) const;

    /*!
     *  Handler for context menu events.
     *
     *      @param [in] event   The context menu event.
     */
    virtual void contextMenuEvent(QContextMenuEvent* event);

    /*!
     *  Gets the action for the remove command.
     *
     *      @return The action for the remove command.
     */
    QAction* getRemoveAction() const;

private slots:
   
    /*!
     *  Called when removeAction is clicked from the context menu.
     */
    virtual void onRemoveItem();

    /*!
     *  Handles the removal of all sub items of the selected index.
     */
    virtual void onRemoveAllSubItems();

    /*!
     *  Handles the deleting of the selected cell.
     */
    void onClearAction();

private:
   
    // Disable copying.
    ConfigurableElementsView(ConfigurableElementsView const& rhs);
    ConfigurableElementsView& operator=(ConfigurableElementsView const& rhs);

    //-----------------------------------------------------------------------------
    // Data.
    //-----------------------------------------------------------------------------

    //! The file dependency item context menu was opened on
    QModelIndex contextMenuIndex_;

    //! Action for removing mappings.
    QAction* removeAction_;

    //! Action for removing all sub items.
    QAction* removeAllAction_;

    //! Action for clearing a cell.
    QAction* clearAction_;
};

//-----------------------------------------------------------------------------

#endif // CONFIGURABLEELEMENTSVIEW_H
