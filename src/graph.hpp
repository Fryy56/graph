#pragma once

#include "mainwindow.hpp"
#include <QPainterPath>


class MainWindow;

class Graph : public QWidget {
public:
	explicit Graph(MainWindow* win, QWidget* parent = nullptr);
	~Graph() override;

public slots:
	void build();

protected:
	void paintEvent(QPaintEvent*) override;

	// Fields
	MainWindow* m_win;
	QPainterPath* m_graphPath;
	QRect m_ogPathRect;
	QPainterPath* m_axesPath;
	bool m_init;
};