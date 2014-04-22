#include "addressbook.h"
#include <QtGui>

AddressBook::AddressBook(QWidget *parent)
	: QWidget(parent)
{
	QLabel *nameLabel = new QLabel(tr("Name:"));
	nameLine = new QLineEdit;
	nameLine->setReadOnly(true);

	QLabel *addressLabel = new QLabel(tr("Address:"));
	addressText = new QTextEdit;
	addressText->setReadOnly(true);

	addButton = new QPushButton(tr("&Add Mode"));
	addButton->show();
	submitButton = new QPushButton(tr("&Submit"));
	submitButton->hide();
	cancelButton = new QPushButton(tr("&Cancel"));
	cancelButton->hide();
	nextButton = new QPushButton(tr("&Next"));
	nextButton->setEnabled(false);
	previousButton = new QPushButton(tr("&Previous"));
	previousButton->setEnabled(false);
	editButton = new QPushButton(tr("&Edit"));
	editButton->setEnabled(false);
	removeButton = new QPushButton(tr("&Remove"));
	removeButton->setEnabled(false);
	findButton = new QPushButton(tr("&Find"));
	findButton->setEnabled(false);
	loadButton = new QPushButton(tr("Load"));
	loadButton->setEnabled(true);
	loadButton->setToolTip(tr("Load contacts from a file"));
	saveButton = new QPushButton(tr("Save"));
	saveButton->setEnabled(false);
	saveButton->setToolTip(tr("Save contacts to a file"));
	exportButton  = new QPushButton(tr("Export"));
	exportButton->setEnabled(false);

	dialog = new FindDialog;

	connect(addButton, SIGNAL(clicked()), this, SLOT(addContact()));
	connect(submitButton, SIGNAL(clicked()), this, SLOT(submitContact()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));
	connect(nextButton, SIGNAL(clicked()), this, SLOT(next()));
	connect(previousButton, SIGNAL(clicked()), this, SLOT(previous()));
	connect(editButton, SIGNAL(clicked()), this, SLOT(editContact()));
	connect(removeButton, SIGNAL(clicked()), this, SLOT(removeContact()));
	connect(findButton, SIGNAL(clicked()),this, SLOT(findContact()));
	connect(loadButton, SIGNAL(clicked()), this, SLOT(loadFromFile()));
	connect(saveButton, SIGNAL(clicked()), this, SLOT(saveToFile()));
	connect(exportButton, SIGNAL(clicked()), this, SLOT(exportAsVCard()));

	QVBoxLayout *buttonLayout1 = new QVBoxLayout;
	buttonLayout1->addWidget(addButton, Qt::AlignTop);
	buttonLayout1->addWidget(submitButton);
	buttonLayout1->addWidget(cancelButton);
	buttonLayout1->addWidget(editButton);
	buttonLayout1->addWidget(removeButton);
	buttonLayout1->addWidget(findButton);
	buttonLayout1->addWidget(loadButton);
	buttonLayout1->addWidget(saveButton);
	buttonLayout1->addWidget(exportButton);
	buttonLayout1->addStretch();

	QHBoxLayout *buttonLayout2 = new QHBoxLayout;
	buttonLayout2->addWidget(previousButton);
	buttonLayout2->addWidget(nextButton);

	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->addWidget(nameLabel, 0, 0);
	mainLayout->addWidget(nameLine, 0 , 1);
	mainLayout->addWidget(addressLabel, 1, 0, Qt::AlignTop);
	mainLayout->addWidget(addressText, 1, 1);
	mainLayout->addLayout(buttonLayout1, 1, 2);
	mainLayout->addLayout(buttonLayout2,2,1);

	this->setLayout(mainLayout);
	this->setWindowTitle(tr("Simple Address Book"));
}


//Start Add mode
void AddressBook::addContact()
{
	oldName = nameLine->text();
	oldAddress = addressText->toPlainText();

	nameLine->clear();
	addressText->clear();

	nameLine->setReadOnly(false);
	nameLine->setFocus(Qt::OtherFocusReason);
	addressText->setReadOnly(false);

	addButton->setEnabled(false);
	submitButton->show();
	cancelButton->show();
	nextButton->setEnabled(false);
	previousButton->setEnabled(false);
	updateInterface(AddingMode);
}

void AddressBook::submitContact()
{
	QString name = nameLine->text();
	QString address = addressText->toPlainText();

	if(name.isEmpty() || address.isEmpty())
	{
		QMessageBox::information(this, tr("Empty filed"), tr("Please enter some data"));

	}

	if(currentMode == AddingMode)
	{
		if(!contacts.contains(name))
		{
			contacts.insert(name, address);
			QMessageBox::information(this, tr("Add successful"), tr("\"%1\" has been added to your address book.").arg(name));
		}
		else
		{
			QMessageBox::information(this, tr("Add unsuccessful"), tr("\"%1\" is already in your address book.").arg(name));
			return;
		}

		if (contacts.isEmpty())
		{
			nameLine->clear();
			addressText->clear();
		}
	}
	else if (currentMode == EditingMode)
	{

		if (oldName != name)
		{
			if (!contacts.contains(name))
			{
				QMessageBox::information(this, tr("Edit Successful"), tr("\"%1\" has been edited in your address book.").arg(oldName));
				contacts.remove(oldName);
				contacts.insert(name, address);
			}
			else
			{
				QMessageBox::information(this, tr("Edit Unsuccessful"), tr("Sorry, \"%1\" is already in your address book.").arg(name));
			}
		}
		else if (oldAddress != address)
		{
			QMessageBox::information(this, tr("Edit Successful"), tr("\"%1\" has been edited in your address book.").arg(name));
			contacts[name] = address;
		}
	}

	updateInterface(NavigationMode);

	nameLine->setReadOnly(true);
	addressText->setReadOnly(true);
	addButton->setEnabled(true);
	submitButton->hide();
	cancelButton->hide();

	int numberOfContacts = contacts.size();
	nextButton->setEnabled(numberOfContacts >1);
	previousButton->setEnabled(numberOfContacts >1);
}

void AddressBook::cancel()
{
	nameLine->setText(oldName);
	nameLine->setReadOnly(true);

	addressText->setText(oldAddress);
	addressText->setReadOnly(true);

	addButton->setEnabled(true);
	submitButton->hide();
	cancelButton->hide();

	int numberOfContacts = contacts.size();
	nextButton->setEnabled(numberOfContacts >1);
	previousButton->setEnabled(numberOfContacts >1);
}

void AddressBook::next()
{
	QString name = nameLine->text();
	QMap<QString, QString>::iterator i = contacts.find(name);

	if (i != contacts.end())
		i++;
	if(i == contacts.end())
		i = contacts.begin();

	nameLine->setText(i.key());
	addressText->setText(i.value());
}

void AddressBook::previous()
{
	QString name = nameLine->text();
	QMap<QString, QString>::iterator i = contacts.find(name);

	if(i == contacts.end())
	{
		nameLine->clear();
		addressText->clear();
		return;
	}

	if (i == contacts.begin())
		i = contacts.end();

	i--;
	nameLine->setText(i.key());
	addressText->setText(i.value());
}


void AddressBook::editContact()
{
	oldName = nameLine->text();
	oldAddress = addressText->toPlainText();

	updateInterface(EditingMode);
}


void AddressBook::removeContact()
{
	QString name = nameLine->text();
	QString address = addressText->toPlainText();

	if(contacts.contains(name))
	{
		int button = QMessageBox::question(this, tr("Confirm Remove"),
										   tr("Are you sure you want to remove \"%1\"?").arg(name),
										   QMessageBox::Yes | QMessageBox::No);

		if (button == QMessageBox::Yes)
		{
			previous();
			contacts.remove(name);

			QMessageBox::information(this, tr("Remove Successful"),
									 tr("\"%1\" has been removed from your address book.").arg(name));
		}
	}
	updateInterface(NavigationMode);
}

void AddressBook::findContact()
{
	dialog->show();
	if(dialog->exec() == QDialog::Accepted)
	{
		QString contactName = dialog->getFindText();
		if(contacts.contains(contactName))
		{
			nameLine->setText(contactName);
			addressText->setText(contacts.value(contactName));
		}
		else
		{
			QMessageBox::information(this, tr("Contact Not Found"),
									 tr("Sorry, \"%1\" is not in your address book.").arg(contactName));
			return;
		}
	}
	updateInterface(NavigationMode);
}

void AddressBook::saveToFile()
{
	QString filename = QFileDialog::getSaveFileName(this, tr("Save Address Book"), "",
													tr("Address Book (*.abk);;All Files (*)"));
	if(filename.isEmpty())
	{
		//Empty File name
		return;
	}
	else
	{
		QFile file(filename);
		if(!file.open(QIODevice::WriteOnly))
		{
			QMessageBox::information(this, tr("Unable to open file"),
									 file.errorString());
			return;
		}
			QDataStream out(&file);
			out.setVersion(QDataStream::Qt_4_5);
			out << contacts;
	}
}

void AddressBook::loadFromFile()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Open Address Book"), "",
													tr("Address Book (*.abk);;All Files (*)"));
	if(filename.isEmpty())
	{
		//Empty file name
		return;
	}
	else
	{
		QFile file(filename);
		if(!file.open(QIODevice::ReadOnly))
		{
			QMessageBox::information(this, tr("Unable to open file"),
							 file.errorString());
						 return;
		}
		QDataStream in (&file);
		in.setVersion(QDataStream::Qt_4_5);
		contacts.empty();
		in >> contacts;

		if (contacts.isEmpty())
		{
			QMessageBox::information(this, tr("No contacts in file"),
									 tr("The file you are attempting to open contains no contacts."));
		}
		else
		{
			QMap<QString, QString>::iterator i = contacts.begin();
			nameLine->setText(i.key());
			addressText->setText(i.value());
		}
	}

	updateInterface(NavigationMode);
}

void AddressBook::exportAsVCard()
{
	QString name = nameLine->text();
	QString address = addressText->toPlainText();
	QString firstName;
	QString lastName;
	QStringList nameList;

	int index = name.indexOf(" ");

	if(index != -1)
	{

	}
}

void AddressBook::updateInterface(Mode mode)
{
	currentMode = mode;

	switch (currentMode) {

	case AddingMode:
	case EditingMode:

		nameLine->setReadOnly(false);
		nameLine->setFocus(Qt::OtherFocusReason);
		addressText->setReadOnly(false);

		addButton->setEnabled(false);
		editButton->setEnabled(false);
		removeButton->setEnabled(false);

		nextButton->setEnabled(false);
		previousButton->setEnabled(false);

		submitButton->show();
		cancelButton->show();

		loadButton->setEnabled(false);
		saveButton->setEnabled(false);
		exportButton->setEnabled(false);

		break;
	case NavigationMode:

		if (contacts.isEmpty()) {
			nameLine->clear();
			addressText->clear();
		}

		nameLine->setReadOnly(true);
		addressText->setReadOnly(true);
		addButton->setEnabled(true);

		int number = contacts.size();
		editButton->setEnabled(number >= 1);
		removeButton->setEnabled(number >= 1);
		nextButton->setEnabled(number > 1);
		previousButton->setEnabled(number >1 );
		findButton->setEnabled(number > 2);

		submitButton->hide();
		cancelButton->hide();

		loadButton->setEnabled(true);
		saveButton->setEnabled(number >= 1);

		exportButton->setEnabled(number >= 1);

		break;
	}
}
QPushButton *AddressBook::getExportButton() const
{
    return exportButton;
}

void AddressBook::setExportButton(QPushButton *value)
{
    exportButton = value;
}


