/* 
 *
 *  Created on: 7.2.2011
 *      Author: Antti Kamppi
 * 		filename: fileextratab.h
 */

#ifndef FILEEXTRATAB_H
#define FILEEXTRATAB_H

#include "filedefinemodel.h"
#include "filedefineview.h"

#include <common/widgets/listManager/dirlistmanager.h>
#include <common/widgets/listManager/listmanager.h>
#include <models/component.h>
#include <models/file.h>

#include <QWidget>
#include <QSharedPointer>

class LibraryInterface;

/*! \brief FileExtraTab is a widget to edit File's settings.
 * 
 */
class FileExtraTab : public QWidget {
	Q_OBJECT

public:

	/*! \brief The constructor
	 *
	 * \param handler Pointer to the instance that manages the library.
	 * \param component Pointer to the component being edited.
	 * \param file Pointer to the file being edited.
	 * \param parent Pointer to the owner of this widget.
	 *
	*/
	FileExtraTab(LibraryInterface* handler,
		QSharedPointer<Component> component,
		QSharedPointer<File> file,
		QWidget *parent);

	//! \brief The destructor
	virtual ~FileExtraTab();

	/*! \brief Apply the changes from the editor to the model.
	 *
	*/
	void apply();

	/*! \brief Restore the changes from the model to the editor.
	 *
	*/
	void restore();

	/*! \brief Check that the editor is in a valid state.
	 *
	 *
	 * \return True if everything is ok. False if changes can not be applied.
	*/
	bool isValid() const;

signals:

	//! \brief Emitted when contents of one widget change.
	void contentChanged();

private:

	//! \brief No copying
	FileExtraTab(const FileExtraTab& other);

	//! No assignment
	FileExtraTab& operator=(const FileExtraTab& other);
	
	//! \brief Editor to set the dependencies of the file.
	DirListManager dependencies_;

	//! \brief Editor to set the exported names of the file.
	ListManager exportedNames_;

	//! \brief editor to set the image types of the file.
	ListManager imageTypes_;

	//! \brief The view to display the file defines
	FileDefineView defineView_;

	//! \brief The model that contains the file defines
	FileDefineModel defineModel_;

	//! \brief Pointer to the file model that is being edited.
	QSharedPointer<File> file_;

};

#endif // FILEEXTRATAB_H