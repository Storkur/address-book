#ifndef ADRESSBOOK_H
#define ADRESSBOOK_H

#include <QWidget>
#include <QMap>
#include "finddialog.h"

class QLabel;
class QLineEdit;
class QTextEdit;
class QPushButton;

class AddressBook : public QWidget
{
    Q_OBJECT
    
public:
	AddressBook(QWidget *parent = 0);
	enum Mode {NavigationMode, AddingMode, EditingMode};

private:
	void updateInterface(Mode mode);
	QLineEdit *nameLine;
	QTextEdit *addressText;
	QPushButton *addButton;
	QPushButton *submitButton;
	QPushButton *cancelButton;
	QPushButton *nextButton;
	QPushButton *previousButton;
	QPushButton *editButton;
	QPushButton *removeButton;

	QMap<QString, QString> contacts;
	QString oldName;
	QString oldAddress;

	Mode currentMode;

public slots:
	void addContact();
	void submitContact();
	void cancel();
	void next();
	void previous();
	void editContact();
	void removeContact();

};

#endif // ADRESSBOOK_H
