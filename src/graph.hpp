#pragma once

#include "mainwindow.hpp"
#include <QPainterPath>


class MainWindow;

class Graph : public QWidget {
	Q_OBJECT

public:
	explicit Graph(MainWindow* win, QWidget* parent = nullptr);
	~Graph() override;

public slots:
	void updateWidth(int newWidth);
	void build();

protected:
	void paintEvent(QPaintEvent*) override;

	// Fields
	MainWindow* m_win;
	QPainterPath* m_graphPath;
	QPainterPath* m_axesPath;
	bool m_init;
};

// ---------------------------------------------------------------------------

class GraphView : public QGraphicsView {
	Q_OBJECT

public:
	explicit GraphView(QGraphicsScene* scene, QWidget* parent = nullptr);
	~GraphView() override {}

protected:
	void resizeEvent(QResizeEvent*) override;

signals:
	void widthUpdated(int width);
};