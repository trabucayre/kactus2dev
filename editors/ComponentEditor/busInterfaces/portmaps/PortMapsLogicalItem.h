//-----------------------------------------------------------------------------
// File: PortMapsLogicalItem.h
//-----------------------------------------------------------------------------
// Project: Kactus 2
// Author: Esko Pekkarinen
// Date: 7.10.2013
//
// Description:
// Port maps tree item representing a logical port.
//-----------------------------------------------------------------------------

#ifndef PORTMAPSLOGICALITEM_H
#define PORTMAPSLOGICALITEM_H

#include "PortMapsTreeItem.h"

#include <IPXACTmodels/common/DirectionTypes.h>
#include <IPXACTmodels/generaldeclarations.h>

#include <QObject>
#include <QString>
#include <QList>
#include <QSharedPointer>

class AbstractionDefinition;
class BusInterface;
class Component;
class PortMap;
class ExpressionParser;

//-----------------------------------------------------------------------------
//! Port maps tree item representing a logical port.
//-----------------------------------------------------------------------------
class PortMapsLogicalItem : public PortMapsTreeItem
{
    Q_OBJECT

public:

    /*!
     *  The constructor.
     *
     *      @param [in] parent              Parent item.
     *      @param [in] logicalName         The logical name.
     *      @param [in] component           Containing component.
     *      @param [in] busif               Used bus interface.
     *      @param [in] absDef              Used abstraction definition.
     *      @param [in] expressionParser    The used expression parser.
     */
    PortMapsLogicalItem(PortMapsTreeItem* parent,
        QString const& logicalName,
        QSharedPointer<Component> component,
        QSharedPointer<BusInterface> busif,
        QSharedPointer<AbstractionDefinition> absDef,
        QSharedPointer<ExpressionParser> expressionParser
        );

    /*!
     *  Destructor.
     */
    virtual ~PortMapsLogicalItem();

    /*!
     *  Updates the item.
     *
     */
    virtual void refresh();

    /*!
     *  Gets the item data for a given section.
     *
     *      @param [in] section   The section whose data to get.
     *
     *      @return The data in section.
     */
    virtual QVariant data(int section) const;

    /*!
     *  Checks the validity of the item.
     *
     *      @return True, if item is valid, otherwise false.
     */
    virtual bool isValid() const;

    /*!
     *  Clears the mappings for all children.
     */
    void clearMappings();

    /*!
     *  Gets the width of the logical signal.
     *
     *      @return width of the logical signal.
     */
    int getWidth() const;

    /*!
     *  Gets the direction of the logical signal.
     *
     *      @return The logical direction.
     */
    DirectionTypes::Direction getDirection() const;

private:
    // Disable copying.
    PortMapsLogicalItem(PortMapsLogicalItem const& rhs);
    PortMapsLogicalItem& operator=(PortMapsLogicalItem const& rhs);
    
    /*!
     *  Gets a string of the connected ports and their bits.
     *     
     *      @return Names and bits of connected physical ports.
     */
    QString getPhysPorts() const;

    /*!
     *  Gets the width of the logical signal.
     *
     *
     *      @return width of the logical signal.
     */
    int getConnectionCount() const;

    /*!
     *  Updates the port width to given value by adding/removing children.
     *
     *      @param [in] width   The new width.
     */
    void updateWidthTo(int width);    

    /*!
     *  Get the size of the port.
     *
     *      @param [in] portName        The name of the port whose size is being searched for.
     *      @param [in] interfaceMode   The used interface mode.
     */
    int getPortSize(QString const& portName, General::InterfaceMode interfaceMode) const;

    //-----------------------------------------------------------------------------
    // Data.
    //-----------------------------------------------------------------------------  

    //! The logical port bus interface.
    QSharedPointer<BusInterface> busIf_;

    //! The abstraction definition that is used.
    QSharedPointer<AbstractionDefinition> absDef_;

    //! The port maps of the logical port item.
	QSharedPointer<QList<QSharedPointer<PortMap> > > portMaps_;

    //! The left bound of the logical port.
    int right_;

    //! The right bound of the logical port.
    int left_;
    
    //! Flag for indicating beginning of mapping when bounds are not yet set.
    bool beginMapping_;
    
    //! The used expression parser.
    QSharedPointer<ExpressionParser> expressionParser_;
};

#endif // PORTMAPSLOGICALITEM_H
