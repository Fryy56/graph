#include "mainwindow.hpp"


// Static colors init
decltype(MainWindow::Colors) const MainWindow::Colors = {
	{25, 25, 25}, // BG
	{45, 45, 45}, // Fields
	{175, 175, 175}, // ButtonText
	{105, 20, 195} // GraphLine
};

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
	// Main window init
	this -> setWindowTitle("Graphing calculator");
	this -> setGeometry(0, 0, 1000, 600);

	// Colors setup
	HighlightedLineEdit::setColors(Qt::darkGray, Qt::white, Colors.Fields, Qt::white);
	auto pal = QPalette();
	pal.setColor(QPalette::Window, Colors.BG);

	// Extra setup
	QRegularExpression intRegEx("^(-??[1-9]\\d{0,9})|(0)$");
	m_intRegExVal = new QRegularExpressionValidator(intRegEx);
	auto pushButtonStyleSheet = QString("QPushButton {"
			"background-color: rgb(%1, %2, %3);"
			"border: 1px solid rgb(60, 60, 60);"
			"border-radius: 5px"
		"}"
		"QPushButton:hover {"
			"background-color: rgb(%4, %5, %6);"
			"border-color: rgb(80, 80, 80);"
			"color: rgb(255, 255, 255)"
		"}"
		"QPushButton:pressed {"
			"border-color: rgb(60, 60, 60)"
		"}"
	).arg(Colors.BG.red()).arg(Colors.BG.green()).arg(Colors.BG.blue())
	.arg(Colors.Fields.red()).arg(Colors.Fields.green()).arg(Colors.Fields.blue());

	// Central widget
	m_centralWidget = new QWidget(this);
	m_centralWidget -> setAutoFillBackground(true);
	m_centralWidget -> setPalette(pal);
	this -> setCentralWidget(m_centralWidget);

	// Layout inits
	m_mainLayout = new QHBoxLayout(m_centralWidget);
	m_graphLayout = new QVBoxLayout;
	m_buttonLayout = new QVBoxLayout;
	m_mainLayout -> addLayout(m_graphLayout);
	m_mainLayout -> addSpacing(15);
	m_mainLayout -> addLayout(m_buttonLayout);

	// ------------------------------ Graph layout ------------------------------
	// Graph
	m_graph = new QWidget;
	m_graph -> setStyleSheet(QString(
			"border-radius: 20px;"
			"background-color: rgb(%1, %2, %3)"
	).arg(Colors.Fields.red()).arg(Colors.Fields.green()).arg(Colors.Fields.blue()));
	m_graph -> setMinimumSize(150, 100);
	m_graph -> setAutoFillBackground(true);
	m_graphLayout -> addWidget(m_graph);

	// Spacing
	m_graphLayout -> addSpacing(15);

	// Function input
	m_function = new HighlightedLineEdit("<i>𝑓(x)</i>", "sin(x)");
	m_function -> setLabelStyleSheet("font: 11pt");
	m_function -> heightViaLabel(25);
	m_function -> setFieldMaxLength(255);
	m_function -> setFieldToolTip("Enter a function relative to x.");
	m_graphLayout -> addWidget(m_function);

	// ------------------------------ Button layout ------------------------------
	// Background
	m_buttonBG = new QWidget;
	m_buttonBG -> setStyleSheet(QString(
		"border-radius: 10px;"
		"background-color: rgb(%1, %2, %3)"
	).arg(Colors.BG.red() + 10).arg(Colors.BG.green() + 10).arg(Colors.BG.blue() + 10));
	m_optionsLayout = new QVBoxLayout(m_buttonBG);

	// Header
	m_buttonHeader = new QLabel("<b>Options</b>");
	m_buttonHeader -> setFont(QFont("Nexa"));
	pal.setColor(QPalette::WindowText, Qt::gray);
	m_buttonHeader -> setPalette(pal);
	m_buttonHeader -> setStyleSheet("font: 19pt");
	m_optionsLayout -> addWidget(m_buttonHeader);

	// X limit input fields
	m_limitsLayout = new QHBoxLayout;
	m_limitMin = new HighlightedLineEdit("Min <i>x</i>", "-10");
	m_limitMin -> setFieldMaxLength(11);
	m_limitMin -> widthViaField(100);
	m_limitMin -> setFieldToolTip("Minimum <b>x</b> value to be calculated.");
	m_limitMin -> setFieldValidator(m_intRegExVal);
	m_limitsLayout -> addWidget(m_limitMin);
	m_limitMax = new HighlightedLineEdit("Max <i>x</i>", "10");
	m_limitMax -> setFieldMaxLength(11);
	m_limitMax -> widthViaField(100);
	m_limitMax -> setFieldToolTip("Maximum <b>x</b> value to be calculated.");
	m_limitMax -> setFieldValidator(m_intRegExVal);
	m_limitsLayout -> addWidget(m_limitMax);
	m_optionsLayout -> addLayout(m_limitsLayout);

	// Finalize the options menu
	m_buttonBG -> setMaximumWidth(m_limitMin -> maximumWidth() + m_limitMax -> maximumWidth());
	m_buttonLayout -> addWidget(m_buttonBG);

	// Stretching
	m_buttonLayout -> addStretch();

	// Reset function button
	m_resetFunctionButton = new QPushButton("Reset the function input field");
	m_resetFunctionButton -> setFixedHeight(25);
	pal.setColor(QPalette::ButtonText, Colors.ButtonText);
	m_resetFunctionButton -> setPalette(pal);
	m_resetFunctionButton -> setStyleSheet(pushButtonStyleSheet);
	connect(
		m_resetFunctionButton,
		QPushButton::clicked,
		&(m_function -> getInputField()),
		QLineEdit::clear
	);
	m_buttonLayout -> addWidget(m_resetFunctionButton);

	// Spacing
	m_buttonLayout -> addSpacing(10);
}

MainWindow::~MainWindow() {
	delete m_centralWidget;

	delete m_mainLayout;
		delete m_graphLayout;
			delete m_graph;
			delete m_function;
		delete m_buttonLayout;
			delete m_buttonBG;
				delete m_optionsLayout;
					delete m_buttonHeader;
					delete m_limitsLayout;
						delete m_limitMin;
						delete m_limitMax;
			delete m_resetFunctionButton;

	delete m_intRegExVal;
}