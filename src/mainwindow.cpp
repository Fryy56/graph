#include "mainwindow.h"
#include <QGraphicsColorizeEffect>
#include <QPropertyAnimation>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
	// Main window init
	this -> setWindowTitle("Graph");
	this -> setGeometry(0, 0, 800, 600);

	// Palette setup
	auto pal = QPalette();
	pal.setColor(QPalette::Window, Colors.BG);

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
	m_graph -> setMinimumSize(150, 100);
	m_graph -> setAutoFillBackground(true);
	pal.setColor(QPalette::Window, Colors.Fields);
	m_graph -> setPalette(pal);
	m_graphLayout -> addWidget(m_graph);

	// Spacing
	m_graphLayout -> addSpacing(15);

	// Function input
	m_functionLayout = new QVBoxLayout;
	// Label
	m_functionLabelLayout = new QHBoxLayout;
	m_functionLabel = new QLabel("<i>𝑓(x)");
	m_functionLabel -> setStyleSheet("font: 11pt");
	pal.setColor(QPalette::WindowText, Qt::gray);
	m_functionLabel -> setPalette(pal);
	m_functionLabel -> setMaximumHeight(20);
	m_functionLabelLayout -> addSpacing(10);
	m_functionLabelLayout -> addWidget(m_functionLabel);
	m_functionLayout -> addLayout(m_functionLabelLayout);
	// Input field
	m_functionField = new QLineEdit;
	m_functionField -> setMaxLength(255);
	m_functionField -> setPlaceholderText("sin(x)");
	m_functionField -> setToolTip("Enter a function relative to x.");
	pal.setColor(QPalette::Base, Colors.Fields);
	pal.setColor(QPalette::Text, Qt::white);
	m_functionField -> setPalette(pal);
	m_functionLayout -> addWidget(m_functionField);

	m_graphLayout -> addLayout(m_functionLayout);


}

MainWindow::~MainWindow() {
	delete m_graph;
	delete m_functionLabel;
	delete m_functionField;

	delete m_functionLabelLayout;
	delete m_functionLayout;
	delete m_graphLayout;
	delete m_buttonLayout;
	delete m_mainLayout;

	delete m_centralWidget;
}

void MainWindow::onFuncFieldSelect() {
	QGraphicsColorizeEffect tint;
	m_functionLabel -> setGraphicsEffect(&tint);
	QPropertyAnimation anim(m_functionLabel, );
	anim.setDuration(400);
	anim.setStartValue()

	return;
}