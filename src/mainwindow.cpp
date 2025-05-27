#include "mainwindow.hpp"
#include <QColorDialog>


// Static structs init
decltype(MainWindow::Colors) const MainWindow::Colors = {
	{25, 25, 25}, // BG
	{45, 45, 45}, // Fields
	{175, 175, 175}, // ButtonText
	{60, 60, 60}, // ButtonBorder
	{150, 150, 150} // LabelText
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
	HighlightedLineEdit::setPresetColors(Qt::darkGray, Qt::white, Colors.Fields, Qt::white, {100, 100, 100});
	auto pal = QPalette();
	pal.setColor(QPalette::Window, Colors.BG);


	// Extras setup
	QRegularExpression intRegEx("^(-??[1-9]\\d{0,5})|(0)$");
	QRegularExpression functionRegEx("^[^ ]*$");
	m_intRegExVal = new QRegularExpressionValidator(intRegEx);
	m_functionRegExVal = new QRegularExpressionValidator(functionRegEx);
	auto pushButtonStyleSheet = QString(R"(
		QPushButton {
			background-color: %1;
			border: 2px solid %2;
			border-radius: 5px;
		}
		QPushButton:hover {
			background-color: %3;
			border-color: rgb(80, 80, 80);
			color: rgb(255, 255, 255);
		}
		QPushButton:pressed {
			border-color: %2;
		}
	)")
	.arg(Colors.BG.name())
	.arg(Colors.ButtonBorder.name())
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
	m_graph = new Graph(this);
	m_graph -> setAxesColor({87, 87, 87});
	m_graph -> setStyleSheet(QString(R"(
		QWidget {
			background-color: %1;
		}
	)").arg(Colors.Fields.name()));
	m_graph -> setFixedHeight(2'000);
	m_graph -> setAutoFillBackground(true);
	m_graphScene = new QGraphicsScene;
	m_graphProxy = m_graphScene -> addWidget(m_graph);
	m_graphProxy -> setPos(0.f, 0.f);
	m_graphView = new GraphView(m_graphScene);
	m_graphView -> setMinimumSize(700, 500);
	m_graphView -> setStyleSheet(R"(
		border-radius: 20px;
		background-color: white;
	)");
	connect(
		m_graphView,
		GraphView::widthUpdated,
		m_graph,
		Graph::updateWidth
	);
	m_graphLayout -> addWidget(m_graphView);
	m_graphView -> show();

	// Spacing
	m_graphLayout -> addSpacing(15);

	// Function input
	m_function = new HighlightedLineEdit("<i>𝑓(x)</i>", "sin(x)");
	m_function -> setLabelStyleSheet(R"(
		QLabel {
			font: 15pt;
		}
	)");
	m_function -> setFieldStyleSheetSafe(QString(R"(
		border-radius: 5px;
	)"));
	m_function -> setHeightViaLabel(25);
	m_function -> setFieldMaxLength(255);
	m_function -> setFieldToolTip("Enter a function relative to <b>x</b>.");
	m_function -> setFieldText(SettingsValues.function);
	m_function -> setFieldValidator(m_functionRegExVal);
	m_graphLayout -> addWidget(m_function);

	// ------------------------------ Button layout ------------------------------
	// Background
	m_buttonBG = new QWidget;
	auto buttonBGColor = QColor(Colors.BG.red() + 10, Colors.BG.green() + 10, Colors.BG.blue() + 10);
	m_buttonBG -> setStyleSheet(QString(R"(
		QWidget {
			border-radius: 10px;
			background-color: %1;
		}
	)").arg(buttonBGColor.name()));
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
	m_optionsSeparator -> setColor(Qt::darkGray);
	m_optionsLayout -> addWidget(m_optionsSeparator);

	HighlightedLineEdit::setPresetColors(buttonBGColor, HighlightedLineEdit::ColorRoles::FieldsColor);
	// X min limit input field
	m_limitsLayout = new QHBoxLayout;
	m_limitMin = new HighlightedLineEdit("Min <b>x</b>", "-10");
	m_limitMin -> setFieldMaxLength(11);
	m_limitMin -> setWidthViaField(130);
	m_limitMin -> setFieldStyleSheetSafe("border-radius: 5px; font: 11px;");
	m_limitMin -> setFieldToolTip("Minimum <b>x</b> value to be calculated.");
	m_limitMin -> setFieldValidator(m_intRegExVal);
	m_limitMin -> setFieldText(SettingsValues.minX);
	m_limitsLayout -> addWidget(m_limitMin);
	// Limits reset button
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
	// X max limit input field
	m_limitMax = new HighlightedLineEdit("Max <b>x</b>", "10");
	m_limitMax -> setFieldMaxLength(11);
	m_limitMax -> setWidthViaField(130);
	m_limitMax -> setFieldStyleSheetSafe("border-radius: 5px; font: 11px;");
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

	// Graph line color selector
	// Label
	m_graphColorLabel = new QLabel("Graph line color");
	pal.setColor(QPalette::WindowText, Colors.LabelText);
	m_graphColorLabel -> setPalette(pal);
	m_optionsLayout -> addWidget(m_graphColorLabel);
	// Color picker
	m_graphColorPicker = new ColorPicker(
		SettingsValues.graphLineColor,
		HighlightedLineEdit::getPresetColors().borderColor
	);
	m_graphColorPicker -> setFixedHeight(30);
	m_optionsLayout -> addWidget(m_graphColorPicker);

	// Finalize the options menu
	m_buttonBG -> setMaximumWidth(m_limitMin -> maximumWidth() + m_limitMax -> maximumWidth());
	m_buttonLayout -> addWidget(m_buttonBG);

	// Stretching
	m_buttonLayout -> addStretch();

	// Plot graph button
	m_plotGraphButton = new QPushButton(QIcon(":/icons/plot.png"), "Plot");
	QFont plotButtonFont;
	plotButtonFont.setPointSize(25);
	//plotButtonFont.setBold(true);
	m_plotGraphButton -> setFont(plotButtonFont);
	m_plotGraphButton -> setIconSize({32, 32});
	m_plotGraphButton -> setFixedSize(220, 50);
	pal.setColor(QPalette::ButtonText, Colors.ButtonText);
	m_plotGraphButton -> setPalette(pal);
	m_plotGraphButton -> setStyleSheet(pushButtonStyleSheet);
	connect(
		m_plotGraphButton,
		QPushButton::clicked,
		m_graph,
		Graph::build
	);
	connect(
		m_plotGraphButton,
		QPushButton::clicked,
		this,
		MainWindow::onPlot
	);
	connect(
		m_function -> getInputField(),
		QLineEdit::returnPressed,
		m_graph,
		Graph::build
	);
	connect(
		m_function -> getInputField(),
		QLineEdit::returnPressed,
		this,
		MainWindow::onPlot
	);
	m_buttonLayout -> addWidget(m_plotGraphButton);

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
		m_settings -> setValue("fields/function", m_function -> getInputField() -> text());
		m_settings -> setValue("fields/minX", m_limitMin -> getInputField() -> text());
		m_settings -> setValue("fields/maxX", m_limitMax -> getInputField() -> text());
	m_settings -> setValue("graphLineColor", SettingsValues.graphLineColor);

	m_settings -> sync();
	return;
}

void MainWindow::onPlot() {
	m_function -> pulse(Qt::darkGreen);

	return;
}

MainWindow::~MainWindow() {
	writeSettings();
	delete m_settings;

	delete m_centralWidget;

	delete m_intRegExVal;
	delete m_functionRegExVal;
}