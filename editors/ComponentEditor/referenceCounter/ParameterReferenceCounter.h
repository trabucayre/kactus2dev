//-----------------------------------------------------------------------------
// File: ParameterReferenceCounter.h
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Mikko Teuho
// Date: 14.01.2015
//
// Description:
// Calculates the amount of references made to parameters and gives this data to the parameters.
//-----------------------------------------------------------------------------

#ifndef PARAMETERREFERENCECOUNTER_H
#define PARAMETERREFERENCECOUNTER_H

#include "ReferenceCounter.h"

#include <editors/ComponentEditor/common/ComponentParameterFinder.h>
#include <editors/ComponentEditor/common/ParameterFinder.h>

class ConfigurableElementValue;

//-----------------------------------------------------------------------------
//! ParameterReferenceCounter class. 
//-----------------------------------------------------------------------------
class ParameterReferenceCounter : public ReferenceCounter
{
    Q_OBJECT

public:

    /*!
     *  The constructor.
     *
     *      @param [in] parameterFinder     The parameter finder.
     */
    ParameterReferenceCounter(QSharedPointer<ParameterFinder> parameterFinder);

    /*!
     *  The destructor.
     */
    ~ParameterReferenceCounter();

    /*!
     *  Count the references made to the selected parameter in the selected parameters.
     *
     *      @param [in] parameterID     ID of the selected parameter.
     *      @param [in] parameters      The selected parameters.
     *
     *      @return The amount of references made to the selected parameter in the selected parameters.
     */
    virtual int countReferencesInParameters(QString const& parameterID,
        QSharedPointer<QList<QSharedPointer<Parameter> > > parameters) const;

    /*!
     *  Count the references made to the selected parameter in the selected parameter.
     *
     *      @param [in] parameterID     ID of the selected parameter.
     *      @param [in] parameter       The selected parameter.
     *
     *      @return The amount of references made to the selected parameter in the selected parameter.
     */
    int countReferencesInSingleParameter(QString const& parameterID, QSharedPointer<Parameter> parameter) const;

    /*!
     *  Count the references made to the selected parameter in the selected configurable element values.
     *
     *      @param [in] parameterID             ID of the selected parameter.
     *      @param [in] configurableElements    The selected configurable element values.
     *
     *      @return The amount of references made to the selected parameter in the selected configurable element
     *              values.
     */
    int countReferencesInConfigurableElementValues(QString const& parameterId,
        QSharedPointer<QList<QSharedPointer<ConfigurableElementValue> > > configurableElements) const;

    /*!
     *  Count the references made to the selected parameter in the selected configurable element value.
     *
     *      @param [in] parameterID     ID of the selected parameter.
     *      @param [in] element         The selected configurable element value.
     *
     *      @return The amount of references made to the selected parameter in the selected configurable element
     *              value.
     */
    int countReferencesInSingleConfigurableElementValue(QString const& parameterID,
        QSharedPointer<ConfigurableElementValue> element) const;
    
    /*!
     *  Count the references made to the selected parameter in the selected expression.
     *
     *      @param [in] parameterID     ID of the selected parameter.
     *      @param [in] expression      The selected expression.
     *
     *      @return The amount of references made to the selected parameter in the selected expression.
     */
    int countReferencesInExpression(QString const& parameterID, QString const& expression) const;

public slots:

    /*!
     *  Increase the reference count of the parameter matching the id.
     *
     *      @param [in] id      The id of the parameter which is being referenced.
     */
    virtual void increaseReferenceCount(QString id);
    
    /*!
     *  Decrease the reference count of the parameter matching the id.
     *
     *      @param [in] id      The id of the parameter which was referenced.
     */
    virtual void decreaseReferenceCount(QString id);

    /*!
     *  Recalculate references made to the selected parameters.
     *
     *      @param [in] parameterList   The selected parameters.
     */
    virtual void recalculateReferencesToParameters(QVector<QSharedPointer<Parameter> > parameterList);

private:

	//! No copying. No assignment.
    ParameterReferenceCounter(const ParameterReferenceCounter& other);
    ParameterReferenceCounter& operator=(const ParameterReferenceCounter& other);

    //-----------------------------------------------------------------------------
    // Data.
    //-----------------------------------------------------------------------------

    //! Object for finding parameters with the correct id.
    QSharedPointer<ParameterFinder> parameterFinder_;
};

#endif // PARAMETERREFERENCECOUNTER_H
