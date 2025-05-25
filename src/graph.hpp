#pragma once

#include "mainwindow.hpp"
#include <QPainterPath>


class MainWindow;

class Graph : public QWidget {
	Q_OBJECT

public:
	explicit Graph(MainWindow* win, QWidget* parent = nullptr);
	~Graph() override;

	// Setters
	void setAxesColor(QColor const& color) { m_axesColor = color; }

public slots:
	void updateWidth(int newWidth);
	void build();

protected:
	void paintEvent(QPaintEvent*) override;

	// Fields
	MainWindow* m_win;
	QPainterPath* m_graphPath;
	QPainterPath* m_axesPath;
	QColor m_axesColor = Qt::black;
	bool m_init;
	int m_gx0;
	int m_y0;
};

// ---------------------------------------------------------------------------

class GraphView : public QGraphicsView {
	Q_OBJECT

public:
	explicit GraphView(QGraphicsScene* scene, QWidget* parent = nullptr) : QGraphicsView(scene, parent) {}
	~GraphView() override {}

protected:
	void resizeEvent(QResizeEvent*) override { emit widthUpdated(this -> viewport() -> width()); }

signals:
	void widthUpdated(int width);
};