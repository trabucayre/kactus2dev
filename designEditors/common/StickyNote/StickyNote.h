//-----------------------------------------------------------------------------
// File: StickyNote.h
//-----------------------------------------------------------------------------
// Project: Kactus 2
// Author: Esko Pekkarinen
// Date: 5.2.2014
//
// Description:
// Sticky note for designs.
//-----------------------------------------------------------------------------

#ifndef STICKYNOTE_H
#define STICKYNOTE_H

#include <QGraphicsItemGroup>

#include <designEditors/common/diagramgrid.h>

#include <common/graphicsItems/GraphicsItemTypes.h>
#include <designEditors/common/Associable.h>

class VendorExtension;
class Kactus2Group;
class Kactus2Position;
class Kactus2Value;
class ColorFillTextItem;

//-----------------------------------------------------------------------------
//! DesignLabel class.
//-----------------------------------------------------------------------------
class StickyNote : public QObject, public QGraphicsItemGroup, public Associable
{
    Q_OBJECT
public:

    static const int TOP_OFFSET = 20;
    static const int DEFAULT_WIDTH = GridSize * 8 * 2;

    enum { Type = GFX_TYPE_DIAGRAM_STICKY_NOTE };

    /*!
     *  The constructor.
     *
     *      @param [in] extension   The vendor extension representing the note.
     *      @param [in] parent      The parent item.
     */
    StickyNote(QSharedPointer<Kactus2Group> extension, QGraphicsItem* parent = 0);

    //! The destructor.
    virtual ~StickyNote();

    /*!
     *  Begins the editing of the note.
     */
    void beginEditing();

    /*!
     *  Handler for item state changes.
     *
     *      @param [in] change      The parameter of the item changing.
     *      @param [in] value       The new value of the parameter.
     *
     *      @return The set new value.
     */
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value);

    /*!
     *  Gets the vendor extension visualized by the note.
     *
     *      @return The vendor extension visualized by the note.
     */
    QSharedPointer<VendorExtension> getVendorExtension() const;

    //! The item type identifier.
    virtual int type() const { return Type; }

    /*!
     *  Defines the connection point for associations in scene coordinates.
     *
     *      @return The connection point of the item.
     */
    virtual QPointF connectionPoint() const;

signals:
   
    void beginAssociation(Associable* originator);

protected:

    //! Handler for mouse press events.
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);

    //! Handler for mouse release event.
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

    
    /*!
     *  Checks if the item's position has changed as a result of a mouse drag.
     *
     *      @return True, if the position has changed, otherwise false.
     */
    bool positionChanged();

private slots:

    //! Called when the notes text has changed.
    virtual void onTextEdited();

private:

    //! Disable copying.
    StickyNote(StickyNote const& rhs);

    //! Disable assignment.
    StickyNote& operator=(StickyNote const& rhs);

     //!Initializes the item flags.     
    void setItemOptions();

    //! Creates the glued edge at the top of the note.
    void createGluedEdge();

    //! Creates the writing area of the note.
    void createWritableArea();

    void createAssociationButton();

    //! Initializes the vendor extensions for the note.
    void initializeExtensions();

    //! Initialized the vendor extension for tracking the note position.
    void initializePosition();
    void initializeContent();

    /*!
     *  Checks if clicking a given position hits the association button.
     *
     *      @param [in] clickPosition   The position of the click.
     *
     *      @return True, if the click hits the association button, otherwise false.
     */
    bool hitsAssociationButton(QPointF const& clickPosition) const;

    //-----------------------------------------------------------------------------
    // Data.
    //-----------------------------------------------------------------------------

    //! The position of the item before move.
    QPointF oldPos_;

    //! The vendor extension represented by the note.
    QSharedPointer<Kactus2Group> extension_;

    //! The vendor extension containing the position of the note.
    QSharedPointer<Kactus2Position> positionExtension_;

    QSharedPointer<Kactus2Value> contentExtension_;

    //! Editor item for the notes.
    ColorFillTextItem* textArea_;

    QGraphicsPixmapItem* associationButton_;
};

#endif // STICKYNOTE_H