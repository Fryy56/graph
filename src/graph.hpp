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
	QPainterPath* m_path;
	bool m_init;
};