#include "graph.hpp"
#include "calc.hpp"
#include <limits>
#include <QPainter>


Graph::Graph(MainWindow* win, QWidget* parent) : QWidget(parent), m_win(win) {
	m_graphPath = new QPainterPath;
	m_axesPath = new QPainterPath;
}

void Graph::updateWidth(int newWidth) {
	this -> setFixedWidth(newWidth);

	return;
}
#include <fstream>
void Graph::build() {
	m_graphPath -> clear();

	int minX = m_win -> m_limitMin -> getShownText().toInt();
	int maxX = m_win -> m_limitMax -> getShownText().toInt();
	double dx = double(maxX - minX) / this -> width();

	QList<std::optional<double>> points;
	points.reserve(this -> width() + 1);
	for (int i = 0; i < this -> width() + 1; ++i)
		points.append(std::nullopt);

	m_init = false;
	size_t gx = 0;
	double prevGx = -1;
	auto tokens = Calc::postfixList(m_win -> m_function -> getShownText());
	CalcX calc;

	std::ofstream O;
	O.open("C:/Users/User/Desktop/debug.txt");
	O << "-------------------- Preinit -------------------------\n";
	O << "Tokens:\n";
	for (auto i : tokens)
		O << i.toStdString() << '\n';
	O << "------------------------------------------------------\n";
	for (double x = minX; x < maxX; x += dx) {
		if (prevGx < 0 && 0 < gx)
			m_gx0 = gx;

		if (abs(x) < __FLT_EPSILON__) // Fix FP precision
			x = 0;
		points[gx] = calc(tokens, x);
		if (points[gx]) {
			O << gx << " | x: " << x << ", y: " << points[gx].value() << '\n';
		}
		prevGx = x;
		++gx;
	}

	gx = 0;
	m_y0 = this -> height() / 2;
	for (auto y : points) {
		if (y) {
			O << "gx: " << gx << ", gy: " << y.value() << '\n';
			if (m_init) { // y has a value and this is not the first point
				m_graphPath -> lineTo(gx, m_y0 - y.value());
			} else { // y has a value and this is the first point
				m_init = true;
				m_graphPath -> moveTo(gx, m_y0 - y.value());
			}
		} else { // y is std::nullopt
			m_graphPath -> moveTo(gx, -1);
		}
		++gx;
	}

	O << "bwaaa\n";
	for (size_t i = 0; i < 10; ++i)
		O << m_graphPath -> elementAt(i).x << " " << m_graphPath -> elementAt(i).y << '\n';

	this -> update();

	return;
}

void Graph::paintEvent(QPaintEvent*) {
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(QPen(m_axesColor, 4));

	m_axesPath -> clear();
	// x
	m_axesPath -> moveTo(0, m_y0);
	m_axesPath -> lineTo(this -> width(), m_y0);
	// y
	m_axesPath -> moveTo(m_gx0, 0);
	m_axesPath -> lineTo(m_gx0, this -> height());

	painter.drawPath(*m_axesPath);

	painter.setPen(QPen(m_win -> SettingsValues.graphLineColor, 4));
	painter.drawPath(*m_graphPath);

	return;
}

Graph::~Graph() {
	delete m_graphPath;
}