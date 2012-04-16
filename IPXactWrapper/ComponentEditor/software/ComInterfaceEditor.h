//-----------------------------------------------------------------------------
// File: ComInterfaceEditor.h
//-----------------------------------------------------------------------------
// Project: Kactus 2
// Author: Joni-Matti M��tt�
// Date: 16.4.2012
//
// Description:
// COM interface editor for the component editor.
//-----------------------------------------------------------------------------

#ifndef COMINTERFACEEDITOR_H
#define COMINTERFACEEDITOR_H

#include "PropertyValueEditor.h"
#include "../itemeditor.h"

#include <common/widgets/vlnvEditor/vlnveditor.h>
#include <common/widgets/nameGroupBox/namegroupbox.h>

#include <QComboBox>
#include <QGroupBox>

class Component;
class ComInterface;
class LibraryInterface;

//-----------------------------------------------------------------------------
//! COM interface editor.
//-----------------------------------------------------------------------------
class ComInterfaceEditor : public ItemEditor
{
    Q_OBJECT

public:
    /*!
     *  Constructor.
     *
     *      @param [in] libHandler   The library handler.
     *      @param [in] component    The component being edited.
     *      @param [in] dataPointer  The COM interface being edited.
     *      @param [in] parent       The parent widget.
     */
    ComInterfaceEditor(LibraryInterface* libHandler, QSharedPointer<Component> component, 
                       void* dataPointer, QWidget *parent);

    /*!
     *  Destructor.
     */
    virtual ~ComInterfaceEditor();

    /*!
     *  Checks for the validity of the edited COM interface.
     *
	 *      @return True if the COM interface is in valid state.
	 */
	bool isValid() const;

    /*!
     *  Removes the edited COM interface from the model.
	 *
	 *      @remarks This function should be used when user wants to remove an element from the component.
	 */
	virtual void removeModel();

	/*!
     *  Applies the changes from the widgets editors to the IP-Xact model.
     */
	void makeChanges();

private:
    // Disable copying.
    ComInterfaceEditor(ComInterfaceEditor const& rhs);
    ComInterfaceEditor& operator=(ComInterfaceEditor const& rhs);

    /*!
     *  Restores changes made to the contents.
     */
    void restoreChanges();

    //-----------------------------------------------------------------------------
    // Data.
    //-----------------------------------------------------------------------------

    //! The library handler.
    LibraryInterface* libInterface_;

    //! Pointer to the bus interface being edited.
    ComInterface* comIf_;

    //! Contains the name, display name and description of the COM interface.
    NameGroupBox nameGroup_;

    //! Editor for setting the COM definition.
    VLNVEditor comType_;

    //! Details group box.
    QGroupBox detailsGroup_;

    //! Data type combo box.
    QComboBox dataTypeCombo_;

    //! Direction combo box.
    QComboBox directionCombo_;

    //! The property value editor.
    PropertyValueEditor propertyValueEditor_;
};

//-----------------------------------------------------------------------------

#endif // COMINTERFACEEDITOR_H