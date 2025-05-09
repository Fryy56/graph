#include "mainwindow.hpp"


// Static structs init
decltype(MainWindow::Colors) const MainWindow::Colors = {
	{25, 25, 25}, // BG
	{45, 45, 45}, // Fields
	{175, 175, 175} // ButtonText
};
decltype(MainWindow::DefaultValues) const MainWindow::DefaultValues = {
	// win/
		{1000, 600}, // windowSize
		false, // isMax
	// fields/
		{""}, // function
		{""}, // minX
		{""}, // maxX
	{105, 20, 195} // graphLineColor
};

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
	// Settings init
	m_settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "Fryy_55", "Graphing calculator");
	readSettings();

	// Main window init
	this -> setWindowTitle("Graphing calculator");
	this -> resize(SettingsValues.windowSize);
	if (SettingsValues.isMax)
		this -> showMaximized();

	// Colors setup
	HighlightedLineEdit::setColors(Qt::darkGray, Qt::white, Colors.Fields, Qt::white, {100, 100, 100});
	auto pal = QPalette();
	pal.setColor(QPalette::Window, Colors.BG);


	// Extras setup
	QRegularExpression intRegEx("^(-??[1-9]\\d{0,9})|(0)$");
	m_intRegExVal = new QRegularExpressionValidator(intRegEx);
	auto pushButtonStyleSheet = QString(R"(
		QPushButton {
			background-color: %1;
			border: 2px solid rgb(60, 60, 60);
			border-radius: 5px;
		}
		QPushButton:hover {
			background-color: %2;
			border-color: rgb(80, 80, 80);
			color: rgb(255, 255, 255);
		}
		QPushButton:pressed {
			border-color: rgb(60, 60, 60);
		}
	)").arg(Colors.BG.name())
	.arg(Colors.Fields.name());

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
	m_graph -> setStyleSheet(QString(R"(
			border-radius: 20px;
			background-color: %1;
	)").arg(Colors.Fields.name()));
	m_graph -> setMinimumSize(150, 100);
	m_graph -> setAutoFillBackground(true);
	m_graphLayout -> addWidget(m_graph);

	// Spacing
	m_graphLayout -> addSpacing(15);

	// Function input
	m_function = new HighlightedLineEdit("<i>𝑓(x)</i>", "sin(x)");
	m_function -> setLabelStyleSheet(R"(
		QLabel {
			font: 15pt;
		}
	)");
	m_function -> setFieldSafeStyleSheet("border-radius: 5px;");
	m_function -> heightViaLabel(25);
	m_function -> setFieldMaxLength(255);
	m_function -> setFieldToolTip("Enter a function relative to <b>x</b>.");
	m_function -> setFieldText(SettingsValues.function);
	m_graphLayout -> addWidget(m_function);

	// ------------------------------ Button layout ------------------------------
	// Background
	m_buttonBG = new QWidget;
	m_buttonBG -> setStyleSheet(QString(R"(
		QWidget {
			border-radius: 10px;
			background-color: rgb(%1, %2, %3);
		}
	)").arg(Colors.BG.red() + 10).arg(Colors.BG.green() + 10).arg(Colors.BG.blue() + 10));
	m_optionsLayout = new QVBoxLayout(m_buttonBG);

	// Header
	m_optionsHeader = new QLabel("<b>Options</b>");
	QFont headerFont = QFont("Nexa");
	headerFont.setStyleStrategy(QFont::PreferAntialias);
	m_optionsHeader -> setFont(headerFont);
	pal.setColor(QPalette::WindowText, Qt::gray);
	m_optionsHeader -> setPalette(pal);
	m_optionsHeader -> setStyleSheet(R"(
		QLabel {
			font: 25pt;
		}
	)");
	m_optionsLayout -> addWidget(m_optionsHeader);

	// Separator
	m_optionsSeparator = new Separator(Separator::H);
	m_optionsSeparator -> setColor(Qt::gray);
	m_optionsLayout -> addWidget(m_optionsSeparator);

	// X limit input fields and reset button
	m_limitsLayout = new QHBoxLayout;
	m_limitMin = new HighlightedLineEdit("Min <b>x</b>", "-10");
	m_limitMin -> setFieldMaxLength(11);
	m_limitMin -> widthViaField(130);
	m_limitMin -> setFieldSafeStyleSheet("border-radius: 5px; font: 11px;");
	m_limitMin -> setFieldToolTip("Minimum <b>x</b> value to be calculated.");
	m_limitMin -> setFieldValidator(m_intRegExVal);
	m_limitMin -> setFieldText(SettingsValues.minX);
	m_limitsLayout -> addWidget(m_limitMin);
	m_resetLimitsButtonOffsetLayout = new QVBoxLayout;
	m_resetLimitsButton = new QPushButton;
	m_resetLimitsButton -> setIcon(QIcon(":/icons/reset.png"));
	m_resetLimitsButton -> setToolTip("Reset <b>x</b> limits.");
	m_resetLimitsButton -> setIconSize({16, 16});
	m_resetLimitsButton -> setFixedSize(24, 24);
	m_resetLimitsButton -> setStyleSheet(pushButtonStyleSheet);
	m_resetLimitsButtonOffsetLayout -> addSpacing(22);
	m_resetLimitsButtonOffsetLayout -> addWidget(m_resetLimitsButton);
	m_limitsLayout -> addLayout(m_resetLimitsButtonOffsetLayout);
	m_limitMax = new HighlightedLineEdit("Max <b>x</b>", "10");
	m_limitMax -> setFieldMaxLength(11);
	m_limitMax -> widthViaField(130);
	m_limitMax -> setFieldSafeStyleSheet("border-radius: 5px; font: 11px;");
	m_limitMax -> setFieldToolTip("Maximum <b>x</b> value to be calculated.");
	m_limitMax -> setFieldValidator(m_intRegExVal);
	m_limitMax -> setFieldText(SettingsValues.maxX);
	m_limitsLayout -> addWidget(m_limitMax);
	connect(
		m_resetLimitsButton,
		QPushButton::clicked,
		m_limitMin,
		HighlightedLineEdit::clearWithPulse
	);
	connect(
		m_resetLimitsButton,
		QPushButton::clicked,
		m_limitMax,
		HighlightedLineEdit::clearWithPulse
	);

	m_optionsLayout -> addLayout(m_limitsLayout);

	// Finalize the options menu
	m_buttonBG -> setMaximumWidth(m_limitMin -> maximumWidth() + m_limitMax -> maximumWidth());
	m_buttonLayout -> addWidget(m_buttonBG);

	// Stretching
	m_buttonLayout -> addStretch();

	// Reset function button
	m_resetFunctionButton = new QPushButton(QIcon(":/icons/reset.png"), "Reset the function input field");
	m_resetFunctionButton -> setIconSize({16, 16});
	m_resetFunctionButton -> setFixedSize(220, 25);
	pal.setColor(QPalette::ButtonText, Colors.ButtonText);
	m_resetFunctionButton -> setPalette(pal);
	m_resetFunctionButton -> setStyleSheet(pushButtonStyleSheet);
	connect(
		m_resetFunctionButton,
		QPushButton::clicked,
		m_function,
		HighlightedLineEdit::clearWithPulse
	);
	m_buttonLayout -> addWidget(m_resetFunctionButton);

	// Spacing
	m_buttonLayout -> addSpacing(10);
}

void MainWindow::readSettings() {
	// win/
		SettingsValues.windowSize = m_settings -> value("win/windowSize", DefaultValues.windowSize).value<QSize>();
		SettingsValues.isMax = m_settings -> value("win/isMax", DefaultValues.isMax).toBool();
	// fields/
		SettingsValues.function = m_settings -> value("fields/function", DefaultValues.function).toString();
		SettingsValues.minX = m_settings -> value("fields/minX", DefaultValues.function).toString();
		SettingsValues.maxX = m_settings -> value("fields/maxX", DefaultValues.function).toString();
	SettingsValues.graphLineColor = m_settings -> value("graphLineColor", DefaultValues.graphLineColor).value<QColor>();

	return;
}

void MainWindow::writeSettings() {
	// win/
		m_settings -> setValue("win/windowSize", this -> size());
		m_settings -> setValue("win/isMax", this -> isMaximized());
	// fields
		m_settings -> setValue("fields/function", m_function -> getInputField().text());
		m_settings -> setValue("fields/minX", m_limitMin -> getInputField().text());
		m_settings -> setValue("fields/maxX", m_limitMax -> getInputField().text());
	//m_settings -> setValue("graphLineColor", ); pls implement this

	m_settings -> sync();
	return;
}

MainWindow::~MainWindow() {
	writeSettings();
	delete m_settings;

	delete m_centralWidget;

	delete m_intRegExVal;
}