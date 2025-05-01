#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
	this -> setWindowTitle("Graph");
	this -> setGeometry(0, 0, 800, 600);

	auto paletteBG = QPalette();
	paletteBG.setColor(QPalette::Window, QColor(25, 25, 25)); // Dark gray or smth

	m_centralWidget = new QWidget(this);
	m_centralWidget -> setAutoFillBackground(true);
	m_centralWidget -> setPalette(paletteBG);
	this -> setCentralWidget(m_centralWidget);

	m_mainLayout = new QHBoxLayout(m_centralWidget);
	m_graphLayout = new QVBoxLayout();
	m_buttonLayout = new QVBoxLayout();
	m_mainLayout -> addLayout(m_graphLayout);
	m_mainLayout -> addLayout(m_buttonLayout);

	m_graph = new QWidget;
	m_graph -> setAutoFillBackground(true);
	paletteBG.setColor(QPalette::Window, QColor(45, 45, 45)); // A not so dark gray or smth
	m_graph -> setPalette(paletteBG);
	m_graphLayout -> addWidget(m_graph);
	m_graphLayout -> addSpacing(15);
	m_function = new QLineEdit;
	m_function -> setMaxLength(128);
	m_function -> setPlaceholderText("sin(x)");
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