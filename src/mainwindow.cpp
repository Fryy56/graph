#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
	auto ColorBG = QColor(25, 25, 25);
	auto ColorFields = QColor(45, 45, 45);

	this -> setWindowTitle("Graph");
	this -> setGeometry(0, 0, 800, 600);

	auto pal = QPalette();
	pal.setColor(QPalette::Window, ColorBG);

	m_centralWidget = new QWidget(this);
	m_centralWidget -> setAutoFillBackground(true);
	m_centralWidget -> setPalette(pal);
	this -> setCentralWidget(m_centralWidget);

	m_mainLayout = new QHBoxLayout(m_centralWidget);
	m_graphLayout = new QVBoxLayout();
	m_buttonLayout = new QVBoxLayout();
	m_mainLayout -> addLayout(m_graphLayout);
	m_mainLayout -> addSpacing(15);
	m_mainLayout -> addLayout(m_buttonLayout);

	m_graph = new QWidget;
	m_graph -> setMinimumSize(150, 100);
	m_graph -> setAutoFillBackground(true);
	pal.setColor(QPalette::Window, ColorFields);
	m_graph -> setPalette(pal);
	m_graphLayout -> addWidget(m_graph);
	m_graphLayout -> addSpacing(15);
	m_function = new QLineEdit;
	m_function -> setMaxLength(128);
	m_function -> setPlaceholderText("sin(x)");
	pal.setColor(QPalette::Base, ColorFields);
	pal.setColor(QPalette::Text, Qt::white);
	m_function -> setPalette(pal);
	m_graphLayout -> addWidget(m_function);
}

MainWindow::~MainWindow() {
	delete m_graph;
	delete m_function;

	delete m_graphLayout;
	delete m_buttonLayout;
	delete m_mainLayout;

	delete m_centralWidget;
}